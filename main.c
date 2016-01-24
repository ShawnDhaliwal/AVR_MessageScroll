/*
 *  AUTHOR: Shawn Dhaliwal
 *  Description: This program will have two messages scrolled across a LCD screen. 
 *  One message is scrolled across first line, the other on the second line. 
 *  The messages should wrap around, never ending scrolling. The user has the ability to slow down scrolling or speed up scrolling with the push of a button. 
 *  Implementation was done in C, on the Lunix platform, and for the AVR processor.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "lcd_drv.h"

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// These are included by the LCD driver code, so 
// we don't need to include them here.
// #include <avr/io.h>
// #include <util/delay.h>


int main( void )
{

	lcd_init();

    unsigned int count = 0;

	ADCSRA = 0x87;

	/* ADMUX - ADC Multiplexer Selection Register
	 *
	 * Select ADC0
     */
	ADMUX = 0x40;

	lcd_xy(0, 0);
	char *msg1 = "Line1.";  //Custom message for Line1
	char *msg2 = "Line2.";  //Custom message for Line2
	char *msg1Pointer = msg1;
	char *msg2Pointer = msg2;
	char copy1[16];
	char copy2[16];
	int speed = 500;        //Speed of scrolling, half a second.

	for(;;){
	// start conversion
	ADCSRA |= 0x40;

	// bit 6 in ADCSRA is 1 while conversion is in progress
	// 0b0100 0000
	// 0x40
	while (ADCSRA & 0x40)
		;
	unsigned int val = ADCL;
	unsigned int val2 = ADCH;

	val += (val2 << 8);

	count = count + 1;
	
	
	if (val > 1000 )    {
        copy(msg1Pointer, copy1, msg1);
		lcd_puts(copy1);
		msg1Pointer++;
        
		if(*msg1Pointer == NULL){
			msg1Pointer = msg1;
		}
        
		lcd_xy(0,1);
        copy(msg2Pointer, copy2, msg2);
		lcd_puts(copy2);
		_delay_ms(speed);
		msg2Pointer++;
        
		if(*msg2Pointer == NULL){
			msg2Pointer = msg2;
		}
        
		lcd_xy(0,0);
	}
			 
    if (val < 50) {
		if(val < 50)
		speed = SlowDown(speed);
	  	copy(msg1Pointer, copy1, msg1);
		lcd_puts(copy1);
		msg1Pointer++;
        
		if(*msg1Pointer == NULL){
			msg1Pointer = msg1;
		}
		lcd_xy(0,1);
        copy(msg2Pointer, copy2, msg2);
		lcd_puts(copy2);
		_delay_ms(speed);
		msg2Pointer++;
        
		if(*msg2Pointer == NULL){
			msg2Pointer = msg2;
		}
		lcd_xy(0,0); 
	}
    else if (val < 195) {

        while( val < 195){
            ADCSRA |= 0x40;

            // bit 6 in ADCSRA is 1 while conversion is in progress
            // 0b0100 0000
            // 0x40
            while (ADCSRA & 0x40){
		
                val = ADCL;
                val2 = ADCH;
                val += (val2 << 8);
            }


            copy(msg1Pointer, copy1, msg1);
            lcd_puts(copy1);
            if(*msg1Pointer == NULL){
                msg1Pointer = msg1;
            }
            lcd_xy(0,1);
            copy(msg2Pointer, copy2, msg2);
            lcd_puts(copy2);
            if(val>194 & val<380)
                _delay_ms(500);
            else
                val = 193;
            
            if(*msg2Pointer == NULL){
                msg2Pointer = msg2;
            }
            lcd_xy(0,0);
		}
	}
    else if (val < 380) {


	  	copy(msg1Pointer, copy1, msg1);
		lcd_puts(copy1);
		msg1Pointer++;
		if(*msg1Pointer == NULL){
			msg1Pointer = msg1;
		}
		lcd_xy(0,1);
        copy(msg2Pointer, copy2, msg2);
		lcd_puts(copy2);
		_delay_ms(500);
		msg2Pointer++;
		if(*msg2Pointer == NULL){
			msg2Pointer = msg2;
		}
		lcd_xy(0,0); 
	}
        
    else if (val < 555)  {
	

		if(val < 555)
		speed = SpeedUp(speed);



	  	copy(msg1Pointer, copy1, msg1);
		lcd_puts(copy1);
		msg1Pointer++;
		if(*msg1Pointer == NULL){
			msg1Pointer = msg1;
		}
		lcd_xy(0,1);
        copy(msg2Pointer, copy2, msg2);
		lcd_puts(copy2);
		_delay_ms(speed);
		msg2Pointer++;
		if(*msg2Pointer == NULL){
			msg2Pointer = msg2;
		}
		lcd_xy(0,0); 
		
    }


    }
}

void copy(char *p, char copy1[16], char *msg){
	int i = 0;
	while(i<16){
		copy1[i] = *p;
		i++;
		p++;
		if(*p == NULL){
			p = msg;
		}
	}
}
//This function speeds up scrolling
int SpeedUp(int delayer){
	delayer = delayer / 1.5;
	return delayer;

}
//This function slows down scrolling
int SlowDown ( int delayer ){
	delayer = delayer * 1.5;
	return delayer;
}




