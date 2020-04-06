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
#define BLOWFISH_F(x) \
(((ctx->sbox[x >> 24] + ctx->sbox[(x >> 16) & 0xFF]) \
^ ctx->sbox[(x >> 8) & 0xFF]) + ctx->sbox[x & 0xFF])

const unsigned long ORIG_P[18] = {
	0x243F6A88, 0x85A308D3, 0x13198A2E, 0x03707344,
	0xA4093822, 0x299F31D0, 0x082EFA98, 0xEC4E6C89,
	0x452821E6, 0x38D01377, 0xBE5466CF, 0x34E90C6C,
	0xC0AC29B7, 0xC97C50DD, 0x3F84D5B5, 0xB5470917,
	0x9216D5D9, 0x8979FB1B
};

const unsigned long ORIG_S[256] = {
	    0xD1310BA6, 0x98DFB5AC, 0x2FFD72DB, 0xD01ADFB7,
		0xB8E1AFED, 0x6A267E96, 0xBA7C9045, 0xF12C7F99,
		0x24A19947, 0xB3916CF7, 0x0801F2E2, 0x858EFC16,
		0x636920D8, 0x71574E69, 0xA458FEA3, 0xF4933D7E,
		0x0D95748F, 0x728EB658, 0x718BCD58, 0x82154AEE,
		0x7B54A41D, 0xC25A59B5, 0x9C30D539, 0x2AF26013,
		0xC5D1B023, 0x286085F0, 0xCA417918, 0xB8DB38EF,
		0x8E79DCB0, 0x603A180E, 0x6C9E0E8B, 0xB01E8A3E,
		0xD71577C1, 0xBD314B27, 0x78AF2FDA, 0x55605C60,
		0xE65525F3, 0xAA55AB94, 0x57489862, 0x63E81440,
		0x55CA396A, 0x2AAB10B6, 0xB4CC5C34, 0x1141E8CE,
		0xA15486AF, 0x7C72E993, 0xB3EE1411, 0x636FBC2A,
		0x2BA9C55D, 0x741831F6, 0xCE5C3E16, 0x9B87931E,
		0xAFD6BA33, 0x6C24CF5C, 0x7A325381, 0x28958677,
		0x3B8F4898, 0x6B4BB9AF, 0xC4BFE81B, 0x66282193,
		0x61D809CC, 0xFB21A991, 0x487CAC60, 0x5DEC8032,
		0xEF845D5D, 0xE98575B1, 0xDC262302, 0xEB651B88,
		0x23893E81, 0xD396ACC5, 0x0F6D6FF3, 0x83F44239,
		0x2E0B4482, 0xA4842004, 0x69C8F04A, 0x9E1F9B5E,
		0x21C66842, 0xF6E96C9A, 0x670C9C61, 0xABD388F0,
		0x6A51A0D2, 0xD8542F68, 0x960FA728, 0xAB5133A3,
		0x6EEF0B6C, 0x137A3BE4, 0xBA3BF050, 0x7EFB2A98,
		0xA1F1651D, 0x39AF0176, 0x66CA593E, 0x82430E88,
		0x8CEE8619, 0x456F9FB4, 0x7D84A5C3, 0x3B8B5EBE,
		0xE06F75D8, 0x85C12073, 0x401A449F, 0x56C16AA6,
		0x4ED3AA62, 0x363F7706, 0x1BFEDF72, 0x429B023D,
		0x37D0D724, 0xD00A1248, 0xDB0FEAD3, 0x49F1C09B,
		0x075372C9, 0x80991B7B, 0x25D479D8, 0xF6E8DEF7,
		0xE3FE501A, 0xB6794C3B, 0x976CE0BD, 0x04C006BA,
		0xC1A94FB6, 0x409F60C4, 0x5E5C9EC2, 0x196A2463,
		0x68FB6FAF, 0x3E6C53B5, 0x1339B2EB, 0x3B52EC6F,
		0x6DFC511F, 0x9B30952C, 0xCC814544, 0xAF5EBD09,
		0xBEE3D004, 0xDE334AFD, 0x660F2807, 0x192E4BB3,
		0xC0CBA857, 0x45C8740F, 0xD20B5F39, 0xB9D3FBDB,
		0x5579C0BD, 0x1A60320A, 0xD6A100C6, 0x402C7279,
		0x679F25FE, 0xFB1FA3CC, 0x8EA5E9F8, 0xDB3222F8,
		0x3C7516DF, 0xFD616B15, 0x2F501EC8, 0xAD0552AB,
		0x323DB5FA, 0xFD238760, 0x53317B48, 0x3E00DF82,
		0x9E5C57BB, 0xCA6F8CA0, 0x1A87562E, 0xDF1769DB,
		0xD542A8F6, 0x287EFFC3, 0xAC6732C6, 0x8C4F5573,
		0x695B27B0, 0xBBCA58C8, 0xE1FFA35D, 0xB8F011A0,
		0x10FA3D98, 0xFD2183B8, 0x4AFCB56C, 0x2DD1D35B,
		0x9A53E479, 0xB6F84565, 0xD28E49BC, 0x4BFB9790,
		0xE1DDF2DA, 0xA4CB7E33, 0x62FB1341, 0xCEE4C6E8,
		0xEF20CADA, 0x36774C01, 0xD07E9EFE, 0x2BF11FB4,
		0x95DBDA4D, 0xAE909198, 0xEAAD8E71, 0x6B93D5A0,
		0xD08ED1D0, 0xAFC725E0, 0x8E3C5B2F, 0x8E7594B7,
		0x8FF6E2FB, 0xF2122B64, 0x8888B812, 0x900DF01C,
		0x4FAD5EA0, 0x688FC31C, 0xD1CFF191, 0xB3A8C1AD,
		0x2F2F2218, 0xBE0E1777, 0xEA752DFE, 0x8B021FA1,
		0xE5A0CC0F, 0xB56F74E8, 0x18ACF3D6, 0xCE89E299,
		0xB4A84FE0, 0xFD13E0B7, 0x7CC43B81, 0xD2ADA8D9,
		0x165FA266, 0x80957705, 0x93CC7314, 0x211A1477,
		0xE6AD2065, 0x77B5FA86, 0xC75442F5, 0xFB9D35CF,
		0xEBCDAF0C, 0x7B3E89A0, 0xD6411BD3, 0xAE1E7E49,
		0x00250E2D, 0x2071B35E, 0x226800BB, 0x57B8E0AF,
		0x2464369B, 0xF009B91E, 0x5563911D, 0x59DFA6AA,
		0x78C14389, 0xD95A537F, 0x207D5BA2, 0x02E5B9C5,
		0x83260376, 0x6295CFA9, 0x11C81968, 0x4E734A41,
		0xB3472DCA, 0x7B14A94A, 0x1B510052, 0x9A532915,
		0xD60F573F, 0xBC9BC6E4, 0x2B60A476, 0x81E67400,
		0x08BA6FB5, 0x571BE91F, 0xF296EC6B, 0x2A0DD915,
		0xB6636521, 0xE7B9F9B6, 0xFF34052E, 0xC5855664,
	0x53B02D5D, 0xA99F8FA1, 0x08BA4799, 0x6E85076A   };
	


void blowfish_encryptblock(blowfish_context_t *ctx, unsigned long *hi, unsigned long *lo)
{
	int i, temp;

	for(i = 0; i < 16; i++) {
		*hi ^= ctx->pbox[i];
		*lo ^= BLOWFISH_F(*hi);
		temp = *hi, *hi = *lo, *lo = temp;
	}
	temp = *hi, *hi = *lo, *lo = temp;

	*lo ^= ctx->pbox[16];
	*hi ^= ctx->pbox[17];
}

void blowfish_decryptblock(blowfish_context_t *ctx, unsigned long *hi, unsigned long *lo)
{
	int i, temp;

	for(i = 17; i > 1; i--) {
		*hi ^= ctx->pbox[i];
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
				calc <<= 8, calc |= key[j++];
				if(j == keybytes)
				j = 0;
			}
			ctx->pbox[i] ^= calc;
		}
	}

	unsigned long hi = 0, lo = 0;

	for(i = 0; i < 18; i += 2) {
		blowfish_encryptblock(ctx, &hi, &lo);
		ctx->pbox[i] = hi;
		ctx->pbox[i + 1] = lo;
	}

//	for(i = 0; i < 1; i++)
		for(j = 0; j < 256; j += 2) {
			blowfish_encryptblock(ctx, &hi, &lo);
			ctx->sbox[j] = hi;
			ctx->sbox[j + 1] = lo;
		}
	}
//}

void blowfish_clean(blowfish_context_t *ctx)
{
	memset(ctx, 0, sizeof(blowfish_context_t));
}


int main(void)
{
	
	lcd_init();
	char str1[50] = "Defsender";
	lcd_gotoxy(0,0);
	lcd_puts(str1);
	
	blowfish_context_t *ctx = (blowfish_context_t*)(sizeof(blowfish_context_t));
	if(!ctx) {
		
		char str1[50] = "Try another";
		lcd_gotoxy(0,0);
		lcd_puts(str1);
		while(1);
	}
	const char* key = "Password";
	blowfish_initiate(ctx, key, strlen(key));
	
	
	const char * text = "Defender"; //hex(space) = 20H;
	////int len = strlen(text);

	////char hex[100];

	// Convert text to hex.
	//for (int i = 0, j = 0; i < len; ++i, j += 2)
	//sprintf(hex + j, "%02x", text[i] & 0xff);

	//printf("'%s' in hex is %s.\n", text, hex);

	unsigned long hi = 0x44656665L, lo = 0x6e646572L;
	
	//unsigned long a=hi;
	//	char convertedStr[50];
	
	
	blowfish_encryptblock(ctx, &hi, &lo);
	//printf("Encoded: %08lX %08lX\n", hi, lo);


	blowfish_decryptblock(ctx, &hi, &lo);
	//if((hi == 0x44656665L) && (lo == 0x6e646572L))
	//puts("Decryption Test Passed");

	   int len = strlen(text);

	   //unsigned long hi = 0x44656665L, lo = 0x6e646572L;
	   char histring[17], lostring[9];
	   char hex[17];
	   char string[17];
	   itoa(hi, histring, 16);
	   itoa(lo, lostring, 16);
	   strcat(histring, lostring);
	   strcpy(hex, histring);
	   
	   len = strlen(hex);
	   int i, j ;
	   for (i = 0, j = 0; j < len; ++i, j += 2) {
		   int val[1];
		   sscanf(hex + j, "%2x", val);
		   string[i] = val[0];
	   }
	   string[i] = '\0';
	   lcd_gotoxy(0,0);
	   lcd_puts(string);
	   
	blowfish_clean(ctx);
	free(ctx);
		
	
	
	
	while (1)
	{//endless transmission
		//send command to switch led ON
		
	}
}


