#include "Arduino.h"
#include <Ticker.h>

#include "ota.h"
#include "wifi.h"
#include "udp.h"
#include "io.h"

Ticker ticker;
bool flag = false;

void setup()
{
    io_reset();

    Serial.begin(115200);

    Serial.println("run setup...");

    wifi_setup();
    ota_setup();

    ticker.attach_ms(10, []() { flag = true; });

    udp_setup(12345);
}

void loop()
{
    static int _ota_cnt = 0;
    static float _ota_cnt_avg = 0;

    static int fcnt = 0;
    static int min_cnt = 0xffffff;
    static int max_cnt = 0;

    static bool led = false;

    if ((_ota_cnt & 0xff) == 0) {
        // NOTE: on esp32, ota_handle is expensive
        // so not call every time

        do {
            ota_handle();
        }
        while (ota_running);
    }

    _ota_cnt++;

    if (flag) {
        flag = false; fcnt++;
        _ota_cnt_avg = _ota_cnt_avg * 0.95f + 0.05f * _ota_cnt;
        if (_ota_cnt > max_cnt) max_cnt = _ota_cnt;
        if (_ota_cnt < min_cnt) min_cnt = _ota_cnt;

        if (fcnt == 50) {
            Serial.printf("-------- avg = %.3f(%d ~ %d), %lu\n",
                          _ota_cnt_avg, min_cnt, max_cnt, millis() / 1000);

            fcnt = 0;
            max_cnt = 0;
            min_cnt = 0xffffff;
        }

        _ota_cnt = 0;

        udp_proc();

        if (fcnt == 0) io_led_set(true);
        if (fcnt == 1) io_led_set(false);
    }
}
