#include "roothamWifi.h"

bool initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiSSID, wifiPassword);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 15000)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nWiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    }

    Serial.println("\nWiFi connection failed");
    return false;
}

bool isWiFiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void maintainWiFi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi disconnected, attempting to reconnect...");
        if (!initWiFi())
        {
            Serial.println("Reconnection failed, retrying...");
        }
    }
    else
    {
        return; // WiFi is already connected, no action needed
    }
}

// One-time initialization task (self-deleting)
void initWiFiTask(void *parameters)
{
    delay(1000);
    if (!initWiFi())
    {
        Serial.println("Failed to initialize WiFi in task");
    }
    vTaskDelete(NULL); // Delete task after initialization (success or failure)
}

// Persistent maintenance task (runs forever)
void maintainWiFiTask(void *parameters)
{
    while (true)
    {
        // Directly implement maintenance logic here
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("WiFi disconnected! Reconnecting...");
            initWiFi(); // Attempt to reconnect
        }
        vTaskDelay(10000 / portTICK_PERIOD_MS); // Check every 10s
    }
}