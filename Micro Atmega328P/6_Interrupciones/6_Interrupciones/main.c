/*
 * 6_Interrupciones.c
 *
 * Created: 18/05/2022 04:46:10 p. m.
 * Author : diego
 6.- INTERRUPCIONES: Interrupción se refiere a una acción del microcontrolador que ocurre solamente cuando sucede un evento externo, no importando la parte del código que se 
 esté ejecutando en el momento, interrumpiendo así la ejecución del programa como lo indica su nombre; estas se ejecutan de forma asíncrona a diferencia de las subrutinas que 
 son síncronas y no se detonan por eventos externos.
 Las interrupciones externas solamente se encuentran en los pines 3 (PD2) y 4 (PD3) del Puerto D, pero cualquier pin de los puertos B, C y D se puede configurar para que 
 funcione como detonantes de interrupciones, esto se hace a partir de los siguientes registros:
	- Puerto B, Tiene 6 pines disponibles:
	PCINT0:		Pin 8	(PB0)
	PCINT1:		Pin 9	(PB1)
	PCINT2:		Pin 10	(PB2)
	PCINT3:		Pin 11	(PB3)
	PCINT4:		Pin 12	(PB4)
	PCINT5:		Pin 13	(PB5)
	- Puerto C, Tiene 6 pines disponibles:
	PCINT8:		Pin A0	(PC0)
	PCINT9:		Pin A1	(PC1)
	PCINT10:	Pin A2	(PC2)
	PCINT11:	Pin A3	(PC3)
	PCINT12:	Pin A4	(PC4)
	PCINT13:	Pin A5	(PC5)
	- Puerto D, Tiene 8 pines disponibles:
	PCINT16:	Pin 0	(PD0)
	PCINT17:	Pin 1	(PD1)
	PCINT18:	Pin 2	(PD2); INT0, Interrupción externa 0
	PCINT19:	Pin 3	(PD3); INT1, Interrupción externa 1
	PCINT20:	Pin 4	(PD4)
	PCINT21:	Pin 5	(PD5)
	PCINT22:	Pin 6	(PD6)
	PCINT23:	Pin 7	(PD7)
Los pines que se vuelvan interrupciones dentro de un mismo puerto, ejecutarán el mismo ISR, ya que solo se recibe un vector de interrupciones por cada puerto B, C o D.
Recordemos que en el proyecto se usa para las siguientes funciones los puertos:
PUERTO C - LCD:
Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada, por lo tanto solamente 6 bits en total contando los 2 de control son los que se conectan y son
los siguientes:
Arduino:	A5,		A4,		A3,		A2,		A1,		A0
LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

PUERTO D - TECLADO MATRICIAL 4X4: Aunque en este caso no se va a conectar, ya que se quiere usar los pines PD2 y PD3 para las interrupciones externas INT0 e INT1.
La asignación de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexión del teclado matricial vistos de izquierda
a derecha son los siguientes:
Puerto D Arduino (Digital PWM):			7,		6,		5,		4,		3,		2,		1,		0
Constantes del teclado matricial 4X4:	F1,		F2,		F3,		F4,		C1,		C2,		C3,		C4
Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8

PUERTO B - DISPOSITIVOS EXTERNOS VARIADOS: Leds, botones, etc.
En este puerto se activan dos interrupciones por cambio de PIN, osea un PCINT o PCI (Pin Change).

En total se cuenta con 4 interrupciones: INT0, INT1 y 2 PCIB (Pin Change del puerto B), al presionar alguna de las interrupciones, se imprimirá un mensaje en el LDC:
INT0	(Pin 2 del Puerto D): Se imprime en pantalla nuestro nombre + 1 caracter especial.
INT1	(Pin 3 del Puerto D): Se imprime en pantalla nuestro apellido + 1 caracter especial distinto.
PCINT0	(Pin 8 del Puerto B): Se imprime en pantalla UPIITA + 1 caracter especial.
PCINT1	(Pin 9 del Puerto B): Se imprime en pantalla otro mensaje + 1 caracter especial.*/


/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
librerías), pero antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librería delay de manejo de tiempos la utiliza


//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>			//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h>		//Librería de manejo de tiempos, como retardos principalmente
#include <avr/interrupt.h>	//Librería para el manejo de los registros de interrupciones
#include "../2_LCD_4bits.h"	//Librería propia para el manejo del LCD


//DECLARACIÓN DE VARIABLES GLOBALES (CABECERAS DE FUNCIÓN):
//ALTERNACIÓN DE ENCENDIDO Y APAGADO DE LEDS: Tarea ejecutada por interrupción.
int a = 0xD7;		//0XD7 = 1101 0111; Esto sirve para prender y apagar leds en el puerto B turnandose entre ellos.
int b = 0xEB;		//0XEB = 1110 1011; Esto sirve para prender y apagar leds en el puerto B turnandose entre ellos.
int c = 0x00;		//0X00 = 0000 0000; Esto sirve para apagar todos los leds en el puerto B.



//MENSAJE_INICIAL: Función normal para mostrar un mensaje inicial después o antes de que se haya presionado una interrupción.
void MENSAJE_INICIAL(){
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
	letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
	ENVIA_CADENA("Presiona una");
	/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
	POS_LINEA2(0);
	/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
	ENVIA_CADENA("Interrupcion ");
	//REGRESO_CASA: Función que regresa el cursor a su posición inicial
	REGRESO_CASA();
}



//Funciones ISR: Tareas ejecutadas por las interrupciones externas y PCI (Pin Change).
/*ISR (INT0_vect): Se le llama ISR o Interruption Service Routine a la función que describa lo que se tiene que hacer cuando sea activada una interrupción, la palabra ISR es 
una palabra reservada de interrupciones y el nombre de esta función no debe ser decladada en ningún lado del código, simplemente por haber sido nombrada como ISR, se ejecuta 
cuando sea detonada la interrupción que recibe como parámetro, para ello la interrupción INT0 ya debe haber sido configurada dentro del método main. Para ello se le pasa como 
parámetro a la función el vector INT0_vect para que ejecute la acción cuando la interrupción externa INT0 sea detonada y se debe hacer una función ISR aparte para manejar la 
acción que detona la interrupción externa INT1.*/
/*Las acciones de las interrupciones externas deben ser descritas en una función fuera del método main, estas reciben como parámetro lo siguiente:
	- INT0_vect: Interrupción externa 0 después de haber sido configurada con los registros EIMSK y EICRA, esta está asociada al pin 2 (PD2) del Puerto D en el Arduino
	- INT1_vect: Interrupción externa 1 después de haber sido configurada con los registros EIMSK y EICRA, esta está asociada al pin 3 (PD3) del Puerto D en el Arduino.*/
//ISR INTERRUPCIONES EXTERNAS:
//INTERRUPCIÓN EXTERNA INT0 DEL PUERTO D; INT0	(Pin 2 del Puerto D): Se imprime en pantalla nuestro nombre + 1 caracter especial.
ISR(INT0_vect){
	_delay_ms(750); //Retraso antirebote
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
	letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
	ENVIA_CADENA("Nombre: Diego");
	/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
	POS_LINEA2(0);
	/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
	ENVIA_CADENA("Caracter: ");
	//Se manda el caracter 4 a la función ENVIA_DATO() para que se muestre en la pantalla del LCD
	ENVIA_DATO(3); //Impresión del caracter persionalizado 4.- CARACTER PERSONALIZADO DE TÍO COSA
	_delay_ms(500); //Retraso de 10 milisegundos para evitar el rebote
	MENSAJE_INICIAL();
	
}
//INTERRUPCIÓN EXTERNA INT1 DEL PUERTO D: INT1	(Pin 3 del Puerto D): Se imprime en pantalla nuestro apellido + 1 caracter especial distinto.
ISR(INT1_vect){
	_delay_ms(750); //Retraso antirebote
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
	letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
	ENVIA_CADENA("Apell: Cervantes");
	/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
	POS_LINEA2(0);
	/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
	ENVIA_CADENA("Caracter: ");
	//Se manda el caracter 2 a la función ENVIA_DATO() para que se muestre en la pantalla del LCD
	ENVIA_DATO(1); //Impresión del caracter persionalizado 2.- CARACTER PERSONALIZADO DE SONRISA
	_delay_ms(500); //Retraso de 10 milisegundos para evitar el rebote
	MENSAJE_INICIAL();
}

//ISR INTERRUPCIONES PIN CHANGE DE LOS PUERTOS B, C o D:
/*Las acciones de las interrupciones PCI (Pin Change) deben ser descritas en una función fuera del método main, estas reciben como parámetro lo siguiente:
	- PCINT0_vect: Detecta una interrupción PCIB, osea cuando cualquier pin del puerto B que haya sido puesto como pin de interrupción, detecte algún cambio.
	- PCINT1_vect: Detecta una interrupción PCIC, osea cuando cualquier pin del puerto C que haya sido puesto como pin de interrupción, detecte algún cambio.
	- PCINT2_vect: Detecta una interrupción PCID, osea cuando cualquier pin del puerto D que haya sido puesto como pin de interrupción, detecte algún cambio.
Todos los pines de interrupción que pertenezcan a un mismo puerto ejecutarán la misma ISR para llevar a cabo su función, aunque sean pines distintos.*/
//INTERRUPCIÓN PCI PCINT0 DEL PUERTO B: PCINT0	(Pin 8 del Puerto B) y PCINT1	(Pin 9 del Puerto B): Se imprime en pantalla un mensaje + 1 caracter especial.
ISR(PCINT0_vect){
	_delay_ms(750); //Retraso antirebote
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
	letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
	ENVIA_CADENA("Ironman");
	/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
	POS_LINEA2(0);
	/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada 
	letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
	ENVIA_CADENA("Reactor arc: ");
	//Se manda el caracter 1 a la función ENVIA_DATO() para que se muestre en la pantalla del LCD
	ENVIA_DATO(0); //Impresión del caracter persionalizado 1.- CARACTER PERSONALIZADO DE REACTOR ARC
	_delay_ms(500); //Retraso de 10 milisegundos para evitar el rebote
	MENSAJE_INICIAL();
}



//MÉTODO MAIN O EJECUTABLE: Desde este método se ejecuta todo el código y es lo que diferencía la librería de un ejecutable.
/*Para configurar correctamente un programa que use interrupciones se utiliza la siguiente arquitectura en el programa:
	- CLI: Activación de interrupciones con el bit GI con valor 0 lógico (MI).
	- Programación de máquina y configuración de interrupciones con los registros EIMSK (máscara de interrupciones) y EICRA (registro de control de interrupciones).
	- SEI: Desactivación de interrupciones con el bit GI con valor 1 lógico (NMI).
	- Ejecución del programa.
Con ella nos aseguramos de manejar propiamente los pines de los puertos B, C y D que manejan las interrupciones.*/
int main(void){
	/*CLI: Cuando se activa una CLI interruption, automáticamente el bit 7 del status register que es GI (General Interruption) se convierte en 0 lógico, lo que convierte a 
	las interrupciones en MI (Maskable Interrupt), esto en términos más simples significa que las interrupciones están activas:
		- Cuando GI tiene valor de 1 lógico no está activada ninguna interrupción, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 lógico está activada la interrupción, osea que es MI, Maskable Interrupt.*/
	cli(); //Evita cualquier interrupción que entre mientras se está ejecutando ya una o antes de programar la máquina, ya que termine, permite la ejecución de interrupciones.
	
	
	
	//PROGRAMACIÓN DE LA MÁQUINA: Se indica por medio de los registros de función específica si los pines de los puertos B, C y D serán entradas o salidas.
	//0: Pin como entrada
	//1: Pin como salida
	DDRB = 0xFC; //0xFC = 1111 1100; Algunos pines del puerto B son entradas y otros salidas. Solo son entradas los que activen interrupciones.
	DDRC = 0xFF; //Todos los pines del puerto C son salidas.
	DDRD = 0x00; //Todos los pines del puerto D son entradas.
	
	//Programación de máquina y configuración de interrupciones con los registros EIMSK y EICRA, además de declaraciones de PCI.
	//INT0 e INT1: Están asociados a los pines 2 y 3 del Puerto D solamente.
	//EIMSK o máscara de interrupciones: Sirve para indicar qué interrupción externa es la que quiero emplear.
	/*Las interrupciones están asociadas a ciertos pines del puerto D solamente en el microcontrolador ATMEGA328P:
	La constante INT0 se refiere a la interrupción externa asociada al pin 2 (PD2) del Puerto D en el Arduino.
	La constante INT1 se refiere a la interrupción externa asociada al pin 3 (PD3) del Puerto D en el Arduino.
		Las constantes INT0 e INT1 son bits parte del registro EIMSK, con 1 activo la interrupción externa y con 0 la desactivo.
		-	la instrucción 1<<LCD_E está indicando que a la constante LCD_RS se le debe asignar un 1.
		-	La instrucción | es un OR, lo que crea es una máscara OR.
		-	La instrucción |= lo que hace es asignar el valor 1 lógico a un bit específico de un número binario cualquiera sin afectar a los demás.
		-	La instrucción & es un AND, lo que crea es una máscara AND.
		-	La instrucción &= lo que hace es asignar el valor 0 lógico a un bit específico de un número binario cualquiera sin afectar a los demás.*/
	//MÁSCARA OR: Convierte en 1 todo lo que esté en contacto con los 1 de la máscara y deja pasar tal cual lo que esté en contacto con los 0 de la máscara.
	//MÁSCARA AND: Convierte en 0 todo lo que esté en contacto con los 0 de la máscara y deja pasar tal cual lo que esté en contacto con los 1 de la máscara.
	EIMSK |= (1<<INT0);	//Activación de la interrupción externa INT0 asociada al pin 2 (PD2) del Puerto D en el Arduino.
	EIMSK |= (1<<INT1);	//Activación de la interrupción externa INT1 asociada al pin 3 (PD3) del Puerto D en el Arduino.
	
	/*EICRA o registro de control de interrupciones: Las interrupciones se pueden ejecutar por distintos factores externos que ocasionan cambios en la señal que entra a los 
	pines de interrupciones externas, configurados con los 4 bits ISC11, ISC10, ISC01 e ISC00 pertenecientes al registro EICRA.
		- ISC11: Bit 3 del registro EICRA, afecta a la interrupción externa INT1: 
		- ISC10: Bit 2 del registro EICRA, afecta a la interrupción externa INT1: 
			- ISC11 = 0 y ISC10 = 0; Interrupción de nivel bajo, osea cuando la señal de factor externo se encuentra en 0 lógico.
			- ISC11 = 0 y ISC10 = 1; Interrupción con cambio de estado lógico en la señal, osea cuando la señal de factor externo cambia de 0 a 1 o 1 a 0.
			- ISC11 = 1 y ISC10 = 0; Interrupción de flanco de bajada, osea cuando la señal de factor externo cambia de 1 a 0.
			- ISC11 = 1 y ISC10 = 1; Interrupción de flanco de subida, osea cuando la señal de factor externo cambia de 0 a 1.
		- ISC01: Bit 1 del registro EICRA, afecta a la interrupción externa INT0:
		- ISC00: Bit 0 del registro EICRA, afecta a la interrupción externa INT0:
			- ISC01 = 0 y ISC00 = 0; Interrupción de nivel bajo, osea cuando la señal de factor externo se encuentra en 0 lógico.
			- ISC01 = 0 y ISC00 = 1; Interrupción con cambio de estado lógico en la señal, osea cuando la señal de factor externo cambia de 0 a 1 o 1 a 0.
			- ISC01 = 1 y ISC00 = 0; Interrupción de flanco de bajada, osea cuando la señal de factor externo cambia de 1 a 0.
			- ISC01 = 1 y ISC00 = 1; Interrupción de flanco de subida, osea cuando la señal de factor externo cambia de 0 a 1.*/
	EICRA |= (1<<ISC01)|(1<<ISC00); //INT0 del registro EIMSK configurada con los bits ISC01 e ISC00 del registro EICRA, activando la interrupción de flanco de subida.
	EICRA |= (1<<ISC11)|(1<<ISC10); //INT1 del registro EIMSK configurada con los bits ISC11 e ISC10 del registro EICRA, activando la interrupción de flanco de subida.
	
	/*Pin Change (PCI): Esto puede hacer que cualquier pin de los puertos B, C o D se convierta en un pin de interrupciones, pero cada conjunto de pines de interrupción de 
	un mismo puerto, ejecutará la misma función de interrupción aunque sean pines distintos.*/
	//PCICR o registro de control de la interupción de cambio de pin: Sirve para convertir cualquier pin normal de los puertos B, C o D en un pin de interrupción.
	/*Se cuenta con 3 bits del registro PCICR (Pin Change Interruption Control Register) para hacer que se puedan usar los pines de los 3 puertos existentes en el ATMEGA328P:
		- PCIE0: Es el bit 0 del registro PCICR y permite que los pines del puerto B sean de interrupción.
		- PCIE1: Es el bit 1 del registro PCICR y permite que los pines del puerto C sean de interrupción.
		- PCIE2: Es el bit 2 del registro PCICR y permite que los pines del puerto D sean de interrupción.
	Cuando se le asigna 1 lógico a un bit del registro se permite a todos los pines del puerto que puedan convertirse en pines de interrupción, si se deja 0 lógico no se 
	podrán convertir a pines de interrupción y se quedarán todos como pines normales.*/
	PCICR |= (1<<PCIE0);	//Los pines del puertos B son pines de interrupción.
	//No se debe mandar ceros a los bits de los registros que no vayamos a usar, sino el programa se confunde y no ejecuta bien las interrupciones PCI.
	/*Los siguientes 3 registros sirven para designar el pin o pines que ejecutan una interrupción en cada uno de los 3 puertos del microcontrolador:
		- PCMSK0: Es el registro que maneja los pines del puerto B para que puedan ejecutar interrupciones:
			- Puerto B, Tiene 6 pines:
				PCINT0, es el bit 0 del registro PCMSK0:	Pin 8	(PB0) del Arduino
				PCINT1, es el bit 1 del registro PCMSK0:	Pin 9	(PB1) del Arduino
				PCINT2, es el bit 2 del registro PCMSK0:	Pin 10	(PB2) del Arduino
				PCINT3, es el bit 3 del registro PCMSK0:	Pin 11	(PB3) del Arduino
				PCINT4, es el bit 4 del registro PCMSK0:	Pin 12	(PB4) del Arduino
				PCINT5, es el bit 5 del registro PCMSK0:	Pin 13	(PB5) del Arduino
		- PCMSK1: Es el registro que maneja los pines del puerto C para que puedan ejecutar interrupciones:
			- Puerto C, Tiene 6 pines:
				PCINT8, es el bit 0 del registro PCMSK1:	Pin A0	(PC0) del Arduino
				PCINT9, es el bit 1 del registro PCMSK1:	Pin A1	(PC1) del Arduino
				PCINT10, es el bit 2 del registro PCMSK1:	Pin A2	(PC2) del Arduino
				PCINT11, es el bit 3 del registro PCMSK1:	Pin A3	(PC3) del Arduino
				PCINT12, es el bit 4 del registro PCMSK1:	Pin A4	(PC4) del Arduino
				PCINT13, es el bit 5 del registro PCMSK1:	Pin A5	(PC5) del Arduino
		- PCMSK2: Es el registro que maneja los pines del puerto D para que puedan ejecutar interrupciones:
			- Puerto D, Tiene 8 pines:
				PCINT16, es el bit 0 del registro PCMSK2:	Pin 0	(PD0) del Arduino
				PCINT17, es el bit 1 del registro PCMSK2:	Pin 1	(PD1) del Arduino
				PCINT18, es el bit 2 del registro PCMSK2:	Pin 2	(PD2) del Arduino; INT0, Interrupción externa 0
				PCINT19, es el bit 3 del registro PCMSK2:	Pin 3	(PD3) del Arduino; INT1, Interrupción externa 1
				PCINT20, es el bit 4 del registro PCMSK2:	Pin 4	(PD4) del Arduino
				PCINT21, es el bit 5 del registro PCMSK2:	Pin 5	(PD5) del Arduino
				PCINT22, es el bit 6 del registro PCMSK2:	Pin 6	(PD6) del Arduino
				PCINT23, es el bit 7 del registro PCMSK2:	Pin 7	(PD7) del Arduino
	Cuando se le asigna 1 lógico a un bit del registro se convierte en pin de interrupción, si se deja 0 lógico se conserva como pin normal.*/
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1); //Los bits PCINT0 y PCINT1 asociados a los pines 8 y 9 del puerto B son pines de interrupción.
	
	/*Algo importante a conocer de los PCI (Pin Change) es que su interrupción solamente se ejecuta con un tipo de interrupción que es:
		Interrupción con cambio de estado lógico en la señal: Osea cuando la señal de factor externo cambia de 0 ? 1 o 1 ? 0. 
	En los PCI no se puede cambiar esta condición, mientras en las interrupciones externas INT0 e INT1 si se puede elegir que tipo de interrupción se quiere.*/
	
	//PCIFR o registro de banderas de la interupción de cambio de pin: Sirve para indicar cualquier activación de alguna interrupción en algún pin de los puertos B, C o D.
	/*Se cuenta con 3 bits del registro PCIFR (Pin Change Interruption Flag Register) para detectar una interrupción en un pin de los 3 puertos existentes en el ATMEGA328P:
		- PCIF0: Es el bit 0 del registro PCIFR y permite detectar una interrupción en algún pín del puerto B.
		- PCIF1: Es el bit 1 del registro PCIFR y permite detectar una interrupción en algún pín del puerto C.
		- PCIF2: Es el bit 2 del registro PCIFR y permite detectar una interrupción en algún pín del puerto D.*/
	
		
	
	/*SEI: Cuando se activa una SEI interruption, automáticamente el bit 7 del status register (GI) se convierte en 1 lógico, lo que convierte a las interrupciones en NMI 
	(No Mask Interruption):
		- Cuando GI tiene valor de 1 lógico no está activada ninguna interrupción, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 lógico está activada la interrupción, osea que es MI, Maskable Interrupt.*/
	sei(); //Permite cualquier interrupción que entre ya después de haber programado la máquina y cuando haya terminado de ejecutarse una interrupción anterior.
	
	
	
	//EJECUCIÓN DEL PROGRAMA: Ya después de haber configurado las interrupciones es cuando se ejecuta la función del programa
	//MANEJO DE LCD:
	/*LCD_INICIALIZA: Función para configurar el LCD a que su cursor esté en el inicio, exista una comunicación de 4 bits, disponga de 2 líneas el LCD, sus caracteres sean de
	5X8 pixeles, su cursor sea visible y no parpadee, su incremenento de posición sea hacia la derecha, su pantalla esté estática y se manden datos al LCD, no comandos.*/
	LCD_INICIALIZA();
	//Inicialización de los caracteres personalizados:
	//Creación del caracter especial 0 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar.
	CAR_ESP0();
	//Creación del caracter especial 1 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar.
	CAR_ESP1();
	//Creación del caracter especial 2 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar.
	CAR_ESP2();
	//Creación del caracter especial 3 en la memoria CGRAM, que es parte de la memoria CGROM, donde se almacenan todos los símbolos a mostrar.
	CAR_ESP3();
	//LIMPIA_LCD: Función que limpia el LCD, borrando su pantalla y regresando su cursor a su posición inicial.
	LIMPIA_LCD();
	//REGRESO_CASA: Función que regresa el cursor a su posición inicial
	REGRESO_CASA();
	/*POS_LINEA1: Se refiere a la primera línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual 
	de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD. Puede obviarse la instrucción POS_LINEA1() y funcionará el programa, pero las posiciones de las 
	demás posiciones si se deben indicar explícitamente.*/
	POS_LINEA1(0);
	//MENSAJE_INICIAL: Función normal para mostrar un mensaje inicial después o antes de que se haya presionado una interrupción.
	MENSAJE_INICIAL();
	
	
	
	//ALTERNACIÓN DE ENCENDIDO Y APAGADO DE LEDS: Tarea principal.
	/*En este caso justo por esta línea de código es importante el uso de interrupciones, ya que si estas no existieran, el programa se quedaría atorado en el bucle while y 
	no podría ejecutar ninguna línea adicional ya que llegue a este punto del código, mientras que con las interrupciones, obliga al programa a salir del bucle y ejecutar las 
	funciones ISR declaradas antes del método main.*/
	while(1){
		//0: Pin como entrada
		//1: Pin como salida
		//DDRB = 0x3F = 0011 1111; Algunos pines del puerto B son entradas y otros salidas. Solo son entradas los que activen interrupciones.
		/*Indicación de los pines de salida, si se declaró como entrada alguno de los pines y se utiliza un 1 en el registro PORT, la entrada llegará al microcontrolador de 
		manera inversa a la original (resistencias pull-up), si se pone un 0 llegará de forma normal (resistencias pull-down). Funciona más o menos como una compuerta NOT.*/
		PORTB = a;	//0XD7 = 1101 0111; Esto sirve para prender y apagar leds en el puerto B turnándose entre ellos.
		_delay_ms(250);
		PORTB = b;	//0XEB = 1110 1011; Esto sirve para prender y apagar leds en el puerto B turnándose entre ellos.
		_delay_ms(250);
		PORTB = c;	//0X00 = 0000 0000; Esto sirve para apagar todos los leds en el puerto B.
		_delay_ms(250);
	}
}