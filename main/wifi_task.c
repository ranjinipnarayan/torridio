#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/spi_master.h"
#include "vospi/vospi.h"
#include "wifi/wifi.h"
#include "shared_frame.h"


#include "lwip/err.h"
#include "lwip/sys.h"

/*
 * This task drives the wifi pipeline
 *
 * The frame semaphore is awaited. Once a frame is available, we preprocess it a
 * segment at a time then send each segment over the wifi
 */

static const char* TAG = "WifiTask";

static vospi_frame_t c_frame_buf;

void wifi_task(c_frame_t* c_frame)
{
  ESP_LOGI(TAG, "Starting WifiTask...");

  for (;;) {

    // Wait for a frame to be available
    if (xSemaphoreTake(c_frame->sem, 1000) == pdTRUE) {

      // Quickly copy the frame into our local buffer to release the sem faster
      memcpy(&c_frame_buf, &c_frame->frame, sizeof(vospi_frame_t));
      xSemaphoreGive(c_frame->sem);

      // Perform a preliminary scan to establish the pixel value range
      uint16_t max = 0, min = UINT16_MAX;
      for (uint8_t seg = 0; seg < VOSPI_SEGMENTS_PER_FRAME; seg ++) {
        for (uint8_t pkt = 0; pkt < VOSPI_PACKETS_PER_SEGMENT_NORMAL; pkt ++) {
          for (uint8_t sym = 0; sym < VOSPI_PACKET_SYMBOLS; sym += 2) {
            uint16_t pix_val = c_frame_buf.segments[seg].packets[pkt].symbols[sym] << 8 |
              c_frame_buf.segments[seg].packets[pkt].symbols[sym + 1];
            max = pix_val > max ? pix_val : max;
            min = pix_val < min ? pix_val : min;
          }
        }
      }

      // Each frame contains 4 segments
      for (uint8_t seg = 0; seg < VOSPI_SEGMENTS_PER_FRAME; seg ++) {

        ESP_LOGI(TAG, "insert wifi code here");

      //   // Allocate space for pixel values for the display
      //   // There will be 4x the bytes because we're scaling to fit a display twice the size
      //   display_segment_t* disp_segment = malloc(sizeof(display_segment_t));

      //   // Render the VoSPI segment onto a display segment
      //   render_vospi_segment(&c_frame_buf.segments[seg], disp_segment, &palettes[0], min, max);

      //   // Draw any graphical additions for this segment ...
      //   gui_render_to_segment(seg, disp_segment);

      //   // Set up the graphic RAM page position for this segment
      //   display_write_segment(seg, disp_segment);

      //   // Free this segment's pixel values
      //   free(disp_segment);
      }
    }
  }

  while (true) ;
}
