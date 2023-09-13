/*
 * 7_Parpadeo_Led_Desborde.c
 *
 * Created: 27/05/2022 01:43:54 p. m.
 * Author : diego
 7.- TEMPORIZADOR POR DESBORDAMIENTO: Es un tipo de temporizador que activa banderas que desencadenan interrupciones cuando realizan un desbordamiento, esto se refiere a
 cuando el contador llega a su valor m�ximo y debe volver a contar desde cero.
 
 Para el ejercicio se configurar�n 3 parpadeos en 3 leds diferentes con tiempos de 250ms, 1.5s y 30s con los 3 timers T0, T1 y T2 respectivamente en modo de temporizador por 
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
 Pin 2	(PD2); INT0, Interrupci�n externa 0
 Pin 3	(PD3); INT1, Interrupci�n externa 1
 Pin 4	(PD4); T0, Temporizador 0
 Pin 5	(PD5); T1, Temporizador 1
 Pin 6	(PD6)
 Pin 7	(PD7)
 
 Cuando se desborden los 3 temporizadores, ejecutar� una funci�n ISR cada uno.

 Recordemos que en el proyecto se usa para las siguientes funciones los puertos:
 PUERTO C - LCD: Aunque en este caso se utilizar� el puerto PC0 para observar una de las se�ales de salida de los temporizadores CTC.
 Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
 DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada, por lo tanto solamente 6 bits en total contando los 2 de control son los que se conectan y son
 los siguientes:
 Arduino:	A5,		A4,		A3,		A2,		A1,		A0
 LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

 PUERTO D - TECLADO MATRICIAL 4X4: En este caso no se va a conectar, ya que se quiere usar el pin PD0 que tiene la salida de se�al cuadrada con frecuencia variable OCR2A.
 La asignaci�n de pines del Puerto D en el Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexi�n del teclado matricial vistos de izquierda
 a derecha son los siguientes:
 Puerto D Arduino (Digital PWM):		7,		6,		5,		4,		3,		2,		1,		0
 Constantes del teclado matricial 4X4:	F1,		F2,		F3,		F4,		C1,		C2,		C3,		C4
 Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8

 PUERTO B - DISPOSITIVOS EXTERNOS VARIADOS: Leds, botones, etc. Aunque en este caso se utilizar� el puerto PB0 para observar una de las se�ales de salida de los
 temporizadores CTC.*/



//IMPORTACI�N DE LIBRER�AS, DIRECTIVAS
#include <avr/io.h>			//Librer�a para todo el manejo de puertos y dem�s aspectos del microcontrolador AVR Atmega328P
#include <avr/interrupt.h>	//Librer�a para el manejo de los registros de interrupciones



//M�TODO MAIN O EJECUTABLE: Desde este m�todo se ejecuta todo el c�digo y es lo que diferenc�a la librer�a de un ejecutable.
/*Para configurar correctamente un programa que use interrupciones se utiliza la siguiente arquitectura en el programa:
	- CLI: Activaci�n de interrupciones asignando un valor 0 l�gico al bit GI, volviendo la interrupci�n de tipo MI.
	- Programaci�n de m�quina y configuraci�n de interrupciones con los registros TCCR0B (registro de control del temporizador B), TIMSK0 y OCR0A (registro de comparaci�n).
	- SEI: Desactivaci�n de interrupciones asignando un valor 1 l�gico al bit GI, volviendo la interrupci�n de tipo NMI.
	- Ejecuci�n del programa.
Con dicha arquitectura nos aseguramos de manejar propiamente los pines de los puertos que manejan las interrupciones, activ�ndolas y desactiv�ndolas cuando es necesario.*/
int main(void){
    /*CLI: Cuando se activa una CLI interruption, autom�ticamente el bit 7 del status register que es GI (General Interruption) se convierte en 0 l�gico, lo que convierte a 
	las interrupciones en MI (Maskable Interrupt), esto en t�rminos m�s simples significa que las interrupciones est�n activas:
		- Cuando GI tiene valor de 1 l�gico no est� activada ninguna interrupci�n, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 l�gico est� activada la interrupci�n, osea que es MI, Maskable Interrupt.*/
	cli(); //Evita cualquier interrupci�n que entre mientras ya se est� ejecutando una o antes de programar la m�quina, ya que termine, permite la ejecuci�n de interrupciones.
	
	
	
	//PROGRAMACI�N DE LA M�QUINA: Se indica por medio de los registros de funci�n espec�fica si los pines de los puertos ser�n entradas o salidas o el estado de los timers.
	//0: Pin como entrada
	//1: Pin como salida
	DDRB = (1<<DDB0); //Pin 8	(PB0) del Puerto B como salida, parpadeo de 250ms;	Temporizador T0.
	DDRC = (1<<DDC0); //Pin A0  (PC0) del Puerto B como salida, parpadeo de 1s;		Temporizador T1.
	DDRD = (1<<DDD0); //Pin 0	(PD0) del Puerto B como salida, parpadeo de 2s;		Temporizador T2.
	
	//GTCCR o registro general de control del temporizador y contador: Sirve para cuando se usa m�s de un timer, reiniciando sus conteos y escalas, logrando su sincronizaci�n.
	/*El registro GTCCR cuenta con varias herramientas para sincronizar los 3 timers:
		- TSM: Bit del registro GTCCR para la sincronizaci�n de timers, con 1 l�gico hace que todos los temporizadores empiecen a contar al mismo tiempo y no manejen tiempos 
		distintos, mientras que con 0 l�gico pueden empezar a contar cuando quieran pero manejar�n tiempos distintos.
		- PSRASY: Bit del registro GTCCR para el reinicio del timer T2, con 1 l�gico se reinicia el conteo de T2 a cero y su escala se pone en 0, con 0 l�gico deja al 
		temporizador T2 con el �ltimo valor de conteo y escala con el que se qued�.
		- PSRSYNC: Bit del registro GTCCR para la reiniciar el conteo de los temporizadores T0 y T1, con 1 l�gico se reinicia su valor y se pone su escala en 0, mientras que 
		con 0 l�gico se deja a los temporizadores T0 y T1 con el �ltimo valor de conteo que ten�an y escala con la que se quedaron.
	Cuando es usa m�s de un timer para manejo de tiempos, es necesario usar este registro, adem�s esto se debe hacer justo antes de configurar los timers, una vez 
	configurados, se debe volver este registro a que valga 0.
	No es tan importante usar el registro GTCCR cuando se usan interrupciones externas, pero cuando se hace con temporizadores si lo es, ya que puede evitar que falle el
	programa, adem�s cuando se usan varios temporizadores a la vez, deben usarse en Puertos diferentes, sino fallar�n*/
	GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC); //TSM -> Timers sincronizados; PSRASY -> Timer T2 reiniciado; PSRSYNC -> Timers T0 y T1 reiniciados.
	
	/*En el microcontrolador ATMEGA328P del Arduino se tienen 3 contadores de 2 n�meros distintos de bits, que afecta en su velocidad de conteo:
	- T0/T2: Los dos contadores T0 y T2 son de 8 bits, osea que cuentan desde 0000 0000 hasta 1111 1111, teniendo como valor m�ximo el 255.
	- T1: El contador T1 es de 16 bits, osea que cuenta desde 0000 0000 0000 0000 hasta  1111 1111 1111 1111, teniendo como valor m�ximo el 65,536.
	Los contadores dependen de la frecuencia de su oscilador, que en el ATMEGA328P es de 16 MHz y de su escala que puede ser de:
		- 1:1
		- 1:8
		- 1:32
		- 1:64
		- 1:128
		- 1:256
		- 1:1024
	T0 y T1:
	La escala en espec�fico es configurable a partir del registro TCCR#B del temporizador y sus bits CS#2, CS#1 y CS#0, donde se tiene las siguientes opciones:
		- Ninguna fuente de reloj: En esta opci�n el temporizador se encuentra detenido, no funciona, esto se hace para ahorrar energ�a en el microcontrolador.
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
		- Esta configuraci�n permite conectar un oscilador externo en el pin T0 (que puede ser un LM555) y que su conteo se ejecute en el flanco de bajada de la se�al de reloj.
			- CS02 = 1
			- CS01 = 1
			- CS00 = 0
		- Esta configuraci�n permite conectar un oscilador externo en el pin T0 (que puede ser un LM555) y que su conteo se ejecute en el flanco de subida de la se�al de reloj.
			- CS02 = 1
			- CS01 = 1
			- CS00 = 1
	T2:
	La escala en espec�fico es configurable a partir del registro TCCR#B del temporizador y sus bits CS#2, CS#1 y CS#0, donde se tiene las siguientes opciones:
		- Ninguna fuente de reloj: En esta opci�n el temporizador se encuentra detenido, no funciona, esto se hace para ahorrar energ�a en el microcontrolador.
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
	Recordemos que la f�rmula para obtener el tiempo total de cada desbordamiento en el contador del microcontrolador es la siguiente, tomando en cuenta la frecuencia del 
	oscilador, el tiempo que toma cada ciclo de m�quina, la escala y el n�mero de conteo que depende del n�mero de bits del contador, ya sea de 8 o 16 bits.
	Escala*CM*N�mero de conteo = Escala*(1/f)*2^#bits del contador = TM (tiempo m�ximo de conteo por ciclo).*/
	//TCCR0B o registro de control del temporizador: Sirve para indicar la configuraci�n antes mencionada en el temporizador.
	TCCR0B |= (1<<CS02)|(0<<CS01)|(1<<CS00); //T0: Escala de 1024; CS02 = 1, CS01 = 0 y CS00 = 1
	TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10); //T1: Escala de 1024; CS12 = 1, CS11 = 0 y CS10 = 1
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20); //T2: Escala de 1024; CS22 = 1, CS21 = 1 y CS20 = 1, no se puede usar osciladores externos con T2, se maneja diferente.
	
	//TIMSK o registro de control del temporizador: Sirve para indicar que el temporizador sea de tipo desbordamiento o comparaci�n.
	/*Los bits TOIE2, TOIE1 Y TOIE0 sirven para indicar cu�l de los temporizadores T2, T1 o T0 queremos que sea de tipo desbordamiento, para que pueda ser de
	ese tipo se debe mandar un 1 l�gico a trav�s de cada uno de ellos:
		- TOIE2: Hace que el timer T2 de 8 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.
		- TOIE1: Hace que el timer T1 de 16 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.
		- TOIE0: Hace que el timer T0 de 8 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.*/
	TIMSK0 |= (1<<TOIE0); //Timer T0 de 8 bits habilitado como tipo desbordamiento.
	TIMSK1 |= (1<<TOIE1); //Timer T1 de 16 bits habilitado como tipo desbordamiento.
	TIMSK2 |= (1<<TOIE2); //Timer T2 de 8 bits habilitado como tipo desbordamiento.
	
	//MANEJO DEL TEMPORIZADOR: Si se quiere cambiar el inicio de conteo de un temporizador se puede modificar el estado inicial de su registro TCNT#
	//TCNT#: Registro que maneja el conteo de un temporizador, su conteo va desde 0 hasta 255, despu�s de contar el n�mero 255 ocurre el desbordamiento y vuelve a valer 0.
	/*Se ejecuta el siguiente procedimiento, tomando en cuenta la frecuencia del microcontrolador, la escala y el tiempo que se quiere alcanzar:
		1.- (f_micro*t_deseado)/escala = conteo_aux
		2.- conteo_total_temporizador - conteo_aux = conteo_TCNT
			conteo_total_temporizador T0 y T2	= 255
			conteo_total_temporizador T1		= 65,536
	Este n�mero se redondea y es el que se debe poner en los registros TCNT.
	Cuando: conteo_aux > conteo_total_temporizador, lo que se debe hacer es la siguiente operaci�n, redondeando el valor de ciclos, el valor de los ciclos igual se tendr� 
	que utilizar en un condicional if cuando se declare la funci�n ISR del temporizador que us� la variable:
		2.1.- ciclos = conteo_aux/conteo_total_temporizador = round(resultado); El redondeo se hace hacia arriba siempre.
		2.2.- conteo_total_temporizador - (conteo_aux/ciclos) = conteo_TCNT*/
	
	/*T0 - Parpadeo de 250 ms:
		1.- (f_micro*t_deseado)/escala = (16X10^6*0.250)/1024 = 3,906.25
	conteo_aux > conteo_total_temporizador = 3,906.25 > 255
		2.1.- ciclos = 3,906.25/255 = 15.3186 = 16
		2.2.- 255 - (3906.25/16) = 10.8593
	Este n�mero se redondea y es el que se debe poner en el registro TCNT0.*/
	TCNT0 = 11; //t_deseado = 250ms; escala = 1024; conteo_TCNT = 10.8593; ciclos = 16
	
	/*T1 - Parpadeo de 1 s:
		1.- (f_micro*t_deseado)/escala = (16X10^6*1)/1024 = 15,625
		2.- 65,536 - 15,625 = 49,911
	Este n�mero se redondea y es el que se debe poner en el registro TCNT1.*/
	TCNT1 = 49911; //t_deseado = 1s; escala = 1024; conteo_TCNT = 49911.
	
	/*T2 - Parpadeo de 2 s:
		1.- (f_micro*t_deseado)/escala = (16X10^6*2)/1024 = 31250
	conteo_aux > conteo_total_temporizador = 31250 > 255
		2.1.- ciclos = 31250/255 = 122.5490 = 125
		2.2.- 255 - (31250/125) = 5
	Este n�mero se redondea y es el que se debe poner en el registro TCNT2.*/
	TCNT2 = 5; //t_deseado = 2; escala = 1024; conteo_TCNT = 5; ciclos = 125
	
	//GTCCR: Una vez configurados los timers, se debe poner como cero el registro que reinicia y sincroniza los timers, liberando as� los timers.
	GTCCR = (0<<TSM)|(0<<PSRASY)|(0<<PSRSYNC); //TSM -> Timers NO sincronizados; PSRASY -> Timer T2 NO reiniciado; PSRSYNC -> Timers T0 y T1 NO reiniciados.
	
	
	
	/*SEI: Cuando se activa una SEI interruption, autom�ticamente el bit 7 del status register (GI) se convierte en 1 l�gico, lo que convierte a las interrupciones en NMI 
	(No Mask Interruption):
		- Cuando GI tiene valor de 1 l�gico no est� activada ninguna interrupci�n, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 l�gico est� activada la interrupci�n, osea que es MI, Maskable Interrupt.*/
	sei(); //Permite cualquier interrupci�n que entre ya despu�s de haber programado la m�quina y cuando haya terminado de ejecutarse una interrupci�n anterior.
	
	
	
	//EJECUCI�N DEL PROGRAMA: Ya despu�s de haber configurado las interrupciones es cuando se ejecuta la funci�n del programa.
    while (1){
		//Dentro del while se deber�a poner una instrucci�n ajena a las interrupciones, pero en este caso no se pondr� nada.
    }
}



//DECLARACI�N DE VARIABLES GLOBALES (CABECERAS DE FUNCI�N): Estas se pueden poner despu�s del m�todo main, siempre y cuando no se usen dentro del mismo m�todo main
/*Para poder imprimir un n�mero en pantalla se debe conocer el formato que utiliza cada uno y los tipos primitivos de datos en C:
int:	N�meros enteros, utilizan un espacio de memoria de 2 o 4 bytes. Su formato es el %d o %i.
float:	N�meros fraccional con m�ximo 7 decimales despu�s del punto, utilizan un espacio de memoria de 4 bytes. Su formato es el %f.
double:	N�meros fraccional con m�ximo 15 decimales despu�s del punto, utilizan un espacio de memoria de 8 bytes. Su formato es el %lf.
char:	Caracater, representa una sola letra o valor ASCII, utilizan un espacio de memoria de 1 byte. Su formato es el %s.*/
int cliclos_transcurridos_t0 = 0;	//Variable que almacena el n�mero de ciclos que se utilizar�n en la funci�n ISR del temporizador 0.
int cliclos_t0 = 16;				//Variable que almacena el n�mero de ciclos que se utilizar�n en la funci�n ISR del temporizador 0.
int cliclos_transcurridos_t2 = 0;	//Variable que almacena el n�mero de ciclos que se utilizar�n en la funci�n ISR del temporizador 2.
int cliclos_t2 = 125;				//Variable que almacena el n�mero de ciclos que se utilizar�n en la funci�n ISR del temporizador 2.



//Funciones ISR: Tareas ejecutadas por las interrupciones externas, PCI (Pin Change) o Temporizadores.
/*ISR (TIMER0_OVF_vect): Se le llama ISR o Interruption Service Routine a la funci�n que describa lo que se tiene que hacer cuando sea activada una interrupci�n, la palabra 
ISR es una palabra reservada de interrupciones y el nombre de esta funci�n no debe ser decladada en ning�n lado del c�digo, simplemente por haber sido nombrada como ISR, se 
ejecuta cuando sea detonada la interrupci�n que recibe como par�metro, para ello el temporizador T0 ya debe haber sido configurado dentro del m�todo main. Para ello se le 
pasa como par�metro a la funci�n el vector TIMER0_OVF_vect para que ejecute la acci�n cuando el temporizador llegue al desbordamiento.*/
/*Las acciones de las interrupciones causadas por temporizadores de desbordamiento deben ser descritas en una funci�n fuera del m�todo main, estas reciben como par�metro lo 
siguiente:
	- TIMER0_OVF_vect: Interrupci�n causada por el temporizador T0 de 8 bits despu�s de haber sido configurado con los registros TCCR0B y TIMSK0, este est� asociado al pin 4 
	(PD4) del Puerto D en el Arduino, ya que se puede configurar el temporizador para recibir una se�al de reloj de un oscilador externo.
	- TIMER1_OVF_vect: Interrupci�n causada por el temporizador T1 de 16 bits despu�s de haber sido configurado con los registros TCCR0B y TIMSK0, este est� asociado al pin 5 
	(PD5) del Puerto D en el Arduino, ya que se puede configurar el temporizador para recibir una se�al de reloj de un oscilador externo.
	- TIMER2_OVF_vect: Interrupci�n causada por el temporizador T2 de 8 bits despu�s de haber sido configurado con los registros TCCR0B y TIMSK0, no cuenta con pin de 
	Arduino.*/
//ISR T0 - 250 ms: Recibe como par�metro el vector TIMER0_OVF_vect porque la interrupci�n es activada por el desbordamiento del timer T0.
ISR (TIMER0_OVF_vect){
	//Dentro del ISR se vuelve a declarar el mismo valor de TCNT0 que se hab�a calculado previamente.
	TCNT0 = 11; //t_deseado = 250 ms; escala = 1024; conteo_TCNT = 10.8593.
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operaci�n del contador, se debe usar una variable global que cuente el n�mero de ciclos cada vez que se ejecute una 
	interrupci�n y al llegar al n�mero de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	cliclos_transcurridos_t0++; //Aumento de la variable que almacena el n�mero de ciclos cada vez que se ejecuta una interrupci�n por el desborde del temporizador.
	
	if(cliclos_transcurridos_t0 >= cliclos_t0){
		//M�SCARA XOR (^=): Deja pasar tal cual todo lo que est� en contacto con los 0 de la m�scara e invierte todo lo que est� en contacto con los 1 de la m�scara.
		PORTB ^= (1<<PORTB0);	//Con esta instrucci�n se invierte lo que haya en el pin PB0 cada que ocurra una interrupci�n causada por el temporizador T0.
		cliclos_transcurridos_t0 = 0; //Reinicio de la variable que almacena el n�mero de ciclos transcurridos.
	}
}

//ISR T1 - 1s: Recibe como par�metro el vector TIMER1_OVF_vect porque la interrupci�n es activada por el desbordamiento del timer T1.
ISR (TIMER1_OVF_vect){
	//Dentro del ISR se vuelve a declarar el mismo valor de TCNT1 que se hab�a calculado previamente.
	TCNT1 = 49911; //t_deseado = 1s; escala = 1024; conteo_TCNT = 49911.
	
	//PARPADEO DEL LED:
	//M�SCARA XOR (^=): Deja pasar tal cual todo lo que est� en contacto con los 0 de la m�scara e invierte todo lo que est� en contacto con los 1 de la m�scara.
	PORTC ^= (1<<PORTC0); //Con esta instrucci�n se invierte lo que haya en el pin PB0 cada que ocurra una interrupci�n causada por el temporizador T0.
}

//ISR T2 - 2s: Recibe como par�metro el vector TIMER2_OVF_vect porque la interrupci�n es activada por el desbordamiento del timer T2.
ISR (TIMER2_OVF_vect){
	//Dentro del ISR se vuelve a declarar el mismo valor de TCNT2 que se hab�a calculado previamente.
	TCNT2 = 5; //t_deseado = 2; escala = 1024; conteo_TCNT = 5; ciclos = 125
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operaci�n del contador, se debe usar una variable global que cuente el n�mero de ciclos cada vez que se ejecute una 
	interrupci�n y al llegar al n�mero de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	cliclos_transcurridos_t2++; //Aumento de la variable que almacena el n�mero de ciclos cada vez que se ejecuta una interrupci�n por el desborde del temporizador.
	
	if(cliclos_transcurridos_t2 >= cliclos_t2){
		//M�SCARA XOR (^=): Deja pasar tal cual todo lo que est� en contacto con los 0 de la m�scara e invierte todo lo que est� en contacto con los 1 de la m�scara.
		PORTD ^= (1<<PORTD0);	//Con esta instrucci�n se invierte lo que haya en el pin PC0 cada que ocurra una interrupci�n causada por el temporizador T2.
		cliclos_transcurridos_t2 = 0; //Reinicio de la variable que almacena el n�mero de ciclos transcurridos.
	}
}