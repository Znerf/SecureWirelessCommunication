#ifndef F_CPU
//define cpu clock speed if not defined
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BlowfishSimplified.h"


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


#define BLOWFISH_F(x) \
(((ctx->sbox[(x >> 24) % sbox_size] + ctx->sbox[((x >> 16) & 0xFF) % sbox_size]) \
^ ctx->sbox[((x >> 8) & 0xFF) % sbox_size]) + ctx->sbox[(x & 0xFF) % sbox_size])

const unsigned long ORIG_P[3] = {
0x243F6A88, 0x85A308D3, 0x13198A2E };

const unsigned long ORIG_S[16] = {
	0xD1310BA6, 0x98DFB5AC, 0x2FFD72DB, 0xD01ADFB7,
	0xB8E1AFED, 0x6A267E96, 0xBA7C9045, 0xF12C7F99,
	0x24A19947, 0xB3916CF7, 0x0801F2E2, 0x858EFC16,
	0x636920D8, 0x71574E69, 0xA458FEA3, 0xF4933D7E
};

const unsigned int pbox_size = 3;
const unsigned int sbox_size = 16;


void blowfish_encryptblock(blowfish_context_t *ctx, unsigned long *hi, unsigned long *lo)
{
	int i, temp;

	for(i = 0; i < 16; i++) {
		*hi ^= ctx->pbox[i % pbox_size];
		*lo ^= BLOWFISH_F(*hi);
		temp = *hi, *hi = *lo, *lo = temp;
	}
	temp = *hi, *hi = *lo, *lo = temp;

	*lo ^= ctx->pbox[16 % pbox_size];
	*hi ^= ctx->pbox[17 % pbox_size];
}

void blowfish_decryptblock(blowfish_context_t *ctx, unsigned long *hi, unsigned long *lo)
{
	int i, temp;

	for(i = 17; i > 1; i--) {
		*hi ^= ctx->pbox[i % pbox_size];
		*lo ^= BLOWFISH_F(*hi);
		temp = *hi, *hi = *lo, *lo = temp;
	}
	temp = *hi, *hi = *lo, *lo = temp;

	*lo ^= ctx->pbox[1];
	*hi ^= ctx->pbox[0];
}

void blowfish_initiate(blowfish_context_t *ctx, const char *keyparam, unsigned int keybytes)
{
	keybytes %= 57;
	const char *key = keyparam;
	int i, k;
	unsigned int j;
	unsigned long calc;

	//for(i = 0; i < 1; i++)
	memcpy(ctx->sbox, ORIG_S, sizeof(ORIG_S));

	memcpy(ctx->pbox, ORIG_P, sizeof(ORIG_P));

	
	if(keybytes) {
		for(i = 0, j = 0; i < 18; i++) {
			for(k = 0, calc = 0; k < 4; k++) {
				calc <<= 8;
				calc |= key[j++];
				if(j == keybytes)
				j = 0;
			}
			ctx->pbox[i % pbox_size] ^= calc;
		}
	}

	unsigned long hi = 0, lo = 0;
	
	

	for(i = 0; i < 18; i += 2) {
		blowfish_encryptblock(ctx, &hi, &lo);
		ctx->pbox[i % pbox_size] = hi;
		ctx->pbox[(i + 1) % pbox_size] = lo;
	}

	for(j = 0; j < 256; j += 2) {
		blowfish_encryptblock(ctx, &hi, &lo);
		ctx->sbox[j % sbox_size] = hi;
		ctx->sbox[(j + 1) % sbox_size] = lo;
	}

	
}


void blowfish_clean(blowfish_context_t *ctx)
{
	memset(ctx, 0, sizeof(blowfish_context_t));
}



int main(void)
{
	
	char histring[17], lostring[9];
	char string[17];
	unsigned int len;
	
	USART_Init();
	lcd_init();
	char str1[50] = "Defender";
	lcd_gotoxy(0,0);
	lcd_puts(str1);
	
	blowfish_context_t *ctx = (blowfish_context_t*)(sizeof(blowfish_context_t));
	if(!ctx) {
		lcd_gotoxy(0,0);
		lcd_puts("Try Again!");
		while(1);
	}
	const char* key = "Password";
	
	blowfish_initiate(ctx, key, strlen(key));
	
	lcd_gotoxy(0,0);
	lcd_puts("Initiated.");
	
	const char * text = "Defender"; //hex(space) = 20H;

	unsigned long hi = 0x44656665L, lo = 0x6e646572L;
	
	
	blowfish_encryptblock(ctx, &hi, &lo);

	itoa(hi, histring, 16);
	itoa(lo, lostring, 16);
	strcat(histring, lostring);
	len = strlen(histring);
	unsigned int  ik, jk ;
	for (ik = 0, jk = 0; jk < len; ++ik, jk += 2) {
		int val[1];
		sscanf(histring + jk, "%2x", val);
		string[ik] = val[0];
	}
	string[ik] = '\0';
	
	blowfish_clean(ctx);
	free(ctx);
	
	
	Send_Packet(RADDR, SYNC);
	//delayms(10);
	printf("sdfdf");
	int i=0;
	while(str1[i] != '\0'){
		Send_Packet(RADDR, string[i]);
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


