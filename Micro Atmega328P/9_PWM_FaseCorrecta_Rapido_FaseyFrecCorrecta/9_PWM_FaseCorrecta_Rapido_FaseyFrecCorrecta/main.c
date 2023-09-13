/*
 * 9_PWM_Fase_Correcta.c
 *
 * Created: 30/05/2022 11:09:18 a. m.
 * Author : diego
 9.- SEÑALES PWM: Este tipo de señales se crean a partir de temporizadores por comparación que desencadenan interrupciones cuando el registro contador TCNT# alcanza el valor 
 de un registro comparador OCR#A (Output Compare Register T# A) u OCR#B. Los modos de señales PWM tienen la gran diferencia con el tipo de comparador CTC que cuando se compara 
 el valor del registro contador con alguno de los registros comparadores y tienen el mismo valor, se dispara la interrupción, creando así un cambio de estado lógico en la 
 señal PWM, pero esto no significa que el valor del contador vuelva a ser cero, ya que el registro TCNT# seguirá contando hasta que se desborde, en ese entonces repetirá el 
 ciclo y creará la siguiente parte de la señal PWM, además el punto máximo del conteo donde se desborda puede ser modificado con un registro OCRA o ICR1, dependiendo del tipo 
 de comparador que se elija en la tabla de cada comparador, existen 3 tipos generales de señales PWM que se pueden crear con el modo comparador:
	-	PWM fase correcta: Realiza su cambio de valor lógico cuando TCNT# = OCR#A, pero no lleva a cabo un desborde cuando ocurre esta igualdad, sino que cuando llegue al 
		valor máximo del contador, decrementará su valor poco a poco hasta llegar nuevamente al cero. Creando así una señal triangular de baja frecuencia con conteo 
		ascendente y descendente, contando durante todo el pico del triángulo de la señal, logrando así que el valor lógico del duty cycle se encuentre en la parte media del 
		ciclo de la señal. NO se permite variar la frecuencia. Se recomienda usarse en aplicaciones de: Control de motores CD.
	-	PWM rápido o Fast PWM: Realiza su cambio de valor lógico cuando TCNT# = OCR#A, pero lleva a cabo su desborde cuando se llega al valor máximo del contador, volviendo a 
		contar desde cero. Crea una señal de diente de sierra de alta frecuencia y su conteo es ascendente solamente. Si se permite variar la frecuencia. Se recomienda usarse 
		en aplicaciones de: Rectificación de señales, regulación de tensión, conversión de señal digital a analógica, convertidor CD-CD, etc.
	-	PWM fase y frecuencia correcta: Es muy parecido al modo fase correcta, con la única diferencia de que si se permite variar la frecuencia, a fuerza se debe indicar con 
		algún registro ya sea OCRA o ICR1 el valor máximo del conteo y no se puede indicar la resolución, siempre será de 8 bits. Se recomienda usarse en aplicaciones de: 
		Control de motores CD.
 Cada uno de los modos de creación de señales PWM tienen sus propias fórmulas para calcular su frecuencia, periodo, tiempo de duty cycle, valor del registro comparador OCR#A u 
 OCR#B y valor TOP del máximo conteo almacenado en los registros OCRA o ICR1.

 Para el ejercicio se configurarán 3 señales PWM con tres modos de creación de señales PWM dtsintos que son: PWM fase correcta, PWM rápido y PWM fase y frecuencia correcta.
 T0: Frecuencia de 1kHz;	T = 1ms;	Fast PWM, ciclo de trabajo de 25% en OCR1A en modo no invertido y de 75% en OCR1B en modo no invertido
 T1: Frecuencia de 500 Hz;	T = 2ms;	Fast PWM, ciclo de trabajo de 15% en OCR1A en modo no invertido y de 50% en OCR1B en modo no invertido, modo ICR1 activado.
 T2: Frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta, 

 Para el ejercicio se utilizarán los puertos: PB0 (pin 8 puerto B), PC0 (pin A0 puerto C) y PD0 (pin 0 puerto D).
 - Puerto B, Tiene 6 pines disponibles:
 Pin 8	(PB0)
 Pin 9	(PB1); OCR1A, Señal de salida PWM del timer T1, temporizador por comparación canal A; Led con frecuencia de 1kHz; T = 1ms; PWM Fase correcta, Modo no inverso
 Pin 10	(PB2); OCR1B, Señal de salida PWM del timer T1, temporizador por comparación canal B; Led con frecuencia de 1kHz; T = 1ms; PWM Fase correcta, Modo inverso
 Pin 11	(PB3); OCR2A, Señal de salida PWM del timer T2, temporizador por comparación canal A
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
 Pin 0	(PD0); Led con frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta, 
 Pin 1	(PD1)
 Pin 2	(PD2); INT0, Interrupción externa 0
 Pin 3	(PD3); OCR2B, Señal de salida PWM del timer T2, temporizador por comparación canal B; INT1, Interrupción externa 1
 Pin 4	(PD4); T0, Temporizador 0
 Pin 5	(PD5); OCR0B, Señal de salida PWM del timer T0, temporizador por comparación canal B
 Pin 6	(PD6); OCR0A, Señal de salida PWM del timer T0, temporizador por comparación canal A
 Pin 7	(PD7)
 
 Recordemos que en el proyecto se usan los puertos descritos a continuación para las siguientes funciones:
 PUERTO C - LCD: Aunque en este caso se utilizará el puerto PC0 para observar una de las señales de salida PWM, la de Fast PWM.
 Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
 DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada o conectándose a tierra, por lo tanto solamente 6 bits en total contando los 2 de control son los 
 que se conectan al Arduino y son los siguientes:
 Arduino:	A5,		A4,		A3,		A2,		A1,		A0
 LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

 PUERTO D - TECLADO MATRICIAL 4X4: En este caso no se va a conectar, ya que se quiere usar el pin PD0 para observar la señal del modo PWM fase y frecuencia correcta.
 La asignación de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexión del teclado matricial vistos de izquierda
 a derecha son los siguientes:
 Puerto D Arduino (Digital PWM):		7,		6,		5,		4,		3,		2,		1,		0
 Constantes del teclado matricial 4X4:	F1,		F2,		F3,		F4,		C1,		C2,		C3,		C4
 Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8

 PUERTO B - DISPOSITIVOS EXTERNOS VARIADOS: Leds, botones, etc. Aunque en este caso se utilizará el puerto PB0 para observar una de las señales PWM Fase correcta.
 temporizadores CTC.*/



//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>			//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <avr/interrupt.h>	//Librería para el manejo de los registros de interrupciones



//Funciones ISR: Cuando se crea una señal PWM, estas no se utilizan.



//MÉTODO MAIN O EJECUTABLE: Desde este método se ejecuta todo el código y es lo que diferencía la librería de un ejecutable.
/*Para configurar correctamente un programa que use interrupciones se utiliza la siguiente arquitectura en el programa:
	- CLI: Activación de interrupciones asignando un valor 0 lógico al bit GI, volviendo la interrupción de tipo MI.
	- Programación de máquina y configuración de interrupciones con los registros TCCR#A, TCCR#B (registro de control del temporizador A y B), TIMSK#,  y OCR#B (registros de 
	comparación canal A y B).
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
	/*Cuando se usan señales PWM existen puertos específicos PWM por los cuales salen los canales A y B de los timers T0, T1 y T2:
	T0 - Pines PWM:
		PD6 (Pin 6):	OC0A, osea el canal A de la señal PWM del timer T0.
		PD5 (Pin 5):	OC0B, osea el canal B de la señal PWM del timer T0.
	T1 - Pines PWM:
		PB1 (Pin 9):	OC1A, osea el canal A de la señal PWM del timer T1.
		PB2 (Pin 10):	OC1B, osea el canal B de la señal PWM del timer T1.
	T2 - Pines PWM:
		PB3 (Pin 11):	OC2A, osea el canal A de la señal PWM del timer T2.
		PD3 (Pin 3):	OC2B, osea el canal B de la señal PWM del timer T2.
	Cuando se declaren los pines se deben declarar todos de jalón cuando pertenezcan a un mismo puerto sino la salida no se va a ver reflejada en el Arduino.*/
	//Temporizador T0: Pines 6 y 5	(PD6 y PD5) del Puerto D como salidas del canal A y B;		frecuencia de 500 Hz;	T = 2ms;	Fast PWM, Valor max OCR0A activado.
	//Temporizador T2: Pines 11 y 3	(PB3 y PD3) del Puerto B y D como salidas del canal A y B;	frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta.
	DDRD = (1<<DDD6)|(1<<DDD5)|(1<<DDD3);
	//Temporizador T1: Pines 9 y 10	(PB1 y PB2) del Puerto B como salidas del canal A y B;		frecuencia de 1 Hz;		T = 1s;		Fast PWM.
	DDRB = (1<<DDB1)|(1<<DDB2)|(1<<DDB3);
	//Temporizador T2: Pines 11 y 3	(PB3 y PD3) del Puerto B y D como salidas del canal A y B;	frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta.
	//DDRB = (1<<DDB3);
	//DDRB = (1<<DDB3);
	
	//GTCCR o registro general de control del temporizador y contador: Sirve para cuando se usa más de un timer, reiniciando sus conteos y escalas, logrando su sincronización.
	/*El registro GTCCR cuenta con varias herramientas para sincronizar los 3 timers:
		- TSM: Bit del registro GTCCR para la sincronización de timers, con 1 lógico hace que todos los temporizadores empiecen a contar al mismo tiempo y no manejen tiempos 
		distintos, mientras que con 0 lógico pueden empezar a contar cuando quieran pero manejarán tiempos distintos.
		- PSRASY: Bit del registro GTCCR para el reinicio del timer T2, con 1 lógico se reinicia el conteo de T2 a cero y su escala se pone en 0, con 0 lógico deja al 
		temporizador T2 con el último valor de conteo y escala con el que se quedó.
		- PSRSYNC: Bit del registro GTCCR para la reiniciar el conteo de los temporizadores T0 y T1, con 1 lógico se reinicia su valor y se pone su escala en 0, mientras que 
		con 0 lógico se deja a los temporizadores T0 y T1 con el último valor de conteo que tenían y escala con la que se quedaron.
	Cuando se usa más de un timer para manejo de tiempos, es necesario usar este registro, además esto se debe hacer justo antes de configurar los timers, una vez 
	configurados, se debe volver a hacer que este registro valga 0.
	No es tan importante usar el registro GTCCR cuando se usan interrupciones externas, pero cuando se usa temporizadores si lo es, ya que puede evitar que falle el programa, 
	además cuando se usan varios temporizadores a la vez, deben usarse en Puertos diferentes, sino fallarán.*/
	GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC); //TSM -> Timers sincronizados; PSRASY -> Timer T2 reiniciado; PSRSYNC -> Timers T0 y T1 reiniciados.
	
	/*Los bits WGM#1 y WGM#0 del registro TCCR#A y el bit WGM#2 del registro TCCR#B se cambian solamente cuando se está utilizando el temporizador por comparación, ya que a 
	través de él se indica el tipo de comparación que se va a efectuar, ya sea CTC o uno de los tipos PWM:
	T0 y T2:
		- Normal: En esta configuración se encuentra desactivado el modo de temporizador por comparación y el timer.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Fase Correcta: Permite usar ambos canales y su resolución es de 8 bits. No permite variar la frecuencia de la señal. Genera una señal triangular para motores CD.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 1 (TCCR#A)
		- CTC (Clear to Compare): Modo de comparación donde cuando el temporizador alcanza el mismo nivel que el valor de comparación, se reinicia a 0 el valor del 
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la señal y solo deja utilizar el comparador OCRA, con este valor se 
		fija el valor máximo al que llegará la señal de diente de sierra.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 1 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Rápido: Permite usar ambos canales y su resolución es de 8 bits. Tiene el doble de la frecuencia de la señal PWM de fase correcta. Genera una señal de diente de 
		sierra. Ideal para servomotores.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 1 (TCCR#A)
			- WMG#0 = 1 (TCCR#A)
		- Reservado: No se puede usar.
			- WMG#2 = 1 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Fase Correcta OCRA: Valor máximo personalizado con el registro OCRA, para utilizar la señal PWM se debe usar el canal B. No permite variar la frecuencia de la 
		señal. Genera una señal triangular. Ideal para motores CD.
			- WMG#2 = 1 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 1 (TCCR#A)
		- Reservado: No se puede usar.
			- WMG#2 = 1 (TCCR#B)
			- WMG#1 = 1 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Rápido OCRA: Valor máximo personalizado con el registro OCRA, para utilizar la señal PWM se debe usar el canal B. Tiene el doble de la frecuencia de la señal 
		PWM de fase correcta. Genera una señal de diente de sierra. Ideal para servomotores.
			- WMG#2 = 1 (TCCR#B)
			- WMG#1 = 1 (TCCR#A)
			- WMG#0 = 1 (TCCR#A)
	La resolución se refiere al número máximo al que puede contar el comparador OCR#A u OCR#B.
	T1:
		- Normal: En esta configuración se encuentra desactivado el modo de temporizador por comparación y el timer.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Fase Correcta (8 bits): Resolución de 8 bits. No permite variar la frecuencia de la señal. Genera una señal triangular, ideal para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM Fase Correcta (9 bits): Resolución de 9 bits. No permite variar la frecuencia de la señal. Genera una señal triangular, ideal para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Fase Correcta (10 bits): Resolución de 10 bits. No permite variar la frecuencia de la señal. Genera una señal triangular, ideal para motores CD.
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
		- PWM Rápido (8 bits): Resolución de 8 bits, osea que puede contar de 0 hasta 1111 1111. Tiene el doble de la frecuencia de la señal PWM de fase correcta, ideal para 
		servomotores y su valor máximo es el 0x00FF = 0000 0000 1111 1111, lo que indica que tiene una resolución de 8 bits, porque es el número máximo al que llega.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM Rápido (9 bits): Resolución de 9 bits, osea que puede contar de 0 hasta 0001 1111 1111. Tiene el doble de la frecuencia de la señal PWM de fase correcta, ideal 
		para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Rápido (10 bits): Resolución de 10 bits, osea que puede contar de 0 hasta 0011 1111 1111. Tiene el doble de la frecuencia de la señal PWM de fase correcta, 
		ideal para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- Fase y frecuencia correcta (ICR1): Valor máximo personalizado con el registro ICR1. Tiene la frecuencia y fase correcta de la señal PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- Fase y frecuencia correcta (OCR1A): Valor máximo personalizado con el registro OCR1A. Tiene la frecuencia y fase correcta de la señal PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM fase correcta (ICR1): Valor máximo personalizado con el registro ICR1. Tiene la fase correcta de la señal PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM fase correcta (OCR1A): Valor máximo personalizado con el registro OCR1A. Tiene la fase correcta de la señal PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- CTC (Clear to Compare) ICR1: Modo de comparación donde cuando el temporizador alcanza el mismo nivel que el valor de comparación, se reinicia a 0 el valor del
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la señal y solo permite utilizar el comparador ICR1 para hacer 0 el 
		registro contador TCNT.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- Reservado: No se puede usar.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM Rápido ICR1: Valor máximo personalizado con el registro ICR1. Permite variar la frecuencia de la señal.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Rápido OCRA: Valor máximo personalizado con el registro OCR1A. Permite variar la frecuencia de la señal.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
			
	Para T0 se utilizará la comparación - PWM Rápido (8 bits).
	Para T1 se utilizará la comparación - PWM Rápido (8 bits), el valor máximo que puede alcanzar es 0x00FF = 0000 0000 1111 1111, resolución de 8 bits.
	Para T2 se utilizará la comparación - PWM Rápido (8 bits).*/
	
	/*TCCR0A o registro de control del temporizador A: Sirve para indicar el tipo de comparación en el temporizador T#, accede a los bits WMG#1 y WMG#0, además por medio de 
	los bits COM#A1 y COM#A0 indica el tipo de señal obtenida en la salida OCR#A.*/
	/*Recordemos que la señal del temporizador es un diente de sierra con frecuencia variable y la señal de salida en OCR#A u OCR#B es una señal cuadrada con la misma 
	frecuencia que la señal de diente de sierra, la forma de la señal cuadrada obtenida es descrita después usando el registro TCCR#A, las distintas salidas de señal cuadrada 
	que se puede obtener en la salida del comparador dictadas por los bits COM#A1 y COM#A0 son descritas a continuación:
		- Operación normal del puerto: OC0 desconectado, osea que no crea una señal cuadrada de salida a partir de la señal de diente de sierra del temporizador.
			- COM0A1 = 0 (Canal A)
			- COM0A0 = 0 (Canal A)
			
			- COM0B1 = 0 (Canal B)
			- COM0B0 = 0 (Canal B)
		- Cambio lógico de OC0: Con cualquier cambio lógico de 0 a 1 o 1 a 0 en la señal de diente de sierra se ejecuta la interrupción, creando la señal cuadrada que cambia 
		de 0 a 1 o 1 a 0 cuando haya un cambio de valor máximo a valor mínimo o mínimo a máximo en la señal original.
			- COM0A1 = 0 (Canal A)
			- COM0A0 = 1 (Canal A)
			
			- COM0B1 = 0 (Canal B)
			- COM0B0 = 1 (Canal B)
		- Cero lógico en OC0 (Modo no invertido): La señal cuadrada empieza valiendo 1 lógico y cuando se ejecute la interrupción en la señal de diente de sierra, cambia de 1 
		a 0 en la señal cuadrada y posteriormente cambia de 0 a 1 cuando se encuentre con otra interrupción. Esta es la que más se elige para obtener una señal con frecuencia 
		variable. Para cuando se usa en señales PWM se llama Modo no invertido e implica que se use directamente el valor calculado de OCR#A o OCR#B.
			- COM0A1 = 1 (Canal A)
			- COM0A0 = 0 (Canal A)

			- COM0B1 = 1 (Canal B)
			- COM0B0 = 0 (Canal B)
		- Uno lógico en OC0 (Modo invertido): La señal cuadrada empieza valiendo 0 lógico y cuando se ejecute la interrupción en la señal de diente de sierra, cambia de 0 a 1 
		en la señal cuadrada y posteriormente cambia de 1 a 0  cuando se encuentre con otra interrupción. Este es el inverso de la configuración pasada.
		Para cuando se usa en señales PWM se llama Modo invertido e implica que cuando se calcule su duty cycle se haga la resta del valor máximo menos el OCR#A o OCR#B 
		calculado.
			- COM0A1 = 1 (Canal A)
			- COM0A0 = 1 (Canal A)
			
			- COM0B1 = 1 (Canal B)
			- COM0B0 = 1 (Canal B)
	Recordemos que con las señales PWM se pueden usar los dos canales A y B por cada uno de los temporizadores T0, T1 y T2, teniendo 6 señales PWM disponibles, en el 
	ejercicio se usarán de forma diferente los canales para obtener distintos modos de señales PWM*/
	//T0: Frecuencia de 1kHz;	T = 1ms;	PWM Fase correcta.
	//T1: Frecuencia de 1Hz;	T = 1s;		Fast PWM, ciclo de trabajo de 25% en OCR1A en modo no invertido y de 75% en OCR1B en modo no invertido.
	//T2: Frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta,
	
	//TCCR#A o registro de control del temporizador A: Sirve para indicar el tipo de salida y tipo de comparación en el temporizador T#, accede a los bits WMG#1 y WMG#0.
	//CONFIGURACIÓN SEÑAL PWM CANAL A:
	TCCR0A |= (1<<COM0A1)|(0<<COM0A0);	//Configuración Cero lógico en OC0 (Modo no invertido); COM0A1 = 1, COM0A0 = 0; T0 - Canal A
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0);	//Configuración Cero lógico en OC1 (Modo no invertido); COM1A1 = 1, COM1A0 = 0; T1 - Canal A
	TCCR2A |= (1<<COM2A1)|(1<<COM2A0);	//Configuración Cero lógico en OC2 (Modo no invertido);	COM2A1 = 1, COM2A0 = 1; T2 - Canal A
	//CONFIGURACIÓN SEÑAL PWM CANAL B:
	TCCR0A |= (1<<COM0B1)|(1<<COM0B0);	//Configuración Uno lógico en OC0 (Modo no invertido);	COM0B1 = 1, COM0B0 = 0; T0 - Canal B
	TCCR1A |= (1<<COM1B1)|(1<<COM1B0);	//Configuración Uno lógico en OC1 (Modo invertido);		COM1B1 = 1, COM1B0 = 1; T1 - Canal B
	TCCR2A |= (1<<COM2B1)|(1<<COM2B0);	//Configuración Uno lógico en OC2 (Modo invertido);		COM2B1 = 1, COM2B0 = 1; T2 - Canal B
	//TIPO DE COMPARADOR - PARTE 1/2: Esto se aplica a ambos canales A y B del mismo temporizador.
	TCCR0A |= (1<<WGM01)|(1<<WGM00);	//Configuración de PWM Rápido (OCRA); TCCR0B -> WMG02 = 1; TCCR0A -> WMG01 = 1 y WMG00 = 1
	TCCR1A |= (0<<WGM11)|(1<<WGM10);	//Configuración de PWM Rápido (8 bits); TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 1; T1 se maneja distinto.
	TCCR2A |= (1<<WGM21)|(0<<WGM20);	//Configuración de PWM Fase Correcta OCRA; TCCR2B -> WMG22 = 1; TCCR2A -> WMG21 = 0 y WMG20 = 1
	
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
	-	La instrucción |= lo que hace es solo asignar el valor de un bit 1 de un número binario sin afectar a los demás.
	Se pueden obtener frecuencias mayores con escalas pequeñas y frecuencias menores con escalas grandes.*/
	//TCCR#B o registro de control del temporizador: Sirve para indicar la configuración de escala y tipo de comparador.
	//ESCALA DEL TEMPORIZADOR:
	TCCR0B |= (1<<CS02)|(0<<CS01)|(1<<CS00);			//T0: Escala de 1024;	CS02 = 1, CS01 = 0 y CS00 = 1
	TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10);			//T1: Escala de 1024;	CS12 = 1, CS11 = 0 y CS10 = 1
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20)|(0<<WGM22);	//T2: Escala de 1024;	CS22 = 1, CS21 = 1 y CS20 = 1, no se puede usar un timer externo con T2, se maneja diferente.
	//TIPO DE COMPARADOR - PARTE 2/2:
	/*Aguas porque asignar ceros solos, normalmente causa problemas, si se quiere poner explícitamente, debe estar incluido dentro de una declaración con varios bits, en 
	específico se encuentra declarada en los registros TCCR#B de arriba, por eso en esta parte se comentó y solo se dejó indicado para que fuera más entendible.*/
	TCCR0B |= (1<<WGM02);				//Configuración de PWM Rápido (OCRA); TCCR0B -> WMG02 = 1; TCCR0A -> WMG01 = 1 y WMG00 = 1
	TCCR1B |= (0<<WGM13)|(1<<WGM12);	//Configuración de PWM Rápido (8 bits); TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 1; T1 se maneja distinto.
	//TCCR2B &= (0<<WGM22);				//Configuración de comparador CTC; TCCR2B -> WMG22 = 0; TCCR2A -> WMG21 = 1 y WMG20 = 0
	
	
	//TIMSK o registro de control del temporizador: Sirve para indicar que el temporizador sea de tipo desbordamiento o comparación e indicar el canal A o B del comparador.
	/*Los bits TOIE2, TOIE1 Y TOIE0 sirven para indicar cuál de los temporizadores T2, T1 o T0 queremos que sea de tipo desbordamiento, para que pueda ser de ese tipo se debe 
	mandar un 1 lógico a través de cada uno de ellos y a través del registro OCIE#A o OCIE#B se elige el canal activado del comparador mandando un 1 lógico, aunque es 
	importante saber que en el comparador CTC solo se puede utilizar el canal A:
		- TOIE2: Hace que el timer T2 de 8 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		- TOIE1: Hace que el timer T1 de 16 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		- TOIE0: Hace que el timer T0 de 8 bits sea de tipo desbordamiento si se le manda un 1 lógico y si se queda como 0 lógico es de tipo comparación.
		
		- OCIE2A: Hace que el timer T2 de 8 bits sea de tipo comparación y que se active el registro OCR0A o OCR0B para marcar el límite del conteo.
		- OCIE1A: Hace que el timer T1 de 16 bits sea de tipo comparación y que se active el registro OCR1A o OCR1B para marcar el límite del conteo.
		- OCIE0A: Hace que el timer T0 de 8 bits sea de tipo comparación y que se active el registro OCR2A o OCR2B para marcar el límite del conteo.*/
	TIMSK0 |= (0<<TOIE0)|(1<<OCIE0A)|(1<<OCIE0B); //Timer T0 de 8 bits de tipo comparación, registro de comparación OCR0A y OCR0B activados, se puede usarlos en PWM.
	TIMSK1 |= (0<<TOIE1)|(1<<OCIE1A)|(1<<OCIE1B); //Timer T1 de 16 bits de tipo comparación, registro de comparación OCR1A y OCR1B activados, se puede usarlos en PWM.
	TIMSK2 |= (0<<TOIE2)|(1<<OCIE2A)|(1<<OCIE2B); //Timer T2 de 8 bits de tipo comparación, registro de comparación OCR2A y OCR2B activados, se puede usarlos en PWM.
	
	/*MANEJO DEL TEMPORIZADOR - FRECUENCIA, PERIODO, DUTY CYCLE Y COMPARADOR PWM: Existen fórmulas específicas para cada tipo de señal PWM, además recordemos que su frecuencia, 
	periodo y conteo del comparador pueden no solo depender de su registro OCR#, sino de su máximo llamado TOP, que es descrito dependiendo de la resolución o tipo de modo 
	PWM.*/
	/*En el modo PWM si se pueden usar ambos canales con cada temporizador, dando como total 6 señales PWM, dos canales A y B por cada uno de los 3 temporizadores T0, T1 y T2,
	pero dependiendo de si se quiere usar el modo TOP para definir cierto valor máximo de conteo personalizado, se pueden usar ambos canales o solo uno.
	
-	Con los timers T0 y T2 solo se puede usar el registro OCR#A para definir el valor TOP, por lo cual si este se usa, solo se podrá usar el canal B para obtener la señal PWM.
-	Con el timer T1 se pueden usar los registros OCRA# o ICR1 para definir el valor TOP, por lo cual se podrán usar ambos canales A y B para obtener la señal PWM, a menos que
	se decida usar el registro OCRA# para definir el valor TOP, en cuyo caso solo se podrá obtener la señal PWM por el canal B.
	
	Normalmente se hace uso del valor de conteo máximo TOP cuando el conteo del registro para llegar a cierta frecuencia frecuencia de la señal PWM requerida es demasiado 
	grande para ser soportado por el registro contador, reduciendo así el valor del conteo:
	T0 y T2: Contadores de 8 bits.
		Si el resultado de OCR#A o OCR#B es mayor que el conteo máximo de los registros contadores:
			Los temporizadores T0 y T2 siempre tienen resolución de 8 bits:	0000 1111 1111 = 255.
		Si el conteo es mayor que el máximo que soportan los registros OCR#A o OCR#B dependiendo de la resolución del temporizador, es necesario usar el valor TOP para poder 
		generar esa señal PWM.
	
	T1: Contadores de 16 bits.
		Si el resultado de OCR#A o OCR#B es mayor que el conteo máximo de los registros contadores, que depende de la resolución de 8, 9 o 10 bits, ya que nunca puede contar 
		los 16 bis de conteo con los que dispone, los valores máximos a los que puede contar sin usar el registro TOP son los siguientes:
			Resolución de 8 bits:	0000 1111 1111 = 255.
			Resolución de 9 bits:	0001 1111 1111 = 511.
			Resolución de 10 bits:	0011 1111 1111 = 1023.
		Si el conteo es mayor que el máximo que soportan los registros OCR#A o OCR#B dependiendo de la resolución elegida, es necesario usar el valor TOP para poder generar 
		esa señal PWM.
	
	FÓRMULAS:
	- PWM Fase correcta: De baja frecuencia, para control de motores CD principalmente, se puede definir el valor máximo de conteo TOP, pero no es necesario si no se quiere, 
	en cualquier caso los resultados del cálculo se guardarán en los registros ICR1, OCR1A o OCR1B, dependiendo de la configuración que se haya elegido y el temporizador que 
	se esté utilizando.
	Modo sin TOP personalizado definido; Como salida se pueden utilizar los canales A (OCR#A) y B (OCR#B); Se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR#, el valor del ConteoMáximo_PWM depende de la resolución del temporizador:
			1.1.0.- frecuencia = fmicro / (2*escala*OCR#)
			1.1.1.-		OCR# = fmicro / (2*escala*frecuencia)
			1.2.0.- Periodo = T = (2*escala*OCR#) / fmicro = 1 / frecuencia
			1.2.1.-		OCR# = (T*fmicro) / (2*escala)
			1.3.-	t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			1.4.-	OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			2.1.-	OCR#I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
			2.2.-	OCR#NI_dutycycle = OCR#_dutycycle
			2.3.-	OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.-	OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			2.3.-	OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.-	OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
	Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definición del valor máximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- ICR1 = TOP
		- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR# con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T * Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.-OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
	Modo OCR#A = TOP; Como salida se puede utilizar solo el canal B (OCR1B); Se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Definición del valor máximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- OCR1A = TOP
		- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR# con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- OCR#A = TOP
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle


	- Fast PWM: De alta frecuencia, para rectificación de señales, regulación de tensión, conversión de señal digital a analógica, convertidor CD-CD, etc. se puede definir el
	valor máximo de conteo TOP, pero no es necesario si no se quiere, en cualquier caso los resultados del cálculo se guardarán en los registros ICR1, OCR1A o OCR1B,
	dependiendo de la configuración que se haya elegido y el temporizador que se esté utilizando.
	Modo sin TOP personalizado definido; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR#, el valor del ConteoMáximo_PWM depende de la resolución del temporizador:
			1.1.0.- frecuencia = fmicro / (escala * (1 + OCR#))
			1.1.1.-		OCR# = (fmicro / (frecuencia*escala)) - 1
			1.2.0.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
			1.2.1.-		OCR# = ((T*fmicro) / escala)) - 1
			1.3.-	t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			1.4.-	OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			2.1.-	OCR#I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
			2.2.-	OCR#NI_dutycycle = OCR#_dutycycle
			2.3.-	OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.-	OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			2.3.-	OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.-	OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle

	Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definición obligatoria del valor máximo de conteo personalizado TOP:
			1.1.- TOP = (fmicro / (frecuencia*escala)) - 1
			1.2.- TOP = ((T*fmicro) / escala)) - 1
			1.3.- ICR1 = TOP
		- Cálculo de frecuencia, periodo y tiempo de duty cycle con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (escala * (1 + OCR#))
			2.2.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T * Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
	Modo OCR#A = TOP; Como salida se puede utilizar solo el canal B (OCR1B); se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Definición obligatoria del valor máximo de conteo personalizado TOP:
			1.1.- TOP = (fmicro / (frecuencia*escala)) - 1
			1.2.- TOP = ((T*fmicro) / escala)) - 1
			1.3.- OCR#A = TOP
		- Cálculo de frecuencia, periodo y tiempo de duty cycle con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (escala * (1 + OCR#))
			2.2.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T * Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- OCR#A = TOP
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle


	- PWM fase y frecuencia correcta: Tiene las mismas fórmulas que el modo PWM Fase correcta, pero además a fuerza debe ser definido el máximo del conteo, también llamado TOP,
	ambos resultados se guardarán en los registros ICR1, OCR1A o OCR1B, dependiendo de la configuración que se haya elegido y el temporizador que se esté utilizando.
	Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definición obligatoria del valor máximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- ICR1 = TOP
		- Cálculo de frecuencia, periodo y tiempo de duty cycle con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR1_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR1I_dutycycle = Modo invertido;	OCR1NI_dutycycle = Modo no invertido:
			3.1.- OCR1I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
			3.2.- OCR1NI_dutycycle = OCR#_dutycycle
			3.3.- OCR1A = OCR1I_dutycycle  o OCR1NI_dutycycle
			3.4.- OCR1B = OCR1I_dutycycle  o OCR1NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1A = OCR1I_dutycycle  o OCR1NI_dutycycle
			3.4.- OCR1B = OCR1I_dutycycle  o OCR1NI_dutycycle
	
	Modo OCR#A = TOP; Como salida se puede utilizar solo el canal B (OCR1B); se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Definición obligatoria del valor máximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- OCR1A = TOP
		- Cálculo de frecuencia, periodo y tiempo de duty cycle con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		PWM, OCR1I_dutycycle = Modo invertido;	OCR1NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1B = OCR1I_dutycycle  o OCR1NI_dutycycle

	Se debe tomar en cuenta que las frecuencias mayores a las cuales pueden acceder los registros T0, T1 y T2 son las siguientes, eligiendo la escala más baja, ya que de esta
	manera se accede a mayores frecuencias, tomando en cuenta el conteo total de los temporizadores, además es importante mencionar que frecuencias mayores se pueden obtener 
	por medio de los temporizadores de 8 bits y menores frecuencias con el temporizador de 16:
	Temporizadores de 8 bits T0 y T2 - Temporizador de 16 bits con resolución de 8 bits:
	- PWM Fase correcta o PWM fase y frecuencia correcta: De baja frecuencia, para control de motores CD.
		1.1- frecuencia = fmicro / (2*escala*OCR#A) = 16X10^6 / (2*escala*OCR#A)
		conteo_total_temporizador T0 y T2, conteo_total_temporizador T1 - resolución de 8 bits = 255 = OCR#A
			f_max -> escala = 1; f = 31372.5490	Hz = 31.3725	kHz
			f_max -> escala = 8; f = 3921.5686	Hz = 3.9215		kHz
		conteo_total_temporizador T1 - resolución de 9 bits = 511 = OCR#A
			f_max -> escala = 1; f = 15655.5773	Hz = 15.6555	kHz
			f_max -> escala = 8; f = 1956.9471	Hz = 1.9569		kHz
		conteo_total_temporizador T1 - resolución de 10 bits = 1,023 = OCR#A
			f_max -> escala = 1; f = 7820.1368	Hz = 7.8201		kHz
			f_max -> escala = 8; f = 977.5171	Hz
	Temporizador de 16 bits T1:
	- Fast PWM: De alta frecuencia, para rectificación de señales, regulación de tensión, conversión de señal digital a analógica, convertidor CD-CD, control de servos, etc.
		1.1- frecuencia = fmicro / (escala*(1+OCR#A)) = 16X10^6 / (escala*(1+OCR#A))
		conteo_total_temporizador T0 y T2, conteo_total_temporizador T1 - resolución de 8 bits = 255 = OCR#A
			f_max -> escala = 1; f = 62500		Hz = 62.5		kHz
			f_max -> escala = 8; f = 7812.5		Hz = 7.8125		kHz
		conteo_total_temporizador T1 - resolución de 9 bits = 511 = OCR#A
			f_max -> escala = 1; f = 31250		Hz = 31.25		kHz
			f_max -> escala = 8; f = 3906.25	Hz = 3.9062		kHz
		conteo_total_temporizador T1 - resolución de 10 bits = 1,023 = OCR#A
			f_max -> escala = 1; f = 15609.7561	Hz = 15.6097	kHz
			f_max -> escala = 8; f = 1951.2195	Hz = 1.9512		kHz
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
	Con el modo Fast PWM se llega a mucho mayores frecuencias que con los modos PWM Fase correcta o PWM fase y frecuencia correcta y mientras menor la resolución, mayor la 
	frecuencia también.
	Cuando: OCR#A > conteo_total_temporizador, se debe calcular el número de ciclos que se utilizará en un condicional if dentro de la función ISR del temporizador y dentro 
	del registro comparador se debe poner el valor máximo del conteo del temporizador:
		1.1.- ciclos = conteo_OCR /conteo_total_temporizador
		1.2.- OCR#A = conteo_total_temporizador
	Al llegar a un resultado de ciclos, simplemente se deberá crear una variable global que tenga este valor y un condicional if que vaya aumentando otra variable que empiece
	a contar desde cero y que ejecute la acción de la interrupción solamente hasta que la variable haya alcanzado los ciclos necesarios.*/
	
	
	/*T0 - Fast PWM - frecuencia de 500 Hz;	T = 2ms;	Fast PWM, ciclo de trabajo de 80% en OCR0B en modo no invertido con valor máximo personalizado con OCR0A; Escala 1024.
	Escalas disponibles:
	T0 y T1:
	- 1:1
	- 1:8
	- 1:64
	- 1:256
	- 1:1024
	Cuando se quiere declarar un valor máximo personalizado, lo que se hace es elegir un modo que tenga un TOP = ICR1 o OCR1A, luego se ejecutan las siguientes fórmulas para 
	encontrar el valor de TOP y posteriormente este valor se intercambia por la variable OCR#A en las fórmulas normales de la señal PWM, esto se hace normalmente para poder
	alcanzar grandes frecuencias que no alcanza a cubrir el registro de 8 bits o de 16 bits:
	CANAL A: Modo no invertido
		1.2.- TOP = (fmicro / (frecuencia*escala)) - 1 = (16X10^6 / (500*1024)) - 1 = 30.25
	CANAL B: Modo no invertido
		2.1.- Periodo = T = (escala*(1 + OCR#A)) / fmicro = (escala*(1 + TOP)) / fmicro = (1024*(1 + 30.25)) / 16X10^6 = 0.002s = 2ms
		3.1.- t_dutyycle = T*Porcentaje_deseado = 0.002*0.8 = 0.0016s = 1.6ms
		4.1.- OCR#A_dutycycle_NI = ((t_dutycycle*fmicro) / escala) - 1 = ((0.0016*16X10^6) / 1024) - 1 = 24; Modo no invertido.*/
	OCR0A = 30.25;	//Fast PWM, Valor max personalizado; OCRA = 30.25
	OCR0B = 24;		//Fast PWM; frecuencia = 500Hz; t_deseado = 2ms; escala = 1024; conteo_OCRB_NI = 24; duty cycle = 80%
	
	/*T1 - Fast PWM - frecuencia de 1Hz;	T = 1s;		Fast PWM, ciclo de trabajo de 25% en OCR1A en modo no invertido y de 75% en OCR1B en modo invertido; Escala 1024.
	Escalas disponibles:
	T0 y T1:
	- 1:1
	- 1:8
	- 1:64
	- 1:256
	- 1:1024
	CANAL A: Modo no invertido
		1.1- frecuencia = 1 Hz.
		2.1.- Periodo = T = 1 / frecuencia = 1s
		3.1.- t_dutyycle = T*Porcentaje_deseado = 1*0.25 = 0.25s
		4.1.- OCR#A_dutycycle_NI = ((t_dutycycle*fmicro) / escala) - 1 = ((0.25*16X10^6) / 1024) - 1 = 3905.25; Modo no invertido
	CANAL B: Modo invertido
		4.2.- OCR#A_dutycycle_I = conteo_total_temporizador - OCR#A_dutycycle_NI = 12528.75; Modo invertido.*/
	OCR1A = 3905.25;	//Fast PWM; frecuencia = 1Hz; t_deseado = 1s; escala = 1024; conteo_OCRA_NI = 3905.25; duty cycle = 25%
	OCR1B = 12528.75;	//Fast PWM; frecuencia = 1Hz; t_deseado = 1s; escala = 1024; conteo_OCRB_I = 12528.75; duty cycle = 75%
	
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



//Funciones ISR: Cuando se crea una señal PWM, estas no se utilizan.