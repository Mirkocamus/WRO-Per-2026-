# WRO 2026 - Future Engineers - [FENIX]

Este repositorio contiene la documentación oficial y el código fuente del vehículo autónomo del equipo [Nombre de tu Equipo], participante en la etapa nacional de la World Robot Olympiad Perú 2026.

## 🛠️ Especificaciones Técnicas
* **Controlador Principal:** Arduino UNO
* **Actuadores:** 1 Servomotor (Dirección delantera), 1 Motor DC con Driver [ej. L298N] (Tracción trasera).
* **Sensores:** 3 Sensores de movimiento/ultrasónicos HC-SR04 instalados en el parachoques delantero y lateral.

## 🤖 Estrategia de Navegación
Debido a las restricciones de hardware y optimización de presupuesto, el vehículo utiliza una estrategia de navegación geométrica basada en la distribución de carriles de la pista oficial:
1. El sensor ultrasónico frontal-izquierdo y frontal-derecho mapean el carril continuamente.
2. Al detectar un obstáculo en una mitad del carril, el algoritmo determina por descarte el color del bloque (Rojo/Verde) según el reglamento y ejecuta el viraje de evasión hacia el espacio libre.
3. Un sensor ultrasónico lateral registra el paso por los muros de control para la gestión del contador de 3 vueltas mediante software, activando la rutina de estacionamiento autónomo al finalizar el ciclo.

## 📁 Estructura del Repositorio
* `/src`: Código fuente en C++/Arduino.
* `/schemes`: Diagramas de conexión eléctrica.
* `/v-photos`: Fotografías del vehículo desde los 6 ángulos reglamentarios.
* `/t-photos`: Fotografía oficial del equipo.
