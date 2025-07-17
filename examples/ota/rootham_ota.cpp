#include "rootham_ota.h"
#include "rootham_wifi.h"

WebServer server(80);

bool is_ota_initialized = false;

void initOTATask(void *parameters)
{
    delay(5000); // Allow time for WiFi to connect
    if (!isWiFiConnected())
    {
        Serial.println("WiFi not connected, OTA cannot start");
        return;
    }

    // OTA Update Handlers
    if (!is_ota_initialized && isWiFiConnected())
    {
        is_ota_initialized = true;
        Serial.println("Initializing OTA Update Server...");

        server.on("/", HTTP_GET, []()
                  {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html",
            "<form method='POST' action='/update' enctype='multipart/form-data'>"
            "<input type='file' name='update'>"
            "<input type='submit' value='Update'>"
            "</form>"); });

        server.on("/update", HTTP_POST, []()
                  {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart(); }, []()
                  {
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
            Serial.printf("Update: %s\n", upload.filename.c_str());
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) {
                Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            } else {
                Update.printError(Serial);
            }
        } });

        server.begin();
        Serial.println("OTA Update Server Ready");
        vTaskDelete(NULL);
    }
    else
    {
        Serial.println("OTA Update Server already initialized or WiFi not connected.");
        return;
    }
}

void startOTAserver(void *parameters)
{
    delay(6000); // Allow time for OTA initialization
    while (true)
    {
        if (isWiFiConnected() && is_ota_initialized)
        {
            server.handleClient();
        }
        delay(1);
    }
}