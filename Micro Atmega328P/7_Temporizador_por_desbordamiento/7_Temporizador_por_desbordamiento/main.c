/*
 * 7_Temporizador_por_desbordamiento.c
 *
 * Created: 18/05/2022 06:48:32 p. m.
 * Author : diego
 7.- TEMPORIZADOR POR DESBORDAMIENTO: Es un tipo de temporizador que activa banderas que desencadenan interrupciones cuando realizan un desbordamiento, esto se refiere a 
 cuando el contador llega a su valor máximo y debe volver a contar desde cero.
 Para el ejercicio se activará la configuración de temporizador externo T0 perteneciente al pin 4 del puerto D del Arduino, donde se conectará un botón, este al ser presionado
 aumentará el conteo de clientes que ingresan a una tienda y lo mostrará en la pantalla del LCD.
 - Puerto B, Tiene 6 pines disponibles:
	 Pin 8	(PB0)
	 Pin 9	(PB1)
	 Pin 10	(PB2)
	 Pin 11	(PB3)
	 Pin 12	(PB4)
	 Pin 13	(PB5)
 - Puerto C, Tiene 6 pines disponibles:
	 Pin A0	(PC0)
	 Pin A1	(PC1)
	 Pin A2	(PC2)
	 Pin A3	(PC3)
	 Pin A4	(PC4)
	 Pin A5	(PC5)
 - Puerto D, Tiene 8 pines disponibles:
	 Pin 0	(PD0)
	 Pin 1	(PD1)
	 Pin 2	(PD2); INT0, Interrupción externa 0
	 Pin 3	(PD3); INT1, Interrupción externa 1
	 Pin 4	(PD4); T0, Temporizador 0; Este es el que se usa para detonar la interrupción de T0.
	 Pin 5	(PD5); T1, Temporizador 1
	 Pin 6	(PD6)
	 Pin 7	(PD7)
El desbordamiento del contador cada que se pulse el temporizador externo conectado al pin 4 del puerto D, ejecutará la función ISR.

Recordemos que en el proyecto se usa para las siguientes funciones los puertos:
PUERTO C - LCD:
Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada, por lo tanto solamente 6 bits en total contando los 2 de control son los que se conectan y son
los siguientes:
Arduino:	A5,		A4,		A3,		A2,		A1,		A0
LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

PUERTO D - TECLADO MATRICIAL 4X4: Aunque en este caso no se va a conectar, ya que se quiere usar los pines PD4 y PD5 para los temporizadores T0 y T1.
La asignación de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexión del teclado matricial vistos de izquierda
a derecha son los siguientes:
Puerto D Arduino (Digital PWM):			7,		6,		5,		4,		3,		2,		1,		0
Constantes del teclado matricial 4X4:	F1,		F2,		F3,		F4,		C1,		C2,		C3,		C4
Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8

PUERTO B - DISPOSITIVOS EXTERNOS VARIADOS: Leds, botones, etc.*/


/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
librerías), pero antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librería delay de manejo de tiempos la utiliza


//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>			//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h>		//Librería de manejo de tiempos, como retardos principalmente
#include <avr/interrupt.h>	//Librería para el manejo de los registros de interrupciones
#include "../2_LCD_4bits.h"	//Librería propia para el manejo del LCD
#include <stdio.h>			//Librería para la entrada, salida, manejo y conversión de datos, en específico se usa para poder utilizar el método sprintf()
#include <string.h>			//Librería para el manejo de cadenas de caracteres, que son un tipo de string en el lenguaje C, en específico para usar el método strcat()


//DECLARACIÓN DE VARIABLES GLOBALES (CABECERAS DE FUNCIÓN):
/*Para poder imprimir un número en pantalla se debe conocer el formato que utiliza cada uno y los tipos primitivos de datos en C:
int:	Números enteros, utilizan un espacio de memoria de 2 o 4 bytes. Su formato es el %d o %i.
float:	Números fraccional con máximo 7 decimales después del punto, utilizan un espacio de memoria de 4 bytes. Su formato es el %f.
double:	Números fraccional con máximo 15 decimales después del punto, utilizan un espacio de memoria de 8 bytes. Su formato es el %lf.
char:	Caracater, representa una sola letra o valor ASCII, utilizan un espacio de memoria de 1 byte. Su formato es el %s.*/
int clientes_numero = 0;				//Variable que realiza el conteo de clientes que pasan por la puerta de un negocio.
char clientes_cadena_caracteres[500];	//Variable guarda una cadena de caracteres representado como un array de caracteres de máximo 1000 caracteres, osea un tipo de string.
//OJO DE CICLÓN: Es una serie de leds que prenden y apagan para crear un flujo de leds en forma consecutiva que cambia de dirección cuando el flujo llega a su límite.
int bandera = 0;						//Variable que indica la dirección del ojo de ciclón.
int j = 1;								//Variable que indica los leds a prenderse en el ojo de ciclón.



//MENSAJE_INICIAL: Función normal para mostrar un mensaje inicial después o antes de que se haya activado una interrupción.
void MENSAJE_INICIAL(){
	//_delay_ms(750); //Retraso antirebote
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
	ENVIA_CADENA("El # de clientes");
}



//Funciones ISR: Tareas ejecutadas por las interrupciones externas, PCI (Pin Change) o Temporizadores.
/*ISR (TIMER0_OVF_vect): Se le llama ISR o Interruption Service Routine a la función que describa lo que se tiene que hacer cuando sea activada una interrupción, la palabra 
ISR es una palabra reservada de interrupciones y el nombre de esta función no debe ser decladada en ningún lado del código, simplemente por haber sido nombrada como ISR, se 
ejecuta cuando sea detonada la interrupción que recibe como parámetro, para ello el temporizador T0 ya debe haber sido configurado dentro del método main. Para ello se le 
pasa como parámetro a la función el vector TIMER0_OVF_vect para que ejecute la acción cuando el temporizador llegue al desbordamiento.*/
/*Las acciones de las interrupciones causadas por temporizadores de desbordamiento deben ser descritas en una función fuera del método main, estas reciben como parámetro lo 
siguiente:
	- TIMER0_OVF_vect: Interrupción causada por el temporizador T0 de 8 bits después de haber sido configurado con los registros TCCR0B y TIMSK0, este está asociado al pin 4 
	(PD4) del Puerto D en el Arduino, ya que se puede configurar el temporizador para recibir una señal de reloj de un oscilador externo.
	- TIMER1_OVF_vect: Interrupción causada por el temporizador T1 de 16 bits después de haber sido configurado con los registros TCCR0B y TIMSK0, este está asociado al pin 5 
	(PD5) del Puerto D en el Arduino, ya que se puede configurar el temporizador para recibir una señal de reloj de un oscilador externo.
	- TIMER2_OVF_vect: Interrupción causada por el temporizador T2 de 8 bits después de haber sido configurado con los registros TCCR0B y TIMSK0, no cuenta con pin de 
	Arduino.*/
ISR (TIMER0_OVF_vect){
	//DELAY ANTIREBOTE
	_delay_ms(500);
	//INICIALIZACIÓN DEL CONTADOR T0 EN 255 PARA QUE SE REALICE EL DESBORDE CON 1 CLIC
	/*Como se quiere lograr que al dar un clic, se detone la interrupción por desbordamiento del temporizador, es necesario ponerlo en su punto máximo, por ello es que se 
	accede al registro TCNT:
	- TCNT0: Registro que accede a los 8 bits del contador T0, que realiza el conteo de 0000 0000 = 0 hasta el 1111 1111 = 255.
		- TCNT0_0: Bit 0, menos significativo del registro TCNT0.
		- TCNT0_1: Bit 1 del registro TCNT0.
		- TCNT0_2: Bit 2 del registro TCNT0.
		- TCNT0_3: Bit 3 del registro TCNT0.
		- TCNT0_4: Bit 4 del registro TCNT0.
		- TCNT0_5: Bit 5 del registro TCNT0.
		- TCNT0_6: Bit 6 del registro TCNT0.
		- TCNT0_7: Bit 7, más significativo del registro TCNT0.
	- TCNT1: Registro que accede a los 16 bits del contador T1, que realiza el conteo de 0000 0000 0000 0000 = 0 hasta el 1111 1111 1111 1111 = 65,535.
		- TCNT1H: Registro que guarda los 8 bits más significativos del registro TCNT1.
			- TCNT1H0: Bit 0, menos significativo del registro TCNTH1.
			- TCNT1H1: Bit 1 del registro TCNTH1.
			- TCNT1H2: Bit 2 del registro TCNTH1.
			- TCNT1H3: Bit 3 del registro TCNTH1.
			- TCNT1H4: Bit 4 del registro TCNTH1.
			- TCNT1H5: Bit 5 del registro TCNTH1.
			- TCNT1H6: Bit 6 del registro TCNTH1.
			- TCNT1H7: Bit 7, más significativo del registro TCNTH1.
		- TCNT1L: Registro que guarda los 8 bits menos significativos del registro TCNT1.
			- TCNT1L0: Bit 0, menos significativo del registro TCNTL1.
			- TCNT1L1: Bit 1 del registro TCNTL1.
			- TCNT1L2: Bit 2 del registro TCNTL1.
			- TCNT1L3: Bit 3 del registro TCNTL1.
			- TCNT1L4: Bit 4 del registro TCNTL1.
			- TCNT1L5: Bit 5 del registro TCNTL1.
			- TCNT1L6: Bit 6 del registro TCNTL1.
			- TCNT1L7: Bit 7, más significativo del registro TCNTL1.
	- TCNT1: Registro que accede a los 8 bits del contador T2, que realiza el conteo de 0000 0000 = 0 hasta el 1111 1111 = 255.
		- TCNT2_0: Bit 0, menos significativo del registro TCNT2.
		- TCNT2_1: Bit 1 del registro TCNT2.
		- TCNT2_2: Bit 2 del registro TCNT2.
		- TCNT2_3: Bit 3 del registro TCNT2.
		- TCNT2_4: Bit 4 del registro TCNT2.
		- TCNT2_5: Bit 5 del registro TCNT2.
		- TCNT2_6: Bit 6 del registro TCNT2.
		- TCNT2_7: Bit 7, más significativo del registro TCNT2.
	Estos 3 registros que acceden al contador de los timers T0, T1 y T2 se pueden modificar cuando sea para cambiar su conteo, como en este caso.*/
	TCNT0 = 255; //255 = 0xFF = 0b11111111 = 1111 1111; Inicialización del contador T0 para que realice su desbordamiento en cada clic.
	//AUMENTO DEL NÚMERO DE CLIENTES
	clientes_numero++;
}


//MÉTODO MAIN O EJECUTABLE: Desde este método se ejecuta todo el código y es lo que diferencía la librería de un ejecutable.
/*Para configurar correctamente un programa que use interrupciones se utiliza la siguiente arquitectura en el programa:
	- CLI: Activación de interrupciones asignando un valor 0 lógico al bit GI, volviendo la interrupción de tipo MI.
	- Programación de máquina y configuración de interrupciones con los registros TCCR0B (registro de control del temporizador B), TIMSK0 y OCR0A (registro de comparación).
	- SEI: Desactivación de interrupciones asignando un valor 1 lógico al bit GI, volviendo la interrupción de tipo NMI.
	- Ejecución del programa.
Con dicha arquitectura nos aseguramos de manejar propiamente los pines de los puertos que manejan las interrupciones, activándolas y desactivándolas cuando es necesario.*/
int main(void){
	/*CLI: Cuando se activa una CLI interruption, automáticamente el bit 7 del status register que es GI (General Interruption) se convierte en 0 lógico, lo que convierte a 
	las interrupciones en MI (Maskable Interrupt), esto en términos más simples significa que las interrupciones están activas:
		- Cuando GI tiene valor de 1 lógico no está activada ninguna interrupción, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 lógico está activada la interrupción, osea que es MI, Maskable Interrupt.*/
	cli(); //Evita cualquier interrupción que entre mientras ya se está ejecutando una o antes de programar la máquina, ya que termine, permite la ejecución de interrupciones.
	
	
	
	//PROGRAMACIÓN DE LA MÁQUINA: Se indica por medio de los registros de función específica si los pines de los puertos serán entradas o salidas o el estado de los timers.
	//0: Pin como entrada
	//1: Pin como salida
	DDRB = 0xFF; //Todos los pines del puerto B son salidas.
	DDRC = 0xFF; //Todos los pines del puerto C son salidas.
	DDRD = 0x00; //Todos los pines del puerto D son entradas, para que por ellas entre el oscilador externo para el contador T0.
	
	//GTCCR o registro general de control del temporizador y contador: Sirve para cuando se usa más de un timer, reiniciando sus conteos y escalas, logrando su sincronización.
	/*El registro GTCCR cuenta con varias herramientas para sincronizar los 3 timers:
		- TSM: Bit del registro GTCCR para la sincronización de timers, con 1 lógico hace que todos los temporizadores empiecen a contar al mismo tiempo y no manejen tiempos 
		distintos, mientras que con 0 lógico pueden empezar a contar cuando quieran pero manejarán tiempos distintos.
		- PSRASY: Bit del registro GTCCR para el reinicio del timer T2, con 1 lógico se reinicia el conteo de T2 a cero y su escala se pone en 0, con 0 lógico deja al 
		temporizador T2 con el último valor de conteo y escala con el que se quedó.
		- PSRSYNC: Bit del registro GTCCR para la reiniciar el conteo de los temporizadores T0 y T1, con 1 lógico se reinicia su valor y se pone su escala en 0, mientras que 
		con 0 lógico se deja a los temporizadores T0 y T1 con el último valor de conteo que tenían y escala con la que se quedaron.
	Cuando es usa más de un timer para manejo de tiempos, es necesario usar este registro, además esto se debe hacer justo antes de configurar los timers, una vez 
	configurados, se debe volver este registro a que valga 0.
	No es tan importante usar el registro GTCCR cuando se usan interrupciones externas, pero cuando se hace con temporizadores si lo es, ya que puede evitar que falle el
	programa, además cuando se usan varios temporizadores a la vez, deben usarse en Puertos diferentes, sino fallarán*/
	GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC); //TSM -> Timers sincronizados; PSRASY -> Timer T2 reiniciado; PSRSYNC -> Timers T0 y T1 reiniciados.
	
	//CONFIGURACIÓN DE TEMPORIZADORES:
	/*En el microcontrolador ATMEGA328P del Arduino se tienen 3 contadores de 2 números distintos de bits, que afecta en su velocidad de conteo:
	- T0/T2: Los dos contadores T0 y T2 son de 8 bits, osea que cuentan desde 0000 0000 hasta 1111 1111, teniendo como valor máximo el 255.
	- T1: El contador T1 es de 16 bits, osea que cuenta desde 0000 0000 0000 0000 hasta  1111 1111 1111 1111, teniendo como valor máximo el 65,536.
	Los contadores dependen de la frecuencia de su oscilador, que en el ATMEGA328P es de 16 MHz y de su escala que puede ser de:
		- 1:1
		- 1:8
		- 1:64
		- 1:256
		- 1:1024
	La escala en específico es configurable a partir del registro TCCR0B del temporizador y sus bits CS02, CS01 y CS00, donde se tiene las siguientes opciones:
		- Ninguna fuente de reloj: En esta opción el temporizador se encuentra detenido, no funciona, esto se hace para ahorrar energía en el microcontrolador.
			- CS02 = 0
			- CS01 = 0
			- CS00 = 0
		- Escala de 1:1 para el conteo del microcontrolador.
			- CS02 = 0
			- CS01 = 0
			- CS00 = 1
		- Escala de 1:8 para el conteo del microcontrolador.
			- CS02 = 0
			- CS01 = 1
			- CS00 = 0
		- Escala de 1:64 para el conteo del microcontrolador.
			- CS02 = 0
			- CS01 = 1
			- CS00 = 1
		- Escala de 1:256 para el conteo del microcontrolador.
			- CS02 = 1
			- CS01 = 0
			- CS00 = 0
		- Escala de 1:1024 para el conteo del microcontrolador.
			- CS02 = 1
			- CS01 = 0
			- CS00 = 1
		- Esta configuración permite conectar un oscilador externo en el pin T0 (que puede ser un LM555) y que su conteo se ejecute en el flanco de bajada de la señal de reloj.
			- CS02 = 1
			- CS01 = 1
			- CS00 = 0
		- Esta configuración permite conectar un oscilador externo en el pin T0 (que puede ser un LM555) y que su conteo se ejecute en el flanco de subida de la señal de reloj.
			- CS02 = 1
			- CS01 = 1
			- CS00 = 1
	Recordemos que la fórmula para obtener el tiempo total de cada desbordamiento en el contador del microcontrolador es la siguiente, tomando en cuenta la frecuencia del 
	oscilador, el tiempo que toma cada ciclo de máquina, la escala y el número de conteo que depende del número de bits del contador, ya sea de 8 o 16 bits.
	Escala*CM*Número de conteo = Escala*(1/f)*2^#bits del contador = TM (tiempo máximo de conteo por ciclo).*/
	//TCCR0B o registro de control del temporizador: Sirve para indicar la configuración antes mencionada en el temporizador.
	TCCR0B |= (1<<CS02)|(1<<CS01)|(1<<CS00); //Oscilador externo T0, flanco de subida; CS02 = 1, CS01 = 1 y CS00 = 1
	
	//TIMSK o registro de control del temporizador: Sirve para indicar que el temporizador sea de tipo desbordamiento o comparación.
	/*Los bits TOIE2, TOIE1 Y TOIE0 sirven para indicar cuál de los temporizadores T2, T1 o T0 queremos que sea de tipo desbordamiento, para que pueda ser de
	ese tipo se debe mandar un 1 lógico a través de cada uno de ellos:
		- TOIE2: Hace que el timer T2 de 8 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		- TOIE1: Hace que el timer T1 de 16 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		- TOIE0: Hace que el timer T0 de 8 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.*/
	TIMSK0 |= (1<<TOIE0); //Timer T0 de 8 bits habilitado como tipo desbordamiento.
	
	//MANEJO DEL TEMPORIZADOR: Si se quiere cambiar el inicio de conteo de un temporizador se puede modificar el estado inicial de su registro TCNT#
	//TCNT#: Registro que maneja el conteo de un temporizador, su conteo va desde 0 hasta 255, después de contar el número 255 ocurre el desbordamiento y vuelve a valer 0.
	/*Para que cada que se dé un clic exista una interrupción por desbordamiento, el temporizador T0 de 8 bits debe estar en su punto máximo que es el 255, ya que el chiste 
	de este programa es contar y aumentar una variable cada que se dé clic en el botón conectado a la interrupción externa del temporizador T0.*/
	TCNT0 = 255; //255 = 0xFF = 0b11111111 = 1111 1111; Inicialización del contador T0 para que realice su desbordamiento en cada clic.
	
	//GTCCR: Una vez configurados los timers, se debe poner como cero el registro que reinicia y sincroniza los timers, liberando así los timers.
	GTCCR = (0<<TSM)|(0<<PSRASY)|(0<<PSRSYNC); //TSM -> Timers NO sincronizados; PSRASY -> Timer T2 NO reiniciado; PSRSYNC -> Timers T0 y T1 NO reiniciados.
	
	
	
	/*SEI: Cuando se activa una SEI interruption, automáticamente el bit 7 del status register (GI) se convierte en 1 lógico, lo que convierte a las interrupciones en NMI 
	(No Mask Interruption):
		- Cuando GI tiene valor de 1 lógico no está activada ninguna interrupción, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 lógico está activada la interrupción, osea que es MI, Maskable Interrupt.*/
	sei(); //Permite cualquier interrupción que entre ya después de haber programado la máquina y cuando haya terminado de ejecutarse una interrupción anterior.
	
	
	
	//EJECUCIÓN DEL PROGRAMA: Ya después de haber configurado las interrupciones es cuando se ejecuta la función del programa.
	//MANEJO DE LCD
	//MENSAJE_INICIAL: Función normal para mostrar un mensaje inicial después o antes de que se haya presionado una interrupción.
	MENSAJE_INICIAL();
	
	
	//ALTERNACIÓN DE ENCENDIDO Y APAGADO DE LEDS: Tarea principal.
	/*OJO DE CICLÓN: El algoritmo para hacer esto lo que hace es agarrar una variable y darle un valor de 1, multiplicarlo por 2 hasta que su valor llegue a 64, cada vez que 
	llegue a 64, se pone una bandera en 1 para indicar que se llegó al límite ascendente y cuando valga esa bandera 1, ya no se multiplicará el número sino que se va a dividir 
	hasta que llegue a cero y cuando alcance el cero la bandera igual volverá a valer 0.*/
	/*En este caso justo por esta línea de código es importante el uso de interrupciones, ya que si estas no existieran, el programa se quedaría atorado en el bucle while y 
	no podría ejecutar ninguna línea adicional ya que llegue a este punto del código, mientras que con las interrupciones, obliga al programa a salir del bucle y ejecutar las 
	funciones ISR declaradas antes del método main.*/
	while(1){
		//OJO DE CICLÓN:
		//Dirección ascendente del ojo de ciclón, bandera = 0
		if (bandera == 0) {
			_delay_ms(50);
			j = j*2; //Cuando va en dirección ascendente, la variable j se multiplica por 2 hasta llegar al límite superior, que es 0x40 = 64.
			if(j == 0x40){
				bandera = 1; //Cuando se ha llegado al límite superior, el ojo de ciclón cambia de dirección.
			}
		}
		//Dirección descendente del ojo de ciclón, bandera = 1
		if (bandera == 1){
			j = j/2; //Cuando va en dirección descendente, la variable j se divide entre 2 hasta llegar al límite inferior, que es 1.
			_delay_ms(50);
			if(j == 1){
				bandera = 0; //Cuando se ha llegado al límite superior, el ojo de ciclón cambia de dirección.
			}
		}
		PORTB = j;
		
		//MOSTRAR CONTEO EN LCD:
		/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de 
		cual de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
		POS_LINEA2(0);
		/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido 
		cada letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
		ENVIA_CADENA("recibidos: ");
	
		//IMPRIMIR UN NÚMERO EN EL LCD:
		/*Para poder imprimir un número en pantalla se debe conocer el formato que utiliza cada uno y los tipos primitivos de datos en C:
		int:	Números enteros, utilizan un espacio de memoria de 2 o 4 bytes. Su formato es el %d o %i.
		float:	Números fraccional con máximo 7 decimales después del punto, utilizan un espacio de memoria de 4 bytes. Su formato es el %f.
		double:	Números fraccional con máximo 15 decimales después del punto, utilizan un espacio de memoria de 8 bytes. Su formato es el %lf.
		char:	Caracater, representa una sola letra o valor ASCII, utilizan un espacio de memoria de 1 byte. Su formato es el %s.
		Ya conocidos los formatos asociados a cada tipo primitivo, se usan dos variables y el formato del número que ingresa al método sprintf() que sirve para realizar la 
		conversión.*/
		//sprintf(variable_que_guarda_el_resultado, "formato_numero", numero_inicial): Método para convertir números a cadenas de caracteres.
		sprintf(clientes_cadena_caracteres, "%d", clientes_numero);
	
		ENVIA_CADENA(clientes_cadena_caracteres);	//Impresión del número de clientes, que aumenta cada vez que se presiona el pin del temporizador.
		
		/*CURSOR_IZQ_CORRE: Permite mover el cursor hacia la izquierda del texto que tenga a la derecha, para que sobreescriba un caracter del texto mostrado en el LCD, sin 
		mover la pantalla del LCD.
		Si esta instrucción no se pone, se irá escribiendo uno tras otro los símbolos de la animación, llenando la LCD y moviendo la pantalla.*/
		CURSOR_IZQ_CORRE();
	}
}