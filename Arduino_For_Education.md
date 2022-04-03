Este archivo presenta la explicación de los distintos componentes del código principal presente en el archivo Arduino_For_Education.ino. 
# Funciones 
---
* **Firmata_config**
    * params:
        * void

Función para realizar la configuración inicial del protocolo Firmata. 

*Funciones usadas en la implementación*

* Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION)
    * Función para establecer la versión del protocolo Firmata utilizada en este proyecto. Para mayor información dirigirse a los archivos [Firmata.h](https://github.com/lstoll/arduino-libraries/blob/master/Firmata/Firmata.h) y [Firmata.cpp](https://github.com/lstoll/arduino-libraries/blob/master/Firmata/Firmata.cpp). 
* Firmata.attach(SET_PIN_MODE, setPinModeCallback)
    * Función donde se registra una función para un tipo de mensaje entrante por medio del protocolo Firmata. Para mayor información dirigirse a los archivos [Firmata.h](https://github.com/lstoll/arduino-libraries/blob/master/Firmata/Firmata.h) y [Firmata.cpp](https://github.com/lstoll/arduino-libraries/blob/master/Firmata/Firmata.cpp).
* 

---
* **setPinModeCallback**
    * params:
        * byte pin: Número del pin a cambiar el modo de operación.
        * int mode: Modo al cual se quiere cambiar. 

El parámetro pin sigue la numeración de pines desde el 0 hasta el 19; siguiendo la siguiente distribución [Playing with Arduino](https://garretlab.web.fc2.com/en/arduino/inside/hardware/arduino/avr/cores/arduino/Arduino.h/digitalPinToPort.html)

El parámetro mode sigue la descripción de las constantes de la implementación de Arduino: 
* INPUT 0x0 
* OUTPUT 0x1 
* INPUT_PULLUP 0x2 

-> [Código fuente Arduino](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h)

*Funciones usadas en la implementación*

* IS_PIN_DIGITAL(pin)
    * Es una macro que se encuentra en el archivo [Boards.h](https://github.com/firmata/arduino/blob/master/Boards.h) de la libreria Firmata para Arduino. En esta función se evalua que el valor enviado como parámetro sea un número entre 2 y 19 inclusive. `((p) >= 2 && (p) <= 19)`
* PIN_TO_DIGITAL(pin)
    * Es una macro que se encuentra en el archivo [Boards.h](https://github.com/firmata/arduino/blob/master/Boards.h) de la libreria Firmata para Arduino. En esta función solo se retorna el valor enviado como parámetro ya que cualquier pin puede ser digital. `(p)`

* pinMode(PIN_TO_DIGITAL(pin), mode)
    * Función implementada en el código fuente del Arduino en el archivo [wiring_digital.c](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/wiring_digital.c) y definida en el [Arduino.h](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h); donde se realiza la configuración del pin como se determina en el parámetro mode. 
---

