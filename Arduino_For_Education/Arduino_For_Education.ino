/* LIBRERIAS */
#include <Firmata.h>
/* Variables para control de tiempo */
unsigned long currentMillis = 0;        // Guarda valor actual en millis()
unsigned long previousMillis = 0;       // Para comparar con la variable de tiempo actual.
unsigned int interval = 19;             // intervalo para la ejecución de la lectura de valores (in ms)
// Variable para determinar si llega un mensaje Firmata para procesar.
bool message = false;
//Enumeracion con los estados del sistema
enum Estados {
  ESPERANDO,
  ENVIANDO_VALORES,
  PROCESANDO
};
// Declaramos una variable que almacenará el estado actual del sistema
Estados estado;

void setup() {
  // Luego de la configuración se empieza en el estado ESPERANDO
  estado = ESPERANDO;
}

void loop() {
  // Switch - case para el control del sistema
  switch (estado) {
    case ESPERANDO:
      currentMillis = millis();
      if (currentMillis - previousMillis > interval) {
        estado = ENVIANDO_VALORES;
      }
      else if (message) {
        estado = PROCESANDO;
      }
      else {
        estado = ESPERANDO;
      }
      break;
    case ENVIANDO_VALORES:
      estado = ESPERANDO;
      break;
    case PROCESANDO:
      estado = ESPERANDO;
      break;
  }

}
