/* 
* 
* File:		display_4b.c
* Date:		1/2014
*
* Copyright (c)2014	Lukas Janik <lucaso.janik@gmail.com>
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
#define F_CPU 1000000UL 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "../lib/mega8.h"

int main(void)
{	
	char * buffer = "000000000000000";

	// initializes the USART
	USART_Init();
	
	// initializes ADC unit
	ADC_Init();
	//ADCSRA |= (1 << ADSC); 

	DDRD = 1;
	DDRC = (15 << DDC0);
	DDRB = (7 << DDB3);
	
	_delay_ms(1000);

	// initializes display
	DisplayInit();

	// enables innterupts			
	sei();

	// writes hello string to USART

	USART_PrintString("4-bit display program with keyboard\r\n");
	
    while(1)
    {
    	// Ẃrites ADC value to first line of LCD 
    	sprintf(buffer,"ADC: %d  ",ADCH);
    	DisplayWriteString(buffer,0);
    	
    	switch(GetKey(ADCH))
    	{
    		case btnUP: strcpy(buffer,"UP   "); break;
    		case btnDOWN: strcpy(buffer,"DOWN"); break;
    		case btnLEFT: strcpy(buffer,"LEFT"); break;
    		case btnRIGHT: strcpy(buffer,"RIGHT"); break;
    	}
    	DisplayWriteString(buffer,40);
    	_delay_ms(50);
    	ADCSRA |= (1 << ADSC); 

    }
}

ISR(USART_RXC_vect)
{	
	char buffer = UDR;

	switch(buffer)
	{
		case 13: DisplayWriteInst(0xC0); break;	// enter
		case 10: DisplayWriteInst(0xC0); break;	// enter
		case 8: DisplayWriteInst(16); DisplayWriteChar(' '); DisplayWriteInst(16); break; 	// backspace = go back, draw space, go back
		case 27: DisplayWriteInst(2); break;				// escape
				
		default: DisplayWriteChar(buffer); break;
	}
	
	UDR=buffer;
}

ISR(ADC_vect)
{

}
