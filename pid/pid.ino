#include <TimerOne.h>

// Arduino
#define FAN 9

// Variables
int input = 0;
double output = 0;

void control();

void setup() {
  pinMode(FAN, OUTPUT);

  Serial.begin(9600);

  // Setup TimerOne
  Timer1.initialize(2000); // interrupts every 2000us == 500Hz frequency
  Timer1.attachInterrupt(control);
  Timer1.pwm(FAN, output);  
}

void loop() {
  // Communication
  if (Serial.available() > 0) {
    input = Serial.parseInt(); // read user input from 0% to 100%
    if (input > 100) input = 100;
    else if (input < 0) input = 0;
    Serial.print("New input value: ");
    Serial.println(input);
  }
}

void control() {
  output = input * 10.24;
  Timer1.setPwmDuty(FAN, output);
}

