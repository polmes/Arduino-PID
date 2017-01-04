#include <TimerOne.h>

// Arduino
#define FAN 9
#define POT 0
#define T_SAMPLE 0.002

// MATLAB
float t;

// PI constants
const float kp = 19.2;
const float ki = 200.0;

// PI variables
int input = 0;
double angle = 0.0;
double output = 0.0;
double e = 0.0, e_prev = 0.0, sum_e = 0.0;
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

  // Setup TimerOne
  Timer1.initialize(2000); // interrupts every 2000us == 500Hz frequency
  Timer1.attachInterrupt(control);
  Timer1.pwm(FAN, output);

  t = 0;
}

void loop() {
  while (true) {
    // Wait 5s, then set input to 50%
    if (millis() < 5000) input = 0;
    else input = 50;

    if (flag) {
      angle = analogRead(POT) * 100.0 / (maximum - minimum); // read position and convert to % of available range
  
      e_prev = e; // previous error
      e = input - angle ; // current error
      sum_e = sum_e + e; // sum of errors
      
      output = (kp * e + ki * T_SAMPLE * sum_e) * 10.24;
      
      // Corrections
      if (output > 1024) {
        output = 1024;
        sum_e = (output / 10.24 - kp * e) / (ki * T_SAMPLE);
      } else if (output < 0) {
        output = 0;
        sum_e = (output / 10.24 - kp * e) / (ki * T_SAMPLE);
      }
      
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

