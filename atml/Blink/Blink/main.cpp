#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0xFF; //Nakes PORTC as Output
	while(1) //infinite loop
	{
		PORTB = 0xFF; //Turns ON All LEDs
		_delay_ms(1000); //1 second delay
		PORTB= 0x00; //Turns OFF All LEDs
		_delay_ms(1000); //1 second delay
		//avrdude -c arduino -P COM4 -b 19200 -p m32 -U flash:w:AVRGCC3.hex:i
		////avrdude -c arduino -P COM4 -b 19200 -p m32 -U hfuse:r:asd.hex:h
		//avrdude -c arduino -P COM4 -b 19200 -p m32 -U hfuse:w:0b11011001:m
	}
}