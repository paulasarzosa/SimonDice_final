#include <Arduino.h>

// Declaración de pines para LEDs y botones
int leds[] = {6, 7, 8, 9};        // Pines de los LEDs
int botones[] = {2, 3, 4, 5};     // Pines de los botones
int secuencia[20] = {0};          // Secuencia del juego
int indice = 0;

// **Prototipos de funciones**
void secuenciaJuego();
bool secuenciaJugador();
void finJuego();
void enciendeLed(int led);
int leeBoton();

// Configuración inicial
void setup() {
  // Configurar LEDs como salida y botones con pull-up interno
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(botones[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(A0)); // Inicializar semilla aleatoria
  Serial.begin(9600);         // Inicializar comunicación serial
}

// Bucle principal del programa
void loop() {
  // Generar un número aleatorio para la secuencia
  secuencia[indice] = random(0, 4);
  indice++;

  // Limitar la secuencia a un máximo de 20 elementos
  if (indice >= 20) {
    indice = 9;
  }

  secuenciaJuego(); // Mostrar la secuencia generada

  // Comprobar la secuencia del jugador
  if (!secuenciaJugador()) {
    finJuego(); // Si el jugador falla, termina el juego
  }

  delay(300);

  // Si el jugador acertó, avanzar al siguiente nivel
  if (indice > 0) {
    delay(300);
  }
}

// Función para encender un LED por un tiempo breve
void enciendeLed(int led) {
  digitalWrite(leds[led], HIGH);
  delay(300);
  digitalWrite(leds[led], LOW);
}

// Función para mostrar la secuencia al jugador
void secuenciaJuego() {
  for (int i = 0; i < indice; i++) {
    int ledActual = secuencia[i];
    enciendeLed(ledActual);
    delay(50);
  }
}

// Función para leer el botón presionado por el usuario
int leeBoton() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      int btn = botones[i];
      if (digitalRead(btn) == LOW) { // Detectar si el botón se ha presionado
        delay(200); // Evitar rebote
        return i;
      }
    }
    delay(1);
  }
}

// Función para verificar si el jugador sigue la secuencia correctamente
bool secuenciaJugador() {
  for (int i = 0; i < indice; i++) {
    int btnSecuencia = secuencia[i];
    int btnJugador = leeBoton();
    enciendeLed(btnJugador);

    // Si el jugador se equivoca, se termina el juego
    if (btnJugador != btnSecuencia) {
      return false;
    }
  }
  return true;
}

// Función para manejar el fin del juego y mostrar el puntaje
void finJuego() {
  Serial.println("Fin del juego");
  Serial.print("Puntaje: ");
  Serial.println(indice - 1);
  indice = 0;
  delay(200);
}
