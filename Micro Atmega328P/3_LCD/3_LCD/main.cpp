/*
 * 3_LCD.c
 *
 * Created: 23/03/2022 10:01:22 a. m.
 * Author : diego
 3.- Uso de la librer�a propia llamada 2_LCD_4bits.h para mostrar en un LCD distintos caracteres.
 */

 /*DECLARACI�N DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
 librer�as), pero antes de declarar nuestras funciones y dem�s. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
 #define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librer�a delay de manejo de tiempos la utiliza

//IMPORTACI�N DE LIBRER�AS, DIRECTIVAS
#include <util/delay.h> //Librer�a de manejo de tiempos, como retardos principalmente
//Importaci�n de librer�a propia y personalizada para el uso de la LCD
 #include "../2_LCD_4bits.h"


//M�TODO MAIN O EJECUTABLE: Desde este m�todo se ejecuta todo el c�digo y es lo que diferenc�a la librer�a de un ejecutable.
 int main(void)
 {
	//PROGRAMACI�N DE LA M�QUINA: Se indica por medio de los registros de funci�n espec�fica si los pines de los puertos B, C y D ser�n entradas o salidas.
	//0: Pin como entrada
	//1: Pin como salida
	DDRC=0xFF; //Todos los pines del puerto C son tomados como salidas, para mandarse hacia el LCD.
	
	/*LCD_INICIALIZA: Funci�n para configurar el LCD a que su cursor est� en el inicio, exista una comunicaci�n de 4 bits, disponga de 2 l�neas el LCD, sus caracteres sean de
	5X8 pixeles, su cursor sea visible y no parpadee, su incremenento de posici�n sea hacia la derecha, su pantalla est� est�tica y se manden datos al LCD, no comandos.*/
	LCD_INICIALIZA();
	
	//LIMPIA_LCD: Funci�n que limpia el LCD, borrando su pantalla y regresando su cursor a su posici�n inicial.
	LIMPIA_LCD();
	
	//Creaci�n del caracter especial 0 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los s�mbolos a mostrar
	CAR_ESP0();
	_delay_ms(100);
	
	//Creaci�n del caracter especial 1 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los s�mbolos a mostrar
	CAR_ESP1();
	_delay_ms(100);
	
	//Creaci�n del caracter especial 2 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los s�mbolos a mostrar
	CAR_ESP2();
	_delay_ms(100);
	
	//Creaci�n del caracter especial 3 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los s�mbolos a mostrar
	CAR_ESP3();
	_delay_ms(100);
	
	//LIMPIA_LCD: Funci�n que limpia el LCD, borrando su pantalla y regresando su cursor a su posici�n inicial.
	LIMPIA_LCD();
	
	//REGRESO_CASA: Funci�n que regresa el cursor a su posici�n inicial
	REGRESO_CASA();
	
	/*POS_LINEA1: Se refiere a la primera l�nea de 16 caracteres que se ve cuando la pantalla est� est�tica o din�mica, en el par�metro de la funci�n se indica despu�s de cual 
	de los 16 caracteres empezar� a imprimir en pantalla el mensaje en el LCD. Puede obviarse la instrucci�n POS_LINEA1() y funcionar� el programa, pero las posiciones de las 
	dem�s posiciones si se deben indicar expl�citamente.*/
	POS_LINEA1(0);
	
	/*ENVIA_CADENA: Funci�n que ejecuta varias veces la funci�n ENVIA_DATO, espec�ficamente hasta que se encuentre con un caracter vac�o, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como par�metro.*/
	ENVIA_CADENA("Diego Cervantes");
	
	/*POS_LINEA2: Se refiere a la segunda l�nea de 16 caracteres que se ve cuando la pantalla est� est�tica o din�mica, en el par�metro de la funci�n se indica despu�s de cual 
	de los 16 caracteres empezar� a imprimir en pantalla el mensaje en el LCD.*/
	POS_LINEA2(0);
	
	/*ENVIA_CADENA: Funci�n que ejecuta varias veces la funci�n ENVIA_DATO, espec�ficamente hasta que se encuentre con un caracter vac�o, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como par�metro.*/
	ENVIA_CADENA("2015340042");	
	
	/*ENVIA_DATO: Funci�n que manda en dos conjuntos de 4 bits (nibble alto y nibble bajo) los 8 bits de datos y a su vez 2 bits de control en los 6 pines del puerto C, 
	indicando as� el caracter a mostrar en el LCD, RS = 1; Env�o de datos.*/
	ENVIA_DATO(' ');
	
	/*ENVIA_CADENA: Funci�n que ejecuta varias veces la funci�n ENVIA_DATO, espec�ficamente hasta que se encuentre con un caracter vac�o, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como par�metro.*/
	ENVIA_CADENA("4MV7");
	
	//animacion: Funci�n para crear una animaci�n personalizada, se debe crear dentro de un bucle while para que se ejecute sin fin la animaci�n
	while(1){
		animacion();
	} //Despu�s de haber terminado el bucle while, no se va a poder imprimir nada m�s, por eso debe ir al final del c�digo.
 }