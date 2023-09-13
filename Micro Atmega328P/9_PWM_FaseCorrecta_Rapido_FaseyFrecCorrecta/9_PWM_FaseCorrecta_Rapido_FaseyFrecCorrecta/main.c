/*
 * 9_PWM_Fase_Correcta.c
 *
 * Created: 30/05/2022 11:09:18 a. m.
 * Author : diego
 9.- SE�ALES PWM: Este tipo de se�ales se crean a partir de temporizadores por comparaci�n que desencadenan interrupciones cuando el registro contador TCNT# alcanza el valor 
 de un registro comparador OCR#A (Output Compare Register T# A) u OCR#B. Los modos de se�ales PWM tienen la gran diferencia con el tipo de comparador CTC que cuando se compara 
 el valor del registro contador con alguno de los registros comparadores y tienen el mismo valor, se dispara la interrupci�n, creando as� un cambio de estado l�gico en la 
 se�al PWM, pero esto no significa que el valor del contador vuelva a ser cero, ya que el registro TCNT# seguir� contando hasta que se desborde, en ese entonces repetir� el 
 ciclo y crear� la siguiente parte de la se�al PWM, adem�s el punto m�ximo del conteo donde se desborda puede ser modificado con un registro OCRA o ICR1, dependiendo del tipo 
 de comparador que se elija en la tabla de cada comparador, existen 3 tipos generales de se�ales PWM que se pueden crear con el modo comparador:
	-	PWM fase correcta: Realiza su cambio de valor l�gico cuando TCNT# = OCR#A, pero no lleva a cabo un desborde cuando ocurre esta igualdad, sino que cuando llegue al 
		valor m�ximo del contador, decrementar� su valor poco a poco hasta llegar nuevamente al cero. Creando as� una se�al triangular de baja frecuencia con conteo 
		ascendente y descendente, contando durante todo el pico del tri�ngulo de la se�al, logrando as� que el valor l�gico del duty cycle se encuentre en la parte media del 
		ciclo de la se�al. NO se permite variar la frecuencia. Se recomienda usarse en aplicaciones de: Control de motores CD.
	-	PWM r�pido o Fast PWM: Realiza su cambio de valor l�gico cuando TCNT# = OCR#A, pero lleva a cabo su desborde cuando se llega al valor m�ximo del contador, volviendo a 
		contar desde cero. Crea una se�al de diente de sierra de alta frecuencia y su conteo es ascendente solamente. Si se permite variar la frecuencia. Se recomienda usarse 
		en aplicaciones de: Rectificaci�n de se�ales, regulaci�n de tensi�n, conversi�n de se�al digital a anal�gica, convertidor CD-CD, etc.
	-	PWM fase y frecuencia correcta: Es muy parecido al modo fase correcta, con la �nica diferencia de que si se permite variar la frecuencia, a fuerza se debe indicar con 
		alg�n registro ya sea OCRA o ICR1 el valor m�ximo del conteo y no se puede indicar la resoluci�n, siempre ser� de 8 bits. Se recomienda usarse en aplicaciones de: 
		Control de motores CD.
 Cada uno de los modos de creaci�n de se�ales PWM tienen sus propias f�rmulas para calcular su frecuencia, periodo, tiempo de duty cycle, valor del registro comparador OCR#A u 
 OCR#B y valor TOP del m�ximo conteo almacenado en los registros OCRA o ICR1.

 Para el ejercicio se configurar�n 3 se�ales PWM con tres modos de creaci�n de se�ales PWM dtsintos que son: PWM fase correcta, PWM r�pido y PWM fase y frecuencia correcta.
 T0: Frecuencia de 1kHz;	T = 1ms;	Fast PWM, ciclo de trabajo de 25% en OCR1A en modo no invertido y de 75% en OCR1B en modo no invertido
 T1: Frecuencia de 500 Hz;	T = 2ms;	Fast PWM, ciclo de trabajo de 15% en OCR1A en modo no invertido y de 50% en OCR1B en modo no invertido, modo ICR1 activado.
 T2: Frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta, 

 Para el ejercicio se utilizar�n los puertos: PB0 (pin 8 puerto B), PC0 (pin A0 puerto C) y PD0 (pin 0 puerto D).
 - Puerto B, Tiene 6 pines disponibles:
 Pin 8	(PB0)
 Pin 9	(PB1); OCR1A, Se�al de salida PWM del timer T1, temporizador por comparaci�n canal A; Led con frecuencia de 1kHz; T = 1ms; PWM Fase correcta, Modo no inverso
 Pin 10	(PB2); OCR1B, Se�al de salida PWM del timer T1, temporizador por comparaci�n canal B; Led con frecuencia de 1kHz; T = 1ms; PWM Fase correcta, Modo inverso
 Pin 11	(PB3); OCR2A, Se�al de salida PWM del timer T2, temporizador por comparaci�n canal A
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
 Pin 2	(PD2); INT0, Interrupci�n externa 0
 Pin 3	(PD3); OCR2B, Se�al de salida PWM del timer T2, temporizador por comparaci�n canal B; INT1, Interrupci�n externa 1
 Pin 4	(PD4); T0, Temporizador 0
 Pin 5	(PD5); OCR0B, Se�al de salida PWM del timer T0, temporizador por comparaci�n canal B
 Pin 6	(PD6); OCR0A, Se�al de salida PWM del timer T0, temporizador por comparaci�n canal A
 Pin 7	(PD7)
 
 Recordemos que en el proyecto se usan los puertos descritos a continuaci�n para las siguientes funciones:
 PUERTO C - LCD: Aunque en este caso se utilizar� el puerto PC0 para observar una de las se�ales de salida PWM, la de Fast PWM.
 Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
 DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada o conect�ndose a tierra, por lo tanto solamente 6 bits en total contando los 2 de control son los 
 que se conectan al Arduino y son los siguientes:
 Arduino:	A5,		A4,		A3,		A2,		A1,		A0
 LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

 PUERTO D - TECLADO MATRICIAL 4X4: En este caso no se va a conectar, ya que se quiere usar el pin PD0 para observar la se�al del modo PWM fase y frecuencia correcta.
 La asignaci�n de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexi�n del teclado matricial vistos de izquierda
 a derecha son los siguientes:
 Puerto D Arduino (Digital PWM):		7,		6,		5,		4,		3,		2,		1,		0
 Constantes del teclado matricial 4X4:	F1,		F2,		F3,		F4,		C1,		C2,		C3,		C4
 Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8

 PUERTO B - DISPOSITIVOS EXTERNOS VARIADOS: Leds, botones, etc. Aunque en este caso se utilizar� el puerto PB0 para observar una de las se�ales PWM Fase correcta.
 temporizadores CTC.*/



//IMPORTACI�N DE LIBRER�AS, DIRECTIVAS
#include <avr/io.h>			//Librer�a para todo el manejo de puertos y dem�s aspectos del microcontrolador AVR Atmega328P
#include <avr/interrupt.h>	//Librer�a para el manejo de los registros de interrupciones



//Funciones ISR: Cuando se crea una se�al PWM, estas no se utilizan.



//M�TODO MAIN O EJECUTABLE: Desde este m�todo se ejecuta todo el c�digo y es lo que diferenc�a la librer�a de un ejecutable.
/*Para configurar correctamente un programa que use interrupciones se utiliza la siguiente arquitectura en el programa:
	- CLI: Activaci�n de interrupciones asignando un valor 0 l�gico al bit GI, volviendo la interrupci�n de tipo MI.
	- Programaci�n de m�quina y configuraci�n de interrupciones con los registros TCCR#A, TCCR#B (registro de control del temporizador A y B), TIMSK#,  y OCR#B (registros de 
	comparaci�n canal A y B).
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
	/*Cuando se usan se�ales PWM existen puertos espec�ficos PWM por los cuales salen los canales A y B de los timers T0, T1 y T2:
	T0 - Pines PWM:
		PD6 (Pin 6):	OC0A, osea el canal A de la se�al PWM del timer T0.
		PD5 (Pin 5):	OC0B, osea el canal B de la se�al PWM del timer T0.
	T1 - Pines PWM:
		PB1 (Pin 9):	OC1A, osea el canal A de la se�al PWM del timer T1.
		PB2 (Pin 10):	OC1B, osea el canal B de la se�al PWM del timer T1.
	T2 - Pines PWM:
		PB3 (Pin 11):	OC2A, osea el canal A de la se�al PWM del timer T2.
		PD3 (Pin 3):	OC2B, osea el canal B de la se�al PWM del timer T2.
	Cuando se declaren los pines se deben declarar todos de jal�n cuando pertenezcan a un mismo puerto sino la salida no se va a ver reflejada en el Arduino.*/
	//Temporizador T0: Pines 6 y 5	(PD6 y PD5) del Puerto D como salidas del canal A y B;		frecuencia de 500 Hz;	T = 2ms;	Fast PWM, Valor max OCR0A activado.
	//Temporizador T2: Pines 11 y 3	(PB3 y PD3) del Puerto B y D como salidas del canal A y B;	frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta.
	DDRD = (1<<DDD6)|(1<<DDD5)|(1<<DDD3);
	//Temporizador T1: Pines 9 y 10	(PB1 y PB2) del Puerto B como salidas del canal A y B;		frecuencia de 1 Hz;		T = 1s;		Fast PWM.
	DDRB = (1<<DDB1)|(1<<DDB2)|(1<<DDB3);
	//Temporizador T2: Pines 11 y 3	(PB3 y PD3) del Puerto B y D como salidas del canal A y B;	frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta.
	//DDRB = (1<<DDB3);
	//DDRB = (1<<DDB3);
	
	//GTCCR o registro general de control del temporizador y contador: Sirve para cuando se usa m�s de un timer, reiniciando sus conteos y escalas, logrando su sincronizaci�n.
	/*El registro GTCCR cuenta con varias herramientas para sincronizar los 3 timers:
		- TSM: Bit del registro GTCCR para la sincronizaci�n de timers, con 1 l�gico hace que todos los temporizadores empiecen a contar al mismo tiempo y no manejen tiempos 
		distintos, mientras que con 0 l�gico pueden empezar a contar cuando quieran pero manejar�n tiempos distintos.
		- PSRASY: Bit del registro GTCCR para el reinicio del timer T2, con 1 l�gico se reinicia el conteo de T2 a cero y su escala se pone en 0, con 0 l�gico deja al 
		temporizador T2 con el �ltimo valor de conteo y escala con el que se qued�.
		- PSRSYNC: Bit del registro GTCCR para la reiniciar el conteo de los temporizadores T0 y T1, con 1 l�gico se reinicia su valor y se pone su escala en 0, mientras que 
		con 0 l�gico se deja a los temporizadores T0 y T1 con el �ltimo valor de conteo que ten�an y escala con la que se quedaron.
	Cuando se usa m�s de un timer para manejo de tiempos, es necesario usar este registro, adem�s esto se debe hacer justo antes de configurar los timers, una vez 
	configurados, se debe volver a hacer que este registro valga 0.
	No es tan importante usar el registro GTCCR cuando se usan interrupciones externas, pero cuando se usa temporizadores si lo es, ya que puede evitar que falle el programa, 
	adem�s cuando se usan varios temporizadores a la vez, deben usarse en Puertos diferentes, sino fallar�n.*/
	GTCCR = (1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC); //TSM -> Timers sincronizados; PSRASY -> Timer T2 reiniciado; PSRSYNC -> Timers T0 y T1 reiniciados.
	
	/*Los bits WGM#1 y WGM#0 del registro TCCR#A y el bit WGM#2 del registro TCCR#B se cambian solamente cuando se est� utilizando el temporizador por comparaci�n, ya que a 
	trav�s de �l se indica el tipo de comparaci�n que se va a efectuar, ya sea CTC o uno de los tipos PWM:
	T0 y T2:
		- Normal: En esta configuraci�n se encuentra desactivado el modo de temporizador por comparaci�n y el timer.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Fase Correcta: Permite usar ambos canales y su resoluci�n es de 8 bits. No permite variar la frecuencia de la se�al. Genera una se�al triangular para motores CD.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 1 (TCCR#A)
		- CTC (Clear to Compare): Modo de comparaci�n donde cuando el temporizador alcanza el mismo nivel que el valor de comparaci�n, se reinicia a 0 el valor del 
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la se�al y solo deja utilizar el comparador OCRA, con este valor se 
		fija el valor m�ximo al que llegar� la se�al de diente de sierra.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 1 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM R�pido: Permite usar ambos canales y su resoluci�n es de 8 bits. Tiene el doble de la frecuencia de la se�al PWM de fase correcta. Genera una se�al de diente de 
		sierra. Ideal para servomotores.
			- WMG#2 = 0 (TCCR#B)
			- WMG#1 = 1 (TCCR#A)
			- WMG#0 = 1 (TCCR#A)
		- Reservado: No se puede usar.
			- WMG#2 = 1 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM Fase Correcta OCRA: Valor m�ximo personalizado con el registro OCRA, para utilizar la se�al PWM se debe usar el canal B. No permite variar la frecuencia de la 
		se�al. Genera una se�al triangular. Ideal para motores CD.
			- WMG#2 = 1 (TCCR#B)
			- WMG#1 = 0 (TCCR#A)
			- WMG#0 = 1 (TCCR#A)
		- Reservado: No se puede usar.
			- WMG#2 = 1 (TCCR#B)
			- WMG#1 = 1 (TCCR#A)
			- WMG#0 = 0 (TCCR#A)
		- PWM R�pido OCRA: Valor m�ximo personalizado con el registro OCRA, para utilizar la se�al PWM se debe usar el canal B. Tiene el doble de la frecuencia de la se�al 
		PWM de fase correcta. Genera una se�al de diente de sierra. Ideal para servomotores.
			- WMG#2 = 1 (TCCR#B)
			- WMG#1 = 1 (TCCR#A)
			- WMG#0 = 1 (TCCR#A)
	La resoluci�n se refiere al n�mero m�ximo al que puede contar el comparador OCR#A u OCR#B.
	T1:
		- Normal: En esta configuraci�n se encuentra desactivado el modo de temporizador por comparaci�n y el timer.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Fase Correcta (8 bits): Resoluci�n de 8 bits. No permite variar la frecuencia de la se�al. Genera una se�al triangular, ideal para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM Fase Correcta (9 bits): Resoluci�n de 9 bits. No permite variar la frecuencia de la se�al. Genera una se�al triangular, ideal para motores CD.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM Fase Correcta (10 bits): Resoluci�n de 10 bits. No permite variar la frecuencia de la se�al. Genera una se�al triangular, ideal para motores CD.
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
		- PWM R�pido (8 bits): Resoluci�n de 8 bits, osea que puede contar de 0 hasta 1111 1111. Tiene el doble de la frecuencia de la se�al PWM de fase correcta, ideal para 
		servomotores y su valor m�ximo es el 0x00FF = 0000 0000 1111 1111, lo que indica que tiene una resoluci�n de 8 bits, porque es el n�mero m�ximo al que llega.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM R�pido (9 bits): Resoluci�n de 9 bits, osea que puede contar de 0 hasta 0001 1111 1111. Tiene el doble de la frecuencia de la se�al PWM de fase correcta, ideal 
		para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM R�pido (10 bits): Resoluci�n de 10 bits, osea que puede contar de 0 hasta 0011 1111 1111. Tiene el doble de la frecuencia de la se�al PWM de fase correcta, 
		ideal para servomotores.
			- WMG13 = 0 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- Fase y frecuencia correcta (ICR1): Valor m�ximo personalizado con el registro ICR1. Tiene la frecuencia y fase correcta de la se�al PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- Fase y frecuencia correcta (OCR1A): Valor m�ximo personalizado con el registro OCR1A. Tiene la frecuencia y fase correcta de la se�al PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 0 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- PWM fase correcta (ICR1): Valor m�ximo personalizado con el registro ICR1. Tiene la fase correcta de la se�al PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM fase correcta (OCR1A): Valor m�ximo personalizado con el registro OCR1A. Tiene la fase correcta de la se�al PWM, ideal para motores CD.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 0 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
		- CTC (Clear to Compare) ICR1: Modo de comparaci�n donde cuando el temporizador alcanza el mismo nivel que el valor de comparaci�n, se reinicia a 0 el valor del
		temporizador para volver a realizar el conteo desde el inicio. Permite variar la frecuencia de la se�al y solo permite utilizar el comparador ICR1 para hacer 0 el 
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
		- PWM R�pido ICR1: Valor m�ximo personalizado con el registro ICR1. Permite variar la frecuencia de la se�al.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 0 (TCCR#A)
		- PWM R�pido OCRA: Valor m�ximo personalizado con el registro OCR1A. Permite variar la frecuencia de la se�al.
			- WMG13 = 1 (TCCR#B)
			- WMG12 = 1 (TCCR#B)
			- WMG11 = 1 (TCCR#A)
			- WMG10 = 1 (TCCR#A)
			
	Para T0 se utilizar� la comparaci�n - PWM R�pido (8 bits).
	Para T1 se utilizar� la comparaci�n - PWM R�pido (8 bits), el valor m�ximo que puede alcanzar es 0x00FF = 0000 0000 1111 1111, resoluci�n de 8 bits.
	Para T2 se utilizar� la comparaci�n - PWM R�pido (8 bits).*/
	
	/*TCCR0A o registro de control del temporizador A: Sirve para indicar el tipo de comparaci�n en el temporizador T#, accede a los bits WMG#1 y WMG#0, adem�s por medio de 
	los bits COM#A1 y COM#A0 indica el tipo de se�al obtenida en la salida OCR#A.*/
	/*Recordemos que la se�al del temporizador es un diente de sierra con frecuencia variable y la se�al de salida en OCR#A u OCR#B es una se�al cuadrada con la misma 
	frecuencia que la se�al de diente de sierra, la forma de la se�al cuadrada obtenida es descrita despu�s usando el registro TCCR#A, las distintas salidas de se�al cuadrada 
	que se puede obtener en la salida del comparador dictadas por los bits COM#A1 y COM#A0 son descritas a continuaci�n:
		- Operaci�n normal del puerto: OC0 desconectado, osea que no crea una se�al cuadrada de salida a partir de la se�al de diente de sierra del temporizador.
			- COM0A1 = 0 (Canal A)
			- COM0A0 = 0 (Canal A)
			
			- COM0B1 = 0 (Canal B)
			- COM0B0 = 0 (Canal B)
		- Cambio l�gico de OC0: Con cualquier cambio l�gico de 0 a 1 o 1 a 0 en la se�al de diente de sierra se ejecuta la interrupci�n, creando la se�al cuadrada que cambia 
		de 0 a 1 o 1 a 0 cuando haya un cambio de valor m�ximo a valor m�nimo o m�nimo a m�ximo en la se�al original.
			- COM0A1 = 0 (Canal A)
			- COM0A0 = 1 (Canal A)
			
			- COM0B1 = 0 (Canal B)
			- COM0B0 = 1 (Canal B)
		- Cero l�gico en OC0 (Modo no invertido): La se�al cuadrada empieza valiendo 1 l�gico y cuando se ejecute la interrupci�n en la se�al de diente de sierra, cambia de 1 
		a 0 en la se�al cuadrada y posteriormente cambia de 0 a 1 cuando se encuentre con otra interrupci�n. Esta es la que m�s se elige para obtener una se�al con frecuencia 
		variable. Para cuando se usa en se�ales PWM se llama Modo no invertido e implica que se use directamente el valor calculado de OCR#A o OCR#B.
			- COM0A1 = 1 (Canal A)
			- COM0A0 = 0 (Canal A)

			- COM0B1 = 1 (Canal B)
			- COM0B0 = 0 (Canal B)
		- Uno l�gico en OC0 (Modo invertido): La se�al cuadrada empieza valiendo 0 l�gico y cuando se ejecute la interrupci�n en la se�al de diente de sierra, cambia de 0 a 1 
		en la se�al cuadrada y posteriormente cambia de 1 a 0  cuando se encuentre con otra interrupci�n. Este es el inverso de la configuraci�n pasada.
		Para cuando se usa en se�ales PWM se llama Modo invertido e implica que cuando se calcule su duty cycle se haga la resta del valor m�ximo menos el OCR#A o OCR#B 
		calculado.
			- COM0A1 = 1 (Canal A)
			- COM0A0 = 1 (Canal A)
			
			- COM0B1 = 1 (Canal B)
			- COM0B0 = 1 (Canal B)
	Recordemos que con las se�ales PWM se pueden usar los dos canales A y B por cada uno de los temporizadores T0, T1 y T2, teniendo 6 se�ales PWM disponibles, en el 
	ejercicio se usar�n de forma diferente los canales para obtener distintos modos de se�ales PWM*/
	//T0: Frecuencia de 1kHz;	T = 1ms;	PWM Fase correcta.
	//T1: Frecuencia de 1Hz;	T = 1s;		Fast PWM, ciclo de trabajo de 25% en OCR1A en modo no invertido y de 75% en OCR1B en modo no invertido.
	//T2: Frecuencia de 0.5Hz;	T = 2s;		PWM fase y frecuencia correcta,
	
	//TCCR#A o registro de control del temporizador A: Sirve para indicar el tipo de salida y tipo de comparaci�n en el temporizador T#, accede a los bits WMG#1 y WMG#0.
	//CONFIGURACI�N SE�AL PWM CANAL A:
	TCCR0A |= (1<<COM0A1)|(0<<COM0A0);	//Configuraci�n Cero l�gico en OC0 (Modo no invertido); COM0A1 = 1, COM0A0 = 0; T0 - Canal A
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0);	//Configuraci�n Cero l�gico en OC1 (Modo no invertido); COM1A1 = 1, COM1A0 = 0; T1 - Canal A
	TCCR2A |= (1<<COM2A1)|(1<<COM2A0);	//Configuraci�n Cero l�gico en OC2 (Modo no invertido);	COM2A1 = 1, COM2A0 = 1; T2 - Canal A
	//CONFIGURACI�N SE�AL PWM CANAL B:
	TCCR0A |= (1<<COM0B1)|(1<<COM0B0);	//Configuraci�n Uno l�gico en OC0 (Modo no invertido);	COM0B1 = 1, COM0B0 = 0; T0 - Canal B
	TCCR1A |= (1<<COM1B1)|(1<<COM1B0);	//Configuraci�n Uno l�gico en OC1 (Modo invertido);		COM1B1 = 1, COM1B0 = 1; T1 - Canal B
	TCCR2A |= (1<<COM2B1)|(1<<COM2B0);	//Configuraci�n Uno l�gico en OC2 (Modo invertido);		COM2B1 = 1, COM2B0 = 1; T2 - Canal B
	//TIPO DE COMPARADOR - PARTE 1/2: Esto se aplica a ambos canales A y B del mismo temporizador.
	TCCR0A |= (1<<WGM01)|(1<<WGM00);	//Configuraci�n de PWM R�pido (OCRA); TCCR0B -> WMG02 = 1; TCCR0A -> WMG01 = 1 y WMG00 = 1
	TCCR1A |= (0<<WGM11)|(1<<WGM10);	//Configuraci�n de PWM R�pido (8 bits); TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 1; T1 se maneja distinto.
	TCCR2A |= (1<<WGM21)|(0<<WGM20);	//Configuraci�n de PWM Fase Correcta OCRA; TCCR2B -> WMG22 = 1; TCCR2A -> WMG21 = 0 y WMG20 = 1
	
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
	-	La instrucci�n |= lo que hace es solo asignar el valor de un bit 1 de un n�mero binario sin afectar a los dem�s.
	Se pueden obtener frecuencias mayores con escalas peque�as y frecuencias menores con escalas grandes.*/
	//TCCR#B o registro de control del temporizador: Sirve para indicar la configuraci�n de escala y tipo de comparador.
	//ESCALA DEL TEMPORIZADOR:
	TCCR0B |= (1<<CS02)|(0<<CS01)|(1<<CS00);			//T0: Escala de 1024;	CS02 = 1, CS01 = 0 y CS00 = 1
	TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10);			//T1: Escala de 1024;	CS12 = 1, CS11 = 0 y CS10 = 1
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20)|(0<<WGM22);	//T2: Escala de 1024;	CS22 = 1, CS21 = 1 y CS20 = 1, no se puede usar un timer externo con T2, se maneja diferente.
	//TIPO DE COMPARADOR - PARTE 2/2:
	/*Aguas porque asignar ceros solos, normalmente causa problemas, si se quiere poner expl�citamente, debe estar incluido dentro de una declaraci�n con varios bits, en 
	espec�fico se encuentra declarada en los registros TCCR#B de arriba, por eso en esta parte se coment� y solo se dej� indicado para que fuera m�s entendible.*/
	TCCR0B |= (1<<WGM02);				//Configuraci�n de PWM R�pido (OCRA); TCCR0B -> WMG02 = 1; TCCR0A -> WMG01 = 1 y WMG00 = 1
	TCCR1B |= (0<<WGM13)|(1<<WGM12);	//Configuraci�n de PWM R�pido (8 bits); TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 1; T1 se maneja distinto.
	//TCCR2B &= (0<<WGM22);				//Configuraci�n de comparador CTC; TCCR2B -> WMG22 = 0; TCCR2A -> WMG21 = 1 y WMG20 = 0
	
	
	//TIMSK o registro de control del temporizador: Sirve para indicar que el temporizador sea de tipo desbordamiento o comparaci�n e indicar el canal A o B del comparador.
	/*Los bits TOIE2, TOIE1 Y TOIE0 sirven para indicar cu�l de los temporizadores T2, T1 o T0 queremos que sea de tipo desbordamiento, para que pueda ser de ese tipo se debe 
	mandar un 1 l�gico a trav�s de cada uno de ellos y a trav�s del registro OCIE#A o OCIE#B se elige el canal activado del comparador mandando un 1 l�gico, aunque es 
	importante saber que en el comparador CTC solo se puede utilizar el canal A:
		- TOIE2: Hace que el timer T2 de 8 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.
		- TOIE1: Hace que el timer T1 de 16 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.
		- TOIE0: Hace que el timer T0 de 8 bits sea de tipo desbordamiento si se le manda un 1 l�gico y si se queda como 0 l�gico es de tipo comparaci�n.
		
		- OCIE2A: Hace que el timer T2 de 8 bits sea de tipo comparaci�n y que se active el registro OCR0A o OCR0B para marcar el l�mite del conteo.
		- OCIE1A: Hace que el timer T1 de 16 bits sea de tipo comparaci�n y que se active el registro OCR1A o OCR1B para marcar el l�mite del conteo.
		- OCIE0A: Hace que el timer T0 de 8 bits sea de tipo comparaci�n y que se active el registro OCR2A o OCR2B para marcar el l�mite del conteo.*/
	TIMSK0 |= (0<<TOIE0)|(1<<OCIE0A)|(1<<OCIE0B); //Timer T0 de 8 bits de tipo comparaci�n, registro de comparaci�n OCR0A y OCR0B activados, se puede usarlos en PWM.
	TIMSK1 |= (0<<TOIE1)|(1<<OCIE1A)|(1<<OCIE1B); //Timer T1 de 16 bits de tipo comparaci�n, registro de comparaci�n OCR1A y OCR1B activados, se puede usarlos en PWM.
	TIMSK2 |= (0<<TOIE2)|(1<<OCIE2A)|(1<<OCIE2B); //Timer T2 de 8 bits de tipo comparaci�n, registro de comparaci�n OCR2A y OCR2B activados, se puede usarlos en PWM.
	
	/*MANEJO DEL TEMPORIZADOR - FRECUENCIA, PERIODO, DUTY CYCLE Y COMPARADOR PWM: Existen f�rmulas espec�ficas para cada tipo de se�al PWM, adem�s recordemos que su frecuencia, 
	periodo y conteo del comparador pueden no solo depender de su registro OCR#, sino de su m�ximo llamado TOP, que es descrito dependiendo de la resoluci�n o tipo de modo 
	PWM.*/
	/*En el modo PWM si se pueden usar ambos canales con cada temporizador, dando como total 6 se�ales PWM, dos canales A y B por cada uno de los 3 temporizadores T0, T1 y T2,
	pero dependiendo de si se quiere usar el modo TOP para definir cierto valor m�ximo de conteo personalizado, se pueden usar ambos canales o solo uno.
	
-	Con los timers T0 y T2 solo se puede usar el registro OCR#A para definir el valor TOP, por lo cual si este se usa, solo se podr� usar el canal B para obtener la se�al PWM.
-	Con el timer T1 se pueden usar los registros OCRA# o ICR1 para definir el valor TOP, por lo cual se podr�n usar ambos canales A y B para obtener la se�al PWM, a menos que
	se decida usar el registro OCRA# para definir el valor TOP, en cuyo caso solo se podr� obtener la se�al PWM por el canal B.
	
	Normalmente se hace uso del valor de conteo m�ximo TOP cuando el conteo del registro para llegar a cierta frecuencia frecuencia de la se�al PWM requerida es demasiado 
	grande para ser soportado por el registro contador, reduciendo as� el valor del conteo:
	T0 y T2: Contadores de 8 bits.
		Si el resultado de OCR#A o OCR#B es mayor que el conteo m�ximo de los registros contadores:
			Los temporizadores T0 y T2 siempre tienen resoluci�n de 8 bits:	0000 1111 1111 = 255.
		Si el conteo es mayor que el m�ximo que soportan los registros OCR#A o OCR#B dependiendo de la resoluci�n del temporizador, es necesario usar el valor TOP para poder 
		generar esa se�al PWM.
	
	T1: Contadores de 16 bits.
		Si el resultado de OCR#A o OCR#B es mayor que el conteo m�ximo de los registros contadores, que depende de la resoluci�n de 8, 9 o 10 bits, ya que nunca puede contar 
		los 16 bis de conteo con los que dispone, los valores m�ximos a los que puede contar sin usar el registro TOP son los siguientes:
			Resoluci�n de 8 bits:	0000 1111 1111 = 255.
			Resoluci�n de 9 bits:	0001 1111 1111 = 511.
			Resoluci�n de 10 bits:	0011 1111 1111 = 1023.
		Si el conteo es mayor que el m�ximo que soportan los registros OCR#A o OCR#B dependiendo de la resoluci�n elegida, es necesario usar el valor TOP para poder generar 
		esa se�al PWM.
	
	F�RMULAS:
	- PWM Fase correcta: De baja frecuencia, para control de motores CD principalmente, se puede definir el valor m�ximo de conteo TOP, pero no es necesario si no se quiere, 
	en cualquier caso los resultados del c�lculo se guardar�n en los registros ICR1, OCR1A o OCR1B, dependiendo de la configuraci�n que se haya elegido y el temporizador que 
	se est� utilizando.
	Modo sin TOP personalizado definido; Como salida se pueden utilizar los canales A (OCR#A) y B (OCR#B); Se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- C�lculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR#, el valor del ConteoM�ximo_PWM depende de la resoluci�n del temporizador:
			1.1.0.- frecuencia = fmicro / (2*escala*OCR#)
			1.1.1.-		OCR# = fmicro / (2*escala*frecuencia)
			1.2.0.- Periodo = T = (2*escala*OCR#) / fmicro = 1 / frecuencia
			1.2.1.-		OCR# = (T*fmicro) / (2*escala)
			1.3.-	t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			1.4.-	OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- C�lculo del valor que tendr� el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la se�al de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			2.1.-	OCR#I_dutycycle = ConteoM�ximo_PWM - OCR#_dutycycle
			2.2.-	OCR#NI_dutycycle = OCR#_dutycycle
			2.3.-	OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.-	OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignaci�n de resultados a los registros correspondientes:
			2.3.-	OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.-	OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
	Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definici�n del valor m�ximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- ICR1 = TOP
		- C�lculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR# con valor m�ximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T * Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- C�lculo del valor que tendr� el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la se�al de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignaci�n de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.-OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
	Modo OCR#A = TOP; Como salida se puede utilizar solo el canal B (OCR1B); Se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Definici�n del valor m�ximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- OCR1A = TOP
		- C�lculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR# con valor m�ximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- C�lculo del valor que tendr� el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la se�al de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignaci�n de resultados a los registros correspondientes:
			1.3.- OCR#A = TOP
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle


	- Fast PWM: De alta frecuencia, para rectificaci�n de se�ales, regulaci�n de tensi�n, conversi�n de se�al digital a anal�gica, convertidor CD-CD, etc. se puede definir el
	valor m�ximo de conteo TOP, pero no es necesario si no se quiere, en cualquier caso los resultados del c�lculo se guardar�n en los registros ICR1, OCR1A o OCR1B,
	dependiendo de la configuraci�n que se haya elegido y el temporizador que se est� utilizando.
	Modo sin TOP personalizado definido; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- C�lculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR#, el valor del ConteoM�ximo_PWM depende de la resoluci�n del temporizador:
			1.1.0.- frecuencia = fmicro / (escala * (1 + OCR#))
			1.1.1.-		OCR# = (fmicro / (frecuencia*escala)) - 1
			1.2.0.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
			1.2.1.-		OCR# = ((T*fmicro) / escala)) - 1
			1.3.-	t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			1.4.-	OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- C�lculo del valor que tendr� el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la se�al de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			2.1.-	OCR#I_dutycycle = ConteoM�ximo_PWM - OCR#_dutycycle
			2.2.-	OCR#NI_dutycycle = OCR#_dutycycle
			2.3.-	OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.-	OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignaci�n de resultados a los registros correspondientes:
			2.3.-	OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.-	OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle

	Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definici�n obligatoria del valor m�ximo de conteo personalizado TOP:
			1.1.- TOP = (fmicro / (frecuencia*escala)) - 1
			1.2.- TOP = ((T*fmicro) / escala)) - 1
			1.3.- ICR1 = TOP
		- C�lculo de frecuencia, periodo y tiempo de duty cycle con valor m�ximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (escala * (1 + OCR#))
			2.2.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T * Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- C�lculo del valor que tendr� el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la se�al de salida 
		PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignaci�n de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
	Modo OCR#A = TOP; Como salida se puede utilizar solo el canal B (OCR1B); se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Definici�n obligatoria del valor m�ximo de conteo personalizado TOP:
			1.1.- TOP = (fmicro / (frecuencia*escala)) - 1
			1.2.- TOP = ((T*fmicro) / escala)) - 1
			1.3.- OCR#A = TOP
		- C�lculo de frecuencia, periodo y tiempo de duty cycle con valor m�ximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (escala * (1 + OCR#))
			2.2.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T * Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- C�lculo del valor que tendr� el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la se�al de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignaci�n de resultados a los registros correspondientes:
			1.3.- OCR#A = TOP
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle


	- PWM fase y frecuencia correcta: Tiene las mismas f�rmulas que el modo PWM Fase correcta, pero adem�s a fuerza debe ser definido el m�ximo del conteo, tambi�n llamado TOP,
	ambos resultados se guardar�n en los registros ICR1, OCR1A o OCR1B, dependiendo de la configuraci�n que se haya elegido y el temporizador que se est� utilizando.
	Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definici�n obligatoria del valor m�ximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- ICR1 = TOP
		- C�lculo de frecuencia, periodo y tiempo de duty cycle con valor m�ximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR1_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- C�lculo del valor que tendr� el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la se�al de salida 
		  PWM, OCR1I_dutycycle = Modo invertido;	OCR1NI_dutycycle = Modo no invertido:
			3.1.- OCR1I_dutycycle = ConteoM�ximo_PWM - OCR#_dutycycle
			3.2.- OCR1NI_dutycycle = OCR#_dutycycle
			3.3.- OCR1A = OCR1I_dutycycle  o OCR1NI_dutycycle
			3.4.- OCR1B = OCR1I_dutycycle  o OCR1NI_dutycycle
		- Asignaci�n de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1A = OCR1I_dutycycle  o OCR1NI_dutycycle
			3.4.- OCR1B = OCR1I_dutycycle  o OCR1NI_dutycycle
	
	Modo OCR#A = TOP; Como salida se puede utilizar solo el canal B (OCR1B); se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Definici�n obligatoria del valor m�ximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- OCR1A = TOP
		- C�lculo de frecuencia, periodo y tiempo de duty cycle con valor m�ximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado = Porcentaje_deseado / frecuencia
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- C�lculo del valor que tendr� el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la se�al de salida 
		PWM, OCR1I_dutycycle = Modo invertido;	OCR1NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = ConteoM�ximo_PWM - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignaci�n de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1B = OCR1I_dutycycle  o OCR1NI_dutycycle

	Se debe tomar en cuenta que las frecuencias mayores a las cuales pueden acceder los registros T0, T1 y T2 son las siguientes, eligiendo la escala m�s baja, ya que de esta
	manera se accede a mayores frecuencias, tomando en cuenta el conteo total de los temporizadores, adem�s es importante mencionar que frecuencias mayores se pueden obtener 
	por medio de los temporizadores de 8 bits y menores frecuencias con el temporizador de 16:
	Temporizadores de 8 bits T0 y T2 - Temporizador de 16 bits con resoluci�n de 8 bits:
	- PWM Fase correcta o PWM fase y frecuencia correcta: De baja frecuencia, para control de motores CD.
		1.1- frecuencia = fmicro / (2*escala*OCR#A) = 16X10^6 / (2*escala*OCR#A)
		conteo_total_temporizador T0 y T2, conteo_total_temporizador T1 - resoluci�n de 8 bits = 255 = OCR#A
			f_max -> escala = 1; f = 31372.5490	Hz = 31.3725	kHz
			f_max -> escala = 8; f = 3921.5686	Hz = 3.9215		kHz
		conteo_total_temporizador T1 - resoluci�n de 9 bits = 511 = OCR#A
			f_max -> escala = 1; f = 15655.5773	Hz = 15.6555	kHz
			f_max -> escala = 8; f = 1956.9471	Hz = 1.9569		kHz
		conteo_total_temporizador T1 - resoluci�n de 10 bits = 1,023 = OCR#A
			f_max -> escala = 1; f = 7820.1368	Hz = 7.8201		kHz
			f_max -> escala = 8; f = 977.5171	Hz
	Temporizador de 16 bits T1:
	- Fast PWM: De alta frecuencia, para rectificaci�n de se�ales, regulaci�n de tensi�n, conversi�n de se�al digital a anal�gica, convertidor CD-CD, control de servos, etc.
		1.1- frecuencia = fmicro / (escala*(1+OCR#A)) = 16X10^6 / (escala*(1+OCR#A))
		conteo_total_temporizador T0 y T2, conteo_total_temporizador T1 - resoluci�n de 8 bits = 255 = OCR#A
			f_max -> escala = 1; f = 62500		Hz = 62.5		kHz
			f_max -> escala = 8; f = 7812.5		Hz = 7.8125		kHz
		conteo_total_temporizador T1 - resoluci�n de 9 bits = 511 = OCR#A
			f_max -> escala = 1; f = 31250		Hz = 31.25		kHz
			f_max -> escala = 8; f = 3906.25	Hz = 3.9062		kHz
		conteo_total_temporizador T1 - resoluci�n de 10 bits = 1,023 = OCR#A
			f_max -> escala = 1; f = 15609.7561	Hz = 15.6097	kHz
			f_max -> escala = 8; f = 1951.2195	Hz = 1.9512		kHz
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
	Con el modo Fast PWM se llega a mucho mayores frecuencias que con los modos PWM Fase correcta o PWM fase y frecuencia correcta y mientras menor la resoluci�n, mayor la 
	frecuencia tambi�n.
	Cuando: OCR#A > conteo_total_temporizador, se debe calcular el n�mero de ciclos que se utilizar� en un condicional if dentro de la funci�n ISR del temporizador y dentro 
	del registro comparador se debe poner el valor m�ximo del conteo del temporizador:
		1.1.- ciclos = conteo_OCR /conteo_total_temporizador
		1.2.- OCR#A = conteo_total_temporizador
	Al llegar a un resultado de ciclos, simplemente se deber� crear una variable global que tenga este valor y un condicional if que vaya aumentando otra variable que empiece
	a contar desde cero y que ejecute la acci�n de la interrupci�n solamente hasta que la variable haya alcanzado los ciclos necesarios.*/
	
	
	/*T0 - Fast PWM - frecuencia de 500 Hz;	T = 2ms;	Fast PWM, ciclo de trabajo de 80% en OCR0B en modo no invertido con valor m�ximo personalizado con OCR0A; Escala 1024.
	Escalas disponibles:
	T0 y T1:
	- 1:1
	- 1:8
	- 1:64
	- 1:256
	- 1:1024
	Cuando se quiere declarar un valor m�ximo personalizado, lo que se hace es elegir un modo que tenga un TOP = ICR1 o OCR1A, luego se ejecutan las siguientes f�rmulas para 
	encontrar el valor de TOP y posteriormente este valor se intercambia por la variable OCR#A en las f�rmulas normales de la se�al PWM, esto se hace normalmente para poder
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



//Funciones ISR: Cuando se crea una se�al PWM, estas no se utilizan.