#include <Servo.h>

Servo servoDireccion;
const int pinServo = 9;       // Pin del servomotor delantero
const int pinMotorPWM = 5;    // Pin de velocidad del motor trasero (Driver L298N)
const int pinMotorIN1 = 6;    // Pin de dirección 1 del motor trasero
const int pinMotorIN2 = 7;    // Pin de dirección 2 del motor trasero

void setup() {
  servoDireccion.attach(pinServo);
  pinMode(pinMotorPWM, OUTPUT);
  pinMode(pinMotorIN1, OUTPUT);
  pinMode(pinMotorIN2, OUTPUT);
}

void loop() {
  // Pruebe la dirección delantera (Izquierda, Centro, Derecha)
  servoDireccion.write(45);  // Girar ruedas a la izquierda
  delay(1000);
  servoDireccion.write(90);  // Ruedas rectas al centro
  delay(1000);
  servoDireccion.write(135); // Girar ruedas a la derecha
  delay(1000);
 
  // Pruebe el motor trasero (Avanzar 2 segundos y frenar)
  digitalWrite(pinMotorIN1, HIGH);
  digitalWrite(pinMotorIN2, LOW);
  analogWrite(pinMotorPWM, 150); // Velocidad media
  delay(2000);
 
  analogWrite(pinMotorPWM, 0);   // Frenar
  delay(2000);
}
