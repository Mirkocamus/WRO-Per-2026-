#include <Servo.h>

// ==========================================
// 1. CONFIGURACIÓN DE PINES (ASIGNACIÓN)
// ==========================================
// Servomotor de Dirección (Eje Delantero)
const int PIN_SERVO = 9;

// Motor de Tracción (Eje Trasero - Driver L298N)
const int PIN_MOTOR_PWM = 5; // Pin Enable (Velocidad)
const int PIN_MOTOR_IN1 = 6; // Dirección de giro 1
const int PIN_MOTOR_IN2 = 7; // Dirección de giro 2

// Sensores Ultrasónicos HC-SR04 (Pines Digitales)
const int TRIG_FRONT_IZQ = 2;
const int ECHO_FRONT_IZQ = 3;
const int TRIG_FRONT_DER = 4;
const int ECHO_FRONT_DER = 8; // Cambiado a 8 para evitar conflicto con el pin 5
const int TRIG_LATERAL   = 11;
const int ECHO_LATERAL   = 12;

// ==========================================
// 2. PARÁMETROS DE CALIBRACIÓN (MODIFICABLES EL VIERNES)
// ==========================================
// Ángulos del Servomotor
const int ANGULO_CENTRO    = 90;  // Auto va recto
const int ANGULO_IZQUIERDA = 55;  // Giro a la izquierda para evadir bloque derecho
const int ANGULO_DERECHA   = 125; // Giro a la derecha para evadir bloque izquierdo

// Distancias de Activación (en centímetros)
const int DIST_EVASION_FRONTAL = 30; // Distancia para empezar a esquivar el bloque
const int DIST_DETECTAR_PARED  = 25; // Distancia lateral para contar vuelta

// Velocidades de Tracción (0 a 255)
const int VELOCIDAD_CRUCERO = 160;   // Velocidad normal en recta
const int VELOCIDAD_MANIOBRA = 110;  // Velocidad reducida durante esquivas o parking

// Tiempos de Maniobra (en milisegundos)
const unsigned long TIEMPO_ESQUIVA = 800; // Cuánto tiempo dura el giro de evasión
const unsigned long TIEMPO_RETORNO = 600; // Cuánto tiempo tarda en volver al carril

// ==========================================
// 3. VARIABLES DE ESTADO LOGÍSTICO
// ==========================================
Servo servoDireccion;
int contadorVueltas = 0;
bool bloqueadoPorPared = false;

// ==========================================
// 4. FUNCIONES DE CONTROL DE HARDWARE
// ==========================================
long obtenerDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  long duracion = pulseIn(echoPin, HIGH, 30000); // Timeout de 30ms para evitar congelamientos
  if (duracion == 0) return 999; // Si no hay eco, asume camino libre
  return (duracion / 29) / 2;
}

void controlarMotor(int velocidad, bool avanzar) {
  if (velocidad == 0) {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
    analogWrite(PIN_MOTOR_PWM, 0);
  } else {
    digitalWrite(PIN_MOTOR_IN1, avanzar ? HIGH : LOW);
    digitalWrite(PIN_MOTOR_IN2, avanzar ? LOW : HIGH);
    analogWrite(PIN_MOTOR_PWM, velocidad);
  }
}

// ==========================================
// 5. CONFIGURACIÓN INICIAL (SETUP)
// ==========================================
void setup() {
  Serial.begin(9600);
 
  // Inicializar Actuadores
  servoDireccion.attach(PIN_SERVO);
  servoDireccion.write(ANGULO_CENTRO);
 
  pinMode(PIN_MOTOR_PWM, OUTPUT);
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);
  controlarMotor(0, true); // Asegurar que arranque detenido
 
  // Inicializar Sensores
  pinMode(TRIG_FRONT_IZQ, OUTPUT); pinMode(ECHO_FRONT_IZQ, INPUT);
  pinMode(TRIG_FRONT_DER, OUTPUT); pinMode(ECHO_FRONT_DER, INPUT);
  pinMode(TRIG_LATERAL,   OUTPUT); pinMode(ECHO_LATERAL,   INPUT);
 
  delay(2000); // Espera de seguridad de 2 segundos antes de iniciar la marcha autónoma
}

// ==========================================
// 6. BUCLE PRINCIPAL (LOOP)
// ==========================================
void loop() {
  // LECTURA DE SENSORES FRONTALES
  long distIzq = obtenerDistancia(TRIG_FRONT_IZQ, ECHO_FRONT_IZQ);
  long distDer = obtenerDistancia(TRIG_FRONT_DER, ECHO_FRONT_DER);
 
  // GESTIÓN DEL CONTADOR DE VUELTAS (SENSOR LATERAL)
  long distLat = obtenerDistancia(TRIG_LATERAL, ECHO_LATERAL);
 
  if (distLat < DIST_DETECTAR_PARED && !bloqueadoPorPared) {
    contadorVueltas++;
    bloqueadoPorPared = true;
    Serial.print("Vuelta detectada de forma autonoma: ");
    Serial.println(contadorVueltas);
  }
  if (distLat > (DIST_DETECTAR_PARED + 10)) {
    bloqueadoPorPared = false; // Antirrebote liberado al pasar la zona de control
  }

  // CONDICIÓN DE ESTACIONAMIENTO AL COMPLETAR LAS 3 VUELTAS
  if (contadorVueltas >= 3) {
    ejecutarEstacionamiento();
  }

  // LÓGICA DE NAVEGACIÓN Y EVASIÓN GEOMÉTRICA DE BLOQUES
  controlarMotor(VELOCIDAD_CRUCERO, true);
  servoDireccion.write(ANGULO_CENTRO);

  // Caso A: Bloque en la izquierda (Asume color VERDE -> Esquiva por la izquierda)
  if (distIzq < DIST_EVASION_FRONTAL) {
    Serial.println("Obstaculo detectado a la izquierda. Iniciando esquiva...");
    controlarMotor(VELOCIDAD_MANIOBRA, true);
   
    // Paso 1: Virar ruedas a la izquierda para rebasar
    servoDireccion.write(ANGULO_IZQUIERDA);
    delay(TIEMPO_ESQUIVA);
   
    // Paso 2: Contra-giro a la derecha para enderezar el coche en el carril libre
    servoDireccion.write(ANGULO_DERECHA);
    delay(TIEMPO_RETORNO);
   
    servoDireccion.write(ANGULO_CENTRO);
  }
  // Caso B: Bloque en la derecha (Asume color ROJO -> Esquiva por la derecha)
  else if (distDer < DIST_EVASION_FRONTAL) {
    Serial.println("Obstaculo detectado a la derecha. Iniciando esquiva...");
    controlarMotor(VELOCIDAD_MANIOBRA, true);
   
    // Paso 1: Virar ruedas a la derecha para rebasar
    servoDireccion.write(ANGULO_DERECHA);
    delay(TIEMPO_ESQUIVA);
   
    // Paso 2: Contra-giro a la izquierda para enderezar el coche en el carril libre
    servoDireccion.write(ANGULO_IZQUIERDA);
    delay(TIEMPO_RETORNO);
   
    servoDireccion.write(ANGULO_CENTRO);
  }

  delay(30); // Estabilización del ciclo de procesamiento
}

// ==========================================
// 7. RUTINA AUTOMÁTICA DE PARQUEO (PARKING)
// ==========================================
void ejecutarEstacionamiento() {
  Serial.println("Iniciando maniobra reglamentaria de Estacionamiento.");
 
  // Frenado en seco inicial
  controlarMotor(0, true);
  delay(500);
 
  // Maniobra automatizada en reversa (Estacionamiento en paralelo tipo Ackerman)
  servoDireccion.write(ANGULO_DERECHA);
  controlarMotor(VELOCIDAD_MANIOBRA, false); // Marcha atrás
  delay(1200);
 
  servoDireccion.write(ANGULO_IZQUIERDA);
  delay(1200);
 
  // Alineación final y detención permanente de motores
  servoDireccion.write(ANGULO_CENTRO);
  controlarMotor(0, true);
  Serial.println("Vehiculo correctamente estacionado dentro del cuadrante. Sistema en espera.");
 
  while(true) {
    // Bucle infinito para apagar el vehículo de forma segura al finalizar la misión
  }
}
