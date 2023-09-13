/*
 * 8_Parpadeo_Led_Comparacion_CTC.c
 *
 * Created: 27/05/2022 01:43:54 p. m.
 * Author : diego
 8.- TEMPORIZADOR POR COMPARACI�N - FRECUENCIAS DE UNA SE�AL CUADRADA: Este tipo de temporizador activa banderas que desencadenan interrupciones cuando el registro contador 
 TCNT# alcanza el valor de un registro OCR#A (Output Compare Register T# A) u OCR#B y se le llama por comparaci�n porque compara el valor del registro contador con alguno de 
 los registros comparadores y cuando tienen el mismo valor se dispara la interrupci�n, existen 3 tipos de comparadores:
	- CTC (Clear to Compare), PWM r�pidoy PWM fase correcta.
 Se utilizar� el modo CTC (Clear to Compare), que es un modo de comparaci�n donde cuando el registro temporizador alcanza el mismo nivel que el valor del registro de 
 comparaci�n, se reinicia a 0 el valor del temporizador, para volver a realizar el conteo desde el inicio, esto crea una se�al de diente de sierra en la salida, en la cual es 
 posible variar su frecuencia, adem�s es importante mencionar que en el modo CTC solo se puede usar el canal A, no el B. El modo comparador CTC crea una se�al de salida de 
 diente de sierra en el micro y posteriormente crea una se�al cuadrada en la salida, esa es la se�al de salida cuya frecuencia es variable.
 
 Se utilizar� este tipo de comparador ya que permite variar la frecuencia de la se�al de salida siguiendo la f�rmula descrita a continuaci�n:
 frecuencia = fmicro / (2*escala*(1+OCR#A)) = (16X10^6) / (2*escala*(1+OCR#A))

 Para el ejercicio se configurar�n 3 se�ales cuadradas con tres se�ales distintas en 3 leds diferentes con frecuencias de 1Hz, 1kHz y 0.5Hz con los 3 timers T0, T1 y T2
 respectivamente en modo de temporizador por comparaci�n CTC:
 T0: Frecuencia de 1kHz;	T = 1ms
 T1: Frecuencia de 1Hz;		T = 1s
 T2: Frecuencia de 0.5Hz;	T = 2s

 Para el ejercicio se utilizar�n los puertos: PB0 (pin 8 puerto B), PC0 (pin A0 puerto C) y PD0 (pin 0 puerto D).
 - Puerto B, Tiene 6 pines disponibles:
 Pin 8	(PB0); Led con frecuencia de 1kHz;	T = 1ms
 Pin 9	(PB1); OCR1A, Se�al de salida con frecuencia variable del timer T1, temporizador por comparaci�n canal A
 Pin 10	(PB2); OCR1B, Se�al de salida con frecuencia variable del timer T1, temporizador por comparaci�n canal B
 Pin 11	(PB3); OCR2A, Se�al de salida con frecuencia variable del timer T2, temporizador por comparaci�n canal A
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
 Pin 2	(PD2); INT0, Interrupci�n externa 0
 Pin 3	(PD3); INT1, Interrupci�n externa 1; OCR2B, Se�al de salida con frecuencia variable del timer T2, temporizador por comparaci�n canal B
 Pin 4	(PD4); T0, Temporizador 0
 Pin 5	(PD5); T1, Temporizador 1; OCR0B, Se�al de salida con frecuencia variable del timer T0, temporizador por comparaci�n canal B
 Pin 6	(PD6); OCR0A, Se�al de salida con frecuencia variable del timer T0, temporizador por comparaci�n canal A
 Pin 7	(PD7)
 
 Cuando los registros TCNT# del contador alcancen el valor de los registros OCR#A, se ejecutar� la funci�n ISR, obteniendo la se�al de salida con frecuencia variable en el
 pin, es importante mencionar que la se�al del temporizador es un diente de sierra con frecuencia variable y la se�al de salida en OCR#A u OCR#B es una se�al cuadrada con
 la misma frecuencia que la se�al de diente de sierra, la forma de la se�al cuadrada obtenida es descrita despu�s usando el registro TCCR#A.
 
 Recordemos que en el proyecto se usan los puertos descritos a continuaci�n para las siguientes funciones:
 PUERTO C - LCD: Aunque en este caso se utilizar� el puerto PC0 para observar una de las se�ales de salida de los temporizadores CTC.
 Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
 DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada o conect�ndose a tierra, por lo tanto solamente 6 bits en total contando los 2 de control son los 
 que se conectan al Arduino y son los siguientes:
 Arduino:	A5,		A4,		A3,		A2,		A1,		A0
 LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

 PUERTO D - TECLADO MATRICIAL 4X4: En este caso no se va a conectar, ya que se quiere usar el pin PD0 que tiene la salida de se�al cuadrada con frecuencia variable OCR2A.
 La asignaci�n de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexi�n del teclado matricial vistos de izquierda
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
	- Programaci�n de m�quina y configuraci�n de interrupciones con los registros TCCR0A, TCCR0B (registro de control del temporizador A y B), TIMSK0 y OCR0A (registro de 
	comparaci�n).
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
	DDRB = (1<<DDB0); //Pin 8	(PB0) del Puerto B como salida, frecuencia de 1kHz;		T = 1ms;	Temporizador T0.
	DDRC = (1<<DDC0); //Pin A0  (PC0) del Puerto C como salida, frecuencia de 1Hz;		T = 1s;		Temporizador T1.
	DDRD = (1<<DDD0); //Pin 0	(PD0) del Puerto D como salida, frecuencia de 0.5Hz;	T = 2s;		Temporizador T2.
	
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
	programa, adem�s cuando se usan varios temporizadores a la vez, deben usarse en Puertos diferentes, sino fallar�n.*/
	GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC); //TSM -> Timers sincronizados; PSRASY -> Timer T2 reiniciado; PSRSYNC -> Timers T0 y T1 reiniciados.
	
	/*Los bits WGM#1 y WGM#0 del registro TCCR#A y el bit WGM#2 del registro TCCR#B se cambia solamente cuando se est� utilizando el temporizador por comparaci�n, ya que a 
	trav�s de �l se indica el tipo de comparaci�n que se va a efectuar:
	T0 y T2:
		- Normal: En esta configuraci�n se encuentra desactivado el modo de temporizador por comparaci�n y el timer.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Fase Correcta OCRB: Solo permite utilizar el comparador OCRB. No permite variar la frecuencia de la se�al. Genera una se�al triangular para motores CD.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- CTC (Clear to Compare): Modo de comparaci�n donde cuando el temporizador alcanza el mismo nivel que el valor de comparaci�n, se reinicia a 0 el valor del 
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la se�al y solo permite utilizar el comparador OCRA.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM R�pido OCRB: Solo permite utilizar el comparador OCRB. Tiene el doble de la frecuencia de la se�al PWM de fase correcta, ideal para servomotores.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- Reservado: .
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Fase Correcta OCRA: Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la se�al. Genera una se�al triangular para motores CD.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- Reservado: .
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM R�pido OCRA: Solo permite utilizar el comparador OCRA. Tiene el doble de la frecuencia de la se�al PWM de fase correcta, ideal para servomotores.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
	T1:
		- Normal: En esta configuraci�n se encuentra desactivado el modo de temporizador por comparaci�n y el timer.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Fase Correcta OCRB (8 bits): Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la se�al. Genera una se�al triangular para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM Fase Correcta OCRB (9 bits): Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la se�al. Genera una se�al triangular para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Fase Correcta OCRB (10 bits): Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la se�al. Genera una se�al triangular para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- CTC (Clear to Compare): Modo de comparaci�n donde cuando el temporizador alcanza el mismo nivel que el valor de comparaci�n, se reinicia a 0 el valor del
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la se�al y solo permite utilizar el comparador OCRA.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM R�pido OCRB (8 bits): Solo permite utilizar el comparador OCRA. Tiene el doble de la frecuencia de la se�al PWM de fase correcta, ideal para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM R�pido OCRB (9 bits): Solo permite utilizar el comparador OCRA. Tiene el doble de la frecuencia de la se�al PWM de fase correcta, ideal para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM R�pido OCRB (10 bits): Solo permite utilizar el comparador OCRA. Tiene el doble de la frecuencia de la se�al PWM de fase correcta, ideal para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- Fase y frecuencia correcta (ICR1): Solo permite utilizar el comparador ICR1. Tiene la frecuencia y fase correcta de la se�al PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- Fase y frecuencia correcta (OCR1A): Solo permite utilizar el comparador ICR1. Tiene la frecuencia y fase correcta de la se�al PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM fase correcta (ICR1): Solo permite utilizar el comparador ICR1. Tiene la fase correcta de la se�al PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM fase correcta (OCR1A): Solo permite utilizar el comparador ICR1. Tiene la fase correcta de la se�al PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- CTC (Clear to Compare) ICR1: Modo de comparaci�n donde cuando el temporizador alcanza el mismo nivel que el valor de comparaci�n, se reinicia a 0 el valor del
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la se�al y solo permite utilizar el comparador ICR1.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- Reservado: .
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM R�pido ICR1: Solo permite utilizar el comparador ICR1. No permite variar la frecuencia de la se�al.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM R�pido OCRA: Solo permite utilizar el comparador OCRA. No permite variar la frecuencia de la se�al.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
	En este caso se utilizar� la comparaci�n CTC:
	Recordemos que lo importante del temporizador por comparaci�n CTC es que la se�al de diente de sierra original se puede modificar, cambiando la amplitud y frecuencia, la 
	f�rmula para obtener la frecuencia deseada en la se�al de salida representada por el registro OCR#A es la siguiente, tomando en cuenta la frecuencia del oscilador, la 
	escala y el l�mite del temporizador por comparaci�n asociado al registro OCR#A:
	frecuencia = fmicro / (2*escala*(1+OCR#A)) = (16X10^6) / (2*escala*(1+OCR#A)).*/
	
	/*TCCR0A o registro de control del temporizador A: Sirve para indicar el tipo de comparaci�n en el temporizador T#, accede a los bits WMG#1 y WMG#0, adem�s por medio de 
	los bits COM#A1 y COM#A0 indica el tipo de se�al obtenida en la salida OCR#A.*/
	/*Recordemos que la se�al del temporizador es un diente de sierra con frecuencia variable y la se�al de salida en OCR#A u OCR#B es una se�al cuadrada con la misma 
	frecuencia que la se�al de diente de sierra, la forma de la se�al cuadrada obtenida es descrita despu�s usando el registro TCCR#A, las distintas salidas de se�al cuadrada 
	que se puede obtener en la salida del comparador dictadas por los bits COM#A1 y COM#A0 son descritas a continuaci�n:
		- Operaci�n normal del puerto: OC0 desconectado, osea que no crea una se�al cuadrada de salida a partir de la se�al de diente de sierra del temporizador.
			- COM0A1 = 0
			- COM0A0 = 0
		- Cambio l�gico de OC0: Con cualquier cambio l�gico de 0 ? 1 o 1 ? 0 en la se�al de diente de sierra se ejecuta la interrupci�n, creando la se�al cuadrada que cambia 
		de 0 a 1 o 1 a 0 cuando haya un cambio de valor m�ximo a valor m�nimo o m�nimo a m�ximo en la se�al original.
			- COM0A1 = 0
			- COM0A0 = 1
		- Cero l�gico en OC0: La se�al cuadrada empieza valiendo 1 l�gico y cuando se ejecute la interrupci�n en la se�al de diente de sierra, cambia de 1 a 0 en la se�al 
		cuadrada y posteriormente cambia de 0 a 1 cuando se encuentre con otra interrupci�n. Esta es la que m�s se elige para obtener una se�al com frecuencia variable.
			- COM0A1 = 1
			- COM0A0 = 0
		- Uno l�gico en OC0: La se�al cuadrada empieza valiendo 0 l�gico y cuando se ejecute la interrupci�n en la se�al de diente de sierra, cambia de 0 a 1 en la se�al
		cuadrada y posteriormente cambia de 1 a 0  cuando se encuentre con otra interrupci�n. Este es el inverso de la configuraci�n pasada.
			- COM0A1 = 1
			- COM0A0 = 1
	La salida determinada con los pines del registro TCCR0A se ver� en el pin 6	(PD6) del Puerto D, conectada a la salida OCR0A.*/
	//TCCR#A o registro de control del temporizador A: Sirve para indicar el tipo de salida y tipo de comparaci�n en el temporizador T#, accede a los bits WMG#1 y WMG#0.
	//TIPO DE SE�AL DE SALIDA CUADRADA:
	TCCR0A |= (1<<COM0A1)|(1<<COM0A0);	//Configuraci�n Uno l�gico en OC0 (Osea la se�al de salida OCR#A y OCR#B); COM0A1 = 1, COM0A0 = 1
	TCCR1A |= (1<<COM1A1)|(1<<COM1A0);	//Configuraci�n Uno l�gico en OC1 (Osea la se�al de salida OCR#A y OCR#B); COM1A1 = 1, COM1A0 = 1
	TCCR2A |= (1<<COM2A1)|(1<<COM2A0);	//Configuraci�n Uno l�gico en OC2 (Osea la se�al de salida OCR#A y OCR#B); COM2A1 = 1, COM2A0 = 1
	//TIPO DE COMPARADOR - PARTE 1/2:
	TCCR0A |= (1<<WGM01)|(0<<WGM00);	//Configuraci�n de comparador CTC; TCCR0B -> WMG02 = 0; TCCR0A -> WMG01 = 1 y WMG00 = 0
	TCCR1A |= (0<<WGM11)|(0<<WGM10);	//Configuraci�n de comparador CTC; TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 0; El timer T1 se maneja distinto.
	TCCR2A |= (1<<WGM21)|(0<<WGM20);	//Configuraci�n de comparador CTC; TCCR2B -> WMG02 = 0; TCCR2A -> WMG21 = 1 y WMG20 = 0
	
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
	La escala en espec�fico es configurable a partir del registro TCCR#B del temporizador y sus bits CS#2, CS#1 y CS#0, donde se tienen las siguientes opciones:
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
	
	-	La instrucci�n &= lo que hace es solo asignar el valor de un bit 0 de un n�mero binario sin afectar a los dem�s.
	-	La instrucci�n |= lo que hace es solo asignar el valor de un bit 1 de un n�mero binario sin afectar a los dem�s.*/
	//TCCR#B o registro de control del temporizador: Sirve para indicar la configuraci�n de escala y tipo de comparador.
	//ESCALA DEL TEMPORIZADOR:
	TCCR0B |= (0<<CS02)|(1<<CS01)|(1<<CS00)|(0<<WGM02);	//T0: Escala de 64;		CS02 = 0, CS01 = 1 y CS00 = 1
	TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10);			//T1: Escala de 256;	CS12 = 1, CS11 = 0 y CS10 = 0
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20)|(0<<WGM22);	//T2: Escala de 1024;	CS22 = 1, CS21 = 1 y CS20 = 1, no se puede usar un timer externo con T2, se maneja diferente.
	//TIPO DE COMPARADOR - PARTE 2/2:
	/*Aguas porque asignar ceros solos, normalmente causa problemas, si se quiere poner expl�citamente, debe estar incluido dentro de una declaraci�n con varios bits, en 
	espec�fico se encuentra declarada en los registros TCCR#B de arriba, por eso en esta parte se coment� y solo se dej� indicado para que fuera m�s entendible.*/
	//TCCR0B &= (0<<WGM02);						//Configuraci�n de comparador CTC; TCCR0B -> WMG02 = 0; TCCR0A -> WMG01 = 1 y WMG00 = 0
	TCCR1B |= (1<<WGM12)|(0<<WGM13);			//Configuraci�n de comparador CTC; TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 0; T1 se maneja distinto.
	//TCCR2B &= (0<<WGM22);						//Configuraci�n de comparador CTC; TCCR2B -> WMG22 = 0; TCCR2A -> WMG21 = 1 y WMG20 = 0
	
	
	//TIMSK o registro de control del temporizador: Sirve para indicar que el temporizador sea de tipo desbordamiento o comparaci�n e indicar el canal A o B del comparador.
	/*Los bits TOIE2, TOIE1 Y TOIE0 sirven para indicar cu�l de los temporizadores T2, T1 o T0 queremos que sea de tipo desbordamiento, para que pueda ser de ese tipo se debe 
	mandar un 1 l�gico a trav�s de cada uno de ellos y a trav�s del registro OCIE#A o OCIE#B se elige el canal activado del comparador mandando un 1 l�gico, aunque es 
	importante saber que en el comparador CTC solo se puede utilizar el canal A:
		- TOIE2: Hace que el timer T2 de 8 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.
		- TOIE1: Hace que el timer T1 de 16 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.
		- TOIE0: Hace que el timer T0 de 8 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.
		
		- OCIE2A: Hace que el timer T2 de 8 bits sea de tipo comparaci�n y que se active el registro OCR0A para marcar el l�mite del conteo.
		- OCIE1A: Hace que el timer T1 de 16 bits sea de tipo comparaci�n y que se active el registro OCR1A para marcar el l�mite del conteo.
		- OCIE0A: Hace que el timer T0 de 8 bits sea de tipo comparaci�n y que se active el registro OCR2A para marcar el l�mite del conteo.*/
	TIMSK0 |= (0<<TOIE0)|(1<<OCIE0A); //Timer T0 de 8 bits habilitado como tipo comparaci�n, registro de comparaci�n OCR0A activado. Solo se puede usar el canal A en CTC.
	TIMSK1 |= (0<<TOIE1)|(1<<OCIE1A); //Timer T1 de 16 bits habilitado como tipo comparaci�n, registro de comparaci�n OCR1A activado. Solo se puede usar el canal A en CTC.
	TIMSK2 |= (0<<TOIE2)|(1<<OCIE2A); //Timer T2 de 8 bits habilitado como tipo comparaci�n, registro de comparaci�n OCR2A activado. Solo se puede usar el canal A en CTC.
	
	//MANEJO DEL TEMPORIZADOR - FRECUENCIA: Si se quiere cambiar el inicio de conteo de un temporizador se puede modificar el estado inicial de su registro OCR#A.
	/*OCR#A: Registro que maneja el l�mite del conteo de un temporizador por comparaci�n, su l�mite va desde 0 hasta el l�mite del temporizador, cuando TCNT# y OCR#A valen lo 
	mismo, ocurre la interrupci�n. La desventaja con utilizar el temporizador en comparaci�n, es que hay tiempos que no se pueden alcanzar, ya que si el valor de OCR#A 
	sobrepasa el l�mite del contador T0 y T2 que es 255 o el l�mite de T1 que es 65,536, no se va a poder ejecutar la interrupci�n.*/
	/*Se ejecuta el siguiente procedimiento, tomando en cuenta la frecuencia del microcontrolador, la escala, el valor m�ximo del timer y el tiempo que se quiere alcanzar:
		0.- frecuencia = fmicro/(2*escala*(1+OCR#A)) = (16X10^6)/(2*escala*(1+OCR#A))
		1.- OCR#A = (fmicro/(frecuencia*2*escala)) - 1 = (16X10^6/(frecuencia*2*escala)) - 1
	Se debe tomar en cuenta que las frecuencias mayores a las cuales pueden acceder los registros T0, T1 y T2 son las siguientes, eligiendo la escala m�s baja, ya que de esta
	manera se accede a mayores frecuencias, tomando en cuenta el conteo total de los temporizadores, adem�s es importante mencionar que frecuencias mayores se pueden obtener 
	por medio de los temporizadores de 8 bits y menores frecuencias con el temporizador de 16:
		conteo_total_temporizador T0 y T2 = 255
			f_max -> escala = 1; f = 31.25 kHz = 31250	 Hz
			f_max -> escala = 8; f = 3.906 kHz = 3906.25 Hz
		conteo_total_temporizador T1 = 65,536
			f_max -> escala = 1; f = 122.0684 Hz
			f_max -> escala = 8; f = 15.25855 Hz
	Este n�mero se redondea y es el que se debe poner en los registros OCR#A. Para ello se utiliza la f�rmula que describe el n�mero del registro comparador que se le debe 
	asignar para obtener como resultado una frecuencia espec�fica, tomando en cuenta el conteo total del temporizador, ya que el n�mero m�ximo que puede alcanzar el comparador
	es igual al que puede alcanzar el temporizador:
			conteo_total_temporizador T0 y T2	= 255		= OCR0A_max y OCR2A_max
			conteo_total_temporizador T1		= 65,536	= OCR1A_max
		1.- OCR#A = (fmicro/(frecuencia*2*escala)) - 1 = (16X10^6/(frecuencia*2*escala)) - 1
	Recordemos que para llegar a altas frecuencias, se debe poner bajas escalas y en este tipo de c�lculo lo que importa mucho son las escalas, es lo que m�s se mueve para 
	obtener una frecuencia espec�fica:
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
	resultado del registro OCR#A entre en el l�mite m�ximo de los contadores.
	Cuando: conteo_OCR > conteo_total_temporizador, no se puede usar el m�todo anterior y se debe calcular el n�mero de ciclos, el valor de los ciclos se utilizar� en un 
	condicional if dentro de la funci�n ISR del temporizador y dentro del registro comparador se debe poner el valor m�ximo del conteo del temporizador:
		1.1.- ciclos = conteo_OCR /conteo_total_temporizador
		1.2.- OCR#A = conteo_total_temporizador
	Al llegar a un resultado de ciclos, simplemente se deber� crear una variable global que tenga este valor y un condicional if que vaya aumentando otra variable que empiece
	a contar desde cero y que ejecute la acci�n de la interrupci�n solamente hasta que la variable haya alcanzado los ciclos necesarios.*/
	
	/*T0 - Parpadeo con frecuencia de 1kHz;		T = 1ms:
	Escalas disponibles:
		T0 y T1:
		- 1:1
		- 1:8
		- 1:64
		- 1:256
		- 1:1024
		En este caso al hacer varias iteraciones con todas las escalas, se eligi� la de 1:64 para alcanzar la frecuencia de 1X10^3 Hz = 1 kHz.
		1.- OCR#A = (16X10^6/(frecuencia*2*escala)) - 1 = (16X10^6/(1X10^3*2*64)) - 1 = 124
			conteo_total_temporizador T0 y T2	= 255		= OCR0A_max y OCR2A_max
			conteo_total_temporizador T1		= 65,536	= OCR1A_max
	En el registro comparador OCR0A se debe poner su conteo m�ximo que es de 255.*/
	OCR0A = 124; //frecuencia = 1kHz; t_deseado = 1ms; escala = 64; conteo_OCR = 124
	
	/*T1 - Parpadeo con frecuencia de 1Hz;		T = 1s:
	Escalas disponibles:
		T0 y T1:
		- 1:1
		- 1:8
		- 1:64
		- 1:256
		- 1:1024
		En este caso al hacer varias iteraciones con todas las escalas, se eligi� la de 1:256 para alcanzar la frecuencia de 1Hz.
		1.- OCR#A = (16X10^6/(frecuencia*2*escala)) - 1 = (16X10^6/(1*2*256)) - 1 = 31,249
		conteo_total_temporizador T0 y T2	= 255		= OCR0A_max y OCR2A_max
		conteo_total_temporizador T1		= 65,536	= OCR1A_max
		En el registro comparador OCR0A se debe poner su conteo m�ximo que es de 255.*/
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
		En este caso al hacer varias iteraciones con todas las escalas, se eligi� la de 1:1024 para alcanzar la frecuencia de 0.5Hz.
		1.- OCR#A = (16X10^6/(frecuencia*2*escala)) - 1 = (16X10^6/(0.5*2*1024)) - 1 = 15,624
		conteo_total_temporizador T0 y T2	= 255		= OCR0A_max y OCR2A_max
		conteo_total_temporizador T1		= 65,536	= OCR1A_max
	En el registro comparador OCR2A se debe poner su conteo m�ximo que es de 255, como en este caso es mayor a ese n�mero se calcula el n�mero de ciclos y se pone en el 
	registro OCR2A su valor m�ximo que es de 255:
		1.2.- ciclos = conteo_OCR#A/conteo_total_temporizador = 15,624/255 = 61.2705 = 62
		1.3.- conteo_OCR#A = 255
	En el registro comparador OCR2A se debe poner su conteo m�ximo que es de 255 y crear una variable que guarde el n�mero de ciclos calculados para usarse en la ISR.*/
	OCR2A = 255; //frecuencia = 1Hz; t_deseado = 2s; escala = 1024; conteo_OCR = 255; ciclos = 62
	
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
int cliclos_transcurridos_t2 = 0;	//Variable que almacena el n�mero de ciclos que se utilizar�n en la funci�n ISR del temporizador 2.
int cliclos_t2 = 62;				//Variable que almacena el n�mero de ciclos que se utilizar�n en la funci�n ISR del temporizador 2.



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
//ISR T0 - 1ms: Recibe como par�metro el vector TIMER0_COMPA_vect porque la interrupci�n es activada por la comparaci�n con el registro OCR0A y TCNT0 del timer T0.
ISR (TIMER0_COMPA_vect){
	/*Dentro del ISR cuando se usa el modo desbordamiento se debe reiniciar manualmente el valor de TCNT0 que se hab�a calculado previamente, pero cuando se usa el modo 
	comparador, no se debe hacer nada, ya que siempre y cuando: 
		OCR#A < conteo_total_temporizador
	Nadam�s se deja la acci�n que se quiere ejecutar cuando se alcance el valor m�ximo del conteo marcado por OCR#A.*/
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operaci�n del contador, se debe usar una variable global que cuente el n�mero de ciclos cada vez que se ejecute una 
	interrupci�n y al llegar al n�mero de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	//M�SCARA XOR: Deja pasar tal cual todo lo que est� en contacto con los 0 de la m�scara e invierte todo lo que est� en contacto con los 1 de la m�scara.
	PORTB ^= (1<<PORTB0);			//Con esta instrucci�n se invierte lo que haya en el pin PB0 cada que ocurra una interrupci�n causada por el temporizador T0.
}

//ISR T1 - 1s: Recibe como par�metro el vector TIMER1_COMPA_vect porque la interrupci�n es activada por la comparaci�n con el registro OCR1A y TCNT1 del timer T1.
ISR (TIMER1_COMPA_vect){
	/*Dentro del ISR cuando se usa el modo desbordamiento se debe reiniciar manualmente el valor de TCNT1 que se hab�a calculado previamente, pero cuando se usa el modo 
	comparador, no se debe hacer nada, ya que siempre y cuando: 
		OCR#A < conteo_total_temporizador
	Nadam�s se deja la acci�n que se quiere ejecutar cuando se alcance el valor m�ximo del conteo marcado por OCR#A.*/
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operaci�n del contador, se debe usar una variable global que cuente el n�mero de ciclos cada vez que se ejecute una 
	interrupci�n y al llegar al n�mero de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	//M�SCARA XOR: Deja pasar tal cual todo lo que est� en contacto con los 0 de la m�scara e invierte todo lo que est� en contacto con los 1 de la m�scara.
	PORTC ^= (1<<PORTC0);			//Con esta instrucci�n se invierte lo que haya en el pin PB0 cada que ocurra una interrupci�n causada por el temporizador T0.
}

//ISR T2 - 2s: Recibe como par�metro el vector TIMER2_COMPA_vect porque la interrupci�n es activada por la comparaci�n con el registro OCR2A y TCNT2 del timer T2.
ISR (TIMER2_COMPA_vect){
	/*Dentro del ISR cuando se usa el modo desbordamiento se debe reiniciar manualmente el valor de TCNT2 que se hab�a calculado previamente, pero cuando se usa el modo 
	comparador, no se debe hacer nada, ya que siempre y cuando: 
		OCR#A < conteo_total_temporizador
	Nadam�s se deja la acci�n que se quiere ejecutar cuando se alcance el valor m�ximo del conteo marcado por OCR#A, sino se debe calcular el n�mero de ciclos y crear un 
	condicional if.*/
	
	/*PARPADEO DEL LED: Cuando existan ciclos en la operaci�n del contador, se debe usar una variable global que cuente el n�mero de ciclos cada vez que se ejecute una 
	interrupci�n y al llegar al n�mero de ciclos calculados, haga el parpadeo y a su vez regrese el valor de la variable global a cero.*/
	cliclos_transcurridos_t2++; //Aumento de la variable que almacena el n�mero de ciclos cada vez que se ejecuta una interrupci�n por el desborde del temporizador.
	
	if(cliclos_transcurridos_t2 >= cliclos_t2){
		//M�SCARA XOR (^=): Deja pasar tal cual todo lo que est� en contacto con los 0 de la m�scara e invierte todo lo que est� en contacto con los 1 de la m�scara.
		PORTD ^= (1<<PORTD0);			//Con esta instrucci�n se invierte lo que haya en el pin PC0 cada que ocurra una interrupci�n causada por el temporizador T2.
		cliclos_transcurridos_t2 = 0;	//Reinicio de la variable que almacena el n�mero de ciclos transcurridos.
	}
}