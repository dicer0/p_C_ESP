/*
 * 2_LCD_4bits.c
 *
 * Created: 22/03/2022 05:25:43 p. m.
 * Author : diego
 2.- LIBRERÍA CREADA PARA EL MANEJO DE LA LCD HD44780 de 16X2 caracteresXfilas: La diferencia principal entre una librería y un ejecutable es que la librería no 
 contiene método main.
 */ 

/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
librerías), pero antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librería delay de manejo de tiempos la utiliza


//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>		//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <inttypes.h>	//Librería que importa los diferentes tipos de enteros de 8 o 16 bits
#include <util/delay.h> //Librería de manejo de tiempos, como retardos principalmente


//DECLARACIÓN DE VARIABLES GLOBALES (CABECERAS DE FUNCIÓN):
//En el LCD se tienen 8 bits de datos para indicar el caracter que se quiere mostrar en el LCD: DB7, DB6, DB5, DB4, DB3, DB2, DB1, DB0
/*En el LCD también se tienen 3 bits de control para indicar la configuración del LCD: RS, R/W y E. En específico R/W se conecta siempre a tierra para que siempre 
se escriba en el LCD en vez de que se lea, ya que esto se hace solo cuando se quiere optimizar mucho tiempos, pero no es tan necesario ya que el tiempo de respuesta 
de las instrucciones del LCD es de 2 miliesgundos.*/
/*El LCD se mandará al Puerto C del Arduino, que corresponde a los pines ANALOG IN: A0, A1, A2, A3, A4 y A5.
Solo tiene 6 pines, por lo que:
A0 y A1 se utilizarán para los pines de control del LCD: RS y E.
A2, A3, A4 y A5 se utilizarán para los nibbles de datos que indican los caracteres a mandar al LCD, nibble se refiere al conjunto de 4 bits, 2 nibbles conforman un byte.*/
#define LCD_PUERTO PORTC	//La constante LCD_PUERTO se refiere a todo el puerto C
#define LCD_RS PC0			//La constante LCD_RS se refiere al pin de control RS del LCD, asociado al pin A0 del Puerto C en el Arduino
#define LCD_E PC1			//La constante LCD_E se refiere al pin de control E del LCD, asociado al pin A1 del Puerto C en el Arduino


//PROTOTIPOS DE FUNCIONES: En esta parte solo se declaran las funciones que se ejecutarán en la parte de abajo
/*Hay un proceso para inicializar el LCD, ya sea usando 8 bits de jalón o de 4 en 4 como lo estamos haciendo actualmente con el Arduino, este viene en el datasheet 
del HD44780*/
void LCD_INICIALIZA();				//Función propia creada para incializar el LCD, a ella se debe mandar un comando o un dato
void ENVIA_CMD(char dato);			//Función para mandar comando
void ENVIA_DATO(char dato);			//Función para mandar datos
void LIMPIA_LCD();					//Función propia para limpiar la pantalla del LCD
void CURSOR_DER();					//Función propia para mover a la derecha el cursor de la pantalla en el LCD
void CURSOR_DER_CORRE();			//Función para hacer que se mueva el cursor hacia la derecha
void CURSOR_IZQ();					//Función propia para mover a la izquierda el cursor de la pantalla en el LCD
void CURSOR_IZQ_CORRE();			//Función para hacer que se mueva el cursor hacia la izquierda
void REGRESO_CASA();				//Función propia para regresar el cursor a su posición inicial
void POS_LINEA1(char posicion);		//Función para acceder a la memoria DDRAM e indicar en qué posición se colocará el caracter que se quiere poner en la pantalla del LCD
void POS_LINEA2(char posicion);		//Función para acceder a la memoria DDRAM e indicar en qué posición se colocará el caracter que se quiere poner en la pantalla del LCD
void POS_LINEA3(char posicion);		//Función para acceder a la memoria DDRAM e indicar en qué posición se colocará el caracter que se quiere poner en la pantalla del LCD
void POS_LINEA4(char posicion);		//Función para acceder a la memoria DDRAM e indicar en qué posición se colocará el caracter que se quiere poner en la pantalla del LCD
void CAR_ESP0();					//Función para crear un primer caracter especial, osea uno personalizado
void CAR_ESP1();					//Función para crear un segundo caracter especial, osea uno personalizado
void CAR_ESP2();					//Función para crear un tercer caracter especial, osea uno personalizado
void CAR_ESP3();					//Función para crear un cuarto caracter especial, osea uno personalizado
void animacion();					//Función para crear una animación personalizada



/*LIBRERÍA: Contiene los métodos o constantes que puede usar un archivo ejecutable para realizar ciertas acciones y no contiene método main.*/
//FUNCIÓN PARA CONFIGURAR LA LCD ANTES DE MANDARLE DATOS, USANDO INSTRUCCIONES DE COMANDO DESCRITAS EN LA TABLA 1.3 COMANDOS DE CONTROL DEL DOCUMENTO DE WORD
/*LCD_INICIALIZA: Función para configurar el LCD a que su cursor esté en el inicio, exista una comunicación de 4 bits, disponga de 2 líneas el LCD, sus caracteres sean de 
5X8 pixeles, su cursor sea visible y no parpadee, su incremenento de posición sea hacia la derecha, su pantalla esté estática y se manden datos al LCD, no comandos.*/
void LCD_INICIALIZA(){
	//0000 0010 = 0000  00DB1x; Comando Retorno a casa; Regresa el cursor al inicio (DB1 = 1, DB0 = X (Don't care))
	ENVIA_CMD(0x02);
	//0010 1000 = 001DL NFxx; Comando Ajuste de Función; Comunicación de 4 bits (DL = 0), LCD de 2 líneas (N = 1) y fuente de caracter de 5X8 puntos o pixeles (F = 0) 
	ENVIA_CMD(0x28);
	//0000 1110 = 0000  1DCB; Comando Control de pantalla; Pantalla encendida (D = 1), cursor visible (C = 1) y no parpadea (B = 0)
	ENVIA_CMD(0x0E); 
	//0000 0110 = 0000  01I/DS; Comando Ajuste modo de entrada; Incremento de posición de caracteres hacia la derecha (I/D = 1) y pantalla estática (S = 0)
	ENVIA_CMD(0x06); 
	//0000 0001 = 0000  000DB0; Comando Limpiar Pantalla; Borra la pantalla del LCD y regresa el cursor al inicio (DB0 = 1)
	ENVIA_CMD(0x01); 
	//1000 0000 = 1000  0000; Comando Escribir en la RAM; Envío de datos (RS = 1)
	ENVIA_CMD(0x80);
}


//ENVIA_CMD: Función que sirve para mandar comandos de configuración a la LCD, RS = 0; Envío de comandos
void ENVIA_CMD(char dato){
	
	char cmd_enviar; //Variable de tipo primitivo char llamada comando enviar o cmd_enviar que guarda el comando a mandar al LCD.
	
	//NIBBLE ALTO:
	/*En el LCD tanto para indicar comandos como para mandar datos se deben manejar 8 bits, por lo que la siguiente función aplica máscaras y realiza operaciones para
	manejar los datos de 8 bits enviados en 2 nibbles (4 bits), primero enviando un nibble por 4 de los 6 pines disponibles en el Arduino y luego enviando el nibble
	faltante, primero se manda el nibble alto:
	Al LCD se pueden mandar solo  4 bits de comandos o datos por el Puerto C del Arduino, es importante saber que siempre se deben conectar solo los 4 pines DB4, DB5, DB6 y 
	DB7 al Puerto C, dejando los otros 4 cables al aire, sin conectarse a nada, por lo tanto solamente 6 bits en total contando los 2 de control son los que se conectan y son 
	los siguientes:
	Arduino:	A5,		A4,		A3,		A2,		A1,		A0
	LCD:		DB7,	DB6,	DB5,	DB4,	E,		RS
	Mandando así el nibble alto.*/
	/*Los pines de datos DB4, DB5, DB6 y DB7 están asociados a los pines A2, A3, A4 y A5 del Arduino para mandar los nibbles de datos que indican los caracteres a mandar 
	al LCD, nibble se refiere al conjunto de 4 bits, 2 nibbles conforman un byte.*/
	/*Del dato recibido como parámetro en la función ENVIA_CMD se remueve el nibble bajo, osea los 4 últimos bits leídos de izquierda a derecha, para ello se usa el 
	signo & que significa and y con el número 1111 0000 = F0, se realiza una compuerta lógica AND para volver cero lo que haya en el nibble bajo y solo dejar igual lo 
	que haya en el nibble alto, osea los primeros 4 bits y este se almacena en la variable cmd_enviar.*/
	//MÁSCARA AND: Convierte en 0 todo lo que esté en contacto con los 0 de la máscara y deja pasar tal cual lo que esté en contacto con los 1 de la máscara.
	//MÁSCARA OR: Convierte en 1 todo lo que esté en contacto con los 1 de la máscara y deja pasar tal cual lo que esté en contacto con los 0 de la máscara.
	/*La constante LCD_RS se refiere al pin de control RS del LCD, asociado al pin A0 del Puerto C en el Arduino
		-	la instrucción 1<<LCD_E está indicando que a la constante LCD_RS se le debe asignar un 1.
		-	la instrucción ~(1<<LCD_RS) está indicando que a la constante LCD_RS se le debe asignar un 0, que es el opuesto al 1.
		-	la instrucción cmd_enviar>>2 está indicando que	el número binario cmd_enviar se recorra dos lugares a la derecha, esto es explicado en el documento 2.-LCD.
		-	La instrucción & es un AND, lo que crea es una máscara AND.
		-	La instrucción | es un OR, lo que crea es una máscara OR.
		-	La instrucción ~ es un NOT.
		-	La instrucción &= lo que hace es solo asignar el valor de un bit 0 de un número binario sin afectar a los demás.
		-	La instrucción |= lo que hace es solo asignar el valor de un bit 1 de un número binario sin afectar a los demás.*/
	cmd_enviar =	(dato & 0xF0);	//Obtener nibble alto = DB7,	DB6,	DB5,	DB4,	DB3,	DB2,	DB1,	DB0 & 1111 0000
	LCD_PUERTO =	cmd_enviar>>2;	//Se recorre el número del nibble alto dos lugares a la derecha para que entre en los 6 pines del Puerto C
	//AÑADIR LOS BITS DE CONTROL E y RS A LOS 6 PINES DEL NÚMERO BINARIO MANDADO AL PUERTO C:
	//LCD_PUERTO	= cmd_enviar = BD7 BD6 DB5 DB4  0 0 0 0, 8 bits de datos, después de la máscara termina siendo el nibble alto diferente de 0 y lo demás cero
	//LCD_RS		= 0 0 0 0 0 PC0, constante declarada arriba asociada a uno de los 6 bits del Puerto C, en específico al pin PC0
	//LCD_E			= 0 0 0 0 PC1 0, constante declarada arriba asociada a uno de los 6 bits del Puerto C, en específico al pin PC1
	//RS = 0, Envío de comandos; RS = 1, Envío de datos
	//E	 = 0, LCD apagado;		 E  = 1, LCD prendido
	//Máscara AND &=: Se usa cuando se quiere añadir un número binario 0 a una variable sin afectar a sus demás bits.
	LCD_PUERTO &=	~(1<<LCD_RS);	//Se asigna un número 0 al pin RS; RS = 0 (Envío de comandos)
	/*La constante LCD_E se refiere al pin de control E del LCD, asociado al pin A1 del Puerto C en el Arduino*/
	//Máscara OR |=: Se usa cuando se quiere añadir un número binario 1 a una variable sin afectar a sus demás bits.
	LCD_PUERTO |=	(1<<LCD_E);		//Se asigna un número 1 al pin E; E = 1 (LCD Encendido)
	_delay_ms(1);
	LCD_PUERTO &=	~(1<<LCD_E);	//E = 0; LCD Apagado, además este se agrega a la variable asignada al Puerto C después de cierto delay
	_delay_ms(10); //Delay de 10 milisegundos después de haber mandado el nibble alto, para mandar ahora el nibble bajo
	
	//NIBBLE BAJO:
	/*Primero se mueve el dato original 2 lugares con el shift right y luego se aplica la máscara 3C para que pueda entrar directo el número en el Puerto C junto con los 
	2 bits de control.*/
	cmd_enviar =	((dato<<2) & 0x3C); //Obtener nibble bajo = DB7,	DB6,	DB5,	DB4,	DB3,	DB2,	DB1,	DB0 & 0011 1100
	LCD_PUERTO =	cmd_enviar;		//Se envía nibble bajo al puerto C
	//AÑADIR LOS BITS DE CONTROL E y RS A LOS 6 PINES DEL NÚMERO BINARIO MANDADO AL PUERTO C:
	LCD_PUERTO &=	~(1<<LCD_RS);	//RS = 0 (Envío de comandos)
	LCD_PUERTO |=	(1<<LCD_E);		//E = 1 (LCD Encendido)
	_delay_ms(1);
	LCD_PUERTO &=	~(1<<LCD_E);	//E = 0; LCD Apagado
	_delay_ms(10); //Delay de 10 milisegundos después de haber mandado el nibble bajo, para mandar ahora el nibble alto de nuevo
	return;
}


/*ENVIA_DATO: Función que manda en dos conjuntos de 4 bits (nibble alto y nibble bajo) los 8 bits de datos y a su vez 2 bits de control en los 6 pines del puerto C, indicando 
así el caracter a mostrar en el LCD, RS = 1; Envío de datos.*/
void ENVIA_DATO(char dato){
	char dto_enviar;							//Variable que almacena el dato a mandar
	//NIBBLE ALTO:
	dto_enviar =	(dato & 0xF0);				//Obtención del nibble alto
	LCD_PUERTO =	dto_enviar>>2;				//Se mueve dos lugares el número binario del nibble alto para caber en los 6 bits Puerto C y enviarse al Arduino
	//A�ADIR LOS BITS DE CONTROL E y RS A LOS 6 PINES DEL NÚMERO BINARIO MANDADO AL PUERTO C:
	LCD_PUERTO |=	(1<<LCD_RS)|(1<<LCD_E);		//RS = 1 (Envío de datos), E = 1 (LCD Encendido)
	_delay_ms(1);
	LCD_PUERTO &=	~(1<<LCD_E);				//E = 0 (LCD Apagado)
	LCD_PUERTO &=	~(1<<LCD_RS);				//RS = 0 (Envío de comandos)
	_delay_ms(10); //Delay de 10 milisegundos después de haber mandado el nibble alto, para mandar ahora el nibble bajo
	
	//NIBBLE BAJO:
	dto_enviar =	((dato<<2) & 0x3C);			//Obtención del nibble bajo y se mueve de una vez para caber en los 6 bits del puerto C para poder enviarse al Arduino
	LCD_PUERTO =	dto_enviar;					//Se envía nibble bajo al número binario a mandarse al puerto C
	//A�ADIR LOS BITS DE CONTROL E y RS A LOS 6 PINES DEL NÚMERO BINARIO MANDADO AL PUERTO C:
	LCD_PUERTO |=	(1<<LCD_RS)|(1<<LCD_E);		//RS = 1 (Envío de datos), E = 1 (LCD Encendido)
	_delay_ms(1);
	LCD_PUERTO &=	~(1<<LCD_E);				//E = 0 (LCD Apagado)
	LCD_PUERTO &=	~(1<<LCD_RS);				//RS = 0 (Envío de comandos)
	_delay_ms(10); //Delay de 10 milisegundos después de haber mandado el nibble bajo, para mandar ahora el nibble alto de nuevo
	return;
}


//FUNCIÓN DE ENVIAR CADENA PARA MANDAR VARIOS CARACTERES A LA VEZ, ejecutando varias veces la función de ENVIAR_DATO()
/*ENVIA_CADENA: Función que ejecuta varias veces la función ENVIA_DATO, específicamente hasta que se encuentre con un caracter vacío, osea cuando ya haya recorrido cada letra 
o caracter de la palabra que recibe como parámetro.
ENVIA_DATO: Función que manda en dos partes los 8 bits de datos (nibble alto y nibble bajo) y a su vez 2 bits de control en los 6 pines del puerto C, indicando así el caracter 
a mostrar en el LCD, RS = 1; Envío de datos.*/
void ENVIA_CADENA(const char *dato){
	int i;
	/*El bucle se ejecuta hasta la condición dato[i]!='\0' porque '\0' representa un caracter vacío, ya que hasta el espacio es un caracter en la tabla del código 
	ASCII, esto porque cada caracter tiene su número en la tabla, pero se empieza a contar los caracteres desde el número 1, no desde el cero, porque el 0 representa 
	un caracter vacío.*/
	for(i = 0; dato[i]!='\0'; i++){
		ENVIA_DATO(dato[i]);
	}
}


/*FUNCIÓN PARA CREAR UN CARACTER PERSONALIZADO: Existe un área denominada CGRAM en la memoria de caracteres predeterminados dentro de la LCD llamada CGROM donde se pueden 
grabar ocho caracteres personalizados definidos por el usuario, estos corresponden al nibble alto 0000 y todas las denominaciones de nibble bajo, para acceder a esta 
parte se debe */

//1.- CARACTER PERSONALIZADO DE REACTOR ARC
void CAR_ESP0(){
	//COMANDO: El comando está apuntando a una dirección de la tabla de caracteres grabado en la CGROM para guardar el caracter en un espacio de la memoria
	ENVIA_CMD(0x40);		//0x40 = 0100 0000; Comando que apunta a la dirección CGRAM de la memoria de caracteres del LCD donde se pueden crear caracteres.
	_delay_ms(1);
	/*DATOS: En los datos se considera que los caracteres son de 5X8 puntos (pixeles), donde se tienen 8 filas y 5 columnas para crear un caracter: 
		- Los 0 hacen que no se pinte el pixel.
		- Los 1 hacen que se pinte de negro.
	Los datos que importan son los 5 bits menos significativos del número binario mandado (los de la derecha).*/
	ENVIA_DATO(0b00000000);	//1.- 0000 0000 = 000 00000
	_delay_ms(1);
	ENVIA_DATO(0b00011111);	//2.- 0001 1111 = 000 11111
	_delay_ms(1);
	ENVIA_DATO(0b00010001);	//3.- 0001 0101 = 000 10001
	_delay_ms(1);
	ENVIA_DATO(0b00011011);	//4.- 0001 1011 = 000 11011
	_delay_ms(1);
	ENVIA_DATO(0b00001110);	//5.- 0000 1010 = 000 01110
	_delay_ms(1);
	ENVIA_DATO(0b00000100);	//6.- 0000 0100 = 000 00100
	_delay_ms(1);
	ENVIA_DATO(0b00000000);	//7.- 0000 0000 = 000 00000
	_delay_ms(1);
	ENVIA_DATO(0b00000000);	//8.- 0000 0000 = 000 00000
	_delay_ms(1);
}


//2.- CARACTER PERSONALIZADO DE SONRISA
void CAR_ESP1(){
	//COMANDO: El comando está apuntando a una dirección de la tabla de caracteres grabado en la CGROM para guardar el caracter en un espacio de la memoria
	ENVIA_CMD(0x48);		//0x48 = 0100 1000; Comando que apunta a la dirección CGRAM de la memoria de caracteres del LCD donde se pueden crear caracteres.
	_delay_ms(1);
	/*DATOS: En los datos se considera que los caracteres son de 5X8 puntos (pixeles), donde se tienen 8 filas y 5 columnas para crear un caracter, los 0 hacen que no se 
	pinte el pixel y los 1 hacen que se pinte de negro, los datos que importan son los 5 bits menos significativos del número binario mandado (los de la derecha).*/
	ENVIA_DATO(0b00000000);	//1.- 0000 0000
	_delay_ms(1);
	ENVIA_DATO(0b00001010);	//2.- 0000 1010
	_delay_ms(1);
	ENVIA_DATO(0b00001010);	//3.- 0000 1010
	_delay_ms(1);
	ENVIA_DATO(0b00001010);	//4.- 0000 1010
	_delay_ms(1);
	ENVIA_DATO(0b00000000);	//5.- 0000 0000
	_delay_ms(1);
	ENVIA_DATO(0b00010001);	//6.- 0001 0001
	_delay_ms(1);
	ENVIA_DATO(0b00001110);	//7.- 0000 1110
	_delay_ms(1);
	ENVIA_DATO(0b00000000);	//8.- 0000 0000
	_delay_ms(1);
}


//3.- CARACTER PERSONALIZADO DE MONITO DE CORAZÓN
void CAR_ESP2(){
	//COMANDO: El comando está apuntando a una dirección de la tabla de caracteres grabado en la CGROM para guardar el caracter en un espacio de la memoria
	ENVIA_CMD(0x50);		//0x40 = 0100 0000; Comando que apunta a la dirección CGRAM de la memoria de caracteres del LCD donde se pueden crear caracteres.
	_delay_ms(1);
	/*DATOS: En los datos se considera que los caracteres son de 5X8 puntos (pixeles), donde se tienen 8 filas y 5 columnas para crear un caracter, los 0 hacen que no se 
	pinte el pixel y los 1 hacen que se pinte de negro, los datos que importan son los 5 bits menos significativos del número binario mandado (los de la derecha).*/
	ENVIA_DATO(0b00000000);	//1.- 0000 0000
	_delay_ms(1);
	ENVIA_DATO(0b00000000);	//2.- 0000 0000
	_delay_ms(1);
	ENVIA_DATO(0b00011011);	//3.- 0001 1011
	_delay_ms(1);
	ENVIA_DATO(0b00010101);	//4.- 0001 0101
	_delay_ms(1);
	ENVIA_DATO(0b00010001);	//5.- 0001 0001
	_delay_ms(1);
	ENVIA_DATO(0b00001010);	//6.- 0000 1010
	_delay_ms(1);
	ENVIA_DATO(0b00000100);	//7.- 0000 0100
	_delay_ms(1);
	ENVIA_DATO(0b00000000);	//8.- 0000 0000
	_delay_ms(1);
}


//4.- CARACTER PERSONALIZADO DE TÍO COSA
void CAR_ESP3(){
	//COMANDO: El comando está apuntando a una dirección de la tabla de caracteres grabado en la CGROM para guardar el caracter en un espacio de la memoria
	ENVIA_CMD(0x58);		//0x40 = 0100 0000; Comando que apunta a la dirección CGRAM de la memoria de caracteres del LCD donde se pueden crear caracteres.
	_delay_ms(1);
	/*DATOS: En los datos se considera que los caracteres son de 5X8 puntos (pixeles), donde se tienen 8 filas y 5 columnas para crear un caracter, los 0 hacen que no se 
	pinte el pixel y los 1 hacen que se pinte de negro, los datos que importan son los 5 bits menos significativos del número binario mandado (los de la derecha).*/
	ENVIA_DATO(0b00011111);	//1.- 0001 1111
	_delay_ms(1);
	ENVIA_DATO(0b00010101);	//2.- 0001 0101
	_delay_ms(1);
	ENVIA_DATO(0b00011111);	//3.- 0001 1111
	_delay_ms(1);
	ENVIA_DATO(0b00011111);	//4.- 0001 1111
	_delay_ms(1);
	ENVIA_DATO(0b00011111);	//5.- 0001 1111
	_delay_ms(1);
	ENVIA_DATO(0b00001010);	//6.- 0000 1010
	_delay_ms(1);
	ENVIA_DATO(0b00001010);	//7.- 0000 1010
	_delay_ms(1);
	ENVIA_DATO(0b00011011);	//8.- 0001 1011
	_delay_ms(1);
}


//LIMPIA_LCD: Función que limpia el LCD, borrando su pantalla y regresando su cursor a su posición inicial.
void LIMPIA_LCD(){
	//0000 0001 = 0000  000DB0; Comando Limpiar Pantalla; Borra la pantalla del LCD y regresa el cursor al inicio (DB0 = 1)
	ENVIA_CMD(0x01);
}


//REGRESO_CASA: Función que regresa el cursor a su posición inicial
void REGRESO_CASA(){
	//0000 0010 = 0000  00DB1x; Comando Retorno a casa; Regresa el cursor al inicio (DB1 = 1, DB0 = X (Don't care))
	ENVIA_CMD(0x02);
}


//CURSOR_DER: Función que permite mover el cursor hacia la derecha, moviendo además la posición del LCD, haciendo que la pantalla se mueva con todo y el cursor.
void CURSOR_DER(){
	//0001 1100 = 0001  S/C R/Lxx; Comando Desplazamiento de Cursor & Pantalla; Movimiento del cursor solo horizontal en una fila (S/C = 1), cursor a la derecha (R/L = 1)
	ENVIA_CMD(0x1C);
}


/*CURSOR_DER_CORRE: Permite mover el cursor hacia la derecha del texto que tenga a la derecha, para que sobreescriba un caracter del texto mostrado en el LCD, sin mover la 
pantalla del LCD.*/
void CURSOR_DER_CORRE(){
	//0001 0100 = 0001  S/C R/Lxx; Comando Desplazamiento de Cursor & Pantalla; Movimiento del cursor a cualquier posición (S/C = 0), cursor a la derecha (R/L = 1)
	ENVIA_CMD(0x14);
}


//CURSOR_IZQ: Función que permite mover el cursor hacia la izquierda, moviendo además la posición del LCD, haciendo que la pantalla se mueva con todo y el cursor.
void CURSOR_IZQ(){
	//0001 1000 = 0001  S/C R/Lxx; Comando Desplazamiento de Cursor & Pantalla; Movimiento del cursor solo horizontal en una fila (S/C = 1), cursor a la izquierda (R/L = 0)
	ENVIA_CMD(0x18);
}


/*CURSOR_IZQ_CORRE: Permite mover el cursor hacia la izquierda del texto que tenga a la derecha, para que sobreescriba un caracter del texto mostrado en el LCD, sin mover la
pantalla del LCD.*/
void CURSOR_IZQ_CORRE(){
	//0001 0000 = 0001  S/C R/Lxx; Comando Desplazamiento de Cursor & Pantalla; Movimiento del cursor a cualquier posición (S/C = 0), cursor a la izquierda (R/L = 0)
	ENVIA_CMD(0x10);
}


/*En cada fila del LCD se puede contar con 16 caracteres, yendo de la posición 0X00 hasta la 0X27 en la primera fila y de la 0X40 hasta la 0X67 en la segunda, se pueden 
dividir estas posiciones en 4 para saber dónde va a aparecer cada caracter o cadena de caracteres, donde las posiciones son las siguientes tomando en cuenta si la pantalla 
se mantiene estática o se mueve de forma dinámica:
	- Posición 1: Se refiere a la primera línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica.
	- Posición 2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica.
	- Posición 3: Se refiere a la primera línea de 16 caracteres que se ve solamente cuando la pantalla es dinámica, ya que se encuentra justo alado de la posición 1.
	- Posición 4: Se refiere a la segunda línea de 16 caracteres que se ve solamente cuando la pantalla es dinámica, ya que se encuentra justo alado de la posición 2.*/
/*POS_LINEA1: Se refiere a la primera línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual de 
los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
void POS_LINEA1(char posicion){
	char linea1 = 0x80; //0x80 = 1000 0000
	//MÁSCARA OR: Convierte en 1 todo lo que esté en contacto con los 1 de la máscara y deja pasar tal cual lo que esté en contacto con los 0 de la máscara.
	posicion = posicion | linea1;
	ENVIA_CMD(posicion);
}


/*POS_LINEA2: Se refiere a la segunda línea de 16 caracteres que se ve cuando la pantalla esté estática o dinámica, en el parámetro de la función se indica después de cual de
los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.*/
void POS_LINEA2(char posicion){
	char linea2 = 0xC0; //0xC0 = 1100 0000
	//MÁSCARA OR: Convierte en 1 todo lo que esté en contacto con los 1 de la máscara y deja pasar tal cual lo que esté en contacto con los 0 de la máscara.
	//La variable posición que es recibida como parámetro de la función indica después de cual de los 16 caracteres empezará a imprimir en pantalla el mensaje en el LCD.
	posicion = posicion | linea2;
	ENVIA_CMD(posicion);
}


//POS_LINEA3: Se refiere a la primera línea de 16 caracteres que se ve solamente cuando la pantalla es dinámica, ya que se encuentra justo alado de la posición 1.
void POS_LINEA3(char posicion){
	char linea3 = 0x94; //0x94 = 1001 0100
	//MÁSCARA OR: Convierte en 1 todo lo que esté en contacto con los 1 de la máscara y deja pasar tal cual lo que esté en contacto con los 0 de la máscara.
	posicion = posicion | linea3;
	ENVIA_CMD(posicion);
}


//POS_LINEA4: Se refiere a la segunda línea de 16 caracteres que se ve solamente cuando la pantalla es dinámica, ya que se encuentra justo alado de la posición 2.
void POS_LINEA4(char posicion){
	char linea4 = 0xD4; //0xD4 = 1101 0100
	//MÁSCARA OR: Convierte en 1 todo lo que esté en contacto con los 1 de la máscara y deja pasar tal cual lo que esté en contacto con los 0 de la máscara.
	posicion = posicion | linea4;
	ENVIA_CMD(posicion);
}


//animacion: Función para crear una animación personalizada
void animacion(){
	//Se manda el caracter 2 a la función ENVIA_DATO() para que se muestre en la pantalla del LCD
	ENVIA_DATO(0); //Impresión del caracter persionalizado 1.- CARACTER PERSONALIZADO DE REACTOR ARC
	/*CURSOR_IZQ_CORRE: Permite mover el cursor hacia la izquierda del texto que tenga a la derecha, para que sobreescriba un caracter del texto mostrado en el LCD, sin mover 
	la pantalla del LCD.
	Si esta instrucción no se pone, se irá escribiendo uno tras otro los símbolos de la animación, llenando la LCD y moviendo la pantalla.*/
	CURSOR_IZQ_CORRE();
	_delay_ms(500); //Retraso de 500 milisegundos después de haber impreso el caracter en pantalla y corrido el cursor hacia la izquierda.
	//Se manda el caracter 2 a la función ENVIA_DATO() para que se muestre en la pantalla del LCD
	ENVIA_DATO(1); //Impresión del caracter persionalizado 2.- CARACTER PERSONALIZADO DE SONRISA
	/*CURSOR_IZQ_CORRE: Permite mover el cursor hacia la izquierda del texto que tenga a la derecha, para que sobreescriba un caracter del texto mostrado en el LCD, sin mover 
	la pantalla del LCD.
	Si esta instrucción no se pone, se irá escribiendo uno tras otro los símbolos de la animación, llenando la LCD y moviendo la pantalla.*/
	CURSOR_IZQ_CORRE();
	_delay_ms(500); //Retraso de 500 milisegundos después de haber impreso el caracter en pantalla y corrido el cursor hacia la izquierda.
	//Se manda el caracter 2 a la función ENVIA_DATO() para que se muestre en la pantalla del LCD
	ENVIA_DATO(2); //Impresión del caracter persionalizado 3.- CARACTER PERSONALIZADO DE MONITO DE CORAZÓN
	/*CURSOR_IZQ_CORRE: Permite mover el cursor hacia la izquierda del texto que tenga a la derecha, para que sobreescriba un caracter del texto mostrado en el LCD, sin mover 
	la pantalla del LCD.
	Si esta instrucción no se pone, se irá escribiendo uno tras otro los símbolos de la animación, llenando la LCD y moviendo la pantalla.*/
	CURSOR_IZQ_CORRE();
	_delay_ms(500); //Retraso de 500 milisegundos después de haber impreso el caracter en pantalla y corrido el cursor hacia la izquierda.
	//Se manda el caracter 3 a la función ENVIA_DATO() para que se muestre en la pantalla del LCD
	ENVIA_DATO(3); //Impresión del caracter persionalizado 4.- CARACTER PERSONALIZADO DE TÍO COSA
	/*CURSOR_IZQ_CORRE: Permite mover el cursor hacia la izquierda del texto que tenga a la derecha, para que sobreescriba un caracter del texto mostrado en el LCD, sin mover 
	la pantalla del LCD.
	Si esta instrucción no se pone, se irá escribiendo uno tras otro los símbolos de la animación, llenando la LCD y moviendo la pantalla.*/
	CURSOR_IZQ_CORRE();
	_delay_ms(500); //Retraso de 500 milisegundos después de haber impreso el caracter en pantalla y corrido el cursor hacia la izquierda.
}


/*
void PARA(){
	//0000 1111 = ?
	ENVIA_CMD(0x0F);   //0000 1111
}

void myfunction(const char *dato){
	int i;
	for(i=1;dato[i]!='\0';i++)
	ENVIA_DATO(dato[i]);
	_delay_ms(500);
	POS_LINEA2(0);
	for(i=0;dato[i]!='\0';i++)
	ENVIA_DATO(dato[i]);
	_delay_ms(500);
	POS_LINEA2(0);
}
*/