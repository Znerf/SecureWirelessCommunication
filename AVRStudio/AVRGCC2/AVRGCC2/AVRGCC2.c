

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "ps2_kb.h"




						 
extern void WritePage (uint16_t page, const uint8_t *buf);

static void avr_init(void);



/********************** MAIN ***********************/

int main(void)
{
	_delay_ms(100);    //some initial delay
	avr_init();
	
    while(1)
	{		
		

	}
    return(0);
}



/* initialise lcd and keyboard, enable interrupts */
static void avr_init(void)
{
    
	KbInit();
	sei(); //enable global interrupts
    return;
}
