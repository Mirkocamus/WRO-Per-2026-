const int triggerLat = 11; // Sensor ultrasónico lateral
const int echoLat = 12;
const int DISTANCIA_PARED = 25; // Distancia límite en cm para detectar la pared lateral

int contadorVueltas = 0;
bool banderaPared = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Lógica de lectura simplificada del sensor lateral
  digitalWrite(triggerLat, LOW); delayMicroseconds(2);
  digitalWrite(triggerLat, HIGH); delayMicroseconds(10);
  digitalWrite(triggerLat, LOW);
  long distancia = pulseIn(echoLat, HIGH) / 29 / 2;

  // Control para evitar falsos positivos (Antirrebote por software)
  if (distancia < DISTANCIA_PARED && !banderaPared) {
    contadorVueltas++;
    banderaPared = true; // Se activa la bandera para no volver a contar la misma pared
    Serial.print("¡Vuelta registrada! Total de vueltas: ");
    Serial.println(contadorVueltas);
  }
 
  if (distancia > DISTANCIA_PARED + 10) {
    banderaPared = false; // Se libera la bandera cuando el auto ya se alejó de la pared
  }

  // Condición final exigida por el concurso
  if (contadorVueltas >= 3) {
    Serial.println("¡3 Vueltas completadas! Ejecutando rutina de ESTACIONAMIENTO...");
    // Aquí se llamará a la función de frenado y acomodo el día viernes
    while(true); // Detiene el código por completo
  }
 
  delay(100);
}
