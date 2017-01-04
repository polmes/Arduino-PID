// Arduino
#define POT 0

// Variables
int angle = 0;
int maximum = 1, minimum = 0;

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
}

void loop() {
  angle = analogRead(POT);
  
  // Communication
  Serial.print("Sensor: ");
  Serial.print(angle);
  Serial.print(" - ");
  Serial.print(angle * 100.0 / (maximum - minimum));

  delay(20);
}

