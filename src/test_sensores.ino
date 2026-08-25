const int triggerIzq = 2;
const int echoIzq = 3;
const int triggerDer = 4;
const int echoDer = 5;

long leerDistancia(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
 
  pinMode(echoPin, INPUT);
  long duracion = pulseIn(echoPin, HIGH);
  long distanciaCm = duracion / 29 / 2;
  return distanciaCm;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  long distIzquierda = leerDistancia(triggerIzq, echoIzq);
  long distDerecha = leerDistancia(triggerDer, echoDer);
 
  Serial.print("Sensor Izquierdo: ");
  Serial.print(distIzquierda);
  Serial.print(" cm | Sensor Derecho: ");
  Serial.print(distDerecha);
  Serial.println(" cm");
 
  delay(500); // Lee cada medio segundo
}
