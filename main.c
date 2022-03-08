/* Ruler 1         2         3         4         5         6         7        */

/********************************  main.c  *********************************/
/*                                                                            */
/*   Proposito: Modulo para la implementación de la comunicacion Firmata      */
/*   entre el programa de Scratch(R) modificado y la plataforma    */
/*   hardware diseñada      */
/*                                                                            */
/*   Origen:  Modulo creado y modificado por el equipo de trabajo desde el  */
/*    8 de Marzo del 2022  */
/*                                                                            */
/*   Notas:   N/A                                                             */
/*                                                                            */
/*   FECHA        RESPONSABLE  COMENTARIO                                     */
/*   -----------------------------------------------------------------------  */
/*   Mar 08/2022  A. Castro  Creación del archivo                          */
/*                                                                            */
/******************************************************************************/

/**************************** Constantes Simbolicas *****************************/

/********************************** Headers ***********************************/

/* --------------------------------- Std Headers ---------------------------- */

/* ----------------------- Inclusion de otros Headers ----------------------- */

/*****************************  Definiciones Globales  *****************************/

typedef enum
{
    CONFIGURANDO,
    ESPERANDO,
    POTENCIOMETRO,
    JOYSTICK_X,
    JOYSTICK_Y,
    JOYSTICK_Z,
    PULSADOR,
    MOTOR_BUZZER,
    MICROFONO,
    LED_RGB,
} STATE_T;

static STATE_T state = CONFIGURANDO;

/*****************************  Funciones Públicas  *****************************/

/******************************** Main Function *******************************/

/*FN****************************************************************************
 *
 *   int
 *   main( );
 *
 *   Proposito:       Función principal del programa que contiene la implemtación
 * de la maquina de estados finitos.
 *
 *   Notas:
 *
 *   Plan:
 *
 *   Registro de revisiones (Debugging):
 *
 *   FECHA         RESPONSABLE   COMENTARIO
 *   -------------------------------------------------------------------------
 *   mAR 08/2022 A. Castro Implementación inicial
 *
 *******************************************************************************/

int main()
{
    while (1)
    {
        switch (state)
        {
        /*................................ Caso CONFIGURANDO ...............................*/
        case CONFIGURANDO:
            configurar();
            state = ESPERANDO;
            break;

        /*................................ Caso ESPERANDO ...............................*/
        case ESPERANDO:
            if (firmata() == "POTENCIOMETRO")
            {
                state = POTENCIOMETRO;
            } // IF POTENCIOMETRO
            else
            {
                if (firmata() == "JOYSTICK_X")
                {
                    state = JOYSTICK_X;
                } // IF JOYSTICK_X
                else
                {
                    if (firmata() == "JOYSTICK_Y")
                    {
                        state = JOYSTICK_Y;
                    } // IF JOYSTICK_Y
                    else
                    {
                        if (firmata() == "JOYSTICK_Z")
                        {
                            state = JOYSTICK_Z;
                        } // IF JOYSTICK_Z
                        else
                        {
                            if (firmata() == "PULSADOR")
                            {
                                state = PULSADOR;
                            } // IF PULSADOR
                            else
                            {
                                if (firmata() == "MOTOR_BUZZER")
                                {
                                    state = MOTOR_BUZZER;
                                } // IF MOTOR_BUZZER
                                else
                                {
                                    if (firmata() == "MICROFONO")
                                    {
                                        state = MICROFONO;
                                    } // IF MICROFONO
                                    else
                                    {
                                        if (firmata() == "LED_RGB")
                                        {
                                            state = LED_RGB;
                                        } // IF LED_RGB
                                        else
                                        {
                                            state = ESPERANDO;
                                        } // ELSE LED_RGB
                                    }     // ELSE MICROFONO
                                }         // ELSE MOTOR_BUZZER
                            }             // ELSE PULSADOR
                        }                 // ELSE JOYSTICK_Z
                    }                     // ELSE JOYSTICK_Y
                }                         // ELSE JOYSTICK_X
            }                             // ELSE POTENCIOMETRO
            break;

        /*................................ Caso POTENCIOMETRO ...............................*/
        case POTENCIOMETRO:
            if (listo_ADC(POTENCIOMETRO))
            {
                enviar_firmata(POTENCIOMETRO, leer_ADC(POTENCIOMETRO));
                state = ESPERANDO;
            }
            else
            {
                state = POTENCIOMETRO;
            }
            break;

        /*................................ Caso JOYSTICK_X ...............................*/
        case JOYSTICK_X:
            if (listo_ADC(JOYSTICK_X))
            {
                enviar_firmata(JOYSTICK_X, leer_ADC(JOYSTICK_X));
                state = ESPERANDO;
            }
            else
            {
                state = JOYSTICK_X;
            }
            break;

        /*................................ Caso JOYSTICK_Y ...............................*/
        case JOYSTICK_Y:
            if (listo_ADC(JOYSTICK_Y))
            {
                enviar_firmata(JOYSTICK_Y, leer_ADC(JOYSTICK_Y));
                state = ESPERANDO;
            }
            else
            {
                state = JOYSTICK_Y;
            }
            break;

        /*................................ Caso JOYSTICK_Z ...............................*/
        case JOYSTICK_Z:
            if (listo_ADC(JOYSTICK_Z))
            {
                enviar_firmata(JOYSTICK_Z, leer_ADC(JOYSTICK_Z));
                state = ESPERANDO;
            }
            else
            {
                state = JOYSTICK_Z;
            }
            break;

        /*................................ Caso PULSADOR ...............................*/
        case PULSADOR:
            enviar_firmata(PULSADOR, leer_digital(PULSADOR));
            state = ESPERANDO;
            break;

        /*................................ Caso MOTOR_BUZZER ...............................*/
        case MOTOR_BUZZER:
            configurar_motor_buzzer(firmata_message);
            state = ESPERANDO;
            break;
        /*................................ Caso MICROFONO ...............................*/
        case MICROFONO:
            if (listo_ADC(MICROFONO))
            {
                enviar_firmata(MICROFONO, leer_ADC(MICROFONO));
                state = ESPERANDO;
            }
            else
            {
                state = MICROFONO;
            }
            break;

        /*................................ Caso LED_RGB ...............................*/
        case LED_RGB:
            configurar_led_rgb(firmata_message);
            state = ESPERANDO;
            break;
        } // END SWITCH
    }     // END WHILE
    return (0);
}