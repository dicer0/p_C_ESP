/*
 * 1_Parpadeo_led_version2.c
 *
 * Created: 21/03/2022 04:13:14 p. m.
 * Author : diego
 */ 

/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan 
librerías), pero antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librería delay de manejo de tiempos la utiliza

//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h> //Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h> //Librería de manejo de tiempos, como retardos principalmente

//DECLARACIÓN DE VARIABLES GLOBALES (CABECERAS DE FUNCIÓN)
int val = 0;	//Variable que almacenará el contenido proveniente del PINB
//int i = 0, val = 0; Se pueden declarar muchas variables también de esta manera

//MÉTODO MAIN O EJECUTABLE: Desde este método se ejecuta todo el código y es lo que diferencía la librería de un ejecutable.
int main(void)
{
    //PROGRAMACIÓN DE LA MÁQUINA: Se indica por medio de los registros de función específica si los pines de los puertos B, C y D serán entradas o salidas.
	//0: Pin como entrada
	//1: Pin como salida
	DDRC = 0x00; //Todos los pines del puerto C son entradas
	DDRD = 0xFF; //Todos los pines del puerto D son salidas
	
	/*Indicación de los pines de salida, si se declaró como entrada alguno de los pines y se utiliza un 1 en este registro, la entrada llegará al microcontrolador 
	de manera inversa a la original (resistencias pull-up), si se pone un 0 llegará de forma normal (resistencias pull-down). Funciona más o menos como una compuerta NOT.*/
	//Los dos pines más significativos del Puerto B y C no se pueden usar, en el Puerto D si se puede.
	//El puerto B tiene 6 pines.
	//El puerto C tiene 6 pines.
	//El puerto D tiene 8 pines.
	PORTC = 0xFF; //La entrada llegará al microcontrolador de manera inversa a la original (Resistencias pull-up)
	
	//Los registros DDR indican si un puerto es entrada o salida, los registros PORT manejan las salidas y los registros PIN reciben las entradas de los puertos B, C y D.
	//DDR dice cual puerto es entrada o salida.
	//PORT saca datos.
	//PIN lee datos recopilados.

    while (1)
    {
		val = PINC;		//Los bits provenientes del puerto C se almacenan en la variable val
		/*Se recomienda en la bibliografía que cuando se hace una asignación al PIN, se le de un tiempo de 2 NOP, osea 2 microsegundos, para que 
		el puerto se regule.*/
		//Cuando se usen delays en las simulaciones, se debe poner un 1 dentro del paréntesis sino se queda mucho tiempo trabado el simulador.
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador
		PORTD = val;	//Los bits guardados en la variable val salen por el puerto D
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador
    }
}