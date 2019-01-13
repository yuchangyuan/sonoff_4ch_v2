#ifndef IO_H
#define IO_H

#define IO_CH 4

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    void io_relay_set(int i, bool v);
    bool io_relay_get(int i);
    void io_led_set(bool v);
    bool io_button_get(int i);

    void io_reset();

#ifdef __cplusplus
}
#endif

#endif
