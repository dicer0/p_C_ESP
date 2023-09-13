// BIBLOTECA PARA EL MANEJO DE UN LCD EN MODO DE 4 BITS para tarjeta ARDUNO

#include <util/delay.h>
#include <inttypes.h>
#define LCD_PUERTO PORTB //Define el Puerto al cual van a ser conectados los pines del LCD
#define LCD_RS PB0       //Define el pin del Microcontrolador al cua se conecta el pin RS del LCD
#define LCD_E PB1        //Define el pin del Microcontrolador al cual se conecta el pin E del LCD

  
//Prototipos de función para LCD en modo de 4 bits
void LCD_INICIALIZA();      //Inicializa el LCD
void ENVIA_CMD(char dato);  //Función para envío de comandos
void ENVIA_DATO(char dato); // Función para envío de Datos
void ENVIA_CADENA(const char *dato);
void LIMPIA_LCD();
void CURSOR_DER();
void CURSOR_DER_CORRE();
void CURSOR_IZQ();
void CURSOR_IZQ_CORRE();
void REGRESO_CASA();
void POS_LINEA1(char posicion);
void POS_LINEA2(char posicion);
void POS_LINEA3(char posicion);
void POS_LINEA4(char posicion);
void CAR_ESP0();
void CAR_ESP1();
void animacion();


void CAR_ESP0(void)//flecha
{
	ENVIA_CMD(0X40);
	_delay_ms(1);
	ENVIA_DATO(0b00011111);
	_delay_ms(1);
	ENVIA_DATO(0b00011011);
	_delay_ms(1);
	ENVIA_DATO(0b00011101);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00011101);
	_delay_ms(1);
	ENVIA_DATO(0b00011011);
	_delay_ms(1);
	ENVIA_DATO(0b00011111);
	_delay_ms(1);
	ENVIA_DATO(0b00011111);
	_delay_ms(1);
	//ENVIA_CMD(0x80);
	//ENVIA_DATO(0);
}

void CAR_ESP1(void)//Stand
{
	ENVIA_CMD(0X48);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00010101);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001010);
	_delay_ms(1);
	ENVIA_DATO(0b00011011);
	_delay_ms(1);
}

void CAR_ESP2(void)//Trans izq
{
	ENVIA_CMD(0X50);
	_delay_ms(1);
	ENVIA_DATO(0b00000011);
	_delay_ms(1);
	ENVIA_DATO(0b00000011);
	_delay_ms(1);
	ENVIA_DATO(0b00000001);
	_delay_ms(1);
	ENVIA_DATO(0b00001111);
	_delay_ms(1);
	ENVIA_DATO(0b00000001);
	_delay_ms(1);
	ENVIA_DATO(0b00000011);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
}

void CAR_ESP3(void)//trans der
{
	ENVIA_CMD(0X58);
	_delay_ms(1);
	ENVIA_DATO(0b00011000);
	_delay_ms(1);
	ENVIA_DATO(0b00011000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00011100);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00010000);
	_delay_ms(1);
	ENVIA_DATO(0b00010000);
	_delay_ms(1);
	ENVIA_DATO(0b00011100);
	_delay_ms(1);
}
void CAR_ESP4(void)//Agachado
{
	ENVIA_CMD(0X60);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00010101);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00011011);
	_delay_ms(1);
}
void CAR_ESP5(void)//trans arriba
{
	ENVIA_CMD(0X68);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00010101);
	_delay_ms(1);
}
void CAR_ESP6(void)//Trans abajo
{
	ENVIA_CMD(0X70);
	_delay_ms(1);
	ENVIA_DATO(0b00011111);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00001110);
	_delay_ms(1);
	ENVIA_DATO(0b00010001);
	_delay_ms(1);
	ENVIA_DATO(0b00010001);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
}
void CAR_ESP7(void)//ú
{
	ENVIA_CMD(0X78);
	_delay_ms(1);
	ENVIA_DATO(0b00000010);
	_delay_ms(1);
	ENVIA_DATO(0b00000100);
	_delay_ms(1);
	ENVIA_DATO(0b00010001);
	_delay_ms(1);
	ENVIA_DATO(0b00010001);
	_delay_ms(1);
	ENVIA_DATO(0b00010001);
	_delay_ms(1);
	ENVIA_DATO(0b00001011);
	_delay_ms(1);
	ENVIA_DATO(0b00001101);
	_delay_ms(1);
	ENVIA_DATO(0b00000000);
	_delay_ms(1);
}

void LCD_INICIALIZA()
{
	ENVIA_CMD(0x02);
	ENVIA_CMD(0x28); // 00101000 DL = 0 comunicacion de 4 bits N = 1 F = 0
	ENVIA_CMD(0x0E); // D=1 Display ON Cursor OFF
	ENVIA_CMD(0x06); // I/D = 1 Incremento Automatico S=0
	ENVIA_CMD(0x01);  // Limpia LCD
	ENVIA_CMD(0X80);
}

void ENVIA_CMD(char dato)
{
	char cmd_enviar;
	cmd_enviar =(dato & 0xF0);      // se envia el nibble alto
	LCD_PUERTO =  cmd_enviar>>2;       // al puerto
	LCD_PUERTO &=~(1<<LCD_RS);  //  RS = 0
	LCD_PUERTO |= (1<<LCD_E);   //   E = 1
	_delay_ms(1);
	LCD_PUERTO &=~(1<<LCD_E);   //   E = 0
	_delay_ms(10);

	cmd_enviar=((dato<<2) & 0x3C);  // Se envia el nibble bajo
	LCD_PUERTO =  cmd_enviar;        // al puerto
	LCD_PUERTO &=~(1<<LCD_RS);   //  RS = 0
	LCD_PUERTO |= (1<<LCD_E);    //   E = 1
	_delay_ms(1);
	LCD_PUERTO &=~(1<<LCD_E);    //    E = 0
	_delay_ms(10);
	return;
}

void ENVIA_DATO(char dato)
{
	char dto_enviar;
	dto_enviar=(dato & 0xF0);      // se envia el nibble alto
	LCD_PUERTO  =  dto_enviar>>2;       // al puerto
	LCD_PUERTO |= (1<<LCD_RS)|(1<<LCD_E);  //  RS = 1   E = 1
	_delay_ms(1);
	LCD_PUERTO &=~(1<<LCD_E);//E=0
	LCD_PUERTO &=~(1<<LCD_RS);//RS=0
	_delay_ms(1);
	dto_enviar=((dato<<2) & 0x3C);  // Se envia el nibble bajo
	LCD_PUERTO =  dto_enviar;        // al puerto
	LCD_PUERTO |= (1<<LCD_RS)|(1<<LCD_E);  //  RS = 1   E = 1
	_delay_ms(1);
	LCD_PUERTO &=~(1<<LCD_E);//E=0
	LCD_PUERTO &=~(1<<LCD_RS);//RS=0
	_delay_ms(1);
	return;
}

void ENVIA_CADENA(const char *dato)
{
	int i;
	for(i=0;dato[i]!='\0';i++)
	ENVIA_DATO(dato[i]);
}

void LIMPIA_LCD()
{
	ENVIA_CMD(0x1);   //0000 0001
}

void REGRESO_CASA()
{
	ENVIA_CMD(0x02);  //0000 0010
}
void CURSOR_DER()
{
	ENVIA_CMD(0x1C);  //0001 1100
}

void CURSOR_DER_CORRE() //sin desplazar pantalla
{
	ENVIA_CMD(0x14);  //0001 1100
}

void CURSOR_IZQ()
{
	ENVIA_CMD(0x18);  // 0001 1000
}

void CURSOR_IZQ_CORRE() //Sin desplazar pantalla
{
	ENVIA_CMD(0x10);  //0001 1100
}


void POS_LINEA1(char posicion)
{
	char linea1 = 0x80;
	posicion= posicion|linea1;
	ENVIA_CMD(posicion);
}
void POS_LINEA2(char posicion)
{
	char linea2 = 0xC0;
	posicion= posicion|linea2;
	ENVIA_CMD(posicion);
}
void POS_LINEA3(char posicion)
{
	char linea3 = 0x94;
	posicion= posicion|linea3;
	ENVIA_CMD(posicion);
}
void POS_LINEA4(char posicion)
{
	char linea4 = 0xD4;
	posicion= posicion|linea4;
	ENVIA_CMD(posicion);
}

void animacion(){
		ENVIA_DATO(2);
		CURSOR_IZQ_CORRE();
		_delay_ms(50);
		ENVIA_DATO(3);
		CURSOR_IZQ_CORRE();
		_delay_ms(50);
}

void barrido()
{
	for (int i=0;i<17;i++)
	{
		POS_LINEA1(16-i);
		ENVIA_DATO(' ');
		POS_LINEA2(16-i);
		ENVIA_DATO(' ');
	}
}

void cursor_on(int i)
{
	if (i==1)
	{
		ENVIA_CMD(0b00001110);
	} 
	else
	{
		ENVIA_CMD(0b00001100);
	}
}
void cursor_blink_on(int i)
{
	if (i==1)
	{
		ENVIA_CMD(0b00001111);
	}
	else
	{
		ENVIA_CMD(0b00001100);
	}
}
void text_dir(int dir)
{
	if (dir==1)
	{
		ENVIA_CMD(0b00000110);//ESCRIBE DE IZQUIERDA A DERECHA
	}
	else
	{
		ENVIA_CMD(0b00000100);//ESCRIBE DE DERECHA A IZQUIERDA
	}
}

