#include "SoftAS5600.h"

SoftAS5600::SoftAS5600(){
}

SoftAS5600::SoftAS5600(uint8_t sda_pin, uint8_t scl_pin, uint32_t clock){
    sw = SoftwareWire(sda_pin, scl_pin);
    sw.setClock(clock);
    sw.begin();
}

uint16_t SoftAS5600::readRawAngle(){
    uint16_t reading = 0;
    sw.beginTransmission(i2c_slave_address);
    sw.write(byte(status_address));
    sw.endTransmission();

    sw.requestFrom(i2c_slave_address, 1);
    uint8_t status = sw.read();
    sw.endTransmission();
    
    bool mh = status & 0b00001000;
    bool ml = status & 0b00010000;
    bool md = status & 0b00100000;
    if(mh==false && ml==false && md==true){
        sw.beginTransmission(i2c_slave_address);
        sw.write(raw_angle_address);
        sw.endTransmission();
        sw.requestFrom(i2c_slave_address, 2);
        reading = sw.read()<<8;
        reading |= sw.read();
        sw.endTransmission();
    }
    else{
        reading = invaid_angle;
    }

    return reading;
}