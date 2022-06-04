#include <Arduino.h>
#include <SoftwareWire.h>
#include <ros.h>
#include <std_msgs/Int16.h>
#include "SoftAS5600/SoftAS5600.h"
SoftAS5600 encoder1;
SoftAS5600 encoder2;

uint32_t last_micros = 0;

ros::NodeHandle nh;
std_msgs::Int16 encoder1_pos;
std_msgs::Int16 encoder2_pos;

ros::Publisher pub1("encoder1_pos", &encoder1_pos);
ros::Publisher pub2("encoder2_pos", &encoder2_pos);


void setup()
{
  encoder1 = SoftAS5600(9,8);
  encoder2 = SoftAS5600(A4, A5);
  nh.initNode();
  nh.advertise(pub1);
  nh.advertise(pub2);
  nh.spinOnce();
}


void loop()
{
  uint32_t current_micros = micros();
  // Serial.print(1/(float)(current_micros-last_micros)*1000000.0);

  uint16_t reading1 = 0;
  reading1 = encoder1.readRawAngle();
  if(reading1 != encoder1.invaid_angle){
    encoder1_pos.data = reading1;
    pub1.publish(&encoder1_pos);
  }

  uint16_t reading2 = 0;
  reading2 = encoder2.readRawAngle();
  if(reading2 != encoder2.invaid_angle){
    encoder2_pos.data = reading2;
    pub2.publish(&encoder2_pos);
  }
  nh.spinOnce();
  last_micros = current_micros;
}


// -- END OF FILE --