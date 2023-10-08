#include <Arduino.h>
#include <SerialMP3Player.h>

const int trigPin = 5;
const int echoPin = 18;
const int ultraSoundButton = 19;
int isUltraSoundActive = LOW;

SerialMP3Player mp3(RX,TX);

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration;
float distanceCm;
float distanceInch;

// Function declarations
int getDistance();

void setup()
{
  mp3.showDebug(1);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ultraSoundButton,INPUT_PULLDOWN);
  mp3.begin(9600);        // start mp3-communication
  delay(500);             // wait for init
  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  mp3.setVol(30);
  delay(500);
  mp3.qTTracks();
  delay(500);
}

void loop()
{
  isUltraSoundActive = digitalRead(ultraSoundButton);
  if(isUltraSoundActive == HIGH){
    getDistance();
    delay(100);
  }
  delay(100);
}

int getDistance(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  return distanceCm;
}