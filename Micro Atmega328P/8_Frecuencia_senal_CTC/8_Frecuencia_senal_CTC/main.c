/*
 * 8_Parpadeo_Led_Comparacion_CTC.c
 *
 * Created: 27/05/2022 01:43:54 p. m.
 * Author : diego
 8.- TEMPORIZADOR POR COMPARACIÓN - FRECUENCIAS DE UNA SEÑAL CUADRADA: Este tipo de temporizador activa banderas que desencadenan interrupciones cuando el registro contador 
 TCNT# alcanza el valor de un registro OCR#A (Output Compare Register T# A) u OCR#B y se le llama por comparación porque compara el valor del registro contador con alguno de 
 los registros comparadores y cuando tienen el mismo valor se dispara la interrupción, existen 3 tipos de comparadores:
	- CTC (Clear to Compare), PWM rápidoy PWM fase correcta.
 Se utilizará el modo CTC (Clear to Compare), que es un modo de comparación donde cuando el registro temporizador alcanza el mismo nivel que el valor del registro de 
 comparación, se reinicia a 0 el valor del temporizador, para volver a realizar el conteo desde el inicio, esto crea una señal de diente de sierra en la salida, en la cual es 
 posible variar su frecuencia, además es importante mencionar que en el modo CTC solo se puede usar el canal A, no el B. El modo comparador CTC crea una señal de salida de 
 diente de sierra en el micro y posteriormente crea una señal cuadrada en la salida, esa es la señal de salida cuya frecuencia es variable.
 
 Se utilizará este tipo de comparador ya que permite variar la frecuencia de la señal de salida siguiendo la fórmula descrita a continuación:
 frecuencia = fmicro / (2*escala*(1+OCR#A)) = (16X10^6) / (2*escala*(1+OCR#A))

 Para el ejercicio se configurarán 3 señales cuadradas con tres señales distintas en 3 leds diferentes con frecuencias de 1Hz, 1kHz y 0.5Hz con los 3 timers T0, T1 y T2
 respectivamente en modo de temporizador por comparación CTC:
 T0: Frecuencia de 1kHz;	T = 1ms
 T1: Frecuencia de 1Hz;		T = 1s
 T2: Frecuencia de 0.5Hz;	T = 2s

 Para el ejercicio se utilizarán los puertos: PB0 (pin 8 puerto B), PC0 (pin A0 puerto C) y PD0 (pin 0 puerto D).
 - Puerto B, Tiene 6 pines disponibles:
 Pin 8	(PB0); Led con frecuencia de 1kHz;	T = 1ms
 Pin 9	(PB1); OCR1A, Señal de salida con frecuencia variable del timer T1, temporizador por comparación canal A
 Pin 10	(PB2); OCR1B, Señal de salida con frecuencia variable del timer T1, temporizador por comparación canal B
 Pin 11	(PB3); OCR2A, Señal de salida con frecuencia variable del timer T2, temporizador por comparación canal A
 Pin 12	(PB4)
 Pin 13	(PB5)
 - Puerto C, Tiene 6 pines disponibles:
 Pin A0	(PC0); Led con frecuencia de 1Hz;	T = 1s
 Pin A1	(PC1)
 Pin A2	(PC2)
 Pin A3	(PC3)
 Pin A4	(PC4)
 Pin A5	(PC5)
 - Puerto D, Tiene 8 pines disponibles:
 Pin 0	(PD0); Led con frecuencia de 0.5Hz;	T = 2s
 Pin 1	(PD1)
 Pin 2	(PD2); INT0, Interrupción externa 0
 Pin 3	(PD3); INT1, Interrupción externa 1; OCR2B, Señal de salida con frecuencia variable del timer T2, temporizador por comparación canal B
 Pin 4	(PD4); T0, Temporizador 0
 Pin 5	(PD5); T1, Temporizador 1; OCR0B, Señal de salida con frecuencia variable del timer T0, temporizador por comparación canal B
 Pin 6	(PD6); OCR0A, Señal de salida con frecuencia variable del timer T0, temporizador por comparación canal A
 Pin 7	(PD7)
 
 Cuando los registros TCNT# del contador alcancen el valor de los registros OCR#A, se ejecutará la función ISR, obteniendo la señal de salida con frecuencia variable en el
 pin, es importante mencionar que la señal del temporizador es un diente de sierra con frecuencia variable y la señal de salida en OCR#A u OCR#B es una señal cuadrada con
 la misma frecuencia que la señal de diente de sierra, la forma de la señal cuadrada obtenida es descrita después usando el registro TCCR#A.
 
 Recordemos que en el proyecto se usan los puertos descritos a continuación para las siguientes funciones:
 PUERTO C - LCD: Aunque en este caso se utilizará el puerto PC0 para observar una de las señales de salida de los temporizadores CTC.
 Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
 DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada o conectándose a tierra, por lo tanto solamente 6 bits en total contando los 2 de control son los 
 que se conectan al Arduino y son los siguientes:
 Arduino:	A5,		A4,		A3,		A2,		A1,		A0
 LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

 PUERTO D - TECLADO MATRICIAL 4X4: En este caso no se va a conectar, ya que se quiere usar el pin PD0 que tiene la salida de señal cuadrada con frecuencia variable OCR2A.
 La asignación de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexión del teclado matricial vistos de izquierda
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
	- Programación de máquina y configuración de interrupciones con los registros TCCR0A, TCCR0B (registro de control del temporizador A y B), TIMSK0 y OCR0A (registro de 
	comparación).
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
	DDRB = (1<<DDB0); //Pin 8	(PB0) del Puerto B como salida, frecuencia de 1kHz;		T = 1ms;	Temporizador T0.
	DDRC = (1<<DDC0); //Pin A0  (PC0) del Puerto C como salida, frecuencia de 1Hz;		T = 1s;		Temporizador T1.
	DDRD = (1<<DDD0); //Pin 0	(PD0) del Puerto D como salida, frecuencia de 0.5Hz;	T = 2s;		Temporizador T2.
	
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
	programa, además cuando se usan varios temporizadores a la vez, deben usarse en Puertos diferentes, sino fallarán.*/
	GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC); //TSM -> Timers sincronizados; PSRASY -> Timer T2 reiniciado; PSRSYNC -> Timers T0 y T1 reiniciados.
	
	/*Los bits WGM#1 y WGM#0 del registro TCCR#A y el bit WGM#2 del registro TCCR#B se cambia solamente cuando se está utilizando el temporizador por comparación, ya que a 
	través de él se indica el tipo de comparación que se va a efectuar:
	T0 y T2:
		- Normal: En esta configuración se encuentra desactivado el modo de temporizador por comparación y el timer.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Fase Correcta OCRB: Solo permite utilizar el comparador OCRB. No permite variar la frecuencia de la señal. Genera una señal triangular para motores CD.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- CTC (Clear to Compare): Modo de comparación donde cuando el temporizador alcanza el mismo nivel que el valor de comparación, se reinicia a 0 el valor del 
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la señal y solo permite utilizar el comparador OCRA.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Rápido OCRB: Solo permite utilizar el comparador OCRB. Tiene el doble de la frecuencia de la señal PWM de fase correcta, ideal para servomotores.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- Reservado: .
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Fase Correcta OCRA: Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la señal. Genera una señal triangular para motores CD.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- Reservado: .
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Rápido OCRA: Solo permite utilizar el comparador OCRA. Tiene el doble de la frecuencia de la señal PWM de fase correcta, ideal para servomotores.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
	T1:
		- Normal: En esta configuración se encuentra desactivado el modo de temporizador por comparación y el timer.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Fase Correcta OCRB (8 bits): Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la señal. Genera una señal triangular para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM Fase Correcta OCRB (9 bits): Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la señal. Genera una señal triangular para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Fase Correcta OCRB (10 bits): Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la señal. Genera una señal triangular para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- CTC (Clear to Compare): Modo de comparación donde cuando el temporizador alcanza el mismo nivel que el valor de comparación, se reinicia a 0 el valor del
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la señal y solo permite utilizar el comparador OCRA.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Rápido OCRB (8 bits): Solo permite utilizar el comparador OCRA. Tiene el doble de la frecuencia de la señal PWM de fase correcta, ideal para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM Rápido OCRB (9 bits): Solo permite utilizar el comparador OCRA. Tiene el doble de la frecuencia de la señal PWM de fase correcta, ideal para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Rápido OCRB (10 bits): Solo permite utilizar el comparador OCRA. Tiene el doble de la frecuencia de la señal PWM de fase correcta, ideal para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- Fase y frecuencia correcta (ICR1): Solo permite utilizar el comparador ICR1. Tiene la frecuencia y fase correcta de la señal PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- Fase y frecuencia correcta (OCR1A): Solo permite utilizar el comparador ICR1. Tiene la frecuencia y fase correcta de la señal PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM fase correcta (ICR1): Solo permite utilizar el comparador ICR1. Tiene la fase correcta de la señal PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM fase correcta (OCR1A): Solo permite utilizar el comparador ICR1. Tiene la fase correcta de la señal PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- CTC (Clear to Compare) ICR1: Modo de comparación donde cuando el temporizador alcanza el mismo nivel que el valor de comparación, se reinicia a 0 el valor del
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la señal y solo permite utilizar el comparador ICR1.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- Reservado: .
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM Rápido ICR1: Solo permite utilizar el comparador ICR1. No permite variar la frecuencia de la señal.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Rápido OCRA: Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la señal.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
	En este caso se utilizará la comparación CTC:
	Recordemos que lo importante del temporizador por comparación CTC es que la señal de diente de sierra original se puede modificar, cambiando la amplitud y frecuencia, la 
	fórmula para obtener la frecuencia deseada en la señal de salida representada por el registro OCR#A es la siguiente, tomando en cuenta la frecuencia del oscilador, la 
	escala y el límite del temporizador por comparación asociado al registro OCR#A:
	frecuencia = fmicro / (2*escala*(1+OCR#A)) = (16X10^6) / (2*escala*(1+OCR#A)).*/
	
	/*TCCR0A o registro de control del temporizador A: Sirve para indicar el tipo de comparación en el temporizador T#, accede a los bits WMG#1 y WMG#0, además por medio de 
	los bits COM#A1 y COM#A0 indica el tipo de señal obtenida en la salida OCR#A.*/
	/*Recordemos que la señal del temporizador es un diente de sierra con frecuencia variable y la señal de salida en OCR#A u OCR#B es una señal cuadrada con la misma 
	frecuencia que la señal de diente de sierra, la forma de la señal cuadrada obtenida es descrita después usando el registro TCCR#A, las distintas salidas de señal cuadrada 
	que se puede obtener en la salida del comparador dictadas por los bits COM#A1 y COM#A0 son descritas a continuación:
		- Operación normal del puerto: OC0 desconectado, osea que no crea una señal cuadrada de salida a partir de la señal de diente de sierra del temporizador.
			- COM0A1 = 0
			- COM0A0 = 0
		- Cambio lógico de OC0: Con cualquier cambio lógico de 0 ? 1 o 1 ? 0 en la señal de diente de sierra se ejecuta la interrupción, creando la señal cuadrada que cambia 
		de 0 a 1 o 1 a 0 cuando haya un cambio de valor máximo a valor mínimo o mínimo a máximo en la señal original.
			- COM0A1 = 0
			- COM0A0 = 1
		- Cero lógico en OC0: La señal cuadrada empieza valiendo 1 lógico y cuando se ejecute la interrupción en la señal de diente de sierra, cambia de 1 a 0 en la señal 
		cuadrada y posteriormente cambia de 0 a 1 cuando se encuentre con otra interrupción. Esta es la que más se elige para obtener una señal com frecuencia variable.
			- COM0A1 = 1
			- COM0A0 = 0
		- Uno lógico en OC0: La señal cuadrada empieza valiendo 0 lógico y cuando se ejecute la interrupción en la señal de diente de sierra, cambia de 0 a 1 en la señal
		cuadrada y posteriormente cambia de 1 a 0  cuando se encuentre con otra interrupción. Este es el inverso de la configuración pasada.
			- COM0A1 = 1
			- COM0A0 = 1
	La salida determinada con los pines del registro TCCR0A se verá en el pin 6	(PD6) del Puerto D, conectada a la salida OCR0A.*/
	//TCCR#A o registro de control del temporizador A: Sirve para indicar el tipo de salida y tipo de comparación en el temporizador T#, accede a los bits WMG#1 y WMG#0.
	//TIPO DE SEÑAL DE SALIDA CUADRADA:
	TCCR0A |= (1<<COM0A1)|(1<<COM0A0);	//Configuración Uno lógico en OC0 (Osea la señal de salida OCR#A y OCR#B); COM0A1 = 1, COM0A0 = 1
	TCCR1A |= (1<<COM1A1)|(1<<COM1A0);	//Configuración Uno lógico en OC1 (Osea la señal de salida OCR#A y OCR#B); COM1A1 = 1, COM1A0 = 1
	TCCR2A |= (1<<COM2A1)|(1<<COM2A0);	//Configuración Uno lógico en OC2 (Osea la señal de salida OCR#A y OCR#B); COM2A1 = 1, COM2A0 = 1
	//TIPO DE COMPARADOR - PARTE 1/2:
	TCCR0A |= (1<<WGM01)|(0<<WGM00);	//Configuración de comparador CTC; TCCR0B -> WMG02 = 0; TCCR0A -> WMG01 = 1 y WMG00 = 0
	TCCR1A |= (0<<WGM11)|(0<<WGM10);	//Configuración de comparador CTC; TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 0; El timer T1 se maneja distinto.
	TCCR2A |= (1<<WGM21)|(0<<WGM20);	//Configuración de comparador CTC; TCCR2B -> WMG02 = 0; TCCR2A -> WMG21 = 1 y WMG20 = 0
	
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
	La escala en específico es configurable a partir del registro TCCR#B del temporizador y sus bits CS#2, CS#1 y CS#0, donde se tienen las siguientes opciones:
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
	
	-	La instrucción &= lo que hace es solo asignar el valor de un bit 0 de un número binario sin afectar a los demás.
	-	La instrucción |= lo que hace es solo asignar el valor de un bit 1 de un número binario sin afectar a los demás.*/
	//TCCR#B o registro de control del temporizador: Sirve para indicar la configuración de escala y tipo de comparador.
	//ESCALA DEL TEMPORIZADOR:
	TCCR0B |= (0<<CS02)|(1<<CS01)|(1<<CS00)|(0<<WGM02);	//T0: Escala de 64;		CS02 = 0, CS01 = 1 y CS00 = 1
	TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10);			//T1: Escala de 256;	CS12 = 1, CS11 = 0 y CS10 = 0
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20)|(0<<WGM22);	//T2: Escala de 1024;	CS22 = 1, CS21 = 1 y CS20 = 1, no se puede usar un timer externo con T2, se maneja diferente.
	//TIPO DE COMPARADOR - PARTE 2/2:
	/*Aguas porque asignar ceros solos, normalmente causa problemas, si se quiere poner explícitamente, debe estar incluido dentro de una declaración con varios bits, en 
	específico se encuentra declarada en los registros TCCR#B de arriba, por eso en esta parte se comentó y solo se dejó indicado para que fuera más entendible.*/
	//TCCR0B &= (0<<WGM02);						//Configuración de comparador CTC; TCCR0B -> WMG02 = 0; TCCR0A -> WMG01 = 1 y WMG00 = 0
	TCCR1B |= (1<<WGM12)|(0<<WGM13);			//Configuración de comparador CTC; TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 0; T1 se maneja distinto.
	//TCCR2B &= (0<<WGM22);						//Configuración de comparador CTC; TCCR2B -> WMG22 = 0; TCCR2A -> WMG21 = 1 y WMG20 = 0
	
	
	//TIMSK o registro de control del temporizador: Sirve para indicar que el temporizador sea de tipo desbordamiento o comparación e indicar el canal A o B del comparador.
	/*Los bits TOIE2, TOIE1 Y TOIE0 sirven para indicar cuál de los temporizadores T2, T1 o T0 queremos que sea de tipo desbordamiento, para que pueda ser de ese tipo se debe 
	mandar un 1 lógico a través de cada uno de ellos y a través del registro OCIE#A o OCIE#B se elige el canal activado del comparador mandando un 1 lógico, aunque es 
	importante saber que en el comparador CTC solo se puede utilizar el canal A:
		- TOIE2: Hace que el timer T2 de 8 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		- TOIE1: Hace que el timer T1 de 16 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		- TOIE0: Hace que el timer T0 de 8 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		
		- OCIE2A: Hace que el timer T2 de 8 bits sea de tipo comparación y que se active el registro OCR0A para marcar el límite del conteo.
		- OCIE1A: Hace que el timer T1 de 16 bits sea de tipo comparación y que se active el registro OCR1A para marcar el límite del conteo.
		- OCIE0A: Hace que el timer T0 de 8 bits sea de tipo comparación y que se active el registro OCR2A para marcar el límite del conteo.*/
	TIMSK0 |= (0<<TOIE0)|(1<<OCIE0A); //Timer T0 de 8 bits habilitado como tipo comparación, registro de comparación OCR0A activado. Solo se puede usar el canal A en CTC.
	TIMSK1 |= (0<<TOIE1)|(1<<OCIE1A); //Timer T1 de 16 bits habilitado como tipo comparación, registro de comparación OCR1A activado. Solo se puede usar el canal A en CTC.
	TIMSK2 |= (0<<TOIE2)|(1<<OCIE2A); //Timer T2 de 8 bits habilitado como tipo comparación, registro de comparación OCR2A activado. Solo se puede usar el canal A en CTC.
	
	//MANEJO DEL TEMPORIZADOR - FRECUENCIA: Si se quiere cambiar el inicio de conteo de un temporizador se puede modificar el estado inicial de su registro OCR#A.
	/*OCR#A: Registro que maneja el límite del conteo de un temporizador por comparación, su límite va desde 0 hasta el límite del temporizador, cuando TCNT# y OCR#A valen lo 
	mismo, ocurre la interrupción. La desventaja con utilizar el temporizador en comparación, es que hay tiempos que no se pueden alcanzar, ya que si el valor de OCR#A 
	sobrepasa el límite del contador T0 y T2 que es 255 o el límite de T1 que es 65,536, no se va a poder ejecutar la interrupción.*/
	/*Se ejecuta el siguiente procedimiento, tomando en cuenta la frecuencia del microcontrolador, la escala, el valor máximo del timer y el tiempo que se quiere alcanzar:
		0.- frecuencia = fmicro/(2*escala*(1+OCR#A)) = (16X10^6)/(2*escala*(1+OCR#A))
		1.- OCR#A = (fmicro/(frecuencia*2*escala)) - 1 = (16X10^6/(frecuencia*2*escala)) - 1
	Se debe tomar en cuenta que las frecuencias mayores a las cuales pueden acceder los registros T0, T1 y T2 son las siguientes, eligiendo la escala más baja, ya que de esta
	manera se accede a mayores frecuencias, tomando en cuenta el conteo total de los temporizadores, además es importante mencionar que frecuencias mayores se pueden obtener 
	por medio de los temporizadores de 8 bits y menores frecuencias con el temporizador de 16:
		conteo_total_temporizador T0 y T2 = 255
			f_max -> escala = 1; f = 31.25 kHz = 31250	 Hz
			f_max -> escala = 8; f = 3.906 kHz = 3906.25 Hz
		conteo_total_temporizador T1 = 65,536
			f_max -> escala = 1; f = 122.0684 Hz
			f_max -> escala = 8; f = 15.25855 Hz
	Este número se redondea y es el que se debe poner en los registros OCR#A. Para ello se utiliza la fórmula que describe el número del registro comparador que se le debe 
	asignar para obtener como resultado una frecuencia específica, tomando en cuenta el conteo total del temporizador, ya que el número máximo que puede alcanzar el comparador
	es igual al que puede alcanzar el temporizador:
			conteo_total_temporizador T0 y T2	= 255		= OCR0A_max y OCR2A_max
			conteo_total_temporizador T1		= 65,536	= OCR1A_max
		1.- OCR#A = (fmicro/(frecuencia*2*escala)) - 1 = (16X10^6/(frecuencia*2*escala)) - 1
	Recordemos que para llegar a altas frecuencias, se debe poner bajas escalas y en este tipo de cálculo lo que importa mucho son las escalas, es lo que más se mueve para 
	obtener una frecuencia específica:
	T0 y T1:
		- 1:1
		- 1:8
		- 1:64
		- 1:256
		- 1:1024
	T2:
		- 1:1
		- 1:8
		- 1:32
		- 1:64
		- 1:128
		- 1:256
		- 1:1024
	Con el temporizador de 16 bits se llega a menores frecuencias, mientras que con los temporizadores de 8 bits se llega a frecuencias mayores, tomando en cuenta que el 
	resultado del registro OCR#A entre en el límite máximo de los contadores.
	Cuando: conteo_OCR > conteo_total_temporizador, no se puede usar el método anterior y se debe calcular el número de ciclos, el valor de los ciclos se utilizará en un 
	condicional if dentro de la función ISR del temporizador y dentro del registro comparador se debe poner el valor máximo del conteo del temporizador:
		1.1.- ciclos = conteo_OCR /conteo_total_temporizador
		1.2.- OCR#A = conteo_total_temporizador
	Al llegar a un resultado de ciclos, simplemente se deberá crear una variable global que tenga este valor y un condicional if que vaya aumentando otra variable que empiece
	a contar desde cero y que ejecute la acción de la interrupción solamente hasta que la variable haya alcanzado los ciclos necesarios.*/
	
	/*T0 - Parpadeo con frecuencia de 1kHz;		T = 1ms:
	Escalas disponibles:
		T0 y T1:
		- 1:1
		- 1:8
		- 1:64
		- 1:256
		- 1:1024
		En este caso al hacer varias iteraciones con todas las escalas, se eligió la de 1:64 para alcanzar la frecuencia de 1X10^3 Hz = 1 kHz.
		1.- OCR#A = (16X10^6/(frecuencia*2*escala)) - 1 = (16X10^6/(1X10^3*2*64)) - 1 = 124
			conteo_total_temporizador T0 y T2	= 255		= OCR0A_max y OCR2A_max
			conteo_total_temporizador T1		= 65,536	= OCR1A_max
	En el registro comparador OCR0A se debe poner su conteo máximo que es de 255.*/
	OCR0A = 124; //frecuencia = 1kHz; t_deseado = 1ms; escala = 64; conteo_OCR = 124
	
	/*T1 - Parpadeo con frecuencia de 1Hz;		T = 1s:
	Escalas disponibles:
		T0 y T1:
		- 1:1
		- 1:8
		- 1:64
		- 1:256
		- 1:1024
		En este caso al hacer varias iteraciones con todas las escalas, se eligió la de 1:256 para alcanzar la frecuencia de 1Hz.
		1.- OCR#A = (16X10^6/(frecuencia*2*escala)) - 1 = (16X10^6/(1*2*256)) - 1 = 31,249
		conteo_total_temporizador T0 y T2	= 255		= OCR0A_max y OCR2A_max
		conteo_total_temporizador T1		= 65,536	= OCR1A_max
		En el registro comparador OCR0A se debe poner su conteo máximo que es de 255.*/
	OCR1A = 31249; //frecuencia = 1Hz; t_deseado = 1s; escala = 256; conteo_OCR = 31,249
	
	/*T2 - Parpadeo con frecuencia de 0.5Hz;	T = 2s:
	Escalas disponibles:
		T0 y T1:
		- 1:1
		- 1:8
		- 1:32
		- 1:64
		- 1:128
		- 1:256
		- 1:1024
		En este caso al hacer varias iteraciones con todas las escalas, se eligió la de 1:1024 para alcanzar la frecuencia de 0.5Hz.
		1.- OCR#A = (16X10^6/(frecuencia*2*escala)) - 1 = (16X10^6/(0.5*2*1024)) - 1 = 15,624
		conteo_total_temporizador T0 y T2	= 255		= OCR0A_max y OCR2A_max
		conteo_total_temporizador T1		= 65,536	= OCR1A_max
	En el registro comparador OCR2A se debe poner su conteo máximo que es de 255, como en este caso es mayor a ese número se calcula el número de ciclos y se pone en el 
	registro OCR2A su valor máximo que es de 255:
		1.2.- ciclos = conteo_OCR#A/conteo_total_temporizador = 15,624/255 = 61.2705 = 62
		1.3.- conteo_OCR#A = 255
	En el registro comparador OCR2A se debe poner su conteo máximo que es de 255 y crear una variable que guarde el número de ciclos calculados para usarse en la ISR.*/
	OCR2A = 255; //frecuencia = 1Hz; t_deseado = 2s; escala = 1024; conteo_OCR = 255; ciclos = 62
	
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
int cliclos_transcurridos_t2 = 0;	//Variable que almacena el número de ciclos que se utilizarán en la función ISR del temporizador 2.
int cliclos_t2 = 62;				//Variable que almacena el número de ciclos que se utilizarán en la función ISR del temporizador 2.



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
//ISR T0 - 1ms: Recibe como parámetro el vector TIMER0_COMPA_vect porque la interrupción es activada por la comparación con el registro OCR0A y TCNT0 del timer T0.
ISR (TIMER0_COMPA_vect){
	/*Dentro del ISR cuando se usa el modo desbordamiento se debe reiniciar manualmente el valor de TCNT0 que se había calculado previamente, pero cuando se usa el modo 
	comparador, no se debe hacer nada, ya que siempre y cuando: 
		OCR#A < conteo_total_temporizador
	Nadamás se deja la acción que se quiere ejecutar cuando se alcance el valor máximo del conteo marcado por OCR#A.*/
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operación del contador, se debe usar una variable global que cuente el número de ciclos cada vez que se ejecute una 
	interrupción y al llegar al número de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	//MÁSCARA XOR: Deja pasar tal cual todo lo que esté en contacto con los 0 de la máscara e invierte todo lo que esté en contacto con los 1 de la máscara.
	PORTB ^= (1<<PORTB0);			//Con esta instrucción se invierte lo que haya en el pin PB0 cada que ocurra una interrupción causada por el temporizador T0.
}

//ISR T1 - 1s: Recibe como parámetro el vector TIMER1_COMPA_vect porque la interrupción es activada por la comparación con el registro OCR1A y TCNT1 del timer T1.
ISR (TIMER1_COMPA_vect){
	/*Dentro del ISR cuando se usa el modo desbordamiento se debe reiniciar manualmente el valor de TCNT1 que se había calculado previamente, pero cuando se usa el modo 
	comparador, no se debe hacer nada, ya que siempre y cuando: 
		OCR#A < conteo_total_temporizador
	Nadamás se deja la acción que se quiere ejecutar cuando se alcance el valor máximo del conteo marcado por OCR#A.*/
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operación del contador, se debe usar una variable global que cuente el número de ciclos cada vez que se ejecute una 
	interrupción y al llegar al número de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	//MÁSCARA XOR: Deja pasar tal cual todo lo que esté en contacto con los 0 de la máscara e invierte todo lo que esté en contacto con los 1 de la máscara.
	PORTC ^= (1<<PORTC0);			//Con esta instrucción se invierte lo que haya en el pin PB0 cada que ocurra una interrupción causada por el temporizador T0.
}

//ISR T2 - 2s: Recibe como parámetro el vector TIMER2_COMPA_vect porque la interrupción es activada por la comparación con el registro OCR2A y TCNT2 del timer T2.
ISR (TIMER2_COMPA_vect){
	/*Dentro del ISR cuando se usa el modo desbordamiento se debe reiniciar manualmente el valor de TCNT2 que se había calculado previamente, pero cuando se usa el modo 
	comparador, no se debe hacer nada, ya que siempre y cuando: 
		OCR#A < conteo_total_temporizador
	Nadamás se deja la acción que se quiere ejecutar cuando se alcance el valor máximo del conteo marcado por OCR#A, sino se debe calcular el número de ciclos y crear un 
	condicional if.*/
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operación del contador, se debe usar una variable global que cuente el número de ciclos cada vez que se ejecute una 
	interrupción y al llegar al número de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	cliclos_transcurridos_t2++; //Aumento de la variable que almacena el número de ciclos cada vez que se ejecuta una interrupción por el desborde del temporizador.
	
	if(cliclos_transcurridos_t2 >= cliclos_t2){
		//MÁSCARA XOR (^=): Deja pasar tal cual todo lo que esté en contacto con los 0 de la máscara e invierte todo lo que esté en contacto con los 1 de la máscara.
		PORTD ^= (1<<PORTD0);			//Con esta instrucción se invierte lo que haya en el pin PC0 cada que ocurra una interrupción causada por el temporizador T2.
		cliclos_transcurridos_t2 = 0;	//Reinicio de la variable que almacena el número de ciclos transcurridos.
	}
}