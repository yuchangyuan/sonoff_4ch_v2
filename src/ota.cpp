#include "Arduino.h"
#include <ArduinoOTA.h>

#include "io.h"

bool ota_running = false;

void ota_handle()
{
    ArduinoOTA.handle();
}

void ota_setup()
{
    int port = 8266;

#if PIO_PLATFORM_ESPRESSIF8266
    port = 8266;
#elif PIO_PLATFORM_ESPRESSIF32
    port = 3232;
#endif

    ArduinoOTA.setPort(port);
    ArduinoOTA.setHostname("esp8266-1");
    ArduinoOTA.setPassword("1234");

    ArduinoOTA.onStart([]() {
            String type;

            if (ArduinoOTA.getCommand() == U_FLASH) {
                type = "sketch";
            } else { // U_SPIFFS
                type = "filesystem";
            }

            // reset io
            io_reset();

            // NOTE: if updating SPIFFS this would be
            // the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);

            ota_running = true;
        });

    ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
        });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });

    ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) {
                Serial.println("Auth Failed");
            } else if (error == OTA_BEGIN_ERROR) {
                Serial.println("Begin Failed");
            } else if (error == OTA_CONNECT_ERROR) {
                Serial.println("Connect Failed");
            } else if (error == OTA_RECEIVE_ERROR) {
                Serial.println("Receive Failed");
            } else if (error == OTA_END_ERROR) {
                Serial.println("End Failed");
            }
        });

    ArduinoOTA.begin();
}
