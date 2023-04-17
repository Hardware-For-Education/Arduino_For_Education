# 👋 Arduino for Education 👋
#### 👨‍💻👩‍💻 Proyecto desarrollado por 👨‍💻👩‍💻
* [Alejandro Castro Martínez](https://github.com/kstro96)
* [Janet Chen He](https://github.com/XingYi98)
* [María José Niño Rodríguez](https://github.com/mjninor99)

* [Juan Diego Sierra Cifuentes](https://github.com/juandisierra10)
* [prueba](https://github.com/juandisierra10)



#### 👨‍🏫👩‍🏫 Bajo la dirección de 👨‍🏫👩‍🏫 
* Ing. MsC. Martha Cano Morales
* Ing. MsC. PhD. Jairo Alberto Hurtado

<img src="https://github.com/Hardware-For-Education/.github-private/blob/main/profile/images/scratch4education-small.png" width="200" />

## 🙋‍♀️ Descripción 🙋‍♀️
Proyecto enfocado en el desarrollo de una plataforma hardware que interactúe con el entorno de programación visual Scratch® a través de sensores y elementos de salida, con fines educativos tecnológicos. 

Específicamente este proyecto está enfocado en el código que se ejecuta en el procesador del Arduino UNO (ATMEGA328P) que tiene como objetivo recibir y procesar los mensajes enviados por el Scratch® modificado a además de enviar los valores de los sensores del shield al Scratch® nmodificado a través del protocolo Firmata. 

Proyecto desarrollado en el marco del trabajo de grado como un requisito para optar por el título de ingenier@ electrónic@ de la Pontificia Universidad Javeriana, Bogotá, Colombia en el año 2022 por parte de los integrantes del grupo resaltados anteriormente. 

### 💻 Programa Principal 💻

El programa desarrollado en Arduino es codificado bajo la premisa de una máquina de estados finitos (FSM por sus siglas en inglés). En sí se encuentran codificados 3 estados descritos a través del siguiente diagrama de estados. 

<img src="https://github.com/Hardware-For-Education/Arduino_For_Education/blob/main/images/estados.png"/>

En este diagrama se presentan tres estados los cuales cumplen las siguientes funciones: 

Estado 1.	Este es un estado de espera, en el cual se mantendrá mientras no se cumpla un intervalo de tiempo específico o no reciba un mensaje Firmata. Desde este estado se tienen dos posibles transiciones, la que va al Estado 2 que se ejecuta cuando el intervalo de tiempo se cumple y la transición al Estado 3 la cual se ejecuta al recibir un mensaje Firmata desde el PC.

En código este estado toma el nombre de ESPERANDO donde se verifican las dos condiciones de transición en dos condicionales diferentes. 

Estado 2.	En este estado se leen los valores de los diferentes sensores de la placa y se envía al computador a través del protocolo Firmata.

En código este estado toma el nombre de PROCESANDO donde se procesa el mensaje de Firmata y se vuelve al estado de espera. 

Estado 3.	Este estado se ejecuta cuando se recibe un mensaje Firmata y se encarga de ejecutar la configuración requerida por el mensaje Firmata recibido.

en código este estado toma el nombre de ENVIANDO_VALORES donde se envian los valores análogos obtenidos de cada uno de los pines y se vuelve al estado de espera.

#### ⚒ Funcionamiento ⚒

A continuación se explicarán algunas de las funciones dentro del programa principal de Arduino. Las demás funciones que no se explican en este documento no fueron modificadas del programa desarrollado por [Alan Yoriks](https://github.com/MrYsLabv) en [FirmataExpress](https://github.com/MrYsLab/FirmataExpress)

* _Firmata_config_

Es una función en la que se asocian los distintos tipos de mensaje de Firmata a una función de Arduino específica. Esto se hace a través de la función _attach_ del objeto _Firmata_ de Arduino de la siguiente forma: 

```c
Firmata.attach(SET_PIN_MODE, setPinModeCallback);
Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
Firmata.attach(START_SYSEX, sysexCallback);
Firmata.attach(REPORT_ANALOG, reportAnalogCallback);
Firmata.attach(REPORT_DIGITAL, reportDigitalCallback);
Firmata.attach(SYSTEM_RESET, systemResetCallback);
```

También se define la versión del software desarrollado a través de: 

```c
Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
```

Al igual que definir la velocidad de la comunicación a través de Firmata: 


```c
Firmata.begin(115200);
```

Se espera por la apertura de dicho canal de comunicación y se realiza un reseteo de las variables y del programa a través de una función para este fin: 

```c
while (!Serial) {
 ; 
}

systemResetCallback();
```

Y, por último se realiza una inicialización de los valores para la pantalla LCD a través de funciones creadas para este fin en la librería para tal fin. 

```c
LcdInitialise();
LcdClear();
```
* _sysexCallback_

Esta funcion es la que se ejecuta cuando llega un mensaje tipo [_sysex_](https://github.com/firmata/protocol/blob/master/protocol.md#sysex-message-format). En esta función realiza una selección a través de un switch case del comando que se envia luego de que se determine que es un tipo _sysex_.

Cada caso se define a través de las constantes de 8 bits que deben corresponder a las mismas constantes enviadas. Estos son las constantes usadas en esta implementación. 

```c
static const int RU_THERE =   0x51;
static const int LED_RGB =  0x55;
static const int LCD =  0x57;
static const int RECTANGLE_LCD =  0x58;
static const int TRIANGLE_LCD =  0x59;
static const int CIRCLE_LCD =  0x60;
```

#### 📚 Información relevante 📚

*Proyecto basado en el desarrollo realizado por [Alan Yoriks](https://github.com/MrYsLabv) en la serie de publicaciones realizadas en su blog [Bots in pieces](https://mryslab.github.io/bots-in-pieces/) bajo el nombre de [Creating a Scratch3 Extension For GPIO Control](https://mryslab.github.io/bots-in-pieces/posts/) en varias partes.*

  * *[Creating a Scratch3 Extension For GPIO Control - Part 1](https://mryslab.github.io/bots-in-pieces/scratch3/gpio/2019/09/15/scratch3-1.html)*
  * *[Creating a Scratch3 Extension For GPIO Control - Part 2 ](https://mryslab.github.io/bots-in-pieces/scratch3/gpio/2019/09/16/scratch3-2.html)*
  * *[Creating a Scratch3 Extension For GPIO Control - Part 3](https://mryslab.github.io/bots-in-pieces/scratch3/gpio/2019/10/03/scratch3-3.html)*
  * *[Creating a Scratch3 Extension For GPIO Control - Part 4](https://mryslab.github.io/bots-in-pieces/scratch3/gpio/2019/10/17/scratch-3-4.html)*
  * *[Scratch 3 Extensions - Part 5 ](https://mryslab.github.io/bots-in-pieces/scratch3/picoboard/circuit-playground-express/2020/02/02/scratch3-5.html)*

* [Libreria Firmata Arduino](https://docs.arduino.cc/hacking/software/FirmataLibrary): Documentación oficial de la librería de Firmata para Arduino. 

* [Código fuente implementación Firmata](https://github.com/firmata/arduino): Código fuente de la implementación de Firmata para Arduino. También se encuentra explicación de implementaciones del lado del cliente en distintos lenguajes. 
    * En la [Wiki](https://github.com/firmata/arduino/wiki) se encuentran otros enlaces de interés donde se describen las distintas placas de las cuales se encuentran códigos de implementación de Firmata.

* [Protocolo Firmata](https://github.com/firmata/protocol): Repositorio de Github donde se encuentra la base de la documentación del protocolo Firmata.  
