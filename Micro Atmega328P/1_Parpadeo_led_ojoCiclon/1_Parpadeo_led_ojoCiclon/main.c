/*
 * 1_Parpadeo_led_ojoCiclon.c
 *
 * Created: 22/03/2022 01:42:30 p. m.
 * Author : diego
 1.C.- PROGRAMA QUE CREA UN OJO DE CICLÓN, ESTO ES QUE UNA SERIE DE LEDS SE PRENDEN Y SE APAGAN PARA CREAR UN FLUJO DE LEDS EN FORMA CONSECUTIVA: El algoritmo para hacer 
 esto lo que hace es agarrar una variable y darle un valor de 1, multiplicarlo por 2 hasta que su valor llegue a 80, cada vez que llgue a 80, se pone una bandera en 1 
 para indicar que se llegó al 80 y cuando valga esa bandera 1, ya no se multiplicará el número sino que se va a dividir hasta que llegue a cero y cuando alcance el cero 
 la bandera igual volverá a valer cero.
 */ 

/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
librerías), pero antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librería delay de manejo de tiempos la utiliza

//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h> //Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h> //Librería de manejo de tiempos, como retardos principalmente

//DECLARACIÓN DE VARIABLES GLOBALES (CABECERAS DE FUNCIÓN)
int x = 0; //Variable a usarse en el bucle for cuando d = 1
int b = 0; //Variable a usarse en el bucle for cuando d > 127
int d = 1; //Se usa para ver el estado de la operación

int main(void)
{
    //PROGRAMACIÓN DE LA MÁQUINA: En este se indica por medio de los registros de función específica, los pines de los puertos B, C y D que serán entradas o salidas.
    //0: Pin como entrada
    //1: Pin como salida
    DDRB = 0xFF; //Todos los pines del puerto B son salidas
    DDRD = 0xFF; //Todos los pines del puerto D son salidas
	
	/*1.C.- PROGRAMA QUE CREA UN OJO DE CICLÓN, ESTO ES QUE UNA SERIE DE LEDS SE PRENDEN Y SE APAGAN PARA CREAR UN FLUJO DE LEDS EN FORMA CONSECUTIVA: El algoritmo para hacer
	esto lo que hace es agarrar una variable y darle un valor de 1, multiplicarlo por 2 hasta que su valor llegue a 80, cada vez que llgue a 80, se pone una bandera en 1
	para indicar que se llegó al 80 y cuando valga esa bandera 1, ya no se multiplicará el número sino que se va a dividir hasta que llegue a cero y cuando alcance el cero
	la bandera igual volverá a valer 0.*/
    while (1)
    {
		if (d == 1){
			for (x = 0; x < 8; x++){
				PORTD = d;
				d = d*2;
				_delay_ms(250);
			}
		} 
		else if (d > 127){
			for (b = 8; b > 0; b--){
				d = d/2;
				PORTD = d;
				_delay_ms(250);
			}
		}
    }
}