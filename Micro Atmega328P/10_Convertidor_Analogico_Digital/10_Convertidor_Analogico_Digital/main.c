/*
 * 10_Convertidor_Analogico_Digital.c
 *
 * Created: 18/05/2022 06:58:05 p. m.
 * Author : diego
 10.- CONVERTIDOR ADC: El tipo de ADC que tiene el Arduino se llama de Aproximaciones Sucesivas, este tiene una resolución de 10 bits (con valores digitales de tensión de 0 a 
 1023), teniendo en cuenta que la versión del Arduino UNO cuenta con 6 pines de entrada analógica que se encuentran en el Puerto C, a los cuales se les puede configurar la 
 entrada analógica que se quiere usar, la referencia analógica interna que asigna valores de tensión reales a los valores digitales de tensión, la frecuencia de muestreo y 
 los factores que causan los muestreos en el ADC.
 
	- Muestrear: Se refiere a tomar valores de tensión cada cierto tiempo para aproximar una señal analógica a una digital, este ritmo de muestreo normalmente va de la mano 
	  con la frecuencia del reloj del convertidor analógico digital, que puede venir incluido en un microcontrolador o puede estar en un circuito integrado aparte. El 
	  muestreo trabaja con el eje horizontal de la señal, osea el tiempo.
		- Sampling rate (Velocidad de muestreo): Se refiere a la velocidad que indica cuántas veces por segundo se está realizando un muestreo de la señal analógica para 
		  crear su señal digital correspondiente.
	- Retener: Es un concepto que tiene que ver con la electrónica del ADC, en donde se retiene información recabada de la señal analógica.
	- Cuantificar: En esta fase se asocia cada valor de amplitud analógica a un valor digital que está limitado al número de bits de resolución del ADC. La cuantificación 
	  trabaja con el eje vertical de la señal, osea la amplitud de la tensión eléctrica.
		- Sampling depth (Resolución): La profundidad de muestreo es la máxima amplitud que puede guardar el convertidor en un número binario, esto depende del circuito 
		  integrado o del microcontrolador que tenga el convertidor analógico digital o digital analógico.

 Con el ADC se harán los siguientes ejercicios:
 1.- ADC con muestreo de desbordamiento usando un factor de división de 128 y el timer T0 de 8 bits, osea conteo de 0 a 255, escala de 1024. 
 Señal analógica con un potenciómetro y mostrar el valor en el LCD.
 2.- ADC con muestreo de corrida libre usando un factor de división de 2. 
 Señal analógica con un potenciómetro y mostrar el valor en el LCD.
 

 Para el ejercicio se utilizarán los puertos: PB0 (pin 8 puerto B), PC0 (pin A0 puerto C) y PD0 (pin 0 puerto D).
 - Puerto B, Tiene 6 pines disponibles: A estos pines se conectarán los leds que muestran la conversión analógica.
 Pin 8	(PB0); Led para señal analógica
 Pin 9	(PB1); Led para señal analógica; OCR1A, Señal de salida con frecuencia variable del timer T1, temporizador por comparación canal A
 Pin 10	(PB2); Led para señal analógica; OCR1B, Señal de salida con frecuencia variable del timer T1, temporizador por comparación canal B
 Pin 11	(PB3); Led para señal analógica; OCR2A, Señal de salida con frecuencia variable del timer T2, temporizador por comparación canal A
 Pin 12	(PB4); Led para señal analógica
 Pin 13	(PB5); Led para señal analógica
 - Puerto C, Tiene 6 pines disponibles para las entradas analógicas, se usa para las entradas del ADC:
 Pin A0	(PC0); Entrada analógica ADC0; Entrada analógica a analizar
 Pin A1	(PC1); Entrada analógica ADC1
 Pin A2	(PC2); Entrada analógica ADC2
 Pin A3	(PC3); Entrada analógica ADC3
 Pin A4	(PC4); Entrada analógica ADC4
 Pin A5	(PC5); Entrada analógica ADC5
 - Puerto D, Tiene 8 pines disponibles:
 Pin 0	(PD0)
 Pin 1	(PD1)
 Pin 2	(PD2)
 Pin 3	(PD3)
 Pin 4	(PD4)
 Pin 5	(PD5); OCR0B, Señal de salida PWM del timer T0, temporizador por comparación canal B
 Pin 6	(PD6); OCR0A, Señal de salida PWM del timer T0, temporizador por comparación canal A
 Pin 7	(PD7)
 
 Registros que controlan el ADC del ATMEGA328P: ADMUX, ADCSCRA y ADCSCRB.
 
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
 temporizadores CTC.
 */ 



//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>			//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <avr/interrupt.h>	//Librería para el manejo de los registros de interrupciones
#include <avr/cpufunc.h>	



//Funciones ISR: Tareas ejecutadas por las interrupciones externas.
/*ISR (ADC_vect): Se le llama ISR o Interruption Service Routine a la función que describa lo que se tiene que hacer cuando sea activada una interrupción, la palabra ISR es 
una palabra reservada de interrupciones y el nombre de esta función no debe ser decladada en ningún lado del código, simplemente por haber sido nombrada como ISR, se ejecuta 
cuando sea detonada la interrupción que recibe como parámetro, para ello la interrupción INT0 ya debe haber sido configurada dentro del método main. Para ello se le pasa como 
parámetro a la función el vector INT0_vect para que ejecute la acción cuando la interrupción externa INT0 sea detonada y se debe hacer una función ISR aparte para manejar la 
acción que detona la interrupción externa INT1.*/
/*Las acciones de las interrupciones externas deben ser descritas en una función fuera del método main, estas reciben como parámetro lo siguiente:
	- ADC_vect: Interrupción activada por la conversión ADC en el Arduino.*/
//ISR INTERRUPCIONES POR ADC:
ISR(ADC_vect){
	/*Almacenamiento del número binario resultado de la conversión analógica digital que representa la amplitud de tensión en la señal analógica: El resultado de la conversión 
	ACD se guarda en un número binario de 10 bits, estos a fuerza se deben guardar en dos registros distintos llamados ADCH y ADCL, donde el bit ADLAR indica el orden en el 
	que se guardan, pero normalmente cuando ADLAR = 0, se acomodan de la siguiente manera:
		- ADCH: Guarda los 2 bits más significativos de los 10 bits resultantes de la conversión ADC.
		- ADCL: Guarda los 8 bits menos significativos de los 10 bits resultantes de la conversión ADC.*/
	PORTB = ADCL;			//Asingación de los 2 bits más significativos de la conversión ADC.
	
	/*INICIALIZAR LA CONVERSIÓN ADC - ADCSRA: Registro encargado de iniciar la conversión del ADC, para activar la conversión analógica digital se debe modificar el estado de 
	los bits ADEN y ADSC. El bit ADEN se modifica en la configuración del ADC y el bit ADSC se modifica en la ejecución del programa porque es el que inicializa al ADC.
		- ADEN: Bit que activa o desactiva el ADC del microcontrolador:
			- ADEN = 1: Activación de la conversión ADC.
				- ADSC = 1: Iniciación de la conversión, mientras el bit del registro se mantenga en su valor uno lógico, la conversión ADC sigue en proceso.
				- ADSC = 0: Fin de la conversión, cuando se acabe la conversión ADC el bit cambiará a cero lógico por sí solo.
			- ADEN = 0: Desactivación de la conversión ADC.*/
	ADCSRA |= (1<<ADSC); //Iniciación de la conversión ADC; ADSC = 1
}



//MÉTODO MAIN O EJECUTABLE: Desde este método se ejecuta todo el código y es lo que diferencía la librería de un ejecutable.
/*Para configurar correctamente un programa que use interrupciones se utiliza la siguiente arquitectura en el programa:
	- CLI: Activación de interrupciones asignando un valor 0 lógico al bit GI, volviendo la interrupción de tipo MI.
	- Programación de máquina y configuración de interrupciones con los registros TCCR0A, TCCR0B (registro de control del temporizador A y B), TIMSK0 y OCR0A (registro de 
	comparación), además se activan las interrupciones externas con los registros EIMSK (máscara de interrupciones) y EICRA (registro de control de interrupciones).
	- SEI: Desactivación de interrupciones asignando un valor 1 lógico al bit GI, volviendo la interrupción de tipo NMI.
	- Ejecución del programa.
Con dicha arquitectura nos aseguramos de manejar propiamente los pines de los puertos que manejan las interrupciones, activándolas y desactivándolas cuando es necesario.*/
int main(void)
{
	/*CLI: Cuando se activa una CLI interruption, automáticamente el bit 7 del status register que es GI (General Interruption) se convierte en 0 lógico, lo que convierte a 
	las interrupciones en MI (Maskable Interrupt), esto en términos más simples significa que las interrupciones están activas:
		- Cuando GI tiene valor de 1 lógico no está activada ninguna interrupción, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 lógico está activada la interrupción, osea que es MI, Maskable Interrupt.*/
	cli(); //Evita cualquier interrupción que entre mientras ya se está ejecutando una o antes de programar la máquina, ya que termine, permite la ejecución de interrupciones.
	
	
	
	//PROGRAMACIÓN DE LA MÁQUINA: Se indica por medio de los registros de función específica si los pines de los puertos serán entradas o salidas o el estado de los timers.
	//0: Pin como entrada
	//1: Pin como salida
	DDRB = 0xFF; //Todos los pines del puerto B son salidas a leds.
	DDRC = 0x00; //Todos los pines del puerto C son entradas disponibles para el ADC.	
	
	//CONFIGURACIÓN DEL ADC: Los registros que controlan principalmente el ADC del ATMEGA328P son ADMUX y ADCSCRA.
	/*ADMUX: Registro que sirve para indicar cuál de los 6 pines del Puerto C se tomará como entrada para el ADC, el tipo de referencia analógica y el tipo de almacenamiento
	del número binario resultante de la conversión analógica-digital.*/
	/*ENTRADA ANALÓGICA - ADMUX: Indica cuales de los 6 pines del Puerto C se tomarán como entradas analógicas, todas las señales analógicas llegan a un Multiplexor porque 
	existe un solo convertidor analógico digital. El multiplexor se controla con los 4 bits MUX3, MUX2, MUX1 y MUX0:
		- ADC0: Pin A0	(PC0) como entrada analógica
			- MUX3 = 0
			- MUX2 = 0
			- MUX1 = 0
			- MUX0 = 0
		- ADC1: Pin A1	(PC1) como entrada analógica
			- MUX3 = 0
			- MUX2 = 0
			- MUX1 = 0
			- MUX0 = 1
		- ADC2: Pin A2	(PC2) como entrada analógica
			- MUX3 = 0
			- MUX2 = 0
			- MUX1 = 1
			- MUX0 = 0
		- ADC3: Pin A3	(PC3) como entrada analógica
			- MUX3 = 0
			- MUX2 = 0
			- MUX1 = 1
			- MUX0 = 1
		- ADC4: Pin A4	(PC4) como entrada analógica
			- MUX3 = 0
			- MUX2 = 1
			- MUX1 = 0
			- MUX0 = 0
		- ADC5: Pin A5	(PC5) como entrada analógica
			- MUX3 = 0
			- MUX2 = 1
			- MUX1 = 0
			- MUX0 = 1
		- ADC8: Activación del sensor de temperatura interno del microcontrolador ATMEGA328P, este es de gran utilidad para medir la temperatura a la que está sometido el 
		mismo microcontrolador cuando se utiliza en condiciones de temperatura extrema, pero su desventaja es que no es muy preciso, teniendo una sensibilidad de ±10°C.
			- MUX3 = 1
			- MUX2 = 0
			- MUX1 = 0
			- MUX0 = 0*/
	ADMUX |= (0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(1<<MUX0); //ADC0: Pin A0 (PC0) como entrada analógica; MUX3 = 0, MUX2 = 0, MUX1 = 0, MUX0 = 0
	/*REFERENCIA ANALÓGICA - ADMUX: Elige una de las 3 opciones de referencia analógica para realizar la conversión del número binario resultante de la conversión ADC a un 
	valor de tensión real a través de los 2 bits REFS1 y REFS0:
		- Referencia Analógica Externa: Es una referencia dictada por la tensión que entre por el pin AREF del Puerto B (2 lugares arriba del pin 13) que puede ir de 0 a 5V.
			- REFS1 = 0
			- REFS0 = 0
		- Referencia Analógica Default: Es la referencia que tiene el mismo valor que la alimentación del Arduino que es VCC = 5V. Su problema es que no es tan precisa, más 
		  aún cuando se alimenta al Arduino con una batería externa, ya que la tensión de esta batería irá bajando mientras se descargue durante su uso.
			- REFS1 = 0
			- REFS0 = 1
		- Reservado: No se puede hacer nada con esta opción.
			- REFS1 = 1
			- REFS0 = 0
		- Referencia Analógica Interna: Es una referencia incorporada dentro del microcontrolador ATMEGA328P que tiene valor de 1.1 V. Es muy estable, pero se debe lograr que 
		  la señal de entrada externa tenga una amplitud máxima de 1.1 V, esto se puede lograr con un divisor de tensión.
			- REFS1 = 1
			- REFS0 = 1*/
	ADMUX |= (1<<REFS1)|(1<<REFS0); //Referencia Analógica Interna; REFS1 = 1, REFS0 = 1
	/*ALMACENAMIENTO DEL NÚMERO BINARIO RESULTADO DEL ADC - ADMUX: Dependiendo del estado del bit ADLAR, el resultado de 10 bits se acomoda de una forma o de otra en los dos 
	registros llamados ADCH y ADCL:
		- ADLAR = 0
			- ADCH: Guarda los 2 bits más significativos de los 10 bits resultantes de la conversión ADC hasta la derecha de su registro.
			- ADCL: Guarda los 8 bits menos significativos de los 10 bits resultantes de la conversión ADC.
		- ADLAR = 1
			- ADCH: Guarda los 8 bits más significativos de los 10 bits resultantes de la conversión ADC.
			- ADCL: Guarda los 2 bits menos significativos de los 10 bits resultantes de la conversión ADC hasta la izquierda de su registro.*/
	ADMUX |= (0<<ADLAR); //ADCL: Guarda los 8 bits menos significativos de los 10 bits resultantes de la conversión ADC; ADLAR = 0
	
	//ADCSRA (Señora ADC): Registro encargado de activar la conversión e indicar la frecuencia de muestreo del ADC.
	/*ACTIVAR LA CONVERSIÓN ADC - ADCSRA: Registro encargado de activar la conversión ADC.
		- El bit ADEN se encarga directamente de activar o desactivar la conversión ADC:
			- ADEN = 0; Conversión ADC desactivada; Valor que viene por default.
			- ADEN = 1; Conversión ADC activada.
		- El bit ADATE se encarga de definir el modo de trabajo del ADC, ya que el ADC del ATMEGA328P es de tipo aproximaciones sucesivas, esto se refiere a que realiza “n” 
		comparaciones sucesivas de la señal analógica para realizar su conversión, pero se puede hacer una sola si se quiere, teniendo las siguientes opciones:
			- ADATE = 0; Conversión única de un dato analógico.
			- ADATE = 1; Activación del ADC de aproximaciones sucesivas.
		- El bit ADIE activa o desactiva las interrupciones causadas por el convertidor ADC, que se activará en una función ISR (Interruption Service Routine), usada para 
		manejar interrupciones en general, utilizando el vector ADC_vect como su parámetro:
			- ADIE = 0; El ADC trabaja sin realizar ninguna interrupción.
			- ADIE = 1; El ADC puede realizar interrupciones, causadas cuando termine de realizar la conversión de una señal analógica a digital.*/
	ADCSRA |= (1<<ADEN)|(0<<ADATE)|(1<<ADIE); //Conversión ADC activada, ADEN = 1; Activación del ADC de aproximaciones sucesivas, ADATE = 1; ADC con interrupciones, ADIE = 1
	/*FRECUENCIA DE MUESTREO DEL ADC (PRESCALER) - ADCSRA: Los bits ADPS2, ADPS1 y ADPS0 sirven para cambiar la frecuencia de muestreo del ADC, dividiendo la frecuencia del 
	reloj del ATMEGA328P que es de 16 MHz entre ciertos factores de división y donde siguiendo el teorema de Nyquist sabemos que su frecuencia debe ser el doble que la máxima 
	del ancho de banda de la señal.
		- Factor de división 2: Se divide la señal del reloj de 16X10^6 Hz:		f_muestreo = fmicro / factor_division = 16X10^6 / 2 = 8X10^6 =		8 MHz
			- ADPS2 = 0
			- ADPS1 = 0
			- ADPS0 = 0
		- Factor de división 2: Se divide la señal del reloj de 16X10^6 Hz:		f_muestreo = fmicro / factor_division = 16X10^6 / 2 = 8X10^6 =		8 MHz
			- ADPS2 = 0
			- ADPS1 = 0
			- ADPS0 = 1
		- Factor de división 4: Se divide la señal del reloj de 16X10^6 Hz:		f_muestreo = fmicro / factor_division = 16X10^6 / 4 = 4X10^6 =		4 MHz
			- ADPS2 = 0
			- ADPS1 = 1
			- ADPS0 = 0
		- Factor de división 8: Se divide la señal del reloj de 16X10^6 Hz:		f_muestreo = fmicro / factor_division = 16X10^6 / 8 = 2X10^6 =		2 MHz
			- ADPS2 = 0
			- ADPS1 = 1
			- ADPS0 = 1
		- Factor de división 16: Se divide la señal del reloj de 16X10^6 Hz:	f_muestreo = fmicro / factor_division = 16X10^6 / 16 = 1X10^6 =		1 MHz
			- ADPS2 = 1
			- ADPS1 = 0
			- ADPS0 = 0
		- Factor de división 32: Se divide la señal del reloj de 16X10^6 Hz:	f_muestreo = fmicro / factor_division = 16X10^6 / 32 = 1X10^6 =		0.5 MHz =	500 kHz
			- ADPS2 = 1
			- ADPS1 = 0
			- ADPS0 = 1
		- Factor de división 64: Se divide la señal del reloj de 16X10^6 Hz:	f_muestreo = fmicro / factor_division = 16X10^6 / 64 = 1X10^6 =		0.25 MHz =	250 kHz
			- ADPS2 = 1
			- ADPS1 = 1
			- ADPS0 = 0
		- Factor de división 128: Se divide la señal del reloj de 16X10^6 Hz:	f_muestreo = fmicro / factor_division = 16X10^6 / 128 = 1X10^6 =	0.125 MHz =	125 kHz
			- ADPS2 = 1
			- ADPS1 = 1
			- ADPS0 = 1*/
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Factor de división de 128, frecuencia de 125 kHz; ADPS2 = 1, ADPS1 = 1, ADPS0 = 1
	
	/*ADCSRB: Registro utilizado para definir un modo de operación del convertidor ADC, solamente se puede usar cuando se trabaja con aproximaciones sucesivas, osea cuando 
	el bit ADATE del registro ADCSRA está como 1 lógico.*/
	/*FRECUENCIA DE MUESTREO DEL ADC (PRESCALER) - ADCSRA: Los bits ADPS2, ADPS1 y ADPS0 sirven para cambiar la frecuencia de muestreo del ADC, dividiendo la frecuencia del 
	reloj del ATMEGA328P que es de 16 MHz entre ciertos factores de división y donde siguiendo el teorema de Nyquist sabemos que su frecuencia debe ser el doble que la máxima 
	del ancho de banda de la señal.
		- Modo de corrida libre: Realiza el muestreo de la señal y la convierte a su equivalente digital sin estar sujeto a ninguna condición.
			- ADTS2 = 0
			- ADTS1 = 0
			- ADTS0 = 0
		- Comparador analógico: Utiliza un comparador interno, entregando una tensión u otra en la salida dependiendo de cuál amplitud es mayor en las dos señales analógicas 
		comparadas.
			- ADTS2 = 0
			- ADTS1 = 0
			- ADTS0 = 1
		- Llamado de la ingterrupción externa: En este modo se activa la conversión analógica digital solamente cuando se active alguna interrupción externa INT0 del pin PD2 
		o INT1 del pin PD3 (ambas del Puerto D).
			- ADTS2 = 0
			- ADTS1 = 1
			- ADTS0 = 0
		- Temporizador T0 por comparación: Activación de la conversión analógica digital solamente cuando se active la interrupción por un temporizador de tipo comparación, 
		ya sea CTC o PWM de algún tipo. Se debe utilizar el temporizador T0 de 8 bits.
			- ADTS2 = 0
			- ADTS1 = 1
			- ADTS0 = 1
		- Temporizador T0 por desbordamiento: Activación de la conversión analógica digital solamente cuando se active la interrupción por desbordamiento. Se debe utilizar el 
		temporizador T0 de 8 bits.
			- ADTS2 = 1
			- ADTS1 = 0
			- ADTS0 = 0
		- Temporizador T1 por comparación B: Activación de la conversión analógica digital solamente cuando se active la interrupción por un temporizador de tipo comparación,
		ya sea CTC o PWM de algún tipo. Se debe utilizar el temporizador T1 de 16 bits.
			- ADTS2 = 1
			- ADTS1 = 0
			- ADTS0 = 1
		- Temporizador T1 por desbordamiento: Activación de la conversión analógica digital solamente cuando se active la interrupción por desbordamiento. Se debe utilizar el
		temporizador T1 de 16 bits.
			- ADTS2 = 1
			- ADTS1 = 1
			- ADTS0 = 0
		- Temporizador T1 por captura de evento: La captura es parte de los timers, pero es activado solamente cuando se utiliza algún tipo de comunicación I2C, Bluetooth, etc. 
		Solamente se puede usar con el timer 1 este tipo de muestreo.
			- ADTS2 = 1
			- ADTS1 = 1
			- ADTS0 = 1*/
	ADCSRB |= (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); //Modo de corrida libre; ADTS2 = 0, ADTS1 = 0, ADTS0 = 0
		
	
	
	/*SEI: Cuando se activa una SEI interruption, automáticamente el bit 7 del status register (GI) se convierte en 1 lógico, lo que convierte a las interrupciones en NMI 
	(No Mask Interruption):
		- Cuando GI tiene valor de 1 lógico no está activada ninguna interrupción, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 lógico está activada la interrupción, osea que es MI, Maskable Interrupt.*/
	sei(); //Permite cualquier interrupción que entre ya después de haber programado la máquina y cuando haya terminado de ejecutarse una interrupción anterior.
	
	
	
	//EJECUCIÓN DEL PROGRAMA:
	/*INICIALIZAR LA CONVERSIÓN ADC - ADCSRA: Registro encargado de iniciar la conversión del ADC, para activar la conversión analógica digital se debe modificar el estado de 
	los bits ADEN y ADSC. El bit ADEN se modifica en la configuración del ADC y el bit ADSC se modifica en la ejecución del programa porque es el que inicializa al ADC.
		- ADEN: Bit que activa o desactiva el ADC del microcontrolador:
			- ADEN = 1: Activación de la conversión ADC.
				- ADSC = 1: Iniciación de la conversión, mientras el bit del registro se mantenga en su valor uno lógico, la conversión ADC sigue en proceso.
				- ADSC = 0: Fin de la conversión, cuando se acabe la conversión ADC el bit cambiará a cero lógico por sí solo.
			- ADEN = 0: Desactivación de la conversión ADC.*/
	ADCSRA |= (1<<ADSC); //Iniciación de la conversión ADC; ADSC = 1
	
	while(1)
	{
		/*Se recomienda en la bibliografía que cuando se hace una asignación al PIN, se le de un tiempo de 2 NOP, osea 2 microsegundos, para que 
		el puerto se regule.*/
		//Cuando se usen delays en las simulaciones, se debe poner un 1 dentro del paréntesis sino se queda mucho tiempo trabado el simulador.
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador
	}
}