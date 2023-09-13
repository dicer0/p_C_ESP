/*
 * 3_LCD.c
 *
 * Created: 23/03/2022 10:01:22 a. m.
 * Author : diego
 3.- Uso de la librería propia llamada 2_LCD_4bits.h para mostrar en un LCD distintos caracteres.
 */

 /*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
 librerías), pero antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
 #define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librería delay de manejo de tiempos la utiliza

//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <util/delay.h> //Librería de manejo de tiempos, como retardos principalmente
//Importación de librería propia y personalizada para el uso de la LCD
 #include "../2_LCD_4bits.h"


//MÉTODO MAIN O EJECUTABLE: Desde este método se ejecuta todo el código y es lo que diferencía la librería de un ejecutable.
 int main(void)
 {
	//PROGRAMACIÓN DE LA MÁQUINA: Se indica por medio de los registros de función específica si los pines de los puertos B, C y D serán entradas o salidas.
	//0: Pin como entrada
	//1: Pin como salida
	DDRC=0xFF; //Todos los pines del puerto C son tomados como salidas, para mandarse hacia el LCD.
	
	/*LCD_INICIALIZA: Función para configurar el LCD a que su cursor esté en el inicio, exista una comunicación de 4 bits, disponga de 2 líneas el LCD, sus caracteres sean de
	5X8 pixeles, su cursor sea visible y no parpadee, su incremenento de posición sea hacia la derecha, su pantalla esté estática y se manden datos al LCD, no comandos.*/
	LCD_INICIALIZA();
	
	//LIMPIA_LCD: Función que limpia el LCD, borrando su pantalla y regresando su cursor a su posición inicial.
	LIMPIA_LCD();
	
	//Creación del caracter especial 0 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar
	CAR_ESP0();
	_delay_ms(100);
	
	//Creación del caracter especial 1 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar
	CAR_ESP1();
	_delay_ms(100);
	
	//Creación del caracter especial 2 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar
	CAR_ESP2();
	_delay_ms(100);
	
	//Creación del caracter especial 3 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar
	CAR_ESP3();
	_delay_ms(100);
	
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
	ENVIA_CADENA("Diego Cervantes");
	
	/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
	POS_LINEA2(0);
	
	/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como parámetro.*/
	ENVIA_CADENA("2015340042");	
	
	/*ENVIA_DATO: Función que manda en dos conjuntos de 4 bits (nibble alto y nibble bajo) los 8 bits de datos y a su vez 2 bits de control en los 6 pines del puerto C, 
	indicando así el caracter a mostrar en el LCD, RS = 1; Envío de datos.*/
	ENVIA_DATO(' ');
	
	/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como parámetro.*/
	ENVIA_CADENA("4MV7");
	
	//animacion: Función para crear una animación personalizada, se debe crear dentro de un bucle while para que se ejecute sin fin la animación
	while(1){
		animacion();
	} //Después de haber terminado el bucle while, no se va a poder imprimir nada más, por eso debe ir al final del código.
 }