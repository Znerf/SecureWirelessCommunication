#ifndef F_CPU
//define cpu clock speed if not defined
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"


//set desired baud rate
#define BAUDRATE 1200
//calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)
//define receive parameters
#define SYNC 0XAA// synchro signal
#define RADDR 0x44

void USART_Init(void)
{
	//Set baud rate
	UBRRL=(uint8_t)UBRRVAL;     //low byte
	UBRRH=(UBRRVAL>>8);   //high byte
	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|
	(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);
	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSRB=(1<<TXEN);
}
void USART_vSendByte(uint8_t u8Data)
{
	// Wait if a byte is being transmitted
	while((UCSRA&(1<<UDRE)) == 0);
	// Transmit data
	UDR = u8Data;
}
void Send_Packet(uint8_t addr, uint8_t cmd)
{
	USART_vSendByte(SYNC);//send synchro byte
	USART_vSendByte(addr);//send receiver address
	USART_vSendByte(cmd);//send increment command
	USART_vSendByte((addr+cmd));//send checksum
}
void delayms(uint8_t t)//delay in ms
{
	uint8_t i;
	for(i=0;i<t;i++)
	_delay_ms(1);
}



int main(void)
{
	
	USART_Init();
	lcd_init();
	
	char str1[50] = "Defender";
	lcd_gotoxy(0,0);
	lcd_puts(str1);
	
	Send_Packet(RADDR, SYNC);
	//delayms(10);
	
	Send_Packet(RADDR, SYNC);
	//delayms(10);
	
	Send_Packet(RADDR, SYNC);
	//delayms(10);
	//send command to switch led ON
	int i=0;
	while(str1[i] != '\0'){
		Send_Packet(RADDR, str1[i]);
		//delayms(10);
		Send_Packet(RADDR, str1[i]);
		//delayms(10);
		Send_Packet(RADDR, str1[i]);
		//delayms(10);
		i++;
	}
	Send_Packet(RADDR, SYNC);
	//delayms(10);
	
	while (1)
	{//endless transmission
		//send command to switch led ON
		
	}
}


