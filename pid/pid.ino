#include <TimerOne.h>

/**
 * FASE 2 - Control Proporcional
 */

/**
 * Concepte: D(t) = Kp * e(t)
 * Implementació: pwm_n = Kp = (target_perc_n - perc_n)
 * En augmentar Kp arriba a ser inestable (valor raonable)
 */

/**
 * Trobar Kp crítica i període d'oscil·lació
 * Circuit #4
 */

#define FAN 9
#define POT 0

int percent = 0;
double in = 0, out = 0, kp = 100; // kp pot ser molt alta per fregament i inèrcia del sistema
bool flag = false;

void setup() {
  pinMode(FAN, OUTPUT);
  pinMode(POT, INPUT);

  // Calibració
  
  Timer1.initialize(2000);
  Timer1.attachInterrupt(control);

  Serial.begin(9600);

  Timer1.pwm(FAN, out);
}

void loop() {
  while (true) {
    // Comunicació, etc.
    if (Serial.available() > 0) {
      // Input és % (s'haurà de comprovar si > 100, etc.)
      percent = Serial.parseInt();
      Serial.println(percent);
    }
  
    // Serial.print(out); // Serial en interrupt no funciona
    // Serial.print(" - ");
    // Serial.println(in);
    // Serial.println(percent - in);
    Serial.println(percent);
    Serial.println(in);
  }

  if (flag) {
    in = analogRead(POT) * 100.0/384; // mitjana per eliminar soroll
    // Serial.println(in);
    
    out = kp * (percent - in) * 10.24; // kp hauria de ser f(percent)
    if (out > 1024) out = 1024;
    if (out < 0) out = 0;
    // Serial.println(percent);
    // Serial.println(out);
    
    Timer1.setPwmDuty(FAN, out);
  }
}

void control() {
  flag = true;
}

