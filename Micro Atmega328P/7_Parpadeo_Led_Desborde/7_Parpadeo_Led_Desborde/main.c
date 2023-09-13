/*
 * 7_Parpadeo_Led_Desborde.c
 *
 * Created: 27/05/2022 01:43:54 p. m.
 * Author : diego
 7.- TEMPORIZADOR POR DESBORDAMIENTO: Es un tipo de temporizador que activa banderas que desencadenan interrupciones cuando realizan un desbordamiento, esto se refiere a
 cuando el contador llega a su valor máximo y debe volver a contar desde cero.
 
 Para el ejercicio se configurarán 3 parpadeos en 3 leds diferentes con tiempos de 250ms, 1.5s y 30s con los 3 timers T0, T1 y T2 respectivamente en modo de temporizador por 
 desbordamiento:
 T0: Parpadeo de 250ms.
 T1: Parpadeo de 1s.
 T2: Parpadeo de 2s.
 
 - Puerto B, Tiene 6 pines disponibles:
 Pin 8	(PB0); Led con parpadeo de 250ms;	Temporizador T0.
 Pin 9	(PB1)
 Pin 10	(PB2)
 Pin 11	(PB3)
 Pin 12	(PB4)
 Pin 13	(PB5)
 - Puerto C, Tiene 6 pines disponibles:
 Pin A0	(PC0); Led con parpadeo de 1s;		Temporizador T1.
 Pin A1	(PC1)
 Pin A2	(PC2)
 Pin A3	(PC3)
 Pin A4	(PC4)
 Pin A5	(PC5)
 - Puerto D, Tiene 8 pines disponibles:
 Pin 0	(PD0); Led con parpadeo de 2s;		Temporizador T2.
 Pin 1	(PD1)
 Pin 2	(PD2); INT0, Interrupción externa 0
 Pin 3	(PD3); INT1, Interrupción externa 1
 Pin 4	(PD4); T0, Temporizador 0
 Pin 5	(PD5); T1, Temporizador 1
 Pin 6	(PD6)
 Pin 7	(PD7)
 
 Cuando se desborden los 3 temporizadores, ejecutará una función ISR cada uno.

 Recordemos que en el proyecto se usa para las siguientes funciones los puertos:
 PUERTO C - LCD: Aunque en este caso se utilizará el puerto PC0 para observar una de las señales de salida de los temporizadores CTC.
 Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
 DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada, por lo tanto solamente 6 bits en total contando los 2 de control son los que se conectan y son
 los siguientes:
 Arduino:	A5,		A4,		A3,		A2,		A1,		A0
 LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

 PUERTO D - TECLADO MATRICIAL 4X4: En este caso no se va a conectar, ya que se quiere usar el pin PD0 que tiene la salida de señal cuadrada con frecuencia variable OCR2A.
 La asignación de pines del Puerto D en el Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexión del teclado matricial vistos de izquierda
 a derecha son los siguientes:
 Puerto D Arduino (Digital PWM):		7,		6,		5,		4,		3,		2,		1,		0
 Constantes del teclado matricial 4X4:	F1,		F2,		F3,		F4,		C1,		C2,		C3,		C4
 Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8

 PUERTO B - DISPOSITIVOS EXTERNOS VARIADOS: Leds, botones, etc. Aunque en este caso se utilizará el puerto PB0 para observar una de las señales de salida de los
 temporizadores CTC.*/



//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>			//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <avr/interrupt.h>	//Librería para el manejo de los registros de interrupciones



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
	DDRB = (1<<DDB0); //Pin 8	(PB0) del Puerto B como salida, parpadeo de 250ms;	Temporizador T0.
	DDRC = (1<<DDC0); //Pin A0  (PC0) del Puerto B como salida, parpadeo de 1s;		Temporizador T1.
	DDRD = (1<<DDD0); //Pin 0	(PD0) del Puerto B como salida, parpadeo de 2s;		Temporizador T2.
	
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
	
	/*En el microcontrolador ATMEGA328P del Arduino se tienen 3 contadores de 2 números distintos de bits, que afecta en su velocidad de conteo:
	- T0/T2: Los dos contadores T0 y T2 son de 8 bits, osea que cuentan desde 0000 0000 hasta 1111 1111, teniendo como valor máximo el 255.
	- T1: El contador T1 es de 16 bits, osea que cuenta desde 0000 0000 0000 0000 hasta  1111 1111 1111 1111, teniendo como valor máximo el 65,536.
	Los contadores dependen de la frecuencia de su oscilador, que en el ATMEGA328P es de 16 MHz y de su escala que puede ser de:
		- 1:1
		- 1:8
		- 1:32
		- 1:64
		- 1:128
		- 1:256
		- 1:1024
	T0 y T1:
	La escala en específico es configurable a partir del registro TCCR#B del temporizador y sus bits CS#2, CS#1 y CS#0, donde se tiene las siguientes opciones:
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
	T2:
	La escala en específico es configurable a partir del registro TCCR#B del temporizador y sus bits CS#2, CS#1 y CS#0, donde se tiene las siguientes opciones:
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
		- Escala de 1:32 para el conteo del microcontrolador.
			- CS02 = 0
			- CS01 = 1
			- CS00 = 1
		- Escala de 1:64 para el conteo del microcontrolador.
			- CS02 = 1
			- CS01 = 0
			- CS00 = 0
		- Escala de 1:128 para el conteo del microcontrolador.
			- CS02 = 1
			- CS01 = 0
			- CS00 = 1
		- Escala de 1:256 para el conteo del microcontrolador.
			- CS02 = 1
			- CS01 = 1
			- CS00 = 0
		- Escala de 1:1024 para el conteo del microcontrolador.
			- CS02 = 1
			- CS01 = 1
			- CS00 = 1
	Recordemos que la fórmula para obtener el tiempo total de cada desbordamiento en el contador del microcontrolador es la siguiente, tomando en cuenta la frecuencia del 
	oscilador, el tiempo que toma cada ciclo de máquina, la escala y el número de conteo que depende del número de bits del contador, ya sea de 8 o 16 bits.
	Escala*CM*Número de conteo = Escala*(1/f)*2^#bits del contador = TM (tiempo máximo de conteo por ciclo).*/
	//TCCR0B o registro de control del temporizador: Sirve para indicar la configuración antes mencionada en el temporizador.
	TCCR0B |= (1<<CS02)|(0<<CS01)|(1<<CS00); //T0: Escala de 1024; CS02 = 1, CS01 = 0 y CS00 = 1
	TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10); //T1: Escala de 1024; CS12 = 1, CS11 = 0 y CS10 = 1
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20); //T2: Escala de 1024; CS22 = 1, CS21 = 1 y CS20 = 1, no se puede usar osciladores externos con T2, se maneja diferente.
	
	//TIMSK o registro de control del temporizador: Sirve para indicar que el temporizador sea de tipo desbordamiento o comparación.
	/*Los bits TOIE2, TOIE1 Y TOIE0 sirven para indicar cuál de los temporizadores T2, T1 o T0 queremos que sea de tipo desbordamiento, para que pueda ser de
	ese tipo se debe mandar un 1 lógico a través de cada uno de ellos:
		- TOIE2: Hace que el timer T2 de 8 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		- TOIE1: Hace que el timer T1 de 16 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		- TOIE0: Hace que el timer T0 de 8 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.*/
	TIMSK0 |= (1<<TOIE0); //Timer T0 de 8 bits habilitado como tipo desbordamiento.
	TIMSK1 |= (1<<TOIE1); //Timer T1 de 16 bits habilitado como tipo desbordamiento.
	TIMSK2 |= (1<<TOIE2); //Timer T2 de 8 bits habilitado como tipo desbordamiento.
	
	//MANEJO DEL TEMPORIZADOR: Si se quiere cambiar el inicio de conteo de un temporizador se puede modificar el estado inicial de su registro TCNT#
	//TCNT#: Registro que maneja el conteo de un temporizador, su conteo va desde 0 hasta 255, después de contar el número 255 ocurre el desbordamiento y vuelve a valer 0.
	/*Se ejecuta el siguiente procedimiento, tomando en cuenta la frecuencia del microcontrolador, la escala y el tiempo que se quiere alcanzar:
		1.- (f_micro*t_deseado)/escala = conteo_aux
		2.- conteo_total_temporizador - conteo_aux = conteo_TCNT
			conteo_total_temporizador T0 y T2	= 255
			conteo_total_temporizador T1		= 65,536
	Este número se redondea y es el que se debe poner en los registros TCNT.
	Cuando: conteo_aux > conteo_total_temporizador, lo que se debe hacer es la siguiente operación, redondeando el valor de ciclos, el valor de los ciclos igual se tendrá 
	que utilizar en un condicional if cuando se declare la función ISR del temporizador que usó la variable:
		2.1.- ciclos = conteo_aux/conteo_total_temporizador = round(resultado); El redondeo se hace hacia arriba siempre.
		2.2.- conteo_total_temporizador - (conteo_aux/ciclos) = conteo_TCNT*/
	
	/*T0 - Parpadeo de 250 ms:
		1.- (f_micro*t_deseado)/escala = (16X10^6*0.250)/1024 = 3,906.25
	conteo_aux > conteo_total_temporizador = 3,906.25 > 255
		2.1.- ciclos = 3,906.25/255 = 15.3186 = 16
		2.2.- 255 - (3906.25/16) = 10.8593
	Este número se redondea y es el que se debe poner en el registro TCNT0.*/
	TCNT0 = 11; //t_deseado = 250ms; escala = 1024; conteo_TCNT = 10.8593; ciclos = 16
	
	/*T1 - Parpadeo de 1 s:
		1.- (f_micro*t_deseado)/escala = (16X10^6*1)/1024 = 15,625
		2.- 65,536 - 15,625 = 49,911
	Este número se redondea y es el que se debe poner en el registro TCNT1.*/
	TCNT1 = 49911; //t_deseado = 1s; escala = 1024; conteo_TCNT = 49911.
	
	/*T2 - Parpadeo de 2 s:
		1.- (f_micro*t_deseado)/escala = (16X10^6*2)/1024 = 31250
	conteo_aux > conteo_total_temporizador = 31250 > 255
		2.1.- ciclos = 31250/255 = 122.5490 = 125
		2.2.- 255 - (31250/125) = 5
	Este número se redondea y es el que se debe poner en el registro TCNT2.*/
	TCNT2 = 5; //t_deseado = 2; escala = 1024; conteo_TCNT = 5; ciclos = 125
	
	//GTCCR: Una vez configurados los timers, se debe poner como cero el registro que reinicia y sincroniza los timers, liberando así los timers.
	GTCCR = (0<<TSM)|(0<<PSRASY)|(0<<PSRSYNC); //TSM -> Timers NO sincronizados; PSRASY -> Timer T2 NO reiniciado; PSRSYNC -> Timers T0 y T1 NO reiniciados.
	
	
	
	/*SEI: Cuando se activa una SEI interruption, automáticamente el bit 7 del status register (GI) se convierte en 1 lógico, lo que convierte a las interrupciones en NMI 
	(No Mask Interruption):
		- Cuando GI tiene valor de 1 lógico no está activada ninguna interrupción, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 lógico está activada la interrupción, osea que es MI, Maskable Interrupt.*/
	sei(); //Permite cualquier interrupción que entre ya después de haber programado la máquina y cuando haya terminado de ejecutarse una interrupción anterior.
	
	
	
	//EJECUCIÓN DEL PROGRAMA: Ya después de haber configurado las interrupciones es cuando se ejecuta la función del programa.
    while (1){
		//Dentro del while se debería poner una instrucción ajena a las interrupciones, pero en este caso no se pondrá nada.
    }
}



//DECLARACIÓN DE VARIABLES GLOBALES (CABECERAS DE FUNCIÓN): Estas se pueden poner después del método main, siempre y cuando no se usen dentro del mismo método main
/*Para poder imprimir un número en pantalla se debe conocer el formato que utiliza cada uno y los tipos primitivos de datos en C:
int:	Números enteros, utilizan un espacio de memoria de 2 o 4 bytes. Su formato es el %d o %i.
float:	Números fraccional con máximo 7 decimales después del punto, utilizan un espacio de memoria de 4 bytes. Su formato es el %f.
double:	Números fraccional con máximo 15 decimales después del punto, utilizan un espacio de memoria de 8 bytes. Su formato es el %lf.
char:	Caracater, representa una sola letra o valor ASCII, utilizan un espacio de memoria de 1 byte. Su formato es el %s.*/
int cliclos_transcurridos_t0 = 0;	//Variable que almacena el número de ciclos que se utilizarán en la función ISR del temporizador 0.
int cliclos_t0 = 16;				//Variable que almacena el número de ciclos que se utilizarán en la función ISR del temporizador 0.
int cliclos_transcurridos_t2 = 0;	//Variable que almacena el número de ciclos que se utilizarán en la función ISR del temporizador 2.
int cliclos_t2 = 125;				//Variable que almacena el número de ciclos que se utilizarán en la función ISR del temporizador 2.



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
//ISR T0 - 250 ms: Recibe como parámetro el vector TIMER0_OVF_vect porque la interrupción es activada por el desbordamiento del timer T0.
ISR (TIMER0_OVF_vect){
	//Dentro del ISR se vuelve a declarar el mismo valor de TCNT0 que se había calculado previamente.
	TCNT0 = 11; //t_deseado = 250 ms; escala = 1024; conteo_TCNT = 10.8593.
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operación del contador, se debe usar una variable global que cuente el número de ciclos cada vez que se ejecute una 
	interrupción y al llegar al número de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	cliclos_transcurridos_t0++; //Aumento de la variable que almacena el número de ciclos cada vez que se ejecuta una interrupción por el desborde del temporizador.
	
	if(cliclos_transcurridos_t0 >= cliclos_t0){
		//MÁSCARA XOR (^=): Deja pasar tal cual todo lo que esté en contacto con los 0 de la máscara e invierte todo lo que esté en contacto con los 1 de la máscara.
		PORTB ^= (1<<PORTB0);	//Con esta instrucción se invierte lo que haya en el pin PB0 cada que ocurra una interrupción causada por el temporizador T0.
		cliclos_transcurridos_t0 = 0; //Reinicio de la variable que almacena el número de ciclos transcurridos.
	}
}

//ISR T1 - 1s: Recibe como parámetro el vector TIMER1_OVF_vect porque la interrupción es activada por el desbordamiento del timer T1.
ISR (TIMER1_OVF_vect){
	//Dentro del ISR se vuelve a declarar el mismo valor de TCNT1 que se había calculado previamente.
	TCNT1 = 49911; //t_deseado = 1s; escala = 1024; conteo_TCNT = 49911.
	
	//PARPADEO DEL LED:
	//MÁSCARA XOR (^=): Deja pasar tal cual todo lo que esté en contacto con los 0 de la máscara e invierte todo lo que esté en contacto con los 1 de la máscara.
	PORTC ^= (1<<PORTC0); //Con esta instrucción se invierte lo que haya en el pin PB0 cada que ocurra una interrupción causada por el temporizador T0.
}

//ISR T2 - 2s: Recibe como parámetro el vector TIMER2_OVF_vect porque la interrupción es activada por el desbordamiento del timer T2.
ISR (TIMER2_OVF_vect){
	//Dentro del ISR se vuelve a declarar el mismo valor de TCNT2 que se había calculado previamente.
	TCNT2 = 5; //t_deseado = 2; escala = 1024; conteo_TCNT = 5; ciclos = 125
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operación del contador, se debe usar una variable global que cuente el número de ciclos cada vez que se ejecute una 
	interrupción y al llegar al número de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	cliclos_transcurridos_t2++; //Aumento de la variable que almacena el número de ciclos cada vez que se ejecuta una interrupción por el desborde del temporizador.
	
	if(cliclos_transcurridos_t2 >= cliclos_t2){
		//MÁSCARA XOR (^=): Deja pasar tal cual todo lo que esté en contacto con los 0 de la máscara e invierte todo lo que esté en contacto con los 1 de la máscara.
		PORTD ^= (1<<PORTD0);	//Con esta instrucción se invierte lo que haya en el pin PC0 cada que ocurra una interrupción causada por el temporizador T2.
		cliclos_transcurridos_t2 = 0; //Reinicio de la variable que almacena el número de ciclos transcurridos.
	}
}