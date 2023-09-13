/*
 * 1_Parpadeo_led.c
 *
 * Created: 15/03/2022 06:25:09 p. m.
 * Author : Diego
1.- PROGRAMA PARA APAGAR Y PRENDER PUERTOS EN EL ARDUINO UNO: ATMEGA328P
Los primeros dos n�meros del Atmega328P significan que el microcontrolador tiene 32 kilobytes de memoria Flash programable.
El tercer n�mero del Atmega328P indica la familia a la que pertenece el microcontrolador.
La letra del final significa el tipo de circuito integrado, en los microcontroladores existen 3 tipos de gamas distintos descritos por la terminaci�n de su 
nombre y donde su �nica diferencia es que, el rango de temperatura en el que opera es distinto:
- Comercial: Rango de temperatura de trabajo bajo.
- Industrial: La P del nombre Atmega328P indica que es un circuito integrado de grado industrial, rango de temperatura de trabajo medio.
- Militar: Rango de temperatura de trabajo alto.
 */


/*DECLARACI�N DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan librer�as) y
antes de declarar nuestras funciones y dem�s partes del c�digo.*/
/*Constante usada con la librer�a delay para definir un reloj de 16 Megahertz para el Arduino, esto afecta los c�lculos en los tiempos de retraso, es de tipo Usigned Long (UL)
y representa la velocidad del microcontrolador. Unsigned Long es porque la variable guarda un n�mero muy grande que es 16X106 o 16 millones*/
#define F_CPU 16000000UL //frecuencia CPU usada en los m�todos de la librer�a delay


//DIRECTIVAS DE C: Es la parte del c�digo donde se indican las librer�as, constantes o variables globales, se encuentra fuera del m�todo main
/*LIBRER�AS O BIBLIOTECAS: C�digo prehecho que contiene m�todos o atributos predefinidos y no contiene m�todo main, adem�s tienen extensi�n .h de header, cuando tiene 
llaves de mayor que o menor que <> es porque busca directamente la librer�a en el compilador de C, osea las incluidas con el lenguaje de programaci�n C y si se pone # include 
con comillas dobles " " es porque se busca una biblioteca propia o local, donde se debe indicar la ruta donde se encuentra dentro del ordenador.*/
/*Librer�a io: Permite utilizar todos los microcontroladores de la familia AVR, osea los Atmega para hacer la conexi�n y control de los puertos, en espec�fico si se 
quiere utilizar solamente el Atmega328P se usa la librer�a <avr/iom328p.h>, llamada input output mega 328P, donde se incluyen todos los registros de funciones especiales 
para controlar los puertos del microcontrolador, pertenece al paquete avr.*/
#include <avr/io.h>
//Librer�a delay: Sirve para hacer el control de tiempos, osea hacer retardos, usa el valor F_CPU para hacer el c�lculo de los retardos, pertenece al paquete util.
#include <util/delay.h>


//DECLARACI�N DE VARIABLES (O CABECERAS DE FUNCI�N) GLOBALES
/*Variable global: El scope indica el �rea donde existe una variable o no, en las variables globales el scope es de alto alcance, osea que se pueden usar en cualquier 
parte del c�digo*/
/*Se indica que una variable ser� de tipo binario cuando al inicio se pone 0b*/
/*Se indica que una variable ser� de tipo hexadecimal cuando al inicio se pone 0x*/
/*Se indica que una variable decimal simplemente poniendo el n�mero*/
int i = 0;
int a = 0B10101010; //binario = 10101010, hexadecimal = AA, decimal = 170
int b = 0X55;		//binario = 01010101, hexadecimal = 55, decimal = 85
int c = 0;			//binario = 00000000, hexadecimal = 00, decimal = 0


//DELARACI�N DE FUNCIONES: Las funciones que se ejecuten varias veces dentro del m�todo main se declaran siempre fuera y antes de �l.


//EJECUTABLE O PRINCIPAL: Contiene el m�todo main a trav�s del cual se ejecuta todo el c�digo
int main(void)
/*Variable local: El scope de este tipo de variables solo existe mientras se est� ejecutando la funci�n, cuando se deje de utilizar, el espacio de memoria donde se 
almacena el valor de la variable se destruye. Adem�s la variable solo puede ser vista por la funci�n que la cre�.*/
{
	//CONFIGURACI�N DE M�QUINA: Es donde se indican los puertos que se van a usar y si estos son de entrada o salida.
	/*PUERTO COMO ENTRADA O SALIDA: Para indicar si los pines de un puerto ser�n entradas o salidas se usa la siguiente nomenclatura en el DDR (Data Direction Register), 
	que es el manejador de puertos, perteneciente a los registros de funciones especiales. Esto se declara en la variable local DDR, que puede ser usada en el puerto 
	B, C o D. Creando as� las variables DDRB (Puerto B), DDRC (Puerto C) o DDRD (Puerto D). Cuando dentro de Microchip Studio alguna palabra se pone de color morado es 
	porque es una palabra reservada, en este caso es porque accede a alguno de los puertos del Atmega328P.*/
	//Puerto como entrada: 0
	//Puerto como salida: 1
	DDRB = 0xFF;//Todos los puertos como salidas
	DDRD = 0xFF;//Todos los puertos como salidas
	
	/*Indicaci�n de los pines de salida, si se declar� como entrada alguno de los pines y se utiliza un 1 en este registro, la entrada llegar� al microcontrolador 
	de manera inversa a la original, si se pone un 0 llegar� de forma normal.*/
	//Los dos pines m�s significativos del Puerto B y C no se pueden usar, en el Puerto D si se puede.
	//El puerto B tiene 6 pines.
	//El puerto C tiene 6 pines.
	//El puerto D tiene 8 pines.
	
	/*PORTB Arduino = 13,		12,		11,		10,		9,		8*/
	
    /*Bucle for: La mayor�a de los c�digos C aplicados en un microcontrolador se deben aplicar dentro de un bucle porque lo que debe realizar es repetitivo*/
    while (1) 
    {
		for (i = 1; i < 256; i++)
		{
			PORTB = a;
			PORTD = b;
			/*Cuando cualquier m�todo de una librer�a empieza con un gui�n bajo es porque est� mandando a llamar un archivo de ensamblador, cuando no empiece con 
			un gui�n bajo el nombre del m�todo es porque est� ejecutando un �rchivo o funci�n hecha en lenguaje C*/
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