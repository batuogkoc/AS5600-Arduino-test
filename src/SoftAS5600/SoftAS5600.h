#ifndef __SOFTAS5600__
#define __SOFTAS5600__

#include <Arduino.h>
#include "SoftwareWire.h"

class SoftAS5600{
    public:
        SoftAS5600();
        SoftAS5600(uint8_t sda_pin, uint8_t scl_pin, uint32_t clock=400000);
        
        uint16_t readRawAngle();
        static const uint16_t invaid_angle = -1;

    private:
        SoftwareWire sw;
        static const uint8_t i2c_slave_address = 0x36;
        static const uint8_t raw_angle_address = 0x0c;
        static const uint8_t status_address = 0x0b;
};

#endif