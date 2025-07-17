#ifndef MY_WIFI_H
#define MY_WIFI_H

#include <WiFi.h>
#include "wifi_config.h" // Include your WiFi configuration header

// WiFi control functions
bool initWiFi();
bool isWiFiConnected();
void maintainWiFi();
void initWiFiTask(void *parameters);
void maintainWiFiTask(void *parameters);

#endif