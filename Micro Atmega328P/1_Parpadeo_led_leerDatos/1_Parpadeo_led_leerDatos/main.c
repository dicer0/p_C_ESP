/*
 * 1_Parpadeo_led_version2.c
 *
 * Created: 21/03/2022 04:13:14 p. m.
 * Author : diego
 */ 

/*DECLARACI�N DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan 
librer�as), pero antes de declarar nuestras funciones y dem�s. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librer�a delay de manejo de tiempos la utiliza

//IMPORTACI�N DE LIBRER�AS, DIRECTIVAS
#include <avr/io.h> //Librer�a para todo el manejo de puertos y dem�s aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h> //Librer�a de manejo de tiempos, como retardos principalmente

//DECLARACI�N DE VARIABLES GLOBALES (CABECERAS DE FUNCI�N)
int val = 0;	//Variable que almacenar� el contenido proveniente del PINB
//int i = 0, val = 0; Se pueden declarar muchas variables tambi�n de esta manera

//M�TODO MAIN O EJECUTABLE: Desde este m�todo se ejecuta todo el c�digo y es lo que diferenc�a la librer�a de un ejecutable.
int main(void)
{
    //PROGRAMACI�N DE LA M�QUINA: Se indica por medio de los registros de funci�n espec�fica si los pines de los puertos B, C y D ser�n entradas o salidas.
	//0: Pin como entrada
	//1: Pin como salida
	DDRC = 0x00; //Todos los pines del puerto C son entradas
	DDRD = 0xFF; //Todos los pines del puerto D son salidas
	
	/*Indicaci�n de los pines de salida, si se declar� como entrada alguno de los pines y se utiliza un 1 en este registro, la entrada llegar� al microcontrolador 
	de manera inversa a la original (resistencias pull-up), si se pone un 0 llegar� de forma normal (resistencias pull-down). Funciona m�s o menos como una compuerta NOT.*/
	//Los dos pines m�s significativos del Puerto B y C no se pueden usar, en el Puerto D si se puede.
	//El puerto B tiene 6 pines.
	//El puerto C tiene 6 pines.
	//El puerto D tiene 8 pines.
	PORTC = 0xFF; //La entrada llegar� al microcontrolador de manera inversa a la original (Resistencias pull-up)
	
	//Los registros DDR indican si un puerto es entrada o salida, los registros PORT manejan las salidas y los registros PIN reciben las entradas de los puertos B, C y D.
	//DDR dice cual puerto es entrada o salida.
	//PORT saca datos.
	//PIN lee datos recopilados.

    while (1)
    {
		val = PINC;		//Los bits provenientes del puerto C se almacenan en la variable val
		/*Se recomienda en la bibliograf�a que cuando se hace una asignaci�n al PIN, se le de un tiempo de 2 NOP, osea 2 microsegundos, para que 
		el puerto se regule.*/
		//Cuando se usen delays en las simulaciones, se debe poner un 1 dentro del par�ntesis sino se queda mucho tiempo trabado el simulador.
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador
		PORTD = val;	//Los bits guardados en la variable val salen por el puerto D
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador
    }
}