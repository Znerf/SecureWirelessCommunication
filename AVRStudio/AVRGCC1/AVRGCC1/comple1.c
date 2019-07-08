#ifndef F_CPU
//define cpu clock speed if not defined
#define F_CPU 4000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//set desired baud rate
#define BAUDRATE 1200
//calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)
//define receive parameters
#define SYNC 0XAA// synchro signal
#define RADDR 0x44
#define LEDON 0x11//LED on command
#define LEDOFF 0x22//LED off command
void USART_Init(void)
{
    //Set baud rate
    UBRRL=(uint8_t)UBRRVAL;     //low byte
    UBRRH=(UBRRVAL>>8);   //high byte
    //Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
    UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|
        (0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0); 
    //Enable Transmitter and Receiver and Interrupt on receive complete
    UCSRB=(1<<RXEN)|(1<<RXCIE);//|(1<<TXEN);
    //enable global interrupts
}
uint8_t USART_vReceiveByte(void)
{
    // Wait until a byte has been received
    while((UCSRA&(1<<RXC)) == 0);
    // Return received data
    return UDR;
}
ISR(USART_RXC_vect)
{
    //define variables
    uint8_t raddress, data, chk;//transmitter address
    //receive destination address
    raddress=USART_vReceiveByte();
    //receive data
    data=USART_vReceiveByte();
    //receive checksum
    chk=USART_vReceiveByte();
    //compare received checksum with calculated
	PORTC=0xff;
    if(chk==(raddress+data))//if match perform operations
    {
		PORTC=0x00;
        //if transmitter address match
        if(raddress==RADDR)
            {
                if(data==LEDON)
                    {
                        PORTC=(1<<0);//LED ON
                    }
                else if(data==LEDOFF)
                    {
                        PORTC=(1<<1);//LED OFF
                    }
                else
                {
                    //blink led as error
                    PORTC=(1<<0);//LED OFF
                    _delay_ms(10);
                    PORTC=~(1<<1);//LED ON   
					_delay_ms(10);
                    
                }
            }
    }
}
void Main_Init(void)
{
    PORTC|=(1<<0);//LED OFF
    DDRC=0Xff;//define port C pin 0 as output;
    //enable global interrupts
    sei();
}
int main(void)
{
    Main_Init();
    USART_Init();
    while(1)
    {
    }
    //nothing here interrupts are working
    return 0;
}