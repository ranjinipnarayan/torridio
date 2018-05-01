#ifndef WIFI_H
#define WIFI_H

#include "esp_system.h"

#define ESP_WIFI_IS_STATION 	true
#define ESP_WIFI_SSID      		"Ranjini"
#define ESP_WIFI_PASS      		"hellothere"
#define MAX_STA_CONN       		4

void wifi_init_station();

#endif
