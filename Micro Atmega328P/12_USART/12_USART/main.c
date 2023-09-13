/*
 * USART.c
 *
 * Created: 11/10/2022 3:00:10 a. m.
 * Author : diego
 */ 


/*DECLARACIÓN DE CONSTANTES USANDO #define: Hay que tener en cuenta que al declarar constantes con #define debemos hacerlo antes de los #include (que importan
librerías), pero antes de declarar nuestras funciones y demás. No se usa el signo de igual para asignar un valor, solo se declara el nombre y luego el valor.*/
#define F_CPU 16000000UL //Frecuencia del reloj, es necesario declararla porque la librería delay de manejo de tiempos la utiliza


//IMPORTACIÓN DE LIBRERÍAS, DIRECTIVAS
#include <avr/io.h>			//Librería para todo el manejo de puertos y demás aspectos del microcontrolador AVR Atmega328P
#include <avr/interrupt.h>	//Librería para el manejo de los registros de interrupciones
#include "../2_LCD_4bits.h"	//Librería propia para el manejo del LCD
#include <stdio.h>			//Librería para la entrada, salida, manejo y conversión de datos, en específico se usa para poder utilizar el método sprintf()
#include <util/delay.h>



unsigned char a = 0;

void ENVIO_USART(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = data;
}

unsigned char RECIVO_USART(void){
	while(!(UCSR0A & (1<<RXC0)));
		return UDR0;
}

int main(void)
{
    cli();
	UCSR0A = (0<<RXC0)|(0<<TXC0)|(0<<UDRE0)|(0<<FE0)|(0<<DOR0);
	UCSR0A |= (0<<U2X0)|(0<<MPCM0); // UCSR0A = 0x00;
	UCSR0B = (0<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
	UCSR0B |= (0<<UCSZ02)|(0<<RXB80)|(0<<TXB80); // UCSR0B = 0b00011000;
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0);  //1 bit de paro
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0); // UCSR0C = 0b00000110;  //trama de 8 bits
	UBRR0H = 0x00;
	UBRR0L = 0x67; //UBRR = 103 -> tasa de baudios 9600
	DDRB = 0xFF; //Puerto B -> salida
	DDRC = 0xFF; //Puerto C como salida (LCD)
	//PORTB = 0;
	PORTC = 0x00;
	sei();
	
	LCD_INICIALIZA();
	//ASCII nueva linea = 10;
    while (1) 
    {
		a = RECIVO_USART();
		//PORTB = a;
		_delay_ms(100);
		ENVIO_USART(a);
		if(a != 10){ //Evita que imprima el equivalente a nueva linea
			POS_LINEA1(0);
			ENVIA_DATO(a);
		}
		_delay_ms(100);
    }
}