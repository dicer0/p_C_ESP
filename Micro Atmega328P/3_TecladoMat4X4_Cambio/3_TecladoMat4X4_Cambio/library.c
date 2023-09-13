/*
 * 3_TecladoMat4X4_Cambio.c
 *
 * Created: 25/03/2022 10:40:18 a. m.
 * Author : diego
 
  3.- LIBRERÍA CREADA PARA EL MANEJO DEL TECLADO MATRICIAL 4X4: El teclado se lee por el Puerto D y se saca el resultado por el Puerto B de forma binaria, el Puerto C
  se utiliza cuando en el proyecto es utilizado un LCD. El teclado emplea 8 pines del Puerto D, ya que es el único que cuenta con este número de pines, ya que el 
  Puerto B y C cuentan con 6 pines. Los 8 pines son usados para referirse a las 4 filas y 4 columnas del teclado, cuando un botón es presionado, la columna y la fila 
  pasan a bajo nivel, para leerlo se establece las columnas como entradas (Bits 0, 1 y 2) y espera a  obtener un 0 en estos bits cuando sea presionado el botón.
  Cuando la columna pasa a bajo nivel, suma al valor (+0, +1 o +2) para que luego se ponga el puerto para leer las filas. Se deben leer las filas y agregar 1, 4, 7 o A
  dependiendo del valor de la fila - después de invertirlo.
  La diferencia principal entre una librería y un ejecutable es que la librería no contiene método main. 
 */ 


/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
librerías), antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librería delay de manejo de tiempos la utiliza


//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h> //Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <util/delay.h> //Librería de manejo de tiempos, como retardos principalmente


//PROTOTIPOS DE FUNCIONES: En esta parte solo se declaran las funciones que se ejecutarán en la parte de abajo.
/*La asignación de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexión del teclado matricial vistos de izquierda
a derecha son los siguientes:
Puerto D Arduino (Digital PWM):			7,		6,		5,		4,		3,		2,		1,		0
Constantes del teclado matricial 4X4:	F4,		F3,		F2,		F1,		C4,		C3,		C2,		C1
Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8
Cuando se lea las filas y columnas del teclado matricial, se invierte la señal porque al presionar uno de los botones, estos hacen corto circuito, por lo mandan un 0 al 
código, pero es más fácil interpretar como un 1 cuando es presionado un botón, por lo que se invierte la señal.*/
void COLUMNA_init(void);			//Función creada para invertir la señal y solamente leer las columnas del teclado matricial, que corresponde al nibble bajo del puerto D.
void FILA_init(void);				//Función creada para invertir la señal y solamente leer las filas del teclado matricial, que corresponde al nibble alto del puerto D.
unsigned char LEER_TECLADO(void);	//Función que lee las columnas y filas del teclado para guardar la posición presionada.


/*LIBRERÍA: Contiene los métodos o constantes que puede usar un archivo ejecutable para realizar ciertas acciones, no contiene método main.*/
//COLUMNA_init: Función creada para solamente leer las columnas del teclado matricial, que corresponde al nibble bajo del puerto D e invertir la señal.
void COLUMNA_init(){
	//DDR:	Registro del microcontrolador que indica cual pin del puerto es entrada o salida.
	//PORT: Registro del puerto que saca datos.
	//PIN:	Registro del puerto que lee datos recopilados.
	
	/*PUERTO COMO ENTRADA O SALIDA: Para indicar si los pines de un puerto serán entradas o salidas se usa la siguiente nomenclatura en el DDR (Data Direction Register), 
	que es el manejador de puertos, perteneciente a los registros de funciones especiales. Esto se declara en la variable local DDR, que puede ser usada en el puerto 
	B, C o D. Creando así las variables DDRB (Puerto B), DDRC (Puerto C) o DDRD (Puerto D). Cuando dentro de Microchip Studio alguna palabra se pone de color morado es 
	porque es una palabra reservada, en este caso es porque accede a alguno de los puertos del Atmega328P, que están declarados en la librería <avr/io.h>.*/
	//Puerto como entrada:	0
	//Puerto como salida:	1
	DDRD = 0xF0; //Se pone el nibble alto (el conjunto de 4 bits más singificativos) como salida y el nibble bajo como entrada, porque analiza las columnas.
	
	/*Indicación de los pines de salida, si se declaró como entrada alguno de los pines y se utiliza un 1 en este registro, la entrada llegará al microcontrolador 
	de manera inversa a la original (resistencias pull-up), si se pone un 0 llegará de forma normal (resistencias pull-down). Funciona más o menos como una compuerta NOT.*/
	//Los dos pines más significativos del Puerto B y C no se pueden usar, en el Puerto D si se puede.
	//El puerto B tiene 6 pines.
	//El puerto C tiene 6 pines.
	//El puerto D tiene 8 pines.
	//En el nibble alto la salida del Puerto D conectado al teclado tendrá en un inicio puros ceros, mientras que en el nibble bajo la señal entrará de forma inversa.
	/*Resistencias de pull-up: En el nibble bajo la entrada se procesará de manera inversa a como ingresa al microcontrolador, si es 0 lo tomará como 1 y viceversa, la señal 
	se maneja de esta manera porque cuando en el teclado matricial se presiona un botón, hace corto circuito por lo que al programa le llega un 0, pero es más lógico manejar
	en el código que un 1 es un botón presionado, por eso se le aplica la inversa a la señal.*/
	PORTD = 0x0F; 
	//RETARDOS ANTI REBOTES: Se incluyen para que cuando al presionar un botón, este rebote, no se tome en cuenta este cambio en la señal de entrada.
	asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador.
	asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador.
}


//FILA_init: Función creada para invertir la señal y solamente leer las filas del teclado matricial, que corresponde al nibble alto del puerto D.
void FILA_init(){
	//DDR: Registro del microcontrolador que indica cual pin del puerto es entrada o salida.
	//PORT: Registro del puerto que saca datos.
	//PIN: Registro del puerto que lee datos recopilados.
	
	/*PUERTO COMO ENTRADA O SALIDA: Para indicar si los pines de un puerto serán entradas o salidas se usa la siguiente nomenclatura en el DDR (Data Direction Register), 
	que es el manejador de puertos, perteneciente a los registros de funciones especiales. Esto se declara en la variable local DDR, que puede ser usada en el puerto 
	B, C o D. Creando así las variables DDRB (Puerto B), DDRC (Puerto C) o DDRD (Puerto D). Cuando dentro de Microchip Studio alguna palabra se pone de color morado es 
	porque es una palabra reservada, en este caso es porque accede a alguno de los puertos del Atmega328P.*/
	//Puerto como entrada:	0
	//Puerto como salida:	1
	DDRD = 0x0F; //Se pone el el nibble alto como entrada y el nibble bajo (el conjunto de 4 bits menos singificativos) como salida.
	
	/*Indicación de los pines de salida, si se declaró como entrada alguno de los pines y se utiliza un 1 en este registro, la entrada llegará al microcontrolador 
	de manera inversa a la original (resistencias pull-up), si se pone un 0 llegará de forma normal(resistencias pull-down). Funciona más o menos como una compuerta NOT.*/
	//Los dos pines más significativos del Puerto B y C no se pueden usar, en el Puerto D si se puede.
	//El puerto B tiene 6 pines.
	//El puerto C tiene 6 pines.
	//El puerto D tiene 8 pines.
	//En el nibble alto la señal entrará de forma inversa, mientras que en el nibble bajo la salida del Puerto D conectado al teclado tendrá en un inicio puros ceros.
	PORTD = 0xF0;
	//RETARDOS ANTI REBOTES: Se incluyen para que cuando al presionar un botón, este rebote, no se tome en cuenta este cambio en la señal de entrada.
	asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador.
	asm volatile ("NOP"); //NOP: Indica un retardo de tiempo de dos ciclos de máquina, el cual está relacionado a la señal de reloj del microcontrolador.
}


//LEER_TECLADO: Función que ejecuta las funciones COLUMNA_init() y FILA_init() para leer las columnas y filas del teclado y guardar la posición presionada.
unsigned char LEER_TECLADO(){
	unsigned char valor;	//Variable que almacena el valor pesionado en el teclado matricial
	
	//Se inicializa en 0 el valor de la variable que guardará la posición del botón presionado
	valor = 0;
	
	//LECTURA DE LAS COLUMNAS: Primero se lee que columna fue presionada para posteriormente analizar la fila.
	COLUMNA_init(); //Función creada para solamente leer las columnas del teclado matricial, que corresponde al nibble alto del puerto D e invertir la señal.
	//PIN:	Registro del puerto que lee datos recopilados.
	//MÁSCARA AND: Convierte en 0 todo lo que esté en contacto con los 0 de la máscara y deja pasar tal cual lo que esté en contacto con los 1 de la máscara.
	//Si el valor del puerto D es distinto al resultado de la máscara (!), osea = ~((PIND) AND (0000 0001)) = ~(0000 0001) = 1111 1110, la columna 4 fue presionada.
	if (!(PIND & 0x01)) {
		valor = 0;		//C4 = 1111 1110; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la máscara (!), osea = ~((PIND) AND (0000 0010)) = ~(0000 0010) = 1111 1101, la columna 3 fue presionada.
	else if (!(PIND & 0x02)) {	
		valor = 1;		//C3 = 1111 1101; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la máscara (!), osea = ~((PIND) AND (0000 0100)) = ~(0000 0100) = 1111 1011, la columna 2 fue presionada.
	else if (!(PIND & 0x04)) {
		valor = 2;		//C2 = 1111 1011; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la máscara (!), osea = ~((PIND) AND (0000 1000)) = ~(0000 1000) = 1111 0111, la columna 1 fue presionada.
	else if (!(PIND & 0x08))  {
		valor = 0x0C;	//C1 = 1111 0111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	
	
	//LECTURA DE LAS FILAS: Ya sabiendo que columna fue presionada, se analiza la fila que fue presionada.
	FILA_init(); //Función creada para solamente leer las filas del teclado matricial, que corresponde al nibble bajo del puerto D e invertir la señal.
	//PIN:	Registro del puerto que lee datos recopilados.
	//MÁSCARA AND: Convierte en 0 todo lo que esté en contacto con los 0 de la máscara y deja pasar tal cual lo que esté en contacto con los 1 de la máscara.
	//Si el valor del puerto D es distinto al resultado de la máscara (!), osea = ~((PIND) AND (0001 0000)) = ~(0001 0000) = 1110 1111, la fila 4 fue presionada.
	if (!(PIND & 0x10)) {
		valor += 0x01;	//F4 = 1110 1111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la máscara (!), osea = ~((PIND) AND (0010 0000)) = ~(0010 0000) = 1101 1111, la fila 3 fue presionada.
	else if (!(PIND & 0x20)){ 
		valor += 0x04;	//F3 = 1101 1111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la máscara (!), osea = ~((PIND) AND (0100 0000)) = ~(0100 0000) = 1011 1111, la fila 2 fue presionada.
	else if (!(PIND & 0x40)){ 
		valor += 0x07;	//F2 = 1011 1111; En este caso se leen las posiciones de las columnas de derecha a izquierda y no viceversa como en el archivo 3_TecladoMat4X4_Barrido
	}
	//Si el valor del puerto D es distinto al resultado de la máscara (!), osea = ~((PIND) AND (1000 0000)) = ~(1000 0000) = 0111 1111, la fila 1 fue presionada.
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
	
	_delay_ms(1);	//Delay de 1 milisegundo después de haber identificado el botón presionado.
	return valor;	//El valor retornado es la suma de las columnas y las filas identificadas, para conocer la posición del botón presionado.
}