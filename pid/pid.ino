#include <TimerOne.h>

// Arduino
#define POT 0

// Variables
int angle = 0;
int maximum = 1, minimum = 0;

void control();

void setup() {
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
}

void loop() {
  // Communication
  Serial.print("Sensor: ");
  Serial.print(angle);
  Serial.print(" - ");
  Serial.println(angle * 100.0 / (maximum - minimum));

  delay(20);
}

void control() {
  angle = analogRead(POT);
}

