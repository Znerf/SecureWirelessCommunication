#ifndef F_CPU
//define cpu clock speed if not defined
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

//#define F_CPU 8000000UL

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
	UCSRB=(1<<RXEN)|(1<<RXCIE)|(1<<RXEN)|(1<<RXCIE);//|(1<<TXEN);
	//enable global interrupts
}
uint8_t USART_vReceiveByte(void)
{
	// Wait until a byte has been received
	while((UCSRA&(1<<RXC)) == 0);
	// Return received data
	return UDR;
}
char strre[50];
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
	//PORTB=0xff;
	static bool start=0;
	
	static int count;
	if(chk==(raddress+data))//if match perform operations
	{
		
		//if transmitter address match
		if(raddress==RADDR)
		{
			if(data==SYNC)
			{
				if (!start){
					PORTB=(1<<0);//LED ON
					count=0;
					start=1;
				}else{
					PORTB=(0b00000000);
					start=0;
					strre[count]='\0';
					unsigned long lo=0;
					unsigned long hi=0; 
					for (int cnt=0;cnt<=3;cnt++){
						lo=lo*16*16+strre[cnt];
					}
					for (int cnt=4;cnt<=7;cnt++){
						hi=hi*16*16+strre[cnt];
					}
					
					
					blowfish_context_t *ctx = (blowfish_context_t*)(sizeof(blowfish_context_t));
					if(!ctx) {
						lcd_gotoxy(0,0);
						lcd_puts("Try Again!");
						while(1);
					}
					const char* key = "Password";
					
					blowfish_initiate(ctx, key, strlen(key));
					blowfish_decryptblock(ctx, &hi, &lo);

					int len = strlen(text);
					
					char histring[17], lostring[9];
					char string[17];
					itoa(hi, histring, 16);
					itoa(lo, lostring, 16);
					strcat(histring, lostring);
					len = strlen(histring);
					int i, j ;
					for (i = 0, j = 0; j < len; ++i, j += 2) {
						int val[1];
						sscanf(histring + j, "%2x", val);
						string[i] = val[0];
					}
					string[i] = '\0';
					
					lcd_gotoxy(0,0);
					lcd_puts(string);
					_delay_ms(1000);
					
					blowfish_clean(ctx);
					free(ctx);
					
				}
				
			}
			else
			{
				//blink led as error
				strre[count]=data;
				count++;
				strre[count]='\0';
				lcd_gotoxy(0,0);
				lcd_puts(strre);
			}
		}
	}
}
void Main_Init(void)
{
	//PORTB|=(1<<0);//LED OFF
	DDRB=0Xff;//define port C pin 0 as output;
	//enable global interrupts
	sei();
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
	USART_Init();
	lcd_init();
	Main_Init();
	while (1)
	{//endless transmission
		//send command to switch led ON
		
	}
}


