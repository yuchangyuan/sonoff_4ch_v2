#include "udp.h"
#include "io.h"

#include <Arduino.h>

#include <WiFiUdp.h>
#include <stdint.h>

static WiFiUDP Udp;
static char _msg_buf[256];

static IPAddress _remote_ip;
static int _remote_port = -1;

void udp_setup(int port)
{
    Udp.begin(port);
}

void udp_set_msg(char *msg)
{
    strncpy(_msg_buf, msg, sizeof(_msg_buf));
    _msg_buf[255] = 0;
}


void udp_proc()
{
    const int SZ = 256;
    int packetSize = Udp.parsePacket();
    char rx_buf[SZ];  // buffer for incoming packets

    if (packetSize) {
        char ret[IO_CH+2] = {0,};

        // receive incoming UDP packets
        _remote_ip = Udp.remoteIP();
        _remote_port = Udp.remotePort();

        Serial.printf("recv %d bytes, set return address, %s:%d\n",
                      packetSize,
                      _remote_ip.toString().c_str(),
                      _remote_port);

        int len = Udp.read(rx_buf, SZ);

        while (len > 0) {
            //Serial.printf("read %d bytes\n", len);
            //rx_buf[len] = 0;
            //Serial.printf("UDP packet contents: %s\n", rx_buf);
            for (int i = 0; i < len; ++i) {
                char c = rx_buf[i];
                int off = c & 0x20;
                int idx = (c | 0x20) - 0x61;

                if ((idx >= 0) && (idx < IO_CH)) {
                    bool v = off ? false : true;
                    io_relay_set(idx, v);
                }
            }

            len = Udp.read(rx_buf, SZ);
        }

        // send back a reply
        Udp.beginPacket(_remote_ip, _remote_port);
        //sprintf(ret, "port %d uptime_ms %lu\n", _remote_port, millis());

        for (int i = 0; i < IO_CH; ++i) {
            ret[i] = io_relay_get(i) ? ('A' + i) : ('a' + i);
        }
        ret[IO_CH] = '\n';
        ret[IO_CH+1] = 0;

        Udp.write((uint8_t*)ret, strlen(ret));
        Udp.endPacket();
    }

    if ((_remote_port > 0) && (_msg_buf[0] != 0)) {
        Udp.beginPacket(_remote_ip, _remote_port);
        Udp.write((uint8_t*)_msg_buf, strlen(_msg_buf));
        Udp.endPacket();

        _msg_buf[0] = 0;
    }
}
