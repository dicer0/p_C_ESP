/*
 * ACD_01.c
 *
 * Created: 07/06/2022 12:53:44 p. m.
 * Author : diego
 */ 


# define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include "LCDA.h"
#include "ADC.h"

int array2int(int arr[]);
int char2int(char c);
char int2char(int i);
void printstr(int n[],size_t sz);
int * int2array(int number);
int numsz(int number);
int numerito;


double intpart;
double x;
double fracp;
float adcV;
float adcV2;
float iadc;
float iadc2;


int main(void)
{
	DDRB = 0xFF;//LCD
	//DDRC &=~ (1<<DDC0);//ADC entrada
	DDRD = 0xF0;//Salida nibble alto teclado
	//DDRB = 0xFE;//Entrada botón, salida PWM
	DDRC |= (1<<DDC2);//Foco comparador
	
	cli();
	ADC_init();
	sei();
	LCD_INICIALIZA();
	LIMPIA_LCD();
	CAR_ESP0();
	_delay_ms(10);
	cursor_blink_on(0);
	POS_LINEA1(6);
	ENVIA_CADENA("ADC: ");
	_delay_ms(1000);
	
	POS_LINEA2(0);
	ENVIA_CADENA("Diego Cervantes_");
	
	while (1)
	{
		iadc=ADC_GetData(0);
		adcV = iadc*5.0/1024.0;
		adcV = (adcV*1024.0*0.2490)/5.0;
		_delay_ms(10);
		POS_LINEA1(2);
		ENVIA_CADENA("VIN:       ");
		POS_LINEA1(7);
		printstr(int2array(adcV),numsz(adcV));
		ENVIA_DATO('.');
		x = (int)adcV;
		fracp=modf(x, &intpart)*10000;
		printstr(int2array(fracp),numsz(fracp));
		ENVIA_DATO('V');
		///////
		iadc2=ADC_GetData(1);
		adcV2= iadc2*5.0/1024.0;
		POS_LINEA2(0);
		_delay_ms(100);
		
	}
}


void printstr(int n[],size_t sz)
{
	int i=0;
	for (i=0;i<sz;i++)
	{
		ENVIA_DATO(int2char(n[i]));
	}
}

int array2int(int a[3])
{
	int n=3;
	int i,k=0;
	for (i=0;i<n+1;i++)
	{
		k=10*k+a[i];
	}
	return k;
}

int char2int(char c)
{
	int num = 0;
	num = c - '0';
	return num;
}

char int2char(int i)
{
	char c = i + '0';
	return c;
}

int * int2array(int number)
{
	int n = log10(number)+1;
	int i;
	int *numberArray = calloc(n, sizeof(int));
	for ( i = 0; i < n; ++i, number /= 10 )
	{
		numberArray[n-i-1] = number % 10;
	}
	return numberArray;
}

int numsz(int number)
{
	int n = log10(number)+1;
	return n;
}