/* LIBRERIAS */
#include <Firmata.h>
#include "LCD.h"
/* 
 *  Comandos para identificacion en los mensajes SYSEX de Firmata para Hardware Para Educacion 
 *  Deben ser iguales a los contenidos en el archivo private_constants.py en el proyecto Python_For_Education
 */
static const int RU_THERE =   0x51;
static const int I_AM_HERE =  0x52;
static const int LED_RGB =  0x55;
static const int LCD =  0x57;
static const int ACC = 0x58;
/*
 * Identificador de la instancia de Arduino
 */
#define ARDUINO_INSTANCE_ID 1

// the minimum interval for sampling analog input
#define MINIMUM_SAMPLING_INTERVAL 1

#define INTER_PING_INTERVAL 40 // 40 ms.
// Ping variables
int numLoops = 0;

/* analog inputs */
int analogInputsToReport = 0; // bitwise array to store pin reporting

boolean isResetting = false;
/* digital input ports */
byte reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
byte previousPINs[TOTAL_PORTS];     // previous 8 bits sent
byte portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else

/* Variables para control de tiempo */
unsigned long currentMillis = 0;        // Guarda valor actual en millis()
unsigned long previousMillis = 0;       // Para comparar con la variable de tiempo actual.
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
  switch (estado) {
    case ESPERANDO:
      /* DIGITALREAD - as fast as possible, check for changes and output them to the
         FTDI buffer using Serial.print()  */
      checkDigitalInputs();
      currentMillis = millis();
      if (Firmata.available()) {
        estado = PROCESANDO;
        break;
      }
      if (currentMillis - previousMillis > samplingInterval) {
        estado = ENVIANDO_VALORES;
        break;
      }
      break;
    case PROCESANDO:
      Firmata.processInput();
      estado = ESPERANDO;
      break;
    case ENVIANDO_VALORES:
      previousMillis += samplingInterval;
      byte analogPin;
      /* ANALOGREAD - do all analogReads() at the configured sampling interval */
      for (byte pin = 0; pin < TOTAL_PINS; pin++) {
        if (IS_PIN_ANALOG(pin) && Firmata.getPinMode(pin) == PIN_MODE_ANALOG) {
          analogPin = PIN_TO_ANALOG(pin);
          if (analogInputsToReport & (1 << analogPin)) {
            Firmata.sendAnalog(analogPin, analogRead(analogPin));
          }
        }
      }
      estado = ESPERANDO;
      break;
    default:
      break;
  }
}

/*
   Firmata_config()
   Función de configuración inicial del Firmata.
*/
void Firmata_config() {
  Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);

  Firmata.attach(SET_PIN_MODE, setPinModeCallback);
  Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  Firmata.attach(START_SYSEX, sysexCallback);

  Firmata.attach(REPORT_ANALOG, reportAnalogCallback);
  Firmata.attach(REPORT_DIGITAL, reportDigitalCallback);

  Firmata.attach(SYSTEM_RESET, systemResetCallback);

  Firmata.begin(115200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for ATmega32u4-based boards and Arduino 101
  }

  systemResetCallback();

  LcdInitialise();
  LcdClear();
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

  if (Firmata.getPinMode(pin) == PIN_MODE_IGNORE)
    return;

  if (IS_PIN_ANALOG(pin)) {
    reportAnalogCallback(PIN_TO_ANALOG(pin), mode == PIN_MODE_ANALOG ? 1 : 0); // turn on/off reporting
  }
  if (IS_PIN_DIGITAL(pin)) {
    if (mode == INPUT || mode == PIN_MODE_PULLUP) {
      portConfigInputs[pin / 8] |= (1 << (pin & 7));
    } else {
      portConfigInputs[pin / 8] &= ~(1 << (pin & 7));
    }
  }
  Firmata.setPinState(pin, 0);
  switch (mode) {
    case PIN_MODE_ANALOG:
      if (IS_PIN_ANALOG(pin)) {
        if (IS_PIN_DIGITAL(pin)) {
          pinMode(PIN_TO_DIGITAL(pin), INPUT);    // disable output driver
#if ARDUINO <= 100
          // deprecated since Arduino 1.0.1 - TODO: drop support in Firmata 2.6
          digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable internal pull-ups
#endif
        }
        Firmata.setPinMode(pin, PIN_MODE_ANALOG);
      }
      break;
    case INPUT:
      if (IS_PIN_DIGITAL(pin)) {
        pinMode(PIN_TO_DIGITAL(pin), INPUT);    // disable output driver
#if ARDUINO <= 100
        // deprecated since Arduino 1.0.1 - TODO: drop support in Firmata 2.6
        digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable internal pull-ups
#endif
        Firmata.setPinMode(pin, INPUT);
      }
      break;
    case PIN_MODE_PULLUP:
      if (IS_PIN_DIGITAL(pin)) {
        pinMode(PIN_TO_DIGITAL(pin), INPUT_PULLUP);
        Firmata.setPinMode(pin, PIN_MODE_PULLUP);
        Firmata.setPinState(pin, 1);
      }
      break;
    case OUTPUT:
      if (IS_PIN_DIGITAL(pin)) {
        if (Firmata.getPinMode(pin) == PIN_MODE_PWM) {
          // Disable PWM if pin mode was previously set to PWM.
          digitalWrite(PIN_TO_DIGITAL(pin), LOW);
        }
        pinMode(PIN_TO_DIGITAL(pin), OUTPUT);
        Firmata.setPinMode(pin, OUTPUT);
      }
      break;
    case PIN_MODE_PWM:
      if (IS_PIN_PWM(pin)) {
        pinMode(PIN_TO_PWM(pin), OUTPUT);
        analogWrite(PIN_TO_PWM(pin), 0);
        Firmata.setPinMode(pin, PIN_MODE_PWM);
      }
      break;
    case PIN_MODE_SERIAL:
#ifdef FIRMATA_SERIAL_FEATURE
      serialFeature.handlePinMode(pin, PIN_MODE_SERIAL);
#endif
      break;
    default:
      Firmata.sendString("Unknown pin mode"); // TODO: put error msgs in EEPROM
      break;
  }
  // TODO: save status to EEPROM here, if changed
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
void reportAnalogCallback(byte analogPin, int value) {
  if (analogPin < TOTAL_ANALOG_PINS) {
    if (value == 0) {
      analogInputsToReport = analogInputsToReport & ~(1 << analogPin);
    } else {
      analogInputsToReport = analogInputsToReport | (1 << analogPin);
      // prevent during system reset or all analog pin values will be reported
      // which may report noise for unconnected analog pins
      if (!isResetting) {
        // Send pin value immediately. This is helpful when connected via
        // ethernet, wi-fi or bluetooth so pin states can be known upon
        // reconnecting.
        Firmata.sendAnalog(analogPin, analogRead(analogPin));
      }
    }
  }
  // TODO: save status to EEPROM here, if changed
}
/*

*/
void reportDigitalCallback(byte port, int value) {
  if (port < TOTAL_PORTS) {
    reportPINs[port] = (byte) value;
    // Send port value immediately. This is helpful when connected via
    // ethernet, wi-fi or bluetooth so pin states can be known upon
    // reconnecting.
    if (value) outputPort(port, readPort(port, portConfigInputs[port]), true);
  }
  // do not disable analog reporting on these 8 pins, to allow some
  // pins used for digital, others analog.  Instead, allow both types
  // of reporting to be enabled, but check if the pin is configured
  // as analog when sampling the analog inputs.  Likewise, while
  // scanning digital pins, portConfigInputs will mask off values from any
  // pins configured as analog
}
/*

*/
void sysexCallback(byte command, byte argc, byte *argv) {
  switch (command) {
    case LCD:
      if(argc == 0){
        LcdClear();
      }else{
        char string_to_write[14] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
        for(int i = 1; i < argc; i++){
          string_to_write[i-1] = argv[i];
        }
        write_line(string_to_write, argv[0]);
      }
      break;
    case LED_RGB:
      if (argc == 3){
        digitalWrite(2,int(argv[0]));
        digitalWrite(3,int(argv[1]));
        digitalWrite(4,int(argv[2]));
      }else{
        if(argc == 0){
          pinMode(2,OUTPUT);
          pinMode(3,OUTPUT);
          pinMode(4,OUTPUT);
        }else{
         //Firmata.sendString("Not enough data"); 
        }
      }
      break;
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
/*

*/
void outputPort(byte portNumber, byte portValue, byte forceSend) {
  // pins not configured as INPUT are cleared to zeros
  portValue = portValue & portConfigInputs[portNumber];
  // only send if the value is different than previously sent
  if (forceSend || previousPINs[portNumber] != portValue) {
    Firmata.sendDigitalPort(portNumber, portValue);
    previousPINs[portNumber] = portValue;
  }
}
/*

*/
/* -----------------------------------------------------------------------------
  check all the active digital inputs for change of state, then add any events
  to the Serial output queue using Serial.print() */
void checkDigitalInputs(void) {
  /* Using non-looping code allows constants to be given to readPort().
     The compiler will apply substantial optimizations if the inputs
     to readPort() are compile-time constants. */
  if (TOTAL_PORTS > 0 && reportPINs[0]) outputPort(0, readPort(0, portConfigInputs[0]), false);
  if (TOTAL_PORTS > 1 && reportPINs[1]) outputPort(1, readPort(1, portConfigInputs[1]), false);
  if (TOTAL_PORTS > 2 && reportPINs[2]) outputPort(2, readPort(2, portConfigInputs[2]), false);
  if (TOTAL_PORTS > 3 && reportPINs[3]) outputPort(3, readPort(3, portConfigInputs[3]), false);
  if (TOTAL_PORTS > 4 && reportPINs[4]) outputPort(4, readPort(4, portConfigInputs[4]), false);
  if (TOTAL_PORTS > 5 && reportPINs[5]) outputPort(5, readPort(5, portConfigInputs[5]), false);
  if (TOTAL_PORTS > 6 && reportPINs[6]) outputPort(6, readPort(6, portConfigInputs[6]), false);
  if (TOTAL_PORTS > 7 && reportPINs[7]) outputPort(7, readPort(7, portConfigInputs[7]), false);
  if (TOTAL_PORTS > 8 && reportPINs[8]) outputPort(8, readPort(8, portConfigInputs[8]), false);
  if (TOTAL_PORTS > 9 && reportPINs[9]) outputPort(9, readPort(9, portConfigInputs[9]), false);
  if (TOTAL_PORTS > 10 && reportPINs[10]) outputPort(10, readPort(10, portConfigInputs[10]), false);
  if (TOTAL_PORTS > 11 && reportPINs[11]) outputPort(11, readPort(11, portConfigInputs[11]), false);
  if (TOTAL_PORTS > 12 && reportPINs[12]) outputPort(12, readPort(12, portConfigInputs[12]), false);
  if (TOTAL_PORTS > 13 && reportPINs[13]) outputPort(13, readPort(13, portConfigInputs[13]), false);
  if (TOTAL_PORTS > 14 && reportPINs[14]) outputPort(14, readPort(14, portConfigInputs[14]), false);
  if (TOTAL_PORTS > 15 && reportPINs[15]) outputPort(15, readPort(15, portConfigInputs[15]), false);
}

/*
 * 
 */
 void systemResetCallback() {
    isResetting = true;

    // initialize a defalt state
    // TODO: option to load config from EEPROM instead of default

#ifdef FIRMATA_SERIAL_FEATURE
    serialFeature.reset();
#endif

    for (byte i = 0; i < TOTAL_PORTS; i++) {
        reportPINs[i] = false;    // by default, reporting off
        portConfigInputs[i] = 0;  // until activated
        previousPINs[i] = 0;
    }

    for (byte i = 0; i < TOTAL_PINS; i++) {
        // pins with analog capability default to analog input
        // otherwise, pins default to digital output
        if (IS_PIN_ANALOG(i)) {
            // turns off pullup, configures everything
            setPinModeCallback(i, PIN_MODE_ANALOG);
        }
        else {
            // sets the output to 0, configures portConfigInputs
            setPinModeCallback(i, OUTPUT);
        }
    }
    // by default, do not report any analog inputs
    analogInputsToReport = 0;
    isResetting = false;
}
