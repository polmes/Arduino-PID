#include <TimerOne.h>

// Arduino
#define FAN 9
#define POT 0
#define T_SAMPLE 0.002

// MATLAB
float t;

// P constants
const float kp = 37; // critical value

// P variables
int input = 0;
double angle = 0.0;
double output = 0.0;
int maximum = 1, minimum = 0;
bool flag = false;

void control();

void setup() {
  pinMode(FAN, OUTPUT);
  pinMode(POT, INPUT);

  Serial.begin(9600);

  // Manual calibration
  maximum = 404;
  minimum = 0;

  t = 0;
}

void loop() {
  while (true) {
    // Wait 5s, then set input to 50%
    if (millis() < 5000) input = 0;
    else input = 50;

    if (flag) {
      angle = analogRead(POT) * 100.0 / (maximum - minimum); // read position and convert to % of available range
  
      output = kp * (input - angle) * 10.24;
      if (output > 1024) output = 1024;
      else if (output < 0) output = 0;
      
      Timer1.setPwmDuty(FAN, output); // output from 0 to 1024
  
      // Format for MATLAB script
      Serial.print(t, 3);
      Serial.print(',');
      Serial.print(input);
      Serial.print(',');
      Serial.println(int(angle));
  
      flag = false;
    }
  }
}

void control() {
  t += T_SAMPLE;
  flag = true;
}

