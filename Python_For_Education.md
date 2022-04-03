# Python For Education 

Programa implementado en Python para la comunicación entre [Scratch For Education]() y [Arduino For Education](). El programa está basado en la implementación realizada por Alan Yorinks en su repositorios se puede encontrar mayor información 
* Scratch 
* Python 
* Arduino 

Para este proyecto se realizan los cambios correspondientes para que el proyecto se ejecute con la versión de Firmata ejecutada desde el IDE de Arduino. [Firmata for Arduino]()

## Modificaciones realizadas
1. Dentro del archivo *pymata_express.py* en la función *_find_arduino* se realiza el cambio para validar el contenido de la respuesta al mensaje **RU_THERE**. 
    * Se encontraba la línea `if len(i_am_here) != 4:` ya que la implementación del FirmataExpress enviaba solo el mensaje de respuesta **I_AM_HERE**. 
    * En la nueva implementación, basada en la libreria Firmata original, se realiza una respuesta un poco más larga ante el mensaje ql cual incluye distintas transmisiones. Esto causa que dicha línea cambie a `if len(i_am_here) != 62:`. 

---
2. Luego de esta sección se procede a realizar una asignación de unas variables para una posterior validación. 
```
if len(i_am_here) == 4:
    var_i_am_here = i_am_here[1]
    var_arduino_instance_id = i_am_here[2]
else:
    var_i_am_here = i_am_here[59]
    var_arduino_instance_id = i_am_here[60]
```
Esta asignación funciona tanto para el programa FirmataExpress como para el programa desarrollado basado en el Firmata. 

---
