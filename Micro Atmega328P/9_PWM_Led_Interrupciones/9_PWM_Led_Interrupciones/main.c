/*
 * 9_PWM_Led_Interrupciones.c
 *
 * Created: 30/05/2022 11:09:18 a. m.
 * Author : diego
 9.- SEÑALES PWM - LEDS: Este tipo de señales se crean a partir de temporizadores por comparación que desencadenan interrupciones cuando el registro contador TCNT# alcanza el 
 valor de un registro comparador OCR#A (Output Compare Register T# A) u OCR#B. Los modos de señales PWM tienen la gran diferencia con el tipo de comparador CTC que cuando se 
 compara el valor del registro contador con alguno de los registros comparadores y tienen el mismo valor, se dispara la interrupción, creando así un cambio de estado lógico en 
 la señal PWM, pero esto no significa que el valor del contador vuelva a ser cero, ya que el registro TCNT# seguirá contando hasta que se desborde, en ese entonces repetirá el 
 ciclo y creará la siguiente parte de la señal PWM, además el punto máximo del conteo donde se desborda puede ser modificado con un registro OCRA o ICR1, dependiendo del tipo 
 de comparador que se elija en la tabla de cada comparador, existen 3 tipos generales de señales PWM que se pueden crear con el modo comparador:
	-	PWM fase correcta: Realiza su cambio de valor lógico cuando TCNT# = OCR#A, pero no lleva a cabo un desborde cuando ocurre esta igualdad, sino que cuando llegue al 
		valor máximo del contador, decrementará su valor poco a poco hasta llegar nuevamente al cero. Creando así una señal triangular de baja frecuencia con conteo 
		ascendente y descendente, contando durante todo el pico del triángulo de la señal, logrando así que el valor lógico del duty cycle se encuentre en la parte media del 
		ciclo de la señal. NO se permite variar la frecuencia. Se recomienda usarse en aplicaciones de: Control de motores CD.
	-	PWM rápido o Fast PWM: Realiza su cambio de valor lógico cuando TCNT# = OCR#A, pero lleva a cabo su desborde cuando se llega al valor máximo del contador, volviendo a 
		contar desde cero. Crea una señal de diente de sierra de alta frecuencia y su conteo es ascendente solamente. Si se permite variar la frecuencia. Se recomienda usarse 
		en aplicaciones de: Rectificación de señales, regulación de tensión, conversión de señal digital a analógica, convertidor CD-CD, control de servomotores, etc.
	-	PWM fase y frecuencia correcta: Es muy parecido al modo fase correcta, con la única diferencia de que si se permite variar la frecuencia, a fuerza se debe indicar con 
		algún registro ya sea OCRA o ICR1 el valor máximo del conteo y no se puede indicar la resolución, siempre será de 8 bits. Se recomienda usarse en aplicaciones de: 
		Control de motores CD.
 Cada uno de los modos de creación de señales PWM tienen sus propias fórmulas para calcular su frecuencia, periodo, tiempo de duty cycle, valor del registro comparador OCR#A u 
 OCR#B y valor TOP del máximo conteo almacenado en los registros OCRA o ICR1.

 Para el ejercicio se requiere escribir un programa en el cual se aumente o disminuya el ciclo de trabajo del PWM en modo de fase correcta y fase rápida mediante dos botones: 
	- El botón 1 se empleara para aumentar mediante la interrupción externa 1 (+).
	- El botón 2 se empleara para disminuir mediante la interrupción externa 0 (-).
Las salidas se ejecutarán utilizando solo el canal A de dos temporizadores distintos para utilizar el modo PWM Fase Correcta con T0 y Fast PWM con T1.

 Para el ejercicio se utilizarán los puertos: PB0 (pin 8 puerto B), PC0 (pin A0 puerto C) y PD0 (pin 0 puerto D).
 - Puerto B, Tiene 6 pines disponibles:
 Pin 8	(PB0)
 Pin 9	(PB1); OCR1A, Señal de salida PWM del timer T1, temporizador por comparación canal A; Led con frecuencia de 500 Hz; Duty Cycle = 2%; Fast PWM, no inverso
 Pin 10	(PB2); OCR1B, Señal de salida PWM del timer T1, temporizador por comparación canal B
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
 Pin 0	(PD0)
 Pin 1	(PD1)
 Pin 2	(PD2); INT0, Interrupción externa 0; Botón de - ; Para bajar duty cycle de la señal PWM
 Pin 3	(PD3); INT1, Interrupción externa 1; Botón de + ; Para subir duty cycle de la señal PWM; OCR2B, Señal de salida PWM del timer T2
 Pin 4	(PD4);
 Pin 5	(PD5); OCR0B, Señal de salida PWM del timer T0, temporizador por comparación canal B
 Pin 6	(PD6); OCR0A, Señal de salida PWM del timer T0, temporizador por comparación canal A; Led con frecuencia de 100 Hz; Duty Cycle = 1%; PWM Fase correcta, no inverso
 Pin 7	(PD7)
 
 Recordemos que en el proyecto se usan los puertos descritos a continuación para las siguientes funciones:
 PUERTO C - LCD:
 Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y
 DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada o conectándose a tierra, por lo tanto solamente 6 bits en total contando los 2 de control son los 
 que se conectan al Arduino y son los siguientes:
 Arduino:	A5,		A4,		A3,		A2,		A1,		A0
 LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS

 PUERTO D - TECLADO MATRICIAL 4X4: En este caso no se va a conectar, ya que se quiere usar el pin PD6 para observar la señal del modo PWM fase correcta.
 La asignación de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexión del teclado matricial vistos de izquierda
 a derecha son los siguientes:
 Puerto D Arduino (Digital PWM):		7,		6,		5,		4,		3,		2,		1,		0
 Constantes del teclado matricial 4X4:	F1,		F2,		F3,		F4,		C1,		C2,		C3,		C4
 Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8

 PUERTO B - DISPOSITIVOS EXTERNOS VARIADOS: Leds, botones, etc. Aunque en este caso se utilizará el puerto PB0 para observar una de las señales PWM Rápido.
 
 Para variar correctamente la luminusidad de un led por medio de una señal PWM, se deben elegir frecuencias en el rango de:		1 a 5		kHz, 
 aunque en este caso se utilizarán frecuencias menores de 100 y 500 Hz, veremos que igual pueden funcionar.
 Para variar correctamente la velocidad de un motor DC por medio de una señal PWM, se deben elegir frecuencias en el rango de:	5 a 10		kHz.
 Para variar correctamente la amplificación de audio por medio de una señal PWM, se deben elegir frecuencias en el rango de:	20 a 200	kHz.*/



//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>			//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <avr/interrupt.h>	//Librería para el manejo de los registros de interrupciones
#include "../2_LCD_4bits.h"	//Librería propia para el manejo del LCD
#include <stdio.h>			//Librería para la entrada, salida, manejo y conversión de datos, en específico se usa para poder utilizar el método sprintf()



//DECLARACIÓN DE VARIABLES GLOBALES (CABECERAS DE FUNCIÓN):
/*Variable que guarda el número que estará asociado al registro OCR#A (Output Compare Register T# A), que detona la interrupción del temporizador por comparación cuando
el registro TCNT# (que realiza el conteo) llegue al mismo valor que el registro OCR#A ((que marca el límite del conteo).*/
double Porcentaje_inicial_At0 = 2.6562;		//Valor de OCR0A inicial, PWM Fase Correcta; frecuencia de 100 [Hz], con un porcentaje inicial de 1%.
double Control_dutyCycle_At0 = 0;			//Aumento o disminución del duty cycle en el temporizador 0, en función del porcentaje inicial.
double Porcentaje_inicial_At1 = 2.125;		//Valor de OCR1A inicial, PWM Rápido; frecuencia de 500 [Hz], con un porcentaje inicial de 2%.
double Control_dutyCycle_At1 = 0;			//Aumento o disminución del duty cycle en el temporizador 0, en función del porcentaje inicial.
//El botón de - estará conectado al pin 8 (PB0) del Puerto B y el botón de + estará conectado al pin 9 (PB1) del Puerto B.
/*Para poder imprimir un número en pantalla se debe conocer el formato que utiliza cada uno y los tipos primitivos de datos en C:
int:	Números enteros, utilizan un espacio de memoria de 2 o 4 bytes. Su formato es el %d o %i.
float:	Números fraccional con máximo 7 decimales después del punto, utilizan un espacio de memoria de 4 bytes. Su formato es el %f.
double:	Números fraccional con máximo 15 decimales después del punto, utilizan un espacio de memoria de 8 bytes. Su formato es el %lf.
char:	Caracater, representa una sola letra o valor ASCII, utilizan un espacio de memoria de 1 byte. Su formato es el %s.*/
int	  pt0 = 0;				//Variable que guarda el porcentaje de duty cycle correspondiente a cada valor de la variable Control_dutyCycle_t0, porcentaje inicial de 5%.
int	  pt1 = 0;				//Variable que guarda el porcentaje de duty cycle correspondiente a cada valor de la variable Control_dutyCycle_t1, porcentaje inicial de 10%.
char  pt0_string[6];		//Variable guarda una cadena de caracteres representado como un array de caracteres de máximo 4 caracteres, osea un tipo de string.
char  pt1_string[6];		//Variable guarda una cadena de caracteres representado como un array de caracteres de máximo 4 caracteres, osea un tipo de string.



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
	ENVIA_CADENA("Duty cycle leds");
}



//Funciones ISR: Cuando se crea una señal PWM, no se deben utilizar las funciones ISR.
//Funciones ISR: Cuando se crean interrupciones si se utilizan las funciones ISR, ya que indican tareas ejecutadas por las interrupciones externas.
/*ISR (INT0_vect): Se le llama ISR o Interruption Service Routine a la función que describa lo que se tiene que hacer cuando sea activada una interrupción, la palabra ISR es 
una palabra reservada de interrupciones y el nombre de esta función no debe ser decladada en ningún lado del código, simplemente por haber sido nombrada como ISR, se ejecuta 
cuando sea detonada la interrupción que recibe como parámetro, para ello la interrupción INT0 ya debe haber sido configurada dentro del método main. Para ello se le pasa como 
parámetro a la función el vector INT0_vect para que ejecute la acción cuando la interrupción externa INT0 sea detonada y se debe hacer una función ISR aparte para manejar la 
acción que detona la interrupción externa INT1.*/
/*Las acciones de las interrupciones externas deben ser descritas en una función fuera del método main, estas reciben como parámetro lo siguiente:
	- INT0_vect: Interrupción externa 0 después de haber sido configurada con los registros EIMSK y EICRA, esta está asociada al pin 2 (PD2) del Puerto D en el Arduino
	- INT1_vect: Interrupción externa 1 después de haber sido configurada con los registros EIMSK y EICRA, esta está asociada al pin 3 (PD3) del Puerto D en el Arduino.*/
//ISR INTERRUPCIONES EXTERNAS:
//INTERRUPCIÓN EXTERNA INT0 DEL PUERTO D; INT0	(Pin 2 del Puerto D): REPRESENTA AL BOTÓN - PARA BAJAR EL DUTY CYCLE DE LA SEÑAL PWM.
ISR(INT0_vect){
	//INTERRUPCIÓN QUE VE SI SE PRESIONÓ EL BOTÓN PARA BAJAR EL PORCENTAJE DEL DUTY CYCLE
	Control_dutyCycle_At0--;		//T0: Porcentaje inicial de 1%
	Control_dutyCycle_At1--;		//T1: Porcentaje inicial de 2%
	
	//VARIABLES QUE MUESTRAN EL PORCENTAJE DEL DUTY CYCLE EN EL LCD
	pt0--;							//T0: Porcentaje inicial de 1%
	pt1-=2;							//T1: Porcentaje inicial de 2%
	
	//FIJAR EL PORCENTAJE MÍNIMO DEL DUTY CYCLE DE T0, BASADO EN LA FRECUENCIA INICIAL, QUE EN ESTE CASO ES DE 1% - PWM MODO FASE CORRECTA
	if(Control_dutyCycle_At0 <= 0){
		Control_dutyCycle_At0 = 0;	//Duty Cycle de 0% para T0.
		pt0 = 0;					//Valor mínimo del porcentaje
	}
	//FIJAR EL PORCENTAJE MÍNIMO DEL DUTY CYCLE DE T1, BASADO EN LA FRECUENCIA INICIAL, QUE EN ESTE CASO ES DE 2% - PWM RÁPIDO
	if(Control_dutyCycle_At1 <= 0){
		Control_dutyCycle_At1 = 0;	//Duty Cycle de 0% para T1.
		pt1 = 0;					//Valor mínimo del porcentaje
	}
}
//INTERRUPCIÓN EXTERNA INT1 DEL PUERTO D: INT1	(Pin 3 del Puerto D): REPRESENTA AL BOTÓN + PARA SUBIR EL DUTY CYCLE DE LA SEÑAL PWM.
ISR(INT1_vect){
	//INTERRUPCIÓN QUE VE SI SE PRESIONÓ EL BOTÓN PARA SUBIR EL PORCENTAJE DEL DUTY CYCLE
	Control_dutyCycle_At0++;		//T0: Porcentaje inicial de 1%
	Control_dutyCycle_At1++;		//T1: Porcentaje inicial de 2%
	
	//VARIABLES QUE MUESTRAN EL PORCENTAJE DEL DUTY CYCLE EN EL LCD
	pt0++;							//T0: Porcentaje inicial de 1%
	pt1+=2;							//T1: Porcentaje inicial de 2%
	
	//FIJAR EL PORCENTAJE MÁXIMO DEL DUTY CYCLE DE T0, BASADO EN LA FRECUENCIA INICIAL, QUE EN ESTE CASO ES DE 1% - PWM MODO FASE CORRECTA
	if(Control_dutyCycle_At0 >= 97){
		Control_dutyCycle_At0 = 96;	//Duty Cycle de 100% para T0 que tiene un porcentaje inicial de 1%.
		pt0 = 100;					//Valor máximo del porcentaje
	}
	//FIJAR EL PORCENTAJE MÁXIMO DEL DUTY CYCLE DE T1, BASADO EN LA FRECUENCIA INICIAL, QUE EN ESTE CASO ES DE 2% - PWM RÁPIDO
	if(Control_dutyCycle_At1 >= 97){
		Control_dutyCycle_At1 = 96;//Duty Cycle de 200% para T1 que tiene un porcentaje inicial de 2%.
		pt1 = 200;					//Valor máximo del porcentaje
	}
}



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
	En este ejercicio solo se usarán las salidas del canal A.*/
	//Temporizador T0: Pin 6 (PD6) del Puerto D como salida del canal A;		frecuencia de 100 Hz;	T = 10ms;	Fase correcta.
	DDRD = (1<<DDD6);
	//Temporizador T1: Pin 9 (PB1) del Puerto B como salida del canal A;		frecuencia de 500 Hz;	T = 2ms;	Fast PWM.
	DDRB = (1<<DDB1);
	DDRC = 0xFF;		//Todos los pines del puerto C son salidas al LCD.
	/*No se deben declarar las entradas de las interrupciones externas, sino el programa se confunde y realiza su función de forma incorrecta.
	DDRD = (0<<DDD2);	Pin 2 (PD2) del Puerto D como entrada, frecuencia variable;	Botón - para bajar duty cycle.
	DDRD = (0<<DDD3);	Pin 3 (PD3) del Puerto D como entrada, frecuencia variable;	Botón + para subir duty cycle.*/
	

	//CONFIGURACIÓN DE INTERRUPCIONES EXTERNAS:
	//Programación de máquina y configuración de interrupciones con los registros EIMSK y EICRA.
	//INT0 e INT1: Interrupciones externas asociadas a los pines 2 y 3 del Puerto D solamente.
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
	pines de interrupciones externas, configurados con los 4 bits ISC11, ISC10, ISC01 e ISC00 pertenecientes al registro EICRA, osea que con este registro se indica cuándo se 
	van a ejecutar las interrupciones cuando se genere un tipo de cambio específico en su señal de entrada.
	INT0:
		- ISC01: Bit 1 del registro EICRA, afecta a la interrupción externa INT0:
		- ISC00: Bit 0 del registro EICRA, afecta a la interrupción externa INT0:
			- ISC01 = 0 y ISC00 = 0; Interrupción de nivel bajo, osea cuando la señal de factor externo se encuentra en 0 lógico.
			- ISC01 = 0 y ISC00 = 1; Interrupción con cambio de estado lógico en la señal, osea cuando la señal de factor externo cambia de 0 a 1 o 1 a 0.
			- ISC01 = 1 y ISC00 = 0; Interrupción de flanco de bajada, osea cuando la señal de factor externo cambia de 1 a 0.
			- ISC01 = 1 y ISC00 = 1; Interrupción de flanco de subida, osea cuando la señal de factor externo cambia de 0 a 1.
	INT1:
		- ISC11: Bit 3 del registro EICRA, afecta a la interrupción externa INT1:
		- ISC10: Bit 2 del registro EICRA, afecta a la interrupción externa INT1:
			- ISC11 = 0 y ISC10 = 0; Interrupción de nivel bajo, osea cuando la señal de factor externo se encuentra en 0 lógico.
			- ISC11 = 0 y ISC10 = 1; Interrupción con cambio de estado lógico en la señal, osea cuando la señal de factor externo cambia de 0 a 1 o 1 a 0.
			- ISC11 = 1 y ISC10 = 0; Interrupción de flanco de bajada, osea cuando la señal de factor externo cambia de 1 a 0.
			- ISC11 = 1 y ISC10 = 1; Interrupción de flanco de subida, osea cuando la señal de factor externo cambia de 0 a 1.*/
	EICRA |= (1<<ISC01)|(1<<ISC00); //INT0 del registro EIMSK configurada con los bits ISC01 e ISC00 del registro EICRA, activando la interrupción de flanco de subida.
	EICRA |= (1<<ISC11)|(1<<ISC10); //INT1 del registro EIMSK configurada con los bits ISC11 e ISC10 del registro EICRA, activando la interrupción de flanco de subida.
	
	
	//CONFIGURACIÓN DE TEMPORIZADORES - PWM:
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
	
	/*Los bits WGM#1 y WGM#0 del registro TCCR#A y el bit WGM#2 o bits WGM#3 y WGM#2 del registro TCCR#B se cambian solamente cuando se está utilizando el temporizador por 
	comparación, ya que a través de estos dos registros se indica el tipo de comparación que se va a efectuar, ya sea CTC o uno de los tipos PWM:
	T0 y T2: Se usan solo 3 bits de configuración.
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
	T1: Se usan 4 bits de configuración.
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
			- WMG10 = 1 (TCCR#A)*/
	
	/*TCCR#A o registro de control del temporizador A: Sirve para indicar el tipo de comparación en el temporizador T#, accede a los bits WMG#1 y WMG#0, además por medio de 
	los bits COM#A1 y COM#A0 indica el tipo de señal obtenida en la salida OCR#A u OCR#B.*/
	/*Las distintas salidas de señal cuadrada PWM que se puede obtener en la salida del comparador dictadas por los bits COM#A1 y COM#A0 son descritas a continuación, las 
	más utilizadas en el modo PWM son las salidas de Cero lógico en OC0 (Modo no invertido) y Uno lógico en OC0 (Modo invertido), las cuales se consideran para realizar cierta 
	operación al calcular el valor de los registros OCR#A u OCR#B posteriormente:
		- Operación normal del puerto: OC0 desconectado, osea que no crea una señal cuadrada de salida a partir de la señal de diente de sierra del temporizador.
			- COM#A1 = 0 (Canal A)
			- COM#A0 = 0 (Canal A)
			
			- COM#B1 = 0 (Canal B)
			- COM#B0 = 0 (Canal B)
		- Cambio lógico de OC0: Con cualquier cambio lógico de 0 a 1 o 1 a 0 en la señal de diente de sierra se ejecuta la interrupción, creando la señal cuadrada que cambia 
		de 0 a 1 o 1 a 0 cuando haya un cambio de valor máximo a valor mínimo o mínimo a máximo en la señal original.
			- COM#A1 = 0 (Canal A)
			- COM#A0 = 1 (Canal A)
			
			- COM#B1 = 0 (Canal B)
			- COM#B0 = 1 (Canal B)
		- Cero lógico en OCR#A u OCR#B (Modo no invertido): La señal cuadrada empieza valiendo 1 lógico y cuando se ejecute la interrupción en la señal de diente de sierra, 
		cambia de 1 a 0 en la señal cuadrada y posteriormente cambia de 0 a 1 cuando se encuentre con otra interrupción. Esta es la que más se elige para obtener una señal 
		con frecuencia variable. Para cuando se usa en señales PWM se llama Modo no invertido e implica que se use directamente el valor calculado de OCR#A o OCR#B.
			- COM#A1 = 1 (Canal A)
			- COM#A0 = 0 (Canal A)

			- COM#B1 = 1 (Canal B)
			- COM#B0 = 0 (Canal B)
		- Uno lógico en OCR#A u OCR#B (Modo invertido): La señal cuadrada empieza valiendo 0 lógico y cuando se ejecute la interrupción en la señal de diente de sierra, 
		cambia de 0 a 1 en la señal cuadrada y posteriormente cambia de 1 a 0  cuando se encuentre con otra interrupción. Este es el inverso de la configuración pasada.
		Para cuando se usa en señales PWM se llama Modo invertido e implica que cuando se calcule su duty cycle se haga la resta del valor máximo menos el OCR#A o OCR#B 
		calculado.
			- COM#A1 = 1 (Canal A)
			- COM#A0 = 1 (Canal A)
			
			- COM#B1 = 1 (Canal B)
			- COM#B0 = 1 (Canal B)
	Recordemos que con las señales PWM se pueden usar los dos canales A y B por cada uno de los temporizadores T0, T1 y T2, teniendo 6 señales PWM disponibles*/
	//T0: Frecuencia de 1kHz;	T = 1ms;	PWM Fase correcta, ciclo de trabajo variable.
	//T1: Frecuencia de 1Hz;	T = 1s;		Fast PWM, ciclo de trabajo variable.
	
	//TCCR#A o registro de control del temporizador A: Sirve para indicar el tipo de salida y tipo de comparación en el temporizador T#, accede a los bits WMG#1 y WMG#0.
	//CONFIGURACIÓN SEÑAL PWM CANAL A:
	TCCR0A |= (1<<COM0A1)|(0<<COM0A0);//Configuración Cero lógico en OCR0A (Modo no invertido); COM0A1 = 1, COM0A0 = 0; T0 - Canal A
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0);//Configuración Cero lógico en OCR1A (Modo no invertido); COM1A1 = 1, COM1A0 = 0; T1 - Canal A
	//TIPO DE COMPARADOR - PARTE 1/2: Esto se aplica a ambos canales A y B del mismo temporizador.
	TCCR0A |= (0<<WGM01)|(1<<WGM00);//T0: Configuración de PWM Fase correcta (8 bits); TCCR0B -> WMG02 = 0; TCCR0A -> WMG01 = 0 y WMG00 = 1
	TCCR1A |= (0<<WGM11)|(1<<WGM10);//T1: Configuración de PWM Rápido (8 bits); TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 1; T1 se maneja distinto.
	
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
	TCCR0B |= (1<<CS02)|(0<<CS01)|(0<<CS00);			//T0: Escala de 256;	CS12 = 1, CS11 = 0 y CS10 = 0
	TCCR1B |= (1<<CS12)|(0<<CS11)|(0<<CS10);			//T1: Escala de 256;	CS12 = 1, CS11 = 0 y CS10 = 0
	//TIPO DE COMPARADOR - PARTE 2/2:
	/*Aguas porque asignar ceros solos, normalmente causa problemas, si se quiere poner explícitamente, debe estar incluido dentro de una declaración con varios bits, en 
	específico se encuentra declarada en los registros TCCR#B de arriba, por eso en esta parte se comentó y solo se dejó indicado para que fuera más entendible.*/
	//TCCR0B &= (0<<WGM02);			//T0: Configuración de PWM Fase correcta (8 bits); TCCR0B -> WMG02 = 0; TCCR0A -> WMG01 = 0 y WMG00 = 1
	TCCR1B |= (0<<WGM13)|(1<<WGM12);//T1: Configuración de PWM Rápido (8 bits); TCCR1B -> WMG13 = 0 y WMG12 = 1; TCCR1A -> WMG11 = 0 y WMG10 = 1; T1 se maneja distinto.
	
	/*TIMSK o registro de control del temporizador: Sirve para indicar que el temporizador sea de tipo desbordamiento o comparación e indicar si el canal A o B se usará. 
	Este registro no se debe colocar cuando se vaya a actualizar el valor del duty cycle de la señal PWM, ya que sino no se verá reflejada en la salida*/
	
	/*MANEJO DEL TEMPORIZADOR - FRECUENCIA, PERIODO, DUTY CYCLE Y COMPARADOR PWM: Existen fórmulas específicas para cada tipo de señal PWM, además recordemos que su frecuencia, 
	periodo y conteo del comparador pueden no solo depender de su registro OCR#, sino de su máximo valor de conteo llamado TOP, que es descrito dependiendo de la resolución o 
	tipo de modo PWM.*/
	/*En el modo PWM si se pueden usar ambos canales con cada temporizador, dando como total 6 señales PWM, dos canales A y B por cada uno de los 3 temporizadores T0, T1 y T2,
	pero dependiendo de si se quiere usar el modo TOP para definir cierto valor máximo de conteo personalizado, se pueden usar ambos canales o solo uno.
	
-	Con los timers T0 y T2 solo se puede usar el registro OCR#A para definir el valor TOP, por lo cual si este se usa, solo se podrá usar el canal B para obtener la señal PWM.
-	Con el timer T1 se pueden usar los registros OCRA# o ICR1 para definir el valor TOP, por lo cual se podrán usar ambos canales A y B para obtener la señal PWM, a menos que
	se decida usar el registro OCRA# para definir el valor TOP, en cuyo caso solo se podrá obtener la señal PWM por el canal B.
	
	Normalmente se hace uso del valor de conteo máximo TOP cuando el conteo del registro para llegar a cierta frecuencia de la señal PWM requerida es demasiado grande para ser 
	soportado por el registro contador, reduciendo así el valor del conteo cuando se usa TOP:
	T0 y T2: Contadores de 8 bits.
		Si el resultado de OCR#A u OCR#B es mayor que el conteo máximo de los registros contadores, que es el siguiente:
			Resolución de 8 bits:	0000 1111 1111 = 255.
		Donde los contadores T0 y T2 siempre tendrán resolución de 8 bits, es necesario usar el valor TOP para poder generar esa señal PWM.
	
	T1: Contadores de 16 bits.
		Si el resultado de OCR#A u OCR#B es mayor que el conteo máximo de los registros contadores, que depende de la resolución de 8, 9 o 10 bits, ya que nunca puede contar 
		los 16 bis de conteo con los que dispone, los valores máximos a los que puede contar sin usar el registro TOP son los siguientes:
			Resolución de 8 bits:	0000 1111 1111 = 255.
			Resolución de 9 bits:	0001 1111 1111 = 511.
			Resolución de 10 bits:	0011 1111 1111 = 1023.
		Si el conteo es mayor que el máximo que soportan los registros OCR#A o OCR#B dependiendo de la resolución elegida del contador T1, es necesario usar el valor TOP para 
		poder generar esa señal PWM.
	
	FÓRMULAS:
	- PWM Fase correcta: Señal PWM de baja frecuencia, para control de motores CD principalmente, se puede definir el valor máximo de conteo TOP, pero no es necesario si no se 
	quiere.
		Modo sin TOP personalizado definido; Como salida se pueden utilizar los canales A (OCR#A) y B (OCR#B) con cualquiera de los temporizadores T0, T1 o T2:
		- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR#, el valor del ConteoMáximo_PWM depende de la resolución del temporizador:
			1.1.0.- frecuencia = fmicro / (2*escala*OCR#)
				1.1.1.-	OCR# = fmicro / (2*escala*frecuencia)
			1.2.0.- Periodo = T = (2*escala*OCR#) / fmicro = 1 / frecuencia
				1.2.1.-	OCR# = (T*fmicro) / (2*escala)
			1.3.- t_dutycycle = T*Porcentaje_deseado_decimal = Porcentaje_deseado_decimal / frecuencia
			1.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			2.1.- OCR#I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
			2.2.- OCR#NI_dutycycle = OCR#_dutycycle
			2.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			2.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
		Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definición del valor máximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- ICR1 = TOP
		- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR# con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado_decimal = Porcentaje_deseado_decimal / frecuencia
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
		Modo OCR#A = TOP; Como salida se puede utilizar solo el canal B (OCR1B); Se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Definición del valor máximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- OCR1A = TOP
		- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR# con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado_decimal = Porcentaje_deseado_decimal / frecuencia
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- OCR#A = TOP
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle


	- Fast PWM: Señal PWM de alta frecuencia, para rectificación de señales, regulación de tensión, conversión de señal digital a analógica, convertidor CD-CD, etc. se puede 
	definir el valor máximo de conteo TOP, pero no es necesario si no se quiere.
		Modo sin TOP personalizado definido; Como salida se pueden utilizar los canales A (OCR#A) y B (OCR#B) con cualquiera de los temporizadores T0, T1 o T2:
		- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR#, el valor del ConteoMáximo_PWM depende de la resolución del temporizador:
			1.1.0.- frecuencia = fmicro / (escala * (1 + OCR#))
				1.1.1.- OCR# = (fmicro / (frecuencia*escala)) - 1
			1.2.0.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
				1.2.1.- OCR# = ((T*fmicro) / escala)) - 1
			1.3.- t_dutycycle = T*Porcentaje_deseado_decimal*(17/20) = Porcentaje_deseado_decimal*17 / frecuencia*20
			1.4.- OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			2.1.- OCR#I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
			2.2.- OCR#NI_dutycycle = OCR#_dutycycle
			2.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			2.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			2.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle

		Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definición obligatoria del valor máximo de conteo personalizado TOP:
			1.1.- TOP = (fmicro / (frecuencia*escala)) - 1
			1.2.- TOP = ((T*fmicro) / escala)) - 1
			1.3.- ICR1 = TOP
		- Cálculo de frecuencia, periodo y tiempo de duty cycle con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (escala * (1 + OCR#))
			2.2.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado_decimal*(17/20) = Porcentaje_deseado_decimal*17 / frecuencia*20
			2.4.- OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1A = OCR#I_dutycycle  o OCR#NI_dutycycle
			3.4.- OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
	
		Modo OCR#A = TOP; Como salida se puede utilizar solo el canal B (OCR1B); se puede utilizar con cualquiera de los temporizadores T0, T1 o T2:
		- Definición obligatoria del valor máximo de conteo personalizado TOP:
			1.1.- TOP = (fmicro / (frecuencia*escala)) - 1
			1.2.- TOP = ((T*fmicro) / escala)) - 1
			1.3.- OCR#A = TOP
		- Cálculo de frecuencia, periodo y tiempo de duty cycle con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (escala * (1 + OCR#))
			2.2.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado_decimal*(17/20) = Porcentaje_deseado_decimal*17 / frecuencia*20
			2.4.- OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		  PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = TOP - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- OCR#A = TOP
			3.3.- OCR#B = OCR#I_dutycycle  o OCR#NI_dutycycle


	- PWM fase y frecuencia correcta: Tiene las mismas fórmulas que el modo PWM Fase correcta, pero además a fuerza debe ser definido y utilizado el máximo del conteo, también 
	llamado TOP.
		Modo ICR1 = TOP; Como salida se pueden utilizar los canales A (OCR1A) y B (OCR1B); Solo se puede utilizar con el temporizador T1:
		- Definición obligatoria del valor máximo de conteo personalizado TOP:
			1.1.- TOP = fmicro / (2*escala*frecuencia)
			1.2.- TOP = (T*fmicro) / (2*escala)
			1.3.- ICR1 = TOP
		- Cálculo de frecuencia, periodo y tiempo de duty cycle con valor máximo de conteo personalizado TOP:
			2.1.- frecuencia = fmicro / (2*escala*TOP)
			2.2.- Periodo = T = (2*escala*TOP) / fmicro = 1 / frecuencia
			2.3.- t_dutycycle = T*Porcentaje_deseado_decimal*(17/20) = Porcentaje_deseado_decimal*17 / frecuencia*20
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
			2.3.- t_dutycycle = T*Porcentaje_deseado_decimal*(17/20) = Porcentaje_deseado_decimal*17 / frecuencia*20
			2.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala)
		- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de salida 
		PWM, OCR1I_dutycycle = Modo invertido;	OCR1NI_dutycycle = Modo no invertido:
			3.1.- OCR#I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
			3.2.- OCR#NI_dutycycle = OCR#_dutycycle
			3.3.- OCR1B = OCR#I_dutycycle  o OCR#NI_dutycycle
		- Asignación de resultados a los registros correspondientes:
			1.3.- ICR1 = TOP
			3.3.- OCR1B = OCR1I_dutycycle  o OCR1NI_dutycycle

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
	El cálculo se debe realizar en la parte de dentro del while, para que se pueda actualizar su valor de duty cycle por medio de las interrupciones externas.*/
	
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
    while (1){
		//MOSTRAR CONTEO EN LCD:
		/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de 
		cual de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
		POS_LINEA2(0);
		/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido 
		cada letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
		ENVIA_CADENA("T0:");
		//IMPRIMIR UN NÚMERO EN EL LCD:
		/*Para poder imprimir un número en pantalla se debe conocer el formato que utiliza cada uno y los tipos primitivos de datos en C:
		int:	Números enteros, utilizan un espacio de memoria de 2 o 4 bytes. Su formato es el %d o %i.
		float:	Números fraccional con máximo 7 decimales después del punto, utilizan un espacio de memoria de 4 bytes. Su formato es el %f.
		double:	Números fraccional con máximo 15 decimales después del punto, utilizan un espacio de memoria de 8 bytes. Su formato es el %lf.
		char:	Caracater, representa una sola letra o valor ASCII, utilizan un espacio de memoria de 1 byte. Su formato es el %s.
		Ya conocidos los formatos asociados a cada tipo primitivo, se usan dos variables y el formato del número que ingresa al método sprintf() que sirve para realizar la 
		conversión.*/
		//sprintf(variable_que_guarda_el_resultado, "formato_numero", numero_inicial): Método para convertir números a cadenas de caracteres.
		sprintf(pt0_string, "%d", pt0);
		ENVIA_CADENA(pt0_string);	//Impresión de la frecuencia de la señal, que aumenta o se reduce cada vez que se presiona el botón + o - del Puerto B.
		/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido 
		cada letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
		ENVIA_CADENA("%, ");
		
		ENVIA_CADENA("T1:");
		//IMPRIMIR UN NÚMERO EN EL LCD:
		/*Para poder imprimir un número en pantalla se debe conocer el formato que utiliza cada uno y los tipos primitivos de datos en C:
		int:	Números enteros, utilizan un espacio de memoria de 2 o 4 bytes. Su formato es el %d o %i.
		float:	Números fraccional con máximo 7 decimales después del punto, utilizan un espacio de memoria de 4 bytes. Su formato es el %f.
		double:	Números fraccional con máximo 15 decimales después del punto, utilizan un espacio de memoria de 8 bytes. Su formato es el %lf.
		char:	Caracater, representa una sola letra o valor ASCII, utilizan un espacio de memoria de 1 byte. Su formato es el %s.
		Ya conocidos los formatos asociados a cada tipo primitivo, se usan dos variables y el formato del número que ingresa al método sprintf() que sirve para realizar la 
		conversión.*/
		//sprintf(variable_que_guarda_el_resultado, "formato_numero", numero_inicial): Método para convertir números a cadenas de caracteres.
		sprintf(pt1_string, "%d", pt1);
		ENVIA_CADENA(pt1_string);	//Impresión de la frecuencia de la señal, que aumenta o se reduce cada vez que se presiona el botón + o - del Puerto B.
		/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido 
		cada letra o caracter de la palabra que recibe como parámetro. Recordemos que cada fila del LCD cuenta con solo 16 caracteres, hay que contarlos para que quepan.*/
		ENVIA_CADENA("%");
		
		/*Cuando se quiera variar el duty cycle de una señal PWM por medio de botones, lo que se hace es realizar un cálculo inicial con cierto valor de porcentaje para 
		calcular así el valor del contador, luego lo que se hará es multiplicar ese valor por una variable i, esa variable i es la que aumentará o disminuirá cuando se 
		presionen los botones que suban o bajen el valor del duty cycle, ya que esta variable multiplicará el porcentaje inicial calculado, supongamos que elejimos como 
		porcentaje inicial un 10%, calculamos así el valor del contador, luego cada que se presione el botón de subir o bajar duty cycle, subirá o bajará en un 10% el 
		porcentaje de nivel alto en la señal PWM, hasta que llegue a valer 10 que da un duty cycle del 100% o 0 que da un duty cycle del 0%, esto se indica por medio de 
		condicionales en las interrupciones que activan los botones. Los resultados de los registros OCR#A u OCR#B se deben guardar en variables globales, así como las 
		variables que aumentan y disminuyen el porcentaje del duty cycle.*/
		/*T0 - Fase correcta, Modo no invertido - frecuencia = 100 Hz; T = 10 ms; duty cycle inicial = 1% = 0.01; duty cycle = variable; Escala = 256.
			Modo sin TOP personalizado definido; Como salida se pueden utilizar los canales A (OCR#A) y B (OCR#B) con cualquiera de los temporizadores T0, T1 o T2:
			- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR#, el valor del ConteoMáximo_PWM depende de la resolución del temporizador:
				1.1.0.- frecuencia = fmicro / (2*escala*OCR#)
					1.1.1.-	OCR# = fmicro / (2*escala*frecuencia) = 16X10^6 / (2*256*100) = 312.5
				1.2.0.- Periodo = T = (2*escala*OCR#) / fmicro = 1 / frecuencia = (2*256*312.5) / 16X10^6 = 0.01 s = 10 ms
					1.2.1.-	OCR# = (T*fmicro) / (2*escala)
				1.3.- t_dutycycle = T*Porcentaje_deseado_decimal*(17/20) = Porcentaje_deseado_decimal*17 / frecuencia*20 = 0.01*0.01*(17/20) = 85X10^-6
				1.4.- OCR#_dutycycle = (t_dutycycle*fmicro) / (2*escala) = (85X10^-6*16X10^6) / (2*256) = 2.6562
			- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de 
			salida PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
				2.1.- OCR#I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
				2.2.- OCR#NI_dutycycle = OCR#_dutycycle = 2.6562
				2.3.- OCR#A = 2.6562
			- Asignación de resultados a los registros correspondientes:
				2.3.- OCR#A = 3.9062*/
		OCR0A = Porcentaje_inicial_At0 * Control_dutyCycle_At0;	//PWM Fase Correcta; frecuencia de 100 Hz; T = 10 ms; Porcentaje_inicial_t0 = 3.9062; Escala  = 256
	
		/*T1 - Fast PWM, Modo no invertido - frecuencia = 500 Hz; T = 2ms; duty cycle inicial = 2% = 0.02; duty cycle = variable; Escala  = 256.
			Modo sin TOP personalizado definido; Como salida se pueden utilizar los canales A (OCR#A) y B (OCR#B) con cualquiera de los temporizadores T0, T1 o T2:
			- Cálculo de frecuencia, periodo, tiempo de duty cycle y conteo OCR#, el valor del ConteoMáximo_PWM depende de la resolución del temporizador:
				1.1.0.- frecuencia = fmicro / (escala * (1 + OCR#))
					1.1.1.- OCR# = (fmicro / (frecuencia*escala)) - 1 = (16X10^6 / (500*256)) - 1 = 124
				1.2.0.- Periodo = T = (escala*(1 + OCR#)) / fmicro = 1 / frecuencia = (256*(1 + 124)) / 16X10^6 = 0.002 = 2ms
					1.2.1.- OCR# = ((T*fmicro) / escala)) - 1
				1.3.- t_dutycycle = T*Porcentaje_deseado_decimal*(17/20) = Porcentaje_deseado_decimal*17 / frecuencia*20 = 0.002*0.02*(17/20) = 34X10^-6
				1.4.- OCR#_dutycycle = ((t_dutyycle*fmicro) / escala) - 1 = ((34X10^-6*16X10^6) / 256) - 1 = 2.125 
			- Cálculo del valor que tendrá el registro de conteo del canal A o B dependiendo del modo invertido o no invertido con el que se haya configurado la señal de 
			salida PWM, OCR#I_dutycycle = Modo invertido;	OCR#NI_dutycycle = Modo no invertido:
				2.1.- OCR#I_dutycycle = ConteoMáximo_PWM - OCR#_dutycycle
				2.2.- OCR#NI_dutycycle = OCR#_dutycycle = 2.125
				2.3.- OCR#A = 2.125
			- Asignación de resultados a los registros correspondientes:
				2.3.- OCR#A = 2.125*/
		OCR1A = Porcentaje_inicial_At0 * Control_dutyCycle_At0;	//Fast PWM; frecuencia = 500 Hz; t_deseado = 2 ms; Porcentaje_inicial_t0 = 2.125; Escala  = 256
    }
}



//Funciones ISR: Cuando se crea una señal PWM, no se deben utilizar las funciones ISR.
//Funciones ISR: Cuando se crean interrupciones si se utilizan las funciones ISR.