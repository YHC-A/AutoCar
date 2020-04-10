// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.
#include <NewPing.h>
#define SONAR_NUM 6      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping
int stopornot1 = 10; //輸出HIGH或LOW給Mega(倒車雷達使用)(中斷用)
int quickstop = 11;   //道出雷達瞬間出現障礙物

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(5, 2, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(5, 3, MAX_DISTANCE), 
  NewPing(5, 4, MAX_DISTANCE),
  NewPing(5, 6, MAX_DISTANCE),
  NewPing(5, 7, MAX_DISTANCE),
  NewPing(5, 8, MAX_DISTANCE),    
};

#include <Wire.h>
#define TWI_FREQ 400000L
int DIS[6];
void setup() {
  Serial.begin(115200);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  pinMode(A5,OUTPUT);
  pinMode(A4,OUTPUT);
}

void loop() {
for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
    delay(30); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    //  exDIS[i]=DIS[i];
    DIS[i]=sonar[i].ping_cm();
    if(DIS[i]==0){
      DIS[i]=200;
    }

  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  //char dis[6]={DIS[0],DIS[1],DIS[2],DIS[3],DIS[4],DIS[5]}; 
  for(int I=0;I<6;I++){
  Wire.write(DIS[I]); // respond with message of 6 bytes
    Serial.print(DIS[I]);
    Serial.print("  ");
}
  Serial.println();
}
