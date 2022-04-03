/* LIBRERIAS */
#include <Firmata.h>
#include <FirmataConstants.h>

// extended command set using sysex (0-127/0x00-0x7F)
/* 0x00-0x0F reserved for user-defined commands */
static const int RU_THERE =   0x51;
static const int I_AM_HERE =  0x52;
#define ARDUINO_INSTANCE_ID 1

// the minimum interval for sampling analog input
#define MINIMUM_SAMPLING_INTERVAL 1

#define INTER_PING_INTERVAL 40 // 40 ms.
// Ping variables
int numLoops = 0;

/* Variables para control de tiempo */
//unsigned long currentMillis = 0;        // Guarda valor actual en millis()
//unsigned long previousMillis = 0;       // Para comparar con la variable de tiempo actual.
unsigned int samplingInterval = 19;             // intervalo para la ejecución de la lectura de valores (in ms)
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

byte previousPORT[TOTAL_PORTS];

/*
   Prototipos de la funciones
*/
void Firmata_config();
void sysexCallback(byte, byte, byte *);
void setPinModeCallback(byte pin, int mode);
void digitalWriteCallback(byte port, int value);
void analogWriteCallback(byte pin, int value);
void systemResetCallback(void);

void setup() {
  // Luego de la configuración se empieza en el estado ESPERANDO
  estado = ESPERANDO;
  // Función de configuración de Firmata
  Firmata_config();
}

void loop() {
  /* STREAMREAD - processing incoming messagse as soon as possible, while still
     checking digital inputs.  */
  while (Firmata.available())
    Firmata.processInput();
}

/*
   Firmata_config()
   Función de configuración inicial del Firmata.
*/
void Firmata_config() {
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);

  Firmata.attach(SET_PIN_MODE, setPinModeCallback);
  Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
  Firmata.attach(START_SYSEX, sysexCallback);

  Firmata.begin(115200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for ATmega32u4-based boards and Arduino 101
  }

  //systemResetCallback();
}
/*
   setPinModeCallback
   @params
   byte pin: Número del pin a cambiar el modo.
   -> Distribución de pines [https://garretlab.web.fc2.com/en/arduino/inside/hardware/arduino/avr/cores/arduino/Arduino.h/digitalPinToPort.html]
   int mode: Modo al cual se quiere cambiar. INPUT 0x0 OUTPUT 0x1 INPUT_PULLUP 0x2
   [https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h]
*/
void setPinModeCallback(byte pin, int mode) {
  if (IS_PIN_DIGITAL(pin)) {
    pinMode(PIN_TO_DIGITAL(pin), mode);
  }
}
/*

*/
void digitalWriteCallback(byte port, int value)
{
  byte i;
  byte currentPinValue, previousPinValue;

  if (port < TOTAL_PORTS && value != previousPORT[port]) {
    for (i = 0; i < 8; i++) {
      currentPinValue = (byte) value & (1 << i);
      previousPinValue = previousPORT[port] & (1 << i);
      if (currentPinValue != previousPinValue) {
        digitalWrite(i + (port * 8), currentPinValue);
      }
    }
    previousPORT[port] = value;
  }
}
/*

*/
void analogWriteCallback(byte pin, int value) {
  if (pin < TOTAL_PINS) {
    switch (Firmata.getPinMode(pin)) {
      case PIN_MODE_PWM:
        if (IS_PIN_PWM(pin))
          analogWrite(PIN_TO_PWM(pin), value);
        Firmata.setPinState(pin, value);
        break;
    }
  }
}
/*

*/
void sysexCallback(byte command, byte argc, byte *argv) {
  switch (command) {
    case RU_THERE:
      Firmata.write(START_SYSEX);
      Firmata.write((byte) I_AM_HERE);
      Firmata.write((byte) ARDUINO_INSTANCE_ID);
      Firmata.write(END_SYSEX);
      break;
    case ANALOG_MAPPING_QUERY:
      Firmata.write(START_SYSEX);
      Firmata.write(ANALOG_MAPPING_RESPONSE);
      for (byte pin = 0; pin < TOTAL_PINS; pin++) {
        Firmata.write(IS_PIN_ANALOG(pin) ? PIN_TO_ANALOG(pin) : 127);
      }
      Firmata.write(END_SYSEX);
      break;
    case SAMPLING_INTERVAL:
      if (argc > 1) {
        samplingInterval = argv[0] + (argv[1] << 7);
        if (samplingInterval < MINIMUM_SAMPLING_INTERVAL) {
          samplingInterval = MINIMUM_SAMPLING_INTERVAL;
        }
        /* calculate number of loops per ping */
        numLoops = INTER_PING_INTERVAL / samplingInterval;
      } else {
        //Firmata.sendString("Not enough data");
      }
      break;
    case CAPABILITY_QUERY:
      Firmata.write(START_SYSEX);
      Firmata.write(CAPABILITY_RESPONSE);
      for (byte pin = 0; pin < TOTAL_PINS; pin++) {
        if (IS_PIN_DIGITAL(pin)) {
          Firmata.write((byte) INPUT);
          Firmata.write(1);
          Firmata.write((byte) PIN_MODE_PULLUP);
          Firmata.write(1);
          Firmata.write((byte) OUTPUT);
          Firmata.write(1);
          Firmata.write((byte) PIN_MODE_STEPPER);
          Firmata.write(1);
        }
        if (IS_PIN_ANALOG(pin)) {
          Firmata.write(PIN_MODE_ANALOG);
          Firmata.write(10); // 10 = 10-bit resolution
        }
        if (IS_PIN_PWM(pin)) {
          Firmata.write(PIN_MODE_PWM);
          Firmata.write(DEFAULT_PWM_RESOLUTION);
        }
        if (IS_PIN_DIGITAL(pin)) {
          Firmata.write(PIN_MODE_SERVO);
          Firmata.write(14);
        }
        if (IS_PIN_I2C(pin)) {
          Firmata.write(PIN_MODE_I2C);
          Firmata.write(1);  // TODO: could assign a number to map to SCL or SDA
        }
#ifdef FIRMATA_SERIAL_FEATURE
        serialFeature.handleCapability(pin);
#endif
        Firmata.write(127);
      }
      Firmata.write(END_SYSEX);
      break;
  }
}
