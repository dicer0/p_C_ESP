/*
 * 5_TecladoMat4X4_LCD.c
 *
 * Created: 23/03/2022 10:01:22 a. m.
 * Author : diego
 5.- Uso de la librer�a propia llamada 2_LCD_4bits.h para mostrar en un LCD distintos caracteres.
 */ 


/*DECLARACI�N DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
 librer�as), pero antes de declarar nuestras funciones y dem�s. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL


//IMPORTACI�N DE LIBRER�AS, DIRECTIVAS
#include <avr/io.h>							//Librer�a para todo el manejo de puertos y dem�s aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h>						//Librer�a de manejo de tiempos, como retardos principalmente
#include "../3_TecladoMat4X4_Barrido.h"		//Librer�a propia para el manejo del Teclado matricial 4X4
#include "../2_LCD_4bits.h"					//Librer�a propia para el manejo del LCD


/*Variable que almacena el valor del teclado matricial 4X4 presionado, es una variable de tipo extern, que se puede utilizar en todos lados del c�digo y que proviene del 
archivo externo (librer�a) 3_TecladoMat4X4_Barrido.h.*/
extern char VALOR_TECLADO;


//TEXTO_LCD: Funci�n que sirve para imprimir un mensaje inicial en el LCD antes de empezar a presionar el teclado matricial 4X4
void TEXTO_LCD(){
	/*LCD_INICIALIZA: Funci�n para configurar el LCD a que su cursor est� en el inicio, exista una comunicaci�n de 4 bits, disponga de 2 l�neas el LCD, sus caracteres sean de
	5X8 pixeles, su cursor sea visible y no parpadee, su incremenento de posici�n sea hacia la derecha, su pantalla est� est�tica y se manden datos al LCD, no comandos.*/
	LCD_INICIALIZA();
	
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
	ENVIA_CADENA("PRESS BUTTON AND");
	
	/*POS_LINEA2: Se refiere a la segunda l�nea de 16 caracteres que se ve cuando la pantalla est� est�tica o din�mica, en el par�metro de la funci�n se indica despu�s de cual 
	de los 16 caracteres empezar� a imprimir en pantalla el mensaje en el LCD.*/
	POS_LINEA2(0);
	
	/*ENVIA_CADENA: Funci�n que ejecuta varias veces la funci�n ENVIA_DATO, espec�ficamente hasta que se encuentre con un caracter vac�o, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como par�metro.*/
	ENVIA_CADENA("SEE IT ON LCD:");
}


/*MOSTRAR_BOTON_PRESIONADO_EN_LCD: Funci�n que limpia el mensaje inicial impreso en el LCD, retorna el cursor a su posici�n inicial, env�a el dato recopilado despu�s de haber 
sido presionado el teclado matricial y muestra el mensaje en pantalla por un segundo, para finalmente mostrar de nuevo el mensaje inicial.*/
void MOSTRAR_BOTON_PRESIONADO_EN_LCD(){
	//LIMPIA_LCD: Funci�n que limpia el LCD, borrando su pantalla y regresando su cursor a su posici�n inicial.
	LIMPIA_LCD();
	
	//REGRESO_CASA: Funci�n que regresa el cursor a su posici�n inicial
	REGRESO_CASA();
	
	/*POS_LINEA1: Se refiere a la primera l�nea de 16 caracteres que se ve cuando la pantalla est� est�tica o din�mica, en el par�metro de la funci�n se indica despu�s de cual 
	de los 16 caracteres empezar� a imprimir en pantalla el mensaje en el LCD. Puede obviarse la instrucci�n POS_LINEA1() y funcionar� el programa, pero las posiciones de las 
	dem�s posiciones si se deben indicar expl�citamente.*/
	POS_LINEA1(0);
	
	/*ENVIA_DATO: Funci�n que manda en dos conjuntos de 4 bits (nibble alto y nibble bajo) los 8 bits de datos y a su vez 2 bits de control en los 6 pines del puerto C, 
	indicando as� el caracter a mostrar en el LCD, RS = 1; Env�o de datos.*/
	ENVIA_DATO(VALOR_TECLADO);
	_delay_ms(1000); //Retraso de 1000 milisegundos despu�s de haber impreso el caracter en pantalla y corrido el cursor hacia la izquierda.
	
	//TEXTO_LCD: Funci�n que sirve para imprimir un mensaje inicial en el LCD antes de empezar a presionar el teclado matricial 4X4
	TEXTO_LCD();
}


int main(void){
	//PROGRAMACI�N DE LA M�QUINA: Se indica por medio de los registros de funci�n espec�fica si los pines de los puertos B, C y D ser�n entradas o salidas.
	//0: Pin como entrada
	//1: Pin como salida
	DDRC = 0xFF;	//Todos los pines del puerto C son tomados como salidas, para mandarse hacia el LCD.
	DDRD = 0xF0;	// Salida nibble alto entradas nibble bajo del teclado.
	
	//Variable que almacena el valor del teclado matricial 4X4 presionado, es una variable de tipo extern, que se puede utilizar en todos lados del c�digo.
	VALOR_TECLADO = 0;
	
	/*LCD_INICIALIZA: Funci�n para configurar el LCD a que su cursor est� en el inicio, exista una comunicaci�n de 4 bits, disponga de 2 l�neas el LCD, sus caracteres sean de
	5X8 pixeles, su cursor sea visible y no parpadee, su incremenento de posici�n sea hacia la derecha, su pantalla est� est�tica y se manden datos al LCD, no comandos.*/
	LCD_INICIALIZA();
	
	//Inicializaci�n de los caracteres personalizados:
	//Creaci�n del caracter especial 0 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los s�mbolos a mostrar
	CAR_ESP0();
	//Creaci�n del caracter especial 1 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los s�mbolos a mostrar
	CAR_ESP1();
	//Creaci�n del caracter especial 2 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los s�mbolos a mostrar
	CAR_ESP2();
	//Creaci�n del caracter especial 3 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los s�mbolos a mostrar
	CAR_ESP3();
	
	//LIMPIA_LCD: Funci�n que limpia el LCD, borrando su pantalla y regresando su cursor a su posici�n inicial.
	LIMPIA_LCD();
	
	//REGRESO_CASA: Funci�n que regresa el cursor a su posici�n inicial
	REGRESO_CASA();
	
	/*POS_LINEA1: Se refiere a la primera l�nea de 16 caracteres que se ve cuando la pantalla est� est�tica o din�mica, en el par�metro de la funci�n se indica despu�s de cual 
	de los 16 caracteres empezar� a imprimir en pantalla el mensaje en el LCD. Puede obviarse la instrucci�n POS_LINEA1() y funcionar� el programa, pero las posiciones de las 
	dem�s posiciones si se deben indicar expl�citamente.*/
	POS_LINEA1(0);
	
	//TEXTO_LCD: Funci�n que sirve para imprimir un mensaje inicial en el LCD antes de empezar a presionar el teclado matricial 4X4
	TEXTO_LCD();

	//Bucle while para checar siempre si alg�n bot�n del teclado matricial fue presionado:
	while (1){
		/*BARRE_TECLADO: Funci�n que recorre todas las columnas para ver qu� bot�n del teclado ha sido presionado, recordemos que cuando se d� clic en el bot�n, al programa le 
		llegan un 0, mientras le llega un 1 cuando todav�a no se presiona nada.*/
		BARRE_TECLADO();
		_delay_ms(10); //Retraso de 500 milisegundos despu�s de haber le�do el estado de las filas y columnas del teclado matricial.
		/*LEER_TECLADO: Con esta funci�n se lee el estado de las columnas y filas provenientes del teclado matricial para saber cu�l de los botones fue presionado en el 
		teclado, con el fin de asociar a cada bot�n presionado un valor correspondiente, que est� escrito sobre el bot�n del teclado. Recordemos que realmente obtenemos un 
		cero cuando se d� clic en los botones del teclado matricial.*/
		LEER_TECLADO();
		
		/*El condicional detectar� cuando el valor del teclado matricial sea diferente a cero y cuando esto pase, lo que har� es mostrar en pantalla el caracter presionado, 
		para luego devolver el valor de la variable VALOR_TECLADO proveniente de la librer�a 3_TecladoMat4X4_Barrido.h a 0.*/
		if (VALOR_TECLADO != 0){
			MOSTRAR_BOTON_PRESIONADO_EN_LCD();
			_delay_ms(50);
			VALOR_TECLADO = 0;
		}
	}
}