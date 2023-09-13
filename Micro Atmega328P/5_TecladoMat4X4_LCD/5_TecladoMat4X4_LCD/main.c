/*
 * 5_TecladoMat4X4_LCD.c
 *
 * Created: 23/03/2022 10:01:22 a. m.
 * Author : diego
 5.- Uso de la librería propia llamada 2_LCD_4bits.h para mostrar en un LCD distintos caracteres.
 */ 


/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
 librerías), pero antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL


//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>							//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h>						//Librería de manejo de tiempos, como retardos principalmente
#include "../3_TecladoMat4X4_Barrido.h"		//Librería propia para el manejo del Teclado matricial 4X4
#include "../2_LCD_4bits.h"					//Librería propia para el manejo del LCD


/*Variable que almacena el valor del teclado matricial 4X4 presionado, es una variable de tipo extern, que se puede utilizar en todos lados del código y que proviene del 
archivo externo (librería) 3_TecladoMat4X4_Barrido.h.*/
extern char VALOR_TECLADO;


//TEXTO_LCD: Función que sirve para imprimir un mensaje inicial en el LCD antes de empezar a presionar el teclado matricial 4X4
void TEXTO_LCD(){
	/*LCD_INICIALIZA: Función para configurar el LCD a que su cursor esté en el inicio, exista una comunicación de 4 bits, disponga de 2 líneas el LCD, sus caracteres sean de
	5X8 pixeles, su cursor sea visible y no parpadee, su incremenento de posición sea hacia la derecha, su pantalla esté estática y se manden datos al LCD, no comandos.*/
	LCD_INICIALIZA();
	
	//LIMPIA_LCD: Función que limpia el LCD, borrando su pantalla y regresando su cursor a su posición inicial.
	LIMPIA_LCD();
	
	//REGRESO_CASA: Función que regresa el cursor a su posición inicial
	REGRESO_CASA();
	
	/*POS_LINEA1: Se refiere a la primera línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD. Puede obviarse la instrucción POS_LINEA1() y funcionará el programa, pero las posiciones de las 
	demás posiciones si se deben indicar explícitamente.*/
	POS_LINEA1(0);
	
	/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como parámetro.*/
	ENVIA_CADENA("PRESS BUTTON AND");
	
	/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
	POS_LINEA2(0);
	
	/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como parámetro.*/
	ENVIA_CADENA("SEE IT ON LCD:");
}


/*MOSTRAR_BOTON_PRESIONADO_EN_LCD: Función que limpia el mensaje inicial impreso en el LCD, retorna el cursor a su posición inicial, envía el dato recopilado después de haber 
sido presionado el teclado matricial y muestra el mensaje en pantalla por un segundo, para finalmente mostrar de nuevo el mensaje inicial.*/
void MOSTRAR_BOTON_PRESIONADO_EN_LCD(){
	//LIMPIA_LCD: Función que limpia el LCD, borrando su pantalla y regresando su cursor a su posición inicial.
	LIMPIA_LCD();
	
	//REGRESO_CASA: Función que regresa el cursor a su posición inicial
	REGRESO_CASA();
	
	/*POS_LINEA1: Se refiere a la primera línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD. Puede obviarse la instrucción POS_LINEA1() y funcionará el programa, pero las posiciones de las 
	demás posiciones si se deben indicar explícitamente.*/
	POS_LINEA1(0);
	
	/*ENVIA_DATO: Función que manda en dos conjuntos de 4 bits (nibble alto y nibble bajo) los 8 bits de datos y a su vez 2 bits de control en los 6 pines del puerto C, 
	indicando así el caracter a mostrar en el LCD, RS = 1; Envío de datos.*/
	ENVIA_DATO(VALOR_TECLADO);
	_delay_ms(1000); //Retraso de 1000 milisegundos después de haber impreso el caracter en pantalla y corrido el cursor hacia la izquierda.
	
	//TEXTO_LCD: Función que sirve para imprimir un mensaje inicial en el LCD antes de empezar a presionar el teclado matricial 4X4
	TEXTO_LCD();
}


int main(void){
	//PROGRAMACIÓN DE LA MÁQUINA: Se indica por medio de los registros de función específica si los pines de los puertos B, C y D serán entradas o salidas.
	//0: Pin como entrada
	//1: Pin como salida
	DDRC = 0xFF;	//Todos los pines del puerto C son tomados como salidas, para mandarse hacia el LCD.
	DDRD = 0xF0;	// Salida nibble alto entradas nibble bajo del teclado.
	
	//Variable que almacena el valor del teclado matricial 4X4 presionado, es una variable de tipo extern, que se puede utilizar en todos lados del código.
	VALOR_TECLADO = 0;
	
	/*LCD_INICIALIZA: Función para configurar el LCD a que su cursor esté en el inicio, exista una comunicación de 4 bits, disponga de 2 líneas el LCD, sus caracteres sean de
	5X8 pixeles, su cursor sea visible y no parpadee, su incremenento de posición sea hacia la derecha, su pantalla esté estática y se manden datos al LCD, no comandos.*/
	LCD_INICIALIZA();
	
	//Inicialización de los caracteres personalizados:
	//Creación del caracter especial 0 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar
	CAR_ESP0();
	//Creación del caracter especial 1 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar
	CAR_ESP1();
	//Creación del caracter especial 2 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar
	CAR_ESP2();
	//Creación del caracter especial 3 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar
	CAR_ESP3();
	
	//LIMPIA_LCD: Función que limpia el LCD, borrando su pantalla y regresando su cursor a su posición inicial.
	LIMPIA_LCD();
	
	//REGRESO_CASA: Función que regresa el cursor a su posición inicial
	REGRESO_CASA();
	
	/*POS_LINEA1: Se refiere a la primera línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD. Puede obviarse la instrucción POS_LINEA1() y funcionará el programa, pero las posiciones de las 
	demás posiciones si se deben indicar explícitamente.*/
	POS_LINEA1(0);
	
	//TEXTO_LCD: Función que sirve para imprimir un mensaje inicial en el LCD antes de empezar a presionar el teclado matricial 4X4
	TEXTO_LCD();

	//Bucle while para checar siempre si algún botón del teclado matricial fue presionado:
	while (1){
		/*BARRE_TECLADO: Función que recorre todas las columnas para ver qué botón del teclado ha sido presionado, recordemos que cuando se dá clic en el botón, al programa le 
		llegan un 0, mientras le llega un 1 cuando todavía no se presiona nada.*/
		BARRE_TECLADO();
		_delay_ms(10); //Retraso de 500 milisegundos después de haber leído el estado de las filas y columnas del teclado matricial.
		/*LEER_TECLADO: Con esta función se lee el estado de las columnas y filas provenientes del teclado matricial para saber cuál de los botones fue presionado en el 
		teclado, con el fin de asociar a cada botón presionado un valor correspondiente, que esté escrito sobre el botón del teclado. Recordemos que realmente obtenemos un 
		cero cuando se dé clic en los botones del teclado matricial.*/
		LEER_TECLADO();
		
		/*El condicional detectará cuando el valor del teclado matricial sea diferente a cero y cuando esto pase, lo que hará es mostrar en pantalla el caracter presionado, 
		para luego devolver el valor de la variable VALOR_TECLADO proveniente de la librería 3_TecladoMat4X4_Barrido.h a 0.*/
		if (VALOR_TECLADO != 0){
			MOSTRAR_BOTON_PRESIONADO_EN_LCD();
			_delay_ms(50);
			VALOR_TECLADO = 0;
		}
	}
}