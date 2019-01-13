#include "io.h"
#include <Arduino.h>

#define B_1 0
#define B_2 9
#define B_3 10
#define B_4 14
#define L_1 12
#define L_2 5
#define L_3 4
#define L_4 15
#define LED 13

const int L_PIN[IO_CH] = { L_1, L_2, L_3, L_4 };
const int B_PIN[IO_CH] = { B_1, B_2, B_3, B_4 };

bool _relay[IO_CH] = {false, false, false, false};

// set all LED/relay off
// set button input mode
void io_reset()
{
    int i;

    for (i = 0; i < IO_CH; ++i) {
        _relay[i] = false;

        digitalWrite(L_PIN[i], _relay[i]);
        pinMode(L_PIN[i], OUTPUT);
        digitalWrite(L_PIN[i], _relay[i]);

        pinMode(B_PIN[i], INPUT);
    }

    pinMode(LED, OUTPUT);
    io_led_set(false);
}

void io_relay_set(int n, bool v)
{
    if ((n < IO_CH) && (_relay[n] != v)) {
        _relay[n] = v;
        digitalWrite(L_PIN[n], v ? HIGH : LOW);
    }
}

bool io_relay_get(int n)
{
    if (n < IO_CH) return _relay[n];

    return false;
}

bool io_button_get(int n)
{
    if (n < IO_CH) return (digitalRead(B_PIN[n]) == HIGH);

    return false;
}

void io_led_set(bool v)
{
    digitalWrite(LED, v ? LOW : HIGH);
}
