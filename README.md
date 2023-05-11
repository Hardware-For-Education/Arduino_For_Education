# 👋 Arduino for Education 👋
#### 👨‍💻👩‍💻 Proyecto desarrollado por 👨‍💻👩‍💻
* [Alejandro Castro Martínez](https://github.com/kstro96)
* [Janet Chen He](https://github.com/XingYi98)
* [María José Niño Rodríguez](https://github.com/mjninor99)

* [Thomas Morales Varón](https://github.com/Thom037)
* [Juan Diego Sierra Cifuentes](https://github.com/juandisierra10)

* [Sergio Enrique González Martínez](https://github.com/SE-GONZALEZ)
* [Juan José Pinilla Varon](https://github.com/JuanPinilla13)
* [Mateo Felipe Ariza Ordoñez](https://github.com/mateoariza)


#### 👨‍🏫👩‍🏫 Bajo la dirección de 👨‍🏫👩‍🏫 
* Ing. MsC. Martha Cano Morales
* Ing. MsC. PhD. Jairo Alberto Hurtado
* Ing. MsC. Eduardo Mejía Rodríguez

<img src="https://github.com/Hardware-For-Education/.github-private/blob/main/profile/images/scratch4education-small.png" width="200" />

## 🙋‍♀️ Descripción 🙋‍♀️
Proyecto enfocado en el desarrollo de una plataforma hardware que interactúe con el entorno de programación visual Scratch® a través de sensores y elementos de salida, con fines educativos tecnológicos. 

Específicamente este proyecto está enfocado en el código que se ejecuta en el procesador del Arduino UNO (ATMEGA328P) que tiene como objetivo recibir y procesar los mensajes enviados por el Scratch® modificado a además de enviar los valores de los sensores del shield al Scratch® nmodificado a través del protocolo Firmata. 

Proyecto desarrollado en el marco del trabajo de grado como un requisito para optar por el título de ingenier@ electrónic@ de la Pontificia Universidad Javeriana, Bogotá, Colombia en el año 2022 por parte de los integrantes del grupo resaltados anteriormente. 

### 💻 Estructura del repositorio 💻

En este repositorio se encuentran los archivos requeridos para la ejecución del programa en el Arduino UNO. Se estructura a través de una carpeta _Arduino_For_Education_. En esta carpeta se encuentran tres archivos que son los requeridos para el funcionamiento desde el lado del hardware del sistema propuesto. 

* Arduino_For_Education.ino: Es el archivo principal, donde se encuentran distintas funciones y las definiciones de _setup_ y _loop_
* ASCII.h: Es el archivo donde se encuentran las definiciones de cada uno de los caracteres que se pueden imprimir en la pantalla LCD.
* LCD.h: Es el archivo donde se encuentran las definiciones de las distintas funciones para el manejo de la pantalla LCD. 

También dentro de este repositorio se encuentra una carpeta denominada _notes_ donde se pueden encontrar archivos correspondientes a la explicación de ejecución de este programa. 

*Proyecto basado en el desarrollo realizado por [Alan Yoriks](https://github.com/MrYsLabv) en la serie de publicaciones realizadas en su blog [Bots in pieces](https://mryslab.github.io/bots-in-pieces/) bajo el nombre de [Creating a Scratch3 Extension For GPIO Control](https://mryslab.github.io/bots-in-pieces/posts/) en varias partes.*

  * *[Creating a Scratch3 Extension For GPIO Control - Part 1](https://mryslab.github.io/bots-in-pieces/scratch3/gpio/2019/09/15/scratch3-1.html)*
  * *[Creating a Scratch3 Extension For GPIO Control - Part 2 ](https://mryslab.github.io/bots-in-pieces/scratch3/gpio/2019/09/16/scratch3-2.html)*
  * *[Creating a Scratch3 Extension For GPIO Control - Part 3](https://mryslab.github.io/bots-in-pieces/scratch3/gpio/2019/10/03/scratch3-3.html)*
  * *[Creating a Scratch3 Extension For GPIO Control - Part 4](https://mryslab.github.io/bots-in-pieces/scratch3/gpio/2019/10/17/scratch-3-4.html)*
  * *[Scratch 3 Extensions - Part 5 ](https://mryslab.github.io/bots-in-pieces/scratch3/picoboard/circuit-playground-express/2020/02/02/scratch3-5.html)*
