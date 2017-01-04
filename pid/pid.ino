#include <TimerOne.h>

// Arduino
#define FAN 9
#define POT 0

// P constants
const float kp = 37; // critical value

// P variables
int input = 0;
double angle = 0.0;
double output = 0.0;
int maximum = 1, minimum = 0;

void control();

void setup() {
  pinMode(FAN, OUTPUT);
  pinMode(POT, INPUT);

  Serial.begin(9600);

  // Calibration
  Serial.print("Starting calibration... ");
  digitalWrite(LED_BUILTIN, HIGH);
  while (millis() < 5000) {
    angle = analogRead(POT);
    if (angle > maximum) maximum = angle;
    else if (angle < minimum) minimum = angle;
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("done. Maximum recorded value: ");
  Serial.println(maximum);

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

  // Debug
  Serial.print("Sensor: ");
  Serial.print(angle); // %
  Serial.print(" - ");
  Serial.print("Output: "); // 0 to 1024
  Serial.println(output);
}

void control() {
  angle = analogRead(POT) * 100.0 / (maximum - minimum); // read position and convert to % of available range

  output = kp * (input - angle) * 10.24;
  if (output > 1024) output = 1024;
  else if (output < 0) output = 0;
  
  Timer1.setPwmDuty(FAN, output); // output from 0 to 1024
}

