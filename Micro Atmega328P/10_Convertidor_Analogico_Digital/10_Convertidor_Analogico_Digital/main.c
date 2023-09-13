/*
 * 10_Convertidor_Analogico_Digital.c
 *
 * Created: 18/05/2022 06:58:05 p. m.
 * Author : diego
 10.- CONVERTIDOR ADC: El tipo de ADC que tiene el Arduino se llama de Aproximaciones Sucesivas, este tiene una resoluci�n de 10 bits (con valores digitales de tensi�n de 0 a 
 1023), teniendo en cuenta que la versi�n del Arduino UNO cuenta con 6 pines de entrada anal�gica que se encuentran en el Puerto C, a los cuales se les puede configurar la 
 entrada anal�gica que se quiere usar, la referencia anal�gica interna que asigna valores de tensi�n reales a los valores digitales de tensi�n, la frecuencia de muestreo y 
 los factores que causan los muestreos en el ADC.
 
	- Muestrear: Se refiere a tomar valores de tensi�n cada cierto tiempo para aproximar una se�al anal�gica a una digital, este ritmo de muestreo normalmente va de la mano 
	  con la frecuencia del reloj del convertidor anal�gico digital, que puede venir incluido en un microcontrolador o puede estar en un circuito integrado aparte. El 
	  muestreo trabaja con el eje horizontal de la se�al, osea el tiempo.
		- Sampling rate (Velocidad de muestreo): Se refiere a la velocidad que indica cu�ntas veces por segundo se est� realizando un muestreo de la se�al anal�gica para 
		  crear su se�al digital correspondiente.
	- Retener: Es un concepto que tiene que ver con la electr�nica del ADC, en donde se retiene informaci�n recabada de la se�al anal�gica.
	- Cuantificar: En esta fase se asocia cada valor de amplitud anal�gica a un valor digital que est� limitado al n�mero de bits de resoluci�n del ADC. La cuantificaci�n 
	  trabaja con el eje vertical de la se�al, osea la amplitud de la tensi�n el�ctrica.
		- Sampling depth (Resoluci�n): La profundidad de muestreo es la m�xima amplitud que puede guardar el convertidor en un n�mero binario, esto depende del circuito 
		  integrado o del microcontrolador que tenga el convertidor anal�gico digital o digital anal�gico.

 Con el ADC se har�n los siguientes ejercicios:
 1.- ADC con muestreo de desbordamiento usando un factor de divisi�n de 128 y el timer T0 de 8 bits, osea conteo de 0 a 255, escala de 1024. 
 Se�al anal�gica con un potenci�metro y mostrar el valor en el LCD.
 2.- ADC con muestreo de corrida libre usando un factor de divisi�n de 2. 
 Se�al anal�gica con un potenci�metro y mostrar el valor en el LCD.
 

 Para el ejercicio se utilizar�n los puertos: PB0 (pin 8 puerto B), PC0 (pin A0 puerto C) y PD0 (pin 0 puerto D).
 - Puerto B, Tiene 6 pines disponibles: A estos pines se conectar�n los leds que muestran la conversi�n anal�gica.
 Pin 8	(PB0); Led para se�al anal�gica
 Pin 9	(PB1); Led para se�al anal�gica; OCR1A, Se�al de salida con frecuencia variable del timer T1, temporizador por comparaci�n canal A
 Pin 10	(PB2); Led para se�al anal�gica; OCR1B, Se�al de salida con frecuencia variable del timer T1, temporizador por comparaci�n canal B
 Pin 11	(PB3); Led para se�al anal�gica; OCR2A, Se�al de salida con frecuencia variable del timer T2, temporizador por comparaci�n canal A
 Pin 12	(PB4); Led para se�al anal�gica
 Pin 13	(PB5); Led para se�al anal�gica
 - Puerto C, Tiene 6 pines disponibles para las entradas anal�gicas, se usa para las entradas del ADC:
 Pin A0	(PC0); Entrada anal�gica ADC0; Entrada anal�gica a analizar
 Pin A1	(PC1); Entrada anal�gica ADC1
 Pin A2	(PC2); Entrada anal�gica ADC2
 Pin A3	(PC3); Entrada anal�gica ADC3
 Pin A4	(PC4); Entrada anal�gica ADC4
 Pin A5	(PC5); Entrada anal�gica ADC5
 - Puerto D, Tiene 8 pines disponibles:
 Pin 0	(PD0)
 Pin 1	(PD1)
 Pin 2	(PD2)
 Pin 3	(PD3)
 Pin 4	(PD4)
 Pin 5	(PD5); OCR0B, Se�al de salida PWM del timer T0, temporizador por comparaci�n canal B
 Pin 6	(PD6); OCR0A, Se�al de salida PWM del timer T0, temporizador por comparaci�n canal A
 Pin 7	(PD7)
 
 Registros que controlan el ADC del ATMEGA328P: ADMUX, ADCSCRA y ADCSCRB.
 
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
 temporizadores CTC.
 */ 



//IMPORTACI�N DE LIBRER�AS, DIRECTIVAS
#include <avr/io.h>			//Librer�a para todo el manejo de puertos y dem�s aspectos del microcontrolador AVR Atmega328P
#include <avr/interrupt.h>	//Librer�a para el manejo de los registros de interrupciones
#include <avr/cpufunc.h>	



//Funciones ISR: Tareas ejecutadas por las interrupciones externas.
/*ISR (ADC_vect): Se le llama ISR o Interruption Service Routine a la funci�n que describa lo que se tiene que hacer cuando sea activada una interrupci�n, la palabra ISR es 
una palabra reservada de interrupciones y el nombre de esta funci�n no debe ser decladada en ning�n lado del c�digo, simplemente por haber sido nombrada como ISR, se ejecuta 
cuando sea detonada la interrupci�n que recibe como par�metro, para ello la interrupci�n INT0 ya debe haber sido configurada dentro del m�todo main. Para ello se le pasa como 
par�metro a la funci�n el vector INT0_vect para que ejecute la acci�n cuando la interrupci�n externa INT0 sea detonada y se debe hacer una funci�n ISR aparte para manejar la 
acci�n que detona la interrupci�n externa INT1.*/
/*Las acciones de las interrupciones externas deben ser descritas en una funci�n fuera del m�todo main, estas reciben como par�metro lo siguiente:
	- ADC_vect: Interrupci�n activada por la conversi�n ADC en el Arduino.*/
//ISR INTERRUPCIONES POR ADC:
ISR(ADC_vect){
	/*Almacenamiento del n�mero binario resultado de la conversi�n anal�gica digital que representa la amplitud de tensi�n en la se�al anal�gica: El resultado de la conversi�n 
	ACD se guarda en un n�mero binario de 10 bits, estos a fuerza se deben guardar en dos registros distintos llamados ADCH y ADCL, donde el bit ADLAR indica el orden en el 
	que se guardan, pero normalmente cuando ADLAR = 0, se acomodan de la siguiente manera:
		- ADCH: Guarda los 2 bits m�s significativos de los 10 bits resultantes de la conversi�n ADC.
		- ADCL: Guarda los 8 bits menos significativos de los 10 bits resultantes de la conversi�n ADC.*/
	PORTB = ADCL;			//Asingaci�n de los 2 bits m�s significativos de la conversi�n ADC.
	
	/*INICIALIZAR LA CONVERSI�N ADC - ADCSRA: Registro encargado de iniciar la conversi�n del ADC, para activar la conversi�n anal�gica digital se debe modificar el estado de 
	los bits ADEN y ADSC. El bit ADEN se modifica en la configuraci�n del ADC y el bit ADSC se modifica en la ejecuci�n del programa porque es el que inicializa al ADC.
		- ADEN: Bit que activa o desactiva el ADC del microcontrolador:
			- ADEN = 1: Activaci�n de la conversi�n ADC.
				- ADSC = 1: Iniciaci�n de la conversi�n, mientras el bit del registro se mantenga en su valor uno l�gico, la conversi�n ADC sigue en proceso.
				- ADSC = 0: Fin de la conversi�n, cuando se acabe la conversi�n ADC el bit cambiar� a cero l�gico por s� solo.
			- ADEN = 0: Desactivaci�n de la conversi�n ADC.*/
	ADCSRA |= (1<<ADSC); //Iniciaci�n de la conversi�n ADC; ADSC = 1
}



//M�TODO MAIN O EJECUTABLE: Desde este m�todo se ejecuta todo el c�digo y es lo que diferenc�a la librer�a de un ejecutable.
/*Para configurar correctamente un programa que use interrupciones se utiliza la siguiente arquitectura en el programa:
	- CLI: Activaci�n de interrupciones asignando un valor 0 l�gico al bit GI, volviendo la interrupci�n de tipo MI.
	- Programaci�n de m�quina y configuraci�n de interrupciones con los registros TCCR0A, TCCR0B (registro de control del temporizador A y B), TIMSK0 y OCR0A (registro de 
	comparaci�n), adem�s se activan las interrupciones externas con los registros EIMSK (m�scara de interrupciones) y EICRA (registro de control de interrupciones).
	- SEI: Desactivaci�n de interrupciones asignando un valor 1 l�gico al bit GI, volviendo la interrupci�n de tipo NMI.
	- Ejecuci�n del programa.
Con dicha arquitectura nos aseguramos de manejar propiamente los pines de los puertos que manejan las interrupciones, activ�ndolas y desactiv�ndolas cuando es necesario.*/
int main(void)
{
	/*CLI: Cuando se activa una CLI interruption, autom�ticamente el bit 7 del status register que es GI (General Interruption) se convierte en 0 l�gico, lo que convierte a 
	las interrupciones en MI (Maskable Interrupt), esto en t�rminos m�s simples significa que las interrupciones est�n activas:
		- Cuando GI tiene valor de 1 l�gico no est� activada ninguna interrupci�n, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 l�gico est� activada la interrupci�n, osea que es MI, Maskable Interrupt.*/
	cli(); //Evita cualquier interrupci�n que entre mientras ya se est� ejecutando una o antes de programar la m�quina, ya que termine, permite la ejecuci�n de interrupciones.
	
	
	
	//PROGRAMACI�N DE LA M�QUINA: Se indica por medio de los registros de funci�n espec�fica si los pines de los puertos ser�n entradas o salidas o el estado de los timers.
	//0: Pin como entrada
	//1: Pin como salida
	DDRB = 0xFF; //Todos los pines del puerto B son salidas a leds.
	DDRC = 0x00; //Todos los pines del puerto C son entradas disponibles para el ADC.	
	
	//CONFIGURACI�N DEL ADC: Los registros que controlan principalmente el ADC del ATMEGA328P son ADMUX y ADCSCRA.
	/*ADMUX: Registro que sirve para indicar cu�l de los 6 pines del Puerto C se tomar� como entrada para el ADC, el tipo de referencia anal�gica y el tipo de almacenamiento
	del n�mero binario resultante de la conversi�n anal�gica-digital.*/
	/*ENTRADA ANAL�GICA - ADMUX: Indica cuales de los 6 pines del Puerto C se tomar�n como entradas anal�gicas, todas las se�ales anal�gicas llegan a un Multiplexor porque 
	existe un solo convertidor anal�gico digital. El multiplexor se controla con los 4 bits MUX3, MUX2, MUX1 y MUX0:
		- ADC0: Pin A0	(PC0) como entrada anal�gica
			- MUX3 = 0
			- MUX2 = 0
			- MUX1 = 0
			- MUX0 = 0
		- ADC1: Pin A1	(PC1) como entrada anal�gica
			- MUX3 = 0
			- MUX2 = 0
			- MUX1 = 0
			- MUX0 = 1
		- ADC2: Pin A2	(PC2) como entrada anal�gica
			- MUX3 = 0
			- MUX2 = 0
			- MUX1 = 1
			- MUX0 = 0
		- ADC3: Pin A3	(PC3) como entrada anal�gica
			- MUX3 = 0
			- MUX2 = 0
			- MUX1 = 1
			- MUX0 = 1
		- ADC4: Pin A4	(PC4) como entrada anal�gica
			- MUX3 = 0
			- MUX2 = 1
			- MUX1 = 0
			- MUX0 = 0
		- ADC5: Pin A5	(PC5) como entrada anal�gica
			- MUX3 = 0
			- MUX2 = 1
			- MUX1 = 0
			- MUX0 = 1
		- ADC8: Activaci�n del sensor de temperatura interno del microcontrolador ATMEGA328P, este es de gran utilidad para medir la temperatura a la que est� sometido el 
		mismo microcontrolador cuando se utiliza en condiciones de temperatura extrema, pero su desventaja es que no es muy preciso, teniendo una sensibilidad de �10�C.
			- MUX3 = 1
			- MUX2 = 0
			- MUX1 = 0
			- MUX0 = 0*/
	ADMUX |= (0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(1<<MUX0); //ADC0: Pin A0 (PC0) como entrada anal�gica; MUX3 = 0, MUX2 = 0, MUX1 = 0, MUX0 = 0
	/*REFERENCIA ANAL�GICA - ADMUX: Elige una de las 3 opciones de referencia anal�gica para realizar la conversi�n del n�mero binario resultante de la conversi�n ADC a un 
	valor de tensi�n real a trav�s de los 2 bits REFS1 y REFS0:
		- Referencia Anal�gica Externa: Es una referencia dictada por la tensi�n que entre por el pin AREF del Puerto B (2 lugares arriba del pin 13) que puede ir de 0 a 5V.
			- REFS1 = 0
			- REFS0 = 0
		- Referencia Anal�gica Default: Es la referencia que tiene el mismo valor que la alimentaci�n del Arduino que es VCC = 5V. Su problema es que no es tan precisa, m�s 
		  a�n cuando se alimenta al Arduino con una bater�a externa, ya que la tensi�n de esta bater�a ir� bajando mientras se descargue durante su uso.
			- REFS1 = 0
			- REFS0 = 1
		- Reservado: No se puede hacer nada con esta opci�n.
			- REFS1 = 1
			- REFS0 = 0
		- Referencia Anal�gica Interna: Es una referencia incorporada dentro del microcontrolador ATMEGA328P que tiene valor de 1.1 V. Es muy estable, pero se debe lograr que 
		  la se�al de entrada externa tenga una amplitud m�xima de 1.1 V, esto se puede lograr con un divisor de tensi�n.
			- REFS1 = 1
			- REFS0 = 1*/
	ADMUX |= (1<<REFS1)|(1<<REFS0); //Referencia Anal�gica Interna; REFS1 = 1, REFS0 = 1
	/*ALMACENAMIENTO DEL N�MERO BINARIO RESULTADO DEL ADC - ADMUX: Dependiendo del estado del bit ADLAR, el resultado de 10 bits se acomoda de una forma o de otra en los dos 
	registros llamados ADCH y ADCL:
		- ADLAR = 0
			- ADCH: Guarda los 2 bits m�s significativos de los 10 bits resultantes de la conversi�n ADC hasta la derecha de su registro.
			- ADCL: Guarda los 8 bits menos significativos de los 10 bits resultantes de la conversi�n ADC.
		- ADLAR = 1
			- ADCH: Guarda los 8 bits m�s significativos de los 10 bits resultantes de la conversi�n ADC.
			- ADCL: Guarda los 2 bits menos significativos de los 10 bits resultantes de la conversi�n ADC hasta la izquierda de su registro.*/
	ADMUX |= (0<<ADLAR); //ADCL: Guarda los 8 bits menos significativos de los 10 bits resultantes de la conversi�n ADC; ADLAR = 0
	
	//ADCSRA (Se�ora ADC): Registro encargado de activar la conversi�n e indicar la frecuencia de muestreo del ADC.
	/*ACTIVAR LA CONVERSI�N ADC - ADCSRA: Registro encargado de activar la conversi�n ADC.
		- El bit ADEN se encarga directamente de activar o desactivar la conversi�n ADC:
			- ADEN = 0; Conversi�n ADC desactivada; Valor que viene por default.
			- ADEN = 1; Conversi�n ADC activada.
		- El bit ADATE se encarga de definir el modo de trabajo del ADC, ya que el ADC del ATMEGA328P es de tipo aproximaciones sucesivas, esto se refiere a que realiza �n� 
		comparaciones sucesivas de la se�al anal�gica para realizar su conversi�n, pero se puede hacer una sola si se quiere, teniendo las siguientes opciones:
			- ADATE = 0; Conversi�n �nica de un dato anal�gico.
			- ADATE = 1; Activaci�n del ADC de aproximaciones sucesivas.
		- El bit ADIE activa o desactiva las interrupciones causadas por el convertidor ADC, que se activar� en una funci�n ISR (Interruption Service Routine), usada para 
		manejar interrupciones en general, utilizando el vector ADC_vect como su par�metro:
			- ADIE = 0; El ADC trabaja sin realizar ninguna interrupci�n.
			- ADIE = 1; El ADC puede realizar interrupciones, causadas cuando termine de realizar la conversi�n de una se�al anal�gica a digital.*/
	ADCSRA |= (1<<ADEN)|(0<<ADATE)|(1<<ADIE); //Conversi�n ADC activada, ADEN = 1; Activaci�n del ADC de aproximaciones sucesivas, ADATE = 1; ADC con interrupciones, ADIE = 1
	/*FRECUENCIA DE MUESTREO DEL ADC (PRESCALER) - ADCSRA: Los bits ADPS2, ADPS1 y ADPS0 sirven para cambiar la frecuencia de muestreo del ADC, dividiendo la frecuencia del 
	reloj del ATMEGA328P que es de 16 MHz entre ciertos factores de divisi�n y donde siguiendo el teorema de Nyquist sabemos que su frecuencia debe ser el doble que la m�xima 
	del ancho de banda de la se�al.
		- Factor de divisi�n 2: Se divide la se�al del reloj de 16X10^6 Hz:		f_muestreo = fmicro / factor_division = 16X10^6 / 2 = 8X10^6 =		8 MHz
			- ADPS2 = 0
			- ADPS1 = 0
			- ADPS0 = 0
		- Factor de divisi�n 2: Se divide la se�al del reloj de 16X10^6 Hz:		f_muestreo = fmicro / factor_division = 16X10^6 / 2 = 8X10^6 =		8 MHz
			- ADPS2 = 0
			- ADPS1 = 0
			- ADPS0 = 1
		- Factor de divisi�n 4: Se divide la se�al del reloj de 16X10^6 Hz:		f_muestreo = fmicro / factor_division = 16X10^6 / 4 = 4X10^6 =		4 MHz
			- ADPS2 = 0
			- ADPS1 = 1
			- ADPS0 = 0
		- Factor de divisi�n 8: Se divide la se�al del reloj de 16X10^6 Hz:		f_muestreo = fmicro / factor_division = 16X10^6 / 8 = 2X10^6 =		2 MHz
			- ADPS2 = 0
			- ADPS1 = 1
			- ADPS0 = 1
		- Factor de divisi�n 16: Se divide la se�al del reloj de 16X10^6 Hz:	f_muestreo = fmicro / factor_division = 16X10^6 / 16 = 1X10^6 =		1 MHz
			- ADPS2 = 1
			- ADPS1 = 0
			- ADPS0 = 0
		- Factor de divisi�n 32: Se divide la se�al del reloj de 16X10^6 Hz:	f_muestreo = fmicro / factor_division = 16X10^6 / 32 = 1X10^6 =		0.5 MHz =	500 kHz
			- ADPS2 = 1
			- ADPS1 = 0
			- ADPS0 = 1
		- Factor de divisi�n 64: Se divide la se�al del reloj de 16X10^6 Hz:	f_muestreo = fmicro / factor_division = 16X10^6 / 64 = 1X10^6 =		0.25 MHz =	250 kHz
			- ADPS2 = 1
			- ADPS1 = 1
			- ADPS0 = 0
		- Factor de divisi�n 128: Se divide la se�al del reloj de 16X10^6 Hz:	f_muestreo = fmicro / factor_division = 16X10^6 / 128 = 1X10^6 =	0.125 MHz =	125 kHz
			- ADPS2 = 1
			- ADPS1 = 1
			- ADPS0 = 1*/
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Factor de divisi�n de 128, frecuencia de 125 kHz; ADPS2 = 1, ADPS1 = 1, ADPS0 = 1
	
	/*ADCSRB: Registro utilizado para definir un modo de operaci�n del convertidor ADC, solamente se puede usar cuando se trabaja con aproximaciones sucesivas, osea cuando 
	el bit ADATE del registro ADCSRA est� como 1 l�gico.*/
	/*FRECUENCIA DE MUESTREO DEL ADC (PRESCALER) - ADCSRA: Los bits ADPS2, ADPS1 y ADPS0 sirven para cambiar la frecuencia de muestreo del ADC, dividiendo la frecuencia del 
	reloj del ATMEGA328P que es de 16 MHz entre ciertos factores de divisi�n y donde siguiendo el teorema de Nyquist sabemos que su frecuencia debe ser el doble que la m�xima 
	del ancho de banda de la se�al.
		- Modo de corrida libre: Realiza el muestreo de la se�al y la convierte a su equivalente digital sin estar sujeto a ninguna condici�n.
			- ADTS2 = 0
			- ADTS1 = 0
			- ADTS0 = 0
		- Comparador anal�gico: Utiliza un comparador interno, entregando una tensi�n u otra en la salida dependiendo de cu�l amplitud es mayor en las dos se�ales anal�gicas 
		comparadas.
			- ADTS2 = 0
			- ADTS1 = 0
			- ADTS0 = 1
		- Llamado de la ingterrupci�n externa: En este modo se activa la conversi�n anal�gica digital solamente cuando se active alguna interrupci�n externa INT0 del pin PD2 
		o INT1 del pin PD3 (ambas del Puerto D).
			- ADTS2 = 0
			- ADTS1 = 1
			- ADTS0 = 0
		- Temporizador T0 por comparaci�n: Activaci�n de la conversi�n anal�gica digital solamente cuando se active la interrupci�n por un temporizador de tipo comparaci�n, 
		ya sea CTC o PWM de alg�n tipo. Se debe utilizar el temporizador T0 de 8 bits.
			- ADTS2 = 0
			- ADTS1 = 1
			- ADTS0 = 1
		- Temporizador T0 por desbordamiento: Activaci�n de la conversi�n anal�gica digital solamente cuando se active la interrupci�n por desbordamiento. Se debe utilizar el 
		temporizador T0 de 8 bits.
			- ADTS2 = 1
			- ADTS1 = 0
			- ADTS0 = 0
		- Temporizador T1 por comparaci�n B: Activaci�n de la conversi�n anal�gica digital solamente cuando se active la interrupci�n por un temporizador de tipo comparaci�n,
		ya sea CTC o PWM de alg�n tipo. Se debe utilizar el temporizador T1 de 16 bits.
			- ADTS2 = 1
			- ADTS1 = 0
			- ADTS0 = 1
		- Temporizador T1 por desbordamiento: Activaci�n de la conversi�n anal�gica digital solamente cuando se active la interrupci�n por desbordamiento. Se debe utilizar el
		temporizador T1 de 16 bits.
			- ADTS2 = 1
			- ADTS1 = 1
			- ADTS0 = 0
		- Temporizador T1 por captura de evento: La captura es parte de los timers, pero es activado solamente cuando se utiliza alg�n tipo de comunicaci�n I2C, Bluetooth, etc. 
		Solamente se puede usar con el timer 1 este tipo de muestreo.
			- ADTS2 = 1
			- ADTS1 = 1
			- ADTS0 = 1*/
	ADCSRB |= (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); //Modo de corrida libre; ADTS2 = 0, ADTS1 = 0, ADTS0 = 0
		
	
	
	/*SEI: Cuando se activa una SEI interruption, autom�ticamente el bit 7 del status register (GI) se convierte en 1 l�gico, lo que convierte a las interrupciones en NMI 
	(No Mask Interruption):
		- Cuando GI tiene valor de 1 l�gico no est� activada ninguna interrupci�n, osea que es NMI, Non Maskable Interrupt.
		- Cuando GI tiene valor de 0 l�gico est� activada la interrupci�n, osea que es MI, Maskable Interrupt.*/
	sei(); //Permite cualquier interrupci�n que entre ya despu�s de haber programado la m�quina y cuando haya terminado de ejecutarse una interrupci�n anterior.
	
	
	
	//EJECUCI�N DEL PROGRAMA:
	/*INICIALIZAR LA CONVERSI�N ADC - ADCSRA: Registro encargado de iniciar la conversi�n del ADC, para activar la conversi�n anal�gica digital se debe modificar el estado de 
	los bits ADEN y ADSC. El bit ADEN se modifica en la configuraci�n del ADC y el bit ADSC se modifica en la ejecuci�n del programa porque es el que inicializa al ADC.
		- ADEN: Bit que activa o desactiva el ADC del microcontrolador:
			- ADEN = 1: Activaci�n de la conversi�n ADC.
				- ADSC = 1: Iniciaci�n de la conversi�n, mientras el bit del registro se mantenga en su valor uno l�gico, la conversi�n ADC sigue en proceso.
				- ADSC = 0: Fin de la conversi�n, cuando se acabe la conversi�n ADC el bit cambiar� a cero l�gico por s� solo.
			- ADEN = 0: Desactivaci�n de la conversi�n ADC.*/
	ADCSRA |= (1<<ADSC); //Iniciaci�n de la conversi�n ADC; ADSC = 1
	
	while(1)
	{
		/*Se recomienda en la bibliograf�a que cuando se hace una asignaci�n al PIN, se le de un tiempo de 2 NOP, osea 2 microsegundos, para que 
		el puerto se regule.*/
		//Cuando se usen delays en las simulaciones, se debe poner un 1 dentro del par�ntesis sino se queda mucho tiempo trabado el simulador.
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador
		asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador
	}
}