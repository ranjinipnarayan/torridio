#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "wifi/wifi.h"
#include "vospi_task.h"
#include "wifi_task.h"
#include "shared_frame.h"

static c_frame_t c_frame;

static const char* TAG = "Main";

void app_main()
{
  ESP_LOGI(TAG, "Thermal Camera System startup");

  //initialize the wifi 
  wifi_init_station();

  // Create the semaphore for the current frame
  c_frame.sem = xSemaphoreCreateBinary();

  // The semaphore isn't held to start with
  xSemaphoreGive(c_frame.sem);

  // Start tasks
  xTaskCreate(&vospi_task, "vospi_task", 30000, &c_frame, 5, NULL);
  //xTaskCreate(&wifi_task, "wifi_task", 16000, &c_frame, 5, NULL);
}
