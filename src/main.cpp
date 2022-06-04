#include <Arduino.h>
#include <SoftwareWire.h>
#include <ros.h>
#include <std_msgs/Int16.h>
#include "SoftAS5600/SoftAS5600.h"

const uint8_t as5600_slave_address = 0x36;
const uint8_t as5600_raw_angle_address = 0x0c;
const uint8_t as5600_status_address = 0x0b;


// SoftwareWire sw1(9,8);
// SoftwareWire sw2(A4,A5);
SoftAS5600 encoder1(9,8);

uint32_t last_micros = 0;

ros::NodeHandle nh;
std_msgs::Int16 encoder1_pos;
ros::Publisher pub("encoder1_pos", &encoder1_pos);

void setup()
{
  Serial.begin(2000000);
  nh.getHardware()->setBaud(2000000);
  nh.initNode();
  nh.advertise(pub);
}


void loop()
{
  uint32_t current_micros = micros();
  // Serial.print(1/(float)(current_micros-last_micros)*1000000.0);

  uint16_t reading1 = 0;
  reading1 = encoder1.readRawAngle();
  if(reading1 != encoder1.invaid_angle){
    encoder1_pos.data = reading1;
    pub.publish(&encoder1_pos);
    nh.spinOnce();
  }
  last_micros = current_micros;

}


// -- END OF FILE --