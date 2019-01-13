#include "Arduino.h"

#if PIO_PLATFORM_ESPRESSIF8266
#  include <ESP8266WiFi.h>
#elif PIO_PLATFORM_ESPRESSIF32
#  include <WiFi.h>
#else
#  error
#endif

#include "wifi.h"

#include "credential.h"

void wifi_setup()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("gateway: ");
    Serial.println(WiFi.gatewayIP());
}
