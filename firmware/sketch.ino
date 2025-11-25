// Alimentador de Gatos Jose Miguel
// --- Bibliotecas ---
#include <ESP32Servo.h> // Necesaria para el control angular preciso del Servomotor

// --- Definición de Pines de E/S ---
const int pinPIR = 2;    // Pin de Entrada (Sensor)
const int pinServo = 13; // Pin de Salida (Actuador de Movimiento)
const int pinLED = 5;    // Pin de Salida (Actuador de Luz/Indicador)

// --- Variables y Objetos del Comedero ---
Servo dispensador;       // Crea un objeto llamado "dispensador"
int anguloDispensa = 140; // POSICIÓN 1: Ángulo para que caiga el alimento
int anguloCerrado = 40;   // POSICIÓN 2: Ángulo de reposo (Tapa cerrada)

// Bandera lógica: evita que el servo gire sin parar
bool dispensado = false; // Inicialmente, no se ha servido comida

void setup() {
  Serial.begin(115200); // Inicia el Monitor Serial
  pinMode(pinPIR, INPUT);    // PIR como entrada
  pinMode(pinLED, OUTPUT);   // LED como salida

  // Configuración del Servomotor
  dispensador.attach(pinServo);
  dispensador.write(anguloCerrado); // Posición inicial (cerrado)
  digitalWrite(pinLED, LOW);        // LED apagado
  Serial.println("Comedero Inteligente: Modo EN ESPERA...");
}

void loop() {
  int hayGato = digitalRead(pinPIR); // Lee el estado del sensor PIR

  if (hayGato == HIGH) {
    if (dispensado == false) {
      // --- INICIO DE LA SECUENCIA DE DISPENSACIÓN ---
      Serial.println(">>> ¡Gato detectado! Dispensando alimento...");
      dispensador.write(anguloDispensa); // Abre tapa
      digitalWrite(pinLED, HIGH);        // LED encendido
      delay(1500);                       // Espera 1.5 segundos
      dispensador.write(anguloCerrado);  // Cierra tapa
      digitalWrite(pinLED, LOW);         // LED apagado
      dispensado = true;                 // Marca como servido
      Serial.println("Dispensación completada. Servo cerrado.");
      delay(10000);                      // Espera antes de permitir nuevo ciclo
    }
  } else {
    if (dispensado == true) {
      // Si el gato se fue, reinicia bandera
      dispensado = false;
      Serial.println("Sistema reiniciado. Listo para la próxima comida.");
    }
    delay(500); // Pausa breve
  }
}
