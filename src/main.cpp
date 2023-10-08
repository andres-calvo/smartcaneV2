#include <Arduino.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "HMC5883L.h"

// compass variables
HMC5883L Compass;
int16_t mx, my, mz;
float declination = -4.4; // medellin inclination

int analizeCompass();

void setup()
{

  Serial.begin(115200);
  Wire.begin(GPIO_NUM_21, GPIO_NUM_22);
  Compass.initialize();
  Wire.setClock(100000L);
  pinMode(GPIO_NUM_21, INPUT_PULLUP);
  pinMode(GPIO_NUM_22, INPUT_PULLUP);

}

void loop()
{
  delay(100);
  analizeCompass();
}

int analizeCompass()
{ // measures compas and outputs the correct audio id to play

  Compass.getHeading(&mx, &my, &mz);
  // There is likely a spurious read in the i2c buffer, I reading again to discard previous reading and apply a delay.
  // TODO: fix this code to improve performance
  delay(5);
  Compass.getHeading(&mx, &my, &mz);
  float theta = atan2(my, mx);
  theta = theta * (180 / M_PI);
  theta -= declination;
  // 186 degrees is the factor of correction because of the positioning of the sensor in the case
  // I measured the difference using a digital compass in a mobile phone as a reference device
  theta -= 186;

  if (theta < 0)
    theta += 360;

  Serial.print("compass reading: ");
  Serial.println(theta);
}