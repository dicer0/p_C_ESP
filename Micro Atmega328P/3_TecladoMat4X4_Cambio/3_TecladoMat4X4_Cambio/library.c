/*
 * 3_TecladoMat4X4_Cambio.c
 *
 * Created: 25/03/2022 10:40:18 a. m.
 * Author : diego
 
  3.- LIBRER�A CREADA PARA EL MANEJO DEL TECLADO MATRICIAL 4X4: El teclado se lee por el Puerto D y se saca el resultado por el Puerto B de forma binaria, el Puerto C
  se utiliza cuando en el proyecto es utilizado un LCD. El teclado emplea 8 pines del Puerto D, ya que es el �nico que cuenta con este n�mero de pines, ya que el 
  Puerto B y C cuentan con 6 pines. Los 8 pines son usados para referirse a las 4 filas y 4 columnas del teclado, cuando un bot�n es presionado, la columna y la fila 
  pasan a bajo nivel, para leerlo se establece las columnas como entradas (Bits 0, 1 y 2) y espera a  obtener un 0 en estos bits cuando sea presionado el bot�n.
  Cuando la columna pasa a bajo nivel, suma al valor (+0, +1 o +2) para que luego se ponga el puerto para leer las filas. Se deben leer las filas y agregar 1, 4, 7 o A
  dependiendo del valor de la fila - despu�s de invertirlo.
  La diferencia principal entre una librer�a y un ejecutable es que la librer�a no contiene m�todo main. 
 */ 


/*DECLARACI�N DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
librer�as), antes de declarar nuestras funciones y dem�s. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librer�a delay de manejo de tiempos la utiliza


//IMPORTACI�N DE LIBRER�AS, DIRECTIVAS
#include <avr/io.h> //Librer�a para todo el manejo de puertos y dem�s aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h> //Librer�a de manejo de tiempos, como retardos principalmente


//PROTOTIPOS DE FUNCIONES: En esta parte solo se declaran las funciones que se ejecutar�n en la parte de abajo.
/*La asignaci�n de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexi�n del teclado matricial vistos de izquierda
a derecha son los siguientes:
Puerto D Arduino (Digital PWM):			7,		6,		5,		4,		3,		2,		1,		0
Constantes del teclado matricial 4X4:	F4,		F3,		F2,		F1,		C4,		C3,		C2,		C1
Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8
Cuando se lea las filas y columnas del teclado matricial, se invierte la se�al porque al presionar uno de los botones, estos hacen corto circuito, por lo mandan un 0 al 
c�digo, pero es m�s f�cil interpretar como un 1 cuando es presionado un bot�n, por lo que se invierte la se�al.*/
void COLUMNA_init(void);			//Funci�n creada para invertir la se�al y solamente leer las columnas del teclado matricial, que corresponde al nibble bajo del puerto D.
void FILA_init(void);				//Funci�n creada para invertir la se�al y solamente leer las filas del teclado matricial, que corresponde al nibble alto del puerto D.
unsigned char LEER_TECLADO(void);	//Funci�n que lee las columnas y filas del teclado para guardar la posici�n presionada.


/*LIBRER�A: Contiene los m�todos o constantes que puede usar un archivo ejecutable para realizar ciertas acciones, no contiene m�todo main.*/
//COLUMNA_init: Funci�n creada para solamente leer las columnas del teclado matricial, que corresponde al nibble bajo del puerto D e invertir la se�al.
void COLUMNA_init(){
	//DDR:	Registro del microcontrolador que indica cual pin del puerto es entrada o salida.
	//PORT: Registro del puerto que saca datos.
	//PIN:	Registro del puerto que lee datos recopilados.
	
	/*PUERTO COMO ENTRADA O SALIDA: Para indicar si los pines de un puerto ser�n entradas o salidas se usa la siguiente nomenclatura en el DDR (Data Direction Register), 
	que es el manejador de puertos, perteneciente a los registros de funciones especiales. Esto se declara en la variable local DDR, que puede ser usada en el puerto 
	B, C o D. Creando as� las variables DDRB (Puerto B), DDRC (Puerto C) o DDRD (Puerto D). Cuando dentro de Microchip Studio alguna palabra se pone de color morado es 
	porque es una palabra reservada, en este caso es porque accede a alguno de los puertos del Atmega328P, que est�n declarados en la librer�a <avr/io.h>.*/
	//Puerto como entrada:	0
	//Puerto como salida:	1
	DDRD = 0xF0; //Se pone el nibble alto (el conjunto de 4 bits m�s singificativos) como salida y el nibble bajo como entrada, porque analiza las columnas.
	
	/*Indicaci�n de los pines de salida, si se declar� como entrada alguno de los pines y se utiliza un 1 en este registro, la entrada llegar� al microcontrolador 
	de manera inversa a la original (resistencias pull-up), si se pone un 0 llegar� de forma normal (resistencias pull-down). Funciona m�s o menos como una compuerta NOT.*/
	//Los dos pines m�s significativos del Puerto B y C no se pueden usar, en el Puerto D si se puede.
	//El puerto B tiene 6 pines.
	//El puerto C tiene 6 pines.
	//El puerto D tiene 8 pines.
	//En el nibble alto la salida del Puerto D conectado al teclado tendr� en un inicio puros ceros, mientras que en el nibble bajo la se�al entrar� de forma inversa.
	/*Resistencias de pull-up: En el nibble bajo la entrada se procesar� de manera inversa a como ingresa al microcontrolador, si es 0 lo tomar� como 1 y viceversa, la se�al 
	se maneja de esta manera porque cuando en el teclado matricial se presiona un bot�n, hace corto circuito por lo que al programa le llega un 0, pero es m�s l�gico manejar
	en el c�digo que un 1 es un bot�n presionado, por eso se le aplica la inversa a la se�al.*/
	PORTD = 0x0F; 
	//RETARDOS ANTI REBOTES: Se incluyen para que cuando al presionar un bot�n, este rebote, no se tome en cuenta este cambio en la se�al de entrada.
	asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador.
	asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador.
}


//FILA_init: Funci�n creada para invertir la se�al y solamente leer las filas del teclado matricial, que corresponde al nibble alto del puerto D.
void FILA_init(){
	//DDR: Registro del microcontrolador que indica cual pin del puerto es entrada o salida.
	//PORT: Registro del puerto que saca datos.
	//PIN: Registro del puerto que lee datos recopilados.
	
	/*PUERTO COMO ENTRADA O SALIDA: Para indicar si los pines de un puerto ser�n entradas o salidas se usa la siguiente nomenclatura en el DDR (Data Direction Register), 
	que es el manejador de puertos, perteneciente a los registros de funciones especiales. Esto se declara en la variable local DDR, que puede ser usada en el puerto 
	B, C o D. Creando as� las variables DDRB (Puerto B), DDRC (Puerto C) o DDRD (Puerto D). Cuando dentro de Microchip Studio alguna palabra se pone de color morado es 
	porque es una palabra reservada, en este caso es porque accede a alguno de los puertos del Atmega328P.*/
	//Puerto como entrada:	0
	//Puerto como salida:	1
	DDRD = 0x0F; //Se pone el el nibble alto como entrada y el nibble bajo (el conjunto de 4 bits menos singificativos) como salida.
	
	/*Indicaci�n de los pines de salida, si se declar� como entrada alguno de los pines y se utiliza un 1 en este registro, la entrada llegar� al microcontrolador 
	de manera inversa a la original (resistencias pull-up), si se pone un 0 llegar� de forma normal(resistencias pull-down). Funciona m�s o menos como una compuerta NOT.*/
	//Los dos pines m�s significativos del Puerto B y C no se pueden usar, en el Puerto D si se puede.
	//El puerto B tiene 6 pines.
	//El puerto C tiene 6 pines.
	//El puerto D tiene 8 pines.
	//En el nibble alto la se�al entrar� de forma inversa, mientras que en el nibble bajo la salida del Puerto D conectado al teclado tendr� en un inicio puros ceros.
	PORTD = 0xF0;
	//RETARDOS ANTI REBOTES: Se incluyen para que cuando al presionar un bot�n, este rebote, no se tome en cuenta este cambio en la se�al de entrada.
	asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador.
	asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de m�quina, el cual est� relacionado a la se�al de reloj del microcontrolador.
}


//LEER_TECLADO: Funci�n que ejecuta las funciones COLUMNA_init() y FILA_init() para leer las columnas y filas del teclado y guardar la posici�n presionada.
unsigned char LEER_TECLADO(){
	unsigned char valor;	//Variable que almacena el valor pesionado en el teclado matricial
	
	//Se inicializa en 0 el valor de la variable que guardar� la posici�n del bot�n presionado
	valor = 0;
	
	//LECTURA DE LAS COLUMNAS: Primero se lee que columna fue presionada para posteriormente analizar la fila.
	COLUMNA_init(); //Funci�n creada para solamente leer las columnas del teclado matricial, que corresponde al nibble alto del puerto D e invertir la se�al.
	//PIN:	Registro del puerto que lee datos recopilados.
	//M�SCARA AND: Convierte en 0 todo lo que est� en contacto con los 0 de la m�scara y deja pasar tal cual lo que est� en contacto con los 1 de la m�scara.
	//Si el valor del puerto D es distinto al resultado de la m�scara (!), osea = ~((PIND) AND (0000 0001)) = ~(0000 0001) = 1111 1110, la columna 4 fue presionada.
	if (!(PIND & 0x01)) {
		valor = 0;		//C4 = 1111 1110; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la m�scara (!), osea = ~((PIND) AND (0000 0010)) = ~(0000 0010) = 1111 1101, la columna 3 fue presionada.
	else if (!(PIND & 0x02)) {	
		valor = 1;		//C3 = 1111 1101; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la m�scara (!), osea = ~((PIND) AND (0000 0100)) = ~(0000 0100) = 1111 1011, la columna 2 fue presionada.
	else if (!(PIND & 0x04)) {
		valor = 2;		//C2 = 1111 1011; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la m�scara (!), osea = ~((PIND) AND (0000 1000)) = ~(0000 1000) = 1111 0111, la columna 1 fue presionada.
	else if (!(PIND & 0x08))  {
		valor = 0x0C;	//C1 = 1111 0111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	
	
	//LECTURA DE LAS FILAS: Ya sabiendo que columna fue presionada, se analiza la fila que fue presionada.
	FILA_init(); //Funci�n creada para solamente leer las filas del teclado matricial, que corresponde al nibble bajo del puerto D e invertir la se�al.
	//PIN:	Registro del puerto que lee datos recopilados.
	//M�SCARA AND: Convierte en 0 todo lo que est� en contacto con los 0 de la m�scara y deja pasar tal cual lo que est� en contacto con los 1 de la m�scara.
	//Si el valor del puerto D es distinto al resultado de la m�scara (!), osea = ~((PIND) AND (0001 0000)) = ~(0001 0000) = 1110 1111, la fila 4 fue presionada.
	if (!(PIND & 0x10)) {
		valor += 0x01;	//F4 = 1110 1111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la m�scara (!), osea = ~((PIND) AND (0010 0000)) = ~(0010 0000) = 1101 1111, la fila 3 fue presionada.
	else if (!(PIND & 0x20)){ 
		valor += 0x04;	//F3 = 1101 1111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la m�scara (!), osea = ~((PIND) AND (0100 0000)) = ~(0100 0000) = 1011 1111, la fila 2 fue presionada.
	else if (!(PIND & 0x40)){ 
		valor += 0x07;	//F2 = 1011 1111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la m�scara (!), osea = ~((PIND) AND (1000 0000)) = ~(1000 0000) = 0111 1111, la fila 1 fue presionada.
	else if (!(PIND & 0x80)){ 
		valor += 0x0A;	//F1 = 0111 1111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	
	
	if (valor == 0x0D){
		valor = 0x0A;
	}
	else if (valor == 0x10){
		valor = 0x0B;
	}
	else if (valor == 0x13){
		valor = 0x0C;
	}
	else if (valor == 0x16){
		valor = 0x0D;
	}
	
	_delay_ms(1);	//Delay de 1 milisegundo despu�s de haber identificado el bot�n presionado.
	return valor;	//El valor retornado es la suma de las columnas y las filas identificadas, para conocer la posici�n del bot�n presionado.
}