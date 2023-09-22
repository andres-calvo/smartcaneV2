#include <Arduino.h>
#include <Wire.h> //I2C Arduino Library

#define addr 0x0D // I2C Address for The HMC5883

float declination = -4.4; //medellin inclination

void setup()
{

  Serial.begin(115200);
  Wire.begin();

  Wire.beginTransmission(addr); // start talking
  Wire.write(0x09);             // Set the Register
  Wire.write(0x1D);             // Tell the HMC5883 to Continuously Measure
  Wire.endTransmission();
}

void loop()
{

  int x, y, z; // triple axis data

  // Tell the HMC what regist to begin writing data into
  Wire.beginTransmission(addr);
  Wire.write(0x00); // start with register 3.
  Wire.endTransmission();

  // Read the data.. 2 bytes for each axis.. 6 total bytes
  Wire.requestFrom(addr, 6);
  if (6 <= Wire.available())
  {
    x = Wire.read() << 8; // MSB x
    x |= Wire.read();     // LSB x
    z = Wire.read() << 8; // MSB z
    z |= Wire.read();     // LSB z
    y = Wire.read() << 8; // MSB y
    y |= Wire.read();     // LSB y
  }

  

  float theta = atan2(y, x);
  theta = theta * (180 / M_PI);
  theta -= declination;
  // 186 degrees is the factor of correction because of the positioning of the sensor in the case
  // I measured the difference using a digital compass in a mobile phone as a reference device
  theta -= 186;

  if (theta < 0) theta += 360;

  Serial.print("Degree: °");
  Serial.println(theta);

  delay(500);
}