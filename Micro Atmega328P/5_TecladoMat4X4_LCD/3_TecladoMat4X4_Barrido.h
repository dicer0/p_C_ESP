/*
 * 3_TecladoMat4X4_Barrido.c
 *
 * Created: 25/03/2022 11:20:20 a. m.
 * Author : diego
 
 3.- LIBRERÍA CREADA PARA EL MANEJO DEL TECLADO MATRICIAL 4X4: El teclado se lee por el Puerto D y se saca el resultado por el Puerto B de forma binaria, el Puerto C
 se utiliza cuando en el proyecto es utilizado un LCD. El teclado emplea 8 pines del Puerto D, ya que es el único que cuenta con este número de pines, ya que el
 Puerto B y C cuentan con 6 pines. Los 8 pines son usados para referirse a las 4 filas y 4 columnas del teclado, cuando un botón es presionado, la columna y la fila
 pasan a bajo nivel, para leerlo se establece las columnas como entradas (Bits 0, 1 y 2) y espera a  obtener un 0 en estos bits cuando sea presionado el botón.
 Cuando la columna pasa a bajo nivel, suma al valor (+0, +1 o +2) para que luego se ponga el puerto para leer las filas. Se deben leer las filas y agregar 1, 4, 7 o A
 dependiendo del valor de la fila - después de invertirlo.
 La diferencia principal entre una librería y un ejecutable es que la librería no contiene método main.
 */


//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>		//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P


/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
librerías), antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
//FILAS DEL TECLADO MATRICIAL: Se representan por medio de constantes
/*Se debe aplicar una máscara AND al número binario que representa cada fila y columna del teclado matricial porque en el documento 3_TecladoMat4X4_Cambio, las entradas 
se manejan como resistencias pull-up, osea que invierte lo que llegue a su entrada. Esto se realiza de esta manera porque cada que se presiona el teclado matricial, hace 
corto circuito, por lo que manda un 0, pero es más fácil interpretar esto como 1 en el código cuando el botón haya sido presionado, por eso es que se hace el cambio.
	- Máscara AND: La máscara se realiza a través del signo & y el número binario que vaya después de este símbolo usualmente es la máscara en sí, lo que esta hace es 
	volver ceros todo lo que tenga 0 en la máscara y dejar pasar tal cual el número binario original donde en la máscara exista un 1.
	- Operador Lógico NOT: Se realiza a través del signo ~ e invierte de valor todos los bits del número binario a donde se aplique.
Los números binarios obtenidos para indicar las filas se lee de derecha a izquierda solo en el nibble bajo, siendo por ejemplo 0000  1110 la posición de hasta la izquierda 
en alguna de las filas, para saber el valor exacto del número en el teclado, deberíamos saber en qué columna nos encontramos.*/
#define  F1 0x0F & ~(0x01) //(0000 1111) AND (not(0000 0001)) = (0000 1111) AND (1111 1110) = 0X0E = 0000 1110 = Primera fila, posición 1 vista de izquierda a derecha
#define  F2 0x0F & ~(0x02) //(0000 1111) AND (not(0000 0010)) = (0000 1111) AND (1111 1101) = 0X0D = 0000 1101 = Segunda fila, posición 2 vista de izquierda a derecha
#define  F3 0x0F & ~(0x04) //(0000 1111) AND (not(0000 0100)) = (0000 1111) AND (1111 1011) = 0X0B = 0000 1011 = Tercera fila, posición 3 vista de izquierda a derecha
#define  F4 0x0F & ~(0x08) //(0000 1111) AND (not(0000 1000)) = (0000 1111) AND (1111 0111) = 0X07 = 0000 0111 = Cuarta fila, posición 4 vista de izquierda a derecha


/*COLUMNAS DEL TECLADO MATRICIAL: Se representan por medio de constantes y se obtiene su valor de la misma forma como se obtuvieron las filas, los números binarios 
obtenidos para indicar las filas se leen de derecha a izquierda solo en el nibble alto, siendo por ejemplo 1110 1111 la posición de la columna de hasta la izquierda, para 
saber su valor exacto, deberíamos saber en qué fila nos encontramos, que también se lee de forma inversa.*/
#define  C1 0xEF //0xFF & ~(0x10) = 0xEF = 1110 1111 = Primera columna vista de izquierda a derecha
#define  C2 0xDF //0xFF & ~(0x20) = 0xDF = 1101 1111 = Segunda columna vista de izquierda a derecha
#define  C3 0xBF //0xFF & ~(0x40) = 0xBF = 1011 1111 = Tercera columna vista de izquierda a derecha
#define  C4 0x7F //0xFF & ~(0x80) = 0x7F = 0111 1111 = Cuarta columna vista de izquierda a derecha


//DECLARACIÓN DE VARIABLES GLOBALES (CABECERAS DE FUNCIÓN)
/*En la variable de tipo primitivo char llamada VALOR_TECLADO lo que se hará es dependiendo de la fila y columna que se haya seleccionado, se asignará un valor de texto a la 
constante de los que están pintados sobre el teclado matricial, correspondiente a cada botón.*/
char VALOR_TECLADO = ' ';


/*LIBRERÍA: Contiene los métodos o constantes que puede usar un archivo ejecutable para realizar ciertas acciones, no contiene método main.*/
/*La asignación de pines del Puerto D del Arduino con las constantes asignadas a las filas y columnas y los pines reales de conexión del teclado matricial vistos de izquierda 
a derecha son los siguientes:
Puerto D Arduino (Digital PWM):			7,		6,		5,		4,		3,		2,		1,		0
Constantes del teclado matricial 4X4:	F1,		F2,		F3,		F4,		C1,		C2,		C3,		C4
Pines del teclado matricial 4X4:		1,		2,		3,		4,		5,		6,		7,		8*/

/*FUNCIÓN PARA LEER LAS COLUMNAS Y FILAS PROVENIENTES DEL TECLADO MATRICIAL: Con esta función se puede saber cuál de los botones fue presionado en el teclado, para poder 
asociar a cada botón presionado un valor correspondiente, que esté escrito sobre el botón del teclado. Recordemos que realmente obtenemos un cero cuando se dé clic en los 
botones del teclado matricial.*/
char LEER_TECLADO(){
	switch(PORTD){
		//TODO LO SIGUIENTE REPRESENTA LOS BOTONES DE LA CUARTA FILA DEL TECLADO MATRICIAL 4X4:
		case C1: //0xEF = 1110 1111 = Cuarta columna vista de izquierda a derecha, solo en el nibble alto que representa las columnas
		switch(PIND & 0x0F){ //El switch realiza una compuerta AND para solo analizar el nibble bajo de los 8 bits provenientes del puerto D, ya que este representa las filas
			case F1: VALOR_TECLADO = 'D'; break; //0X0E = 0000 1110, esto representa el número 1, ya que se lee de derecha a izquierda el nibble bajo
			case F2: VALOR_TECLADO = '#'; break; //0X0D = 0000 1101, esto representa el número 2, ya que se lee de derecha a izquierda el nibble bajo
			case F3: VALOR_TECLADO = '0'; break; //0X0B = 0000 1011, esto representa el número 3, ya que se lee de derecha a izquierda el nibble bajo
			case F4: VALOR_TECLADO = '*'; break; //0X07 = 0000 0111, esto representa el número A, ya que se lee de derecha a izquierda el nibble bajo
		}
		break;
		//TODO LO SIGUIENTE REPRESENTA LOS BOTONES DE LA TERCERA FILA DEL TECLADO MATRICIAL 4X4:
		case C2: //0xDF = 1101 1111 = Tercera columna vista de izquierda a derecha, solo en el nibble alto que representa las columnas
		switch(PIND & 0x0F){ //Analisis del nibble bajo de los 8 bits del puerto D que representa las filas
			case F1: VALOR_TECLADO = 'C'; break; //0X0E = 0000 1110, esto representa el número 4, ya que se lee de derecha a izquierda el nibble bajo
			case F2: VALOR_TECLADO = '9'; break; //0X0D = 0000 1101, esto representa el número 5, ya que se lee de derecha a izquierda el nibble bajo
			case F3: VALOR_TECLADO = '8'; break; //0X0B = 0000 1011, esto representa el número 6, ya que se lee de derecha a izquierda el nibble bajo
			case F4: VALOR_TECLADO = '7'; break; //0X07 = 0000 0111, esto representa el número B, ya que se lee de derecha a izquierda el nibble bajo
		}
		break;
		//TODO LO SIGUIENTE REPRESENTA LOS BOTONES DE LA SEGUNDA FILA DEL TECLADO MATRICIAL 4X4:
		case C3: //0xBF = 1011 1111 = Segunda columna vista de izquierda a derecha, solo en el nibble alto que representa las columnas
		switch(PIND & 0x0F){ //Analisis del nibble bajo de los 8 bits del puerto D que representa las filas
			case F1: VALOR_TECLADO = 'B'; break; //0X0E = 0000 1110, esto representa el número 7, ya que se lee de derecha a izquierda el nibble bajo
			case F2: VALOR_TECLADO = '6'; break; //0X0D = 0000 1101, esto representa el número 8, ya que se lee de derecha a izquierda el nibble bajo
			case F3: VALOR_TECLADO = '5'; break; //0X0B = 0000 1011, esto representa el número 9, ya que se lee de derecha a izquierda el nibble bajo
			case F4: VALOR_TECLADO = '4'; break; //0X07 = 0000 0111, esto representa el número C, ya que se lee de derecha a izquierda el nibble bajo
		}
		break;
		//TODO LO SIGUIENTE REPRESENTA LOS BOTONES DE LA PRIMERA FILA DEL TECLADO MATRICIAL 4X4:
		case C4: //0x7F = 0111 1111 = Primera columna vista de izquierda a derecha, solo en el nibble alto que representa las columnas
		switch(PIND & 0x0F){ //Analisis del nibble bajo de los 8 bits del puerto D que representa las filas
			case F1: VALOR_TECLADO = 'A'; break; //0X0E = 0000 1110, esto representa el número *, ya que se lee de derecha a izquierda el nibble bajo
			case F2: VALOR_TECLADO = '3'; break; //0X0D = 0000 1101, esto representa el número 0, ya que se lee de derecha a izquierda el nibble bajo
			case F3: VALOR_TECLADO = '2'; break; //0X0B = 0000 1011, esto representa el número #, ya que se lee de derecha a izquierda el nibble bajo
			case F4: VALOR_TECLADO = '1'; break; //0X07 = 0000 0111, esto representa el número D, ya que se lee de derecha a izquierda el nibble bajo
		}
		break;
	}
	return VALOR_TECLADO;
}


/*BARRE_TECLADO: Función que recorre todas las columnas para ver qué botón del teclado ha sido presionado, recordemos que cuando se dá clic en el botón, al programa le llega
un 0, mientras le llega un 1 cuando todavía no se presiona nada.*/
//DDR:	Registro del microcontrolador que indica cual pin del puerto es entrada o salida.
//PORT: Registro del puerto que saca datos.
//PIN:	Registro del puerto que lee datos recopilados.
//FUNCIÓN PARA 
void BARRE_TECLADO(){
	switch(PORTD){
		//C1 = 0xEF = 1110 1111; Primera columna del teclado matricial, vista de izquierda a derecha.
		case C1: 
			PORTD = C2;
			break;
		//C2 = 0xDF = 1101 1111; Segunda columna del teclado matricial, vista de izquierda a derecha.
		case C2:
			PORTD = C3;
			break;
		//C3 = 0xBF = 1011 1111; Tercer columna del teclado matricial, vista de izquierda a derecha.
		case C3: 
			PORTD = C4;
			break;
		//C4 = 0x7F = 0111 1111; Cuarta columna del teclado matricial, vista de izquierda a derecha.
		default:
			PORTD = C1;
	}
}