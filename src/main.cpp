#include <Arduino.h>
#include <SoftwareWire.h>
#include <ros.h>
#include <std_msgs/Int16.h>

const uint8_t as5600_slave_address = 0x36;
const uint8_t as5600_raw_angle_address = 0x0c;
const uint8_t as5600_status_address = 0x0b;


SoftwareWire sw1(9,8);
SoftwareWire sw2(A4,A5);

uint32_t last_micros = 0;

ros::NodeHandle nh;
std_msgs::Int16 encoder1_pos;
ros::Publisher pub("encoder1_pos", &encoder1_pos);

void setup()
{
  sw1.begin();
  sw1.setClock(400000);
  sw1.beginTransmission(as5600_slave_address);
  sw1.write(byte(as5600_raw_angle_address));
  sw1.endTransmission();

  sw2.begin();
  sw2.setClock(400000);
  sw2.beginTransmission(as5600_slave_address);
  sw2.write(byte(as5600_raw_angle_address));
  sw2.endTransmission();

  nh.initNode();
  nh.advertise(pub);
}


void loop()
{
  uint32_t current_micros = micros();
  // Serial.print(1/(float)(current_micros-last_micros)*1000000.0);

  uint16_t reading1 = 0;
  uint8_t status1 = 0;
  sw1.beginTransmission(as5600_slave_address);
  sw1.write(byte(as5600_status_address));
  sw1.endTransmission();

  sw1.beginTransmission(as5600_slave_address);
  sw1.write(byte(as5600_raw_angle_address));
  sw1.endTransmission();
  sw1.requestFrom(as5600_slave_address, 2);
  reading1 = sw1.read()<<8;
  reading1 |= sw1.read();
  sw1.endTransmission();

  uint16_t reading2 = 0;
  sw2.requestFrom(as5600_slave_address, 2);
  reading2 = sw2.read()<<8;
  reading2 |= sw2.read();
  sw2.endTransmission();

  encoder1_pos.data = reading1;
  pub.publish(&encoder1_pos);
  nh.spinOnce();
  last_micros = current_micros;
}


// -- END OF FILE --