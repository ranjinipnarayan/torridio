#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "vospi/vospi.h"
#include "shared_frame.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"


#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (1024)

/*
 * This task drives the Lepton camera interface.
 *
 * The task transfers frames in a tight loop from the camera and tries to update
 * the current frame, if it's currently not available, we don't wait for the
 * semaphore and just advance to the next frame. This way the system runs as
 * fast as the display update pipeline and we won't lose sync with the camera
 * even if the display update is slow.
 */

static const char* TAG = "VoSPITask";

void vospi_task(c_frame_t* c_frame)
{
  ESP_LOGI(TAG, "start VoSPI task...");

  // Allocate space (32-bit multiple for DMA)
  ESP_LOGI(TAG, "preallocating space for segments... (%d bytes)", (sizeof(vospi_frame_t) / 4) * 4);
  vospi_frame_t* frame = heap_caps_malloc((sizeof(vospi_frame_t)/4)*4, MALLOC_CAP_DMA);
  for (int seg = 0; seg < VOSPI_SEGMENTS_PER_FRAME; seg ++) {
    frame->segments[seg].packet_count = VOSPI_PACKETS_PER_SEGMENT_NORMAL;
  }

  // Initialise SPI hardware
  ESP_LOGI(TAG, "starting VoSPI initialisation...");
  vospi_init();

  ESP_LOGI(TAG, "UART Config starting...")

  uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
  uart_param_config(UART_NUM_1, &uart_config);
  uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
  uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);

  ESP_LOGI(TAG, "UART Config setup...")

    // Configure a temporary buffer for the incoming data
  //uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

  while(1) {

    // Synchronise and transfer a single frame
    ESP_LOGI(TAG, "aquiring VoSPI synchronisation");
    if (0 == sync_and_transfer_frame(frame)) {
      ESP_LOGI(TAG, "failed to obtain frame from device.");
      continue;
    }

    ESP_LOGI(TAG, "VoSPI stream synchronised");

    while (1) {

      if (!transfer_frame(frame)) {
        ESP_LOGI(TAG, "resynchronising...");
        break;
      }

      // Obtain the semaphore to update the current frame, if we can't update
      // the current frame, just drop this one and try again next time
      if (xSemaphoreTake(c_frame->sem, 0) == pdTRUE) {
        ESP_LOGI(TAG, "We in here...");
        // Copy the frame into place
        memcpy(&c_frame->frame, frame, sizeof(vospi_frame_t));
        ESP_LOGI(TAG, "Copied to frame...");
        uart_write_bytes(UART_NUM_1, (const char *) frame, sizeof(vospi_frame_t));
        ESP_LOGI(TAG, "Sent over UART...");
        xSemaphoreGive(c_frame->sem);
        exit(0);

      } else {
        ESP_LOGW(TAG, "couldn't obtain c_frame sem, dropping frame");
      }

    }
  }
}
