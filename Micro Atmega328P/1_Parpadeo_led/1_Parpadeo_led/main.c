/*
 * 1_Parpadeo_led.c
 *
 * Created: 15/03/2022 06:25:09 p. m.
 * Author : Diego
1.- PROGRAMA PARA APAGAR Y PRENDER PUERTOS EN EL ARDUINO UNO: ATMEGA328P
Los primeros dos números del Atmega328P significan que el microcontrolador tiene 32 kilobytes de memoria Flash programable.
El tercer número del Atmega328P indica la familia a la que pertenece el microcontrolador.
La letra del final significa el tipo de circuito integrado, en los microcontroladores existen 3 tipos de gamas distintos descritos por la terminación de su 
nombre y donde su única diferencia es que, el rango de temperatura en el que opera es distinto:
- Comercial: Rango de temperatura de trabajo bajo.
- Industrial: La P del nombre Atmega328P indica que es un circuito integrado de grado industrial, rango de temperatura de trabajo medio.
- Militar: Rango de temperatura de trabajo alto.
 */


/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan librerías) y
antes de declarar nuestras funciones y demás partes del código.*/
/*Constante usada con la librería delay para definir un reloj de 16 Megahertz para el Arduino, esto afecta los cálculos en los tiempos de retraso, es de tipo Usigned Long (UL)
y representa la velocidad del microcontrolador. Unsigned Long es porque la variable guarda un número muy grande que es 16X106 o 16 millones*/
#define F_CPU 16000000UL //frecuencia CPU usada en los métodos de la librería delay


//DIRECTIVAS DE C: Es la parte del código donde se indican las librerías, constantes o variables globales, se encuentra fuera del método main
/*LIBRERÍAS O BIBLIOTECAS: Código prehecho que contiene métodos o atributos predefinidos y no contiene método main, además tienen extensión .h de header, cuando tiene 
llaves de mayor que o menor que <> es porque busca directamente la librería en el compilador de C, osea las incluidas con el lenguaje de programación C y si se pone # include 
con comillas dobles " " es porque se busca una biblioteca propia o local, donde se debe indicar la ruta donde se encuentra dentro del ordenador.*/
/*Librería io: Permite utilizar todos los microcontroladores de la familia AVR, osea los Atmega para hacer la conexión y control de los puertos, en específico si se 
quiere utilizar solamente el Atmega328P se usa la librería <avr/iom328p.h>, llamada input output mega 328P, donde se incluyen todos los registros de funciones especiales 
para controlar los puertos del microcontrolador, pertenece al paquete avr.*/
#include <avr/io.h>
//Librería delay: Sirve para hacer el control de tiempos, osea hacer retardos, usa el valor F_CPU para hacer el cálculo de los retardos, pertenece al paquete util.
#include <util/delay.h>


//DECLARACIÓN DE VARIABLES (O CABECERAS DE FUNCIÓN) GLOBALES
/*Variable global: El scope indica el área donde existe una variable o no, en las variables globales el scope es de alto alcance, osea que se pueden usar en cualquier 
parte del código*/
/*Se indica que una variable será de tipo binario cuando al inicio se pone 0b*/
/*Se indica que una variable será de tipo hexadecimal cuando al inicio se pone 0x*/
/*Se indica que una variable decimal simplemente poniendo el número*/
int i = 0;
int a = 0B10101010; //binario = 10101010, hexadecimal = AA, decimal = 170
int b = 0X55;		//binario = 01010101, hexadecimal = 55, decimal = 85
int c = 0;			//binario = 00000000, hexadecimal = 00, decimal = 0


//DELARACIÓN DE FUNCIONES: Las funciones que se ejecuten varias veces dentro del método main se declaran siempre fuera y antes de él.


//EJECUTABLE O PRINCIPAL: Contiene el método main a través del cual se ejecuta todo el código
int main(void)
/*Variable local: El scope de este tipo de variables solo existe mientras se esté ejecutando la función, cuando se deje de utilizar, el espacio de memoria donde se 
almacena el valor de la variable se destruye. Además la variable solo puede ser vista por la función que la creó.*/
{
	//CONFIGURACIÓN DE MÁQUINA: Es donde se indican los puertos que se van a usar y si estos son de entrada o salida.
	/*PUERTO COMO ENTRADA O SALIDA: Para indicar si los pines de un puerto serán entradas o salidas se usa la siguiente nomenclatura en el DDR (Data Direction Register), 
	que es el manejador de puertos, perteneciente a los registros de funciones especiales. Esto se declara en la variable local DDR, que puede ser usada en el puerto 
	B, C o D. Creando así las variables DDRB (Puerto B), DDRC (Puerto C) o DDRD (Puerto D). Cuando dentro de Microchip Studio alguna palabra se pone de color morado es 
	porque es una palabra reservada, en este caso es porque accede a alguno de los puertos del Atmega328P.*/
	//Puerto como entrada: 0
	//Puerto como salida: 1
	DDRB = 0xFF;//Todos los puertos como salidas
	DDRD = 0xFF;//Todos los puertos como salidas
	
	/*Indicación de los pines de salida, si se declaró como entrada alguno de los pines y se utiliza un 1 en este registro, la entrada llegará al microcontrolador 
	de manera inversa a la original, si se pone un 0 llegará de forma normal.*/
	//Los dos pines más significativos del Puerto B y C no se pueden usar, en el Puerto D si se puede.
	//El puerto B tiene 6 pines.
	//El puerto C tiene 6 pines.
	//El puerto D tiene 8 pines.
	
	/*PORTB Arduino = 13,		12,		11,		10,		9,		8*/
	
    /*Bucle for: La mayoría de los códigos C aplicados en un microcontrolador se deben aplicar dentro de un bucle porque lo que debe realizar es repetitivo*/
    while (1) 
    {
		for (i = 1; i < 256; i++)
		{
			PORTB = a;
			PORTD = b;
			/*Cuando cualquier método de una librería empieza con un guión bajo es porque está mandando a llamar un archivo de ensamblador, cuando no empiece con 
			un guión bajo el nombre del método es porque está ejecutando un árchivo o función hecha en lenguaje C*/
			//_delay_ms(): Retardo de tiempo indicado en milisegundos, usa la variable F_CPU
			_delay_ms(500);
			PORTB = b;
			PORTD = a;
			_delay_ms(500);
			PORTB = c;
			PORTD = c;
			//_delay_us(): Retardo de tiempo indicado en microsegundos, usa la variable F_CPU.
			_delay_us(100);
		}
    }
}