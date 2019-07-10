#define F_CPU 8000000UL  // 8 MHz
#include <avr/io.h>     
// For AVR registers - change device in Project - Configuration Options
#include <util/delay.h>  
// for delay routine

/* Read Keypad on PortD and output binary result on PortB LEDs
   Keypad has 7 pins - 3 columns and 4 rows. When a key is pressed, Column and
   row goes low. To read it, set columns as inputs (bit 0..2) and wait for 0 
	 on these bits. When column goes low, add to value (+0, +1 or +2) and then 
	 set port to read rows. Read row and add 1, 4, 7 or A to value depending on 
	 row Output value on LED - after inverting it (0 = LED on) output is 
	 keypad.hex in default folder */



void Col_init(void)
{
   DDRA = 0xF8;  // bit 0.2 are columns (0 is input)
   PORTA = 0x07;  //pullups on these bits 
   //very short delay
    asm volatile ("nop");  
    asm volatile ("nop"); 
}

void Row_init(void)
{
    DDRA = 0x87;  // bit 3..6 used for rows
    PORTA = 0x78;  //pullups on these bits 
    asm volatile ("nop");
    asm volatile ("nop");

}

unsigned char Read_key(void)
{  
  unsigned char value;			
  
  Col_init();      //set up columns to read
 
   value =0;       // init value
  		// read columns first - 0 if key in that column pressed
       if (!(PINA & 0x01)) 
			    {value = 1; }// Col2 = bit0 is low
       else if (!(PINA & 0x02)) 
			    {value = 2;}   // Col1 = bit1 is low
       else if (!(PINA & 0x04)) 
			    {value = 3; }	// Col0 = bit2 is low 

       Row_init();   //set up rows to read
				
						
       if (!(PINA & 0x08)) 
			    {	PORTD=0xf0;
					value += 0x0A; }
       else if (!(PINA & 0x10)) 
			    { value += 0x07; } //row2 = bit 4 is low
       else if (!(PINA & 0x20)) 
			    { value += 0x04; } // row1 = bit 5 is low
       else if (!(PINA & 0x40)) 
			    { value += 0x01; } //row0 = bit 6 is low      
	    
	   _delay_ms(100); // switch debounce 
	   Col_init();
	   return value;  // value is sum of row and column   
}

int main( void )
{    
	unsigned char  led;
	    
    
	  led = 0x00;      // init variable
	  Col_init();      // set keypad column read
		DDRC=0xff;DDRD=0xff;
		PORTC=0x00;PORTD=0x00;
    while(1)        // Eternal loop
    {          
	    if (!(PINA == 0x07)) {      
// if a column is pressed bit 0,1 or 2 will go low active 0
	        
			led= Read_key();   // read keypad
			if (led==0x01) PORTC=0x01;
			if (led==0x02) PORTC=0x02;
			if (led==0x03) PORTC=0x03;
			if (led==0x04) PORTC=0x04;
			if (led==0x05) PORTC=0x05;;
			if (led==0x06) PORTC=0x06;;
		} 
	       // display result        
    }
}