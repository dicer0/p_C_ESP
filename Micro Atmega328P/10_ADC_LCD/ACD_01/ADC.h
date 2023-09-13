/*
 * ADC.h
 *
 * Created: 26/03/2019 06:43:42 p.m.
 *  Author: Marlon
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <util/delay.h>

void ADC_init();
int ADC_GetData(int canal);


void ADC_init()
{
	// Referencia AVCC
	ADMUX = (1 << REFS0);
	
	// ADC habilitado con preescaler de 128
	// F_ADC = 8000000/128 = 62.5 KHz
	// F_MUESTREO = 62500/13 = 4.8Khz
	// T_MUESTREO = 1/4800 = 208us
	// La conversión tarda 208us.
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}

int ADC_GetData(int canal)
{
	// seleccionamos el nuevo canal a leer solo modificando solo
	// los bits MUX0 MUX1 y MUX2.
	ADMUX =(ADMUX & 0xF8) | (canal & 0x07);
	
	// se inicia la conversión.
	ADCSRA |= (1 << ADSC);
	
	// espera a que termina la conversión.
	while(ADCSRA & (1 << ADSC));
	
	// regresamos el valor de la conversión.
	return ADC;
}

#endif /* ADC_H_ */