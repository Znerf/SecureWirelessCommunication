/*
 * BlowfishSimplified.h
 *
 * Created: 9/07/2019 1:17:17 PM
 *  Author: Rajil
 */ 


#ifndef BLOWFISHSIMPLIFIED_H_
#define BLOWFISHSIMPLIFIED_H_

typedef struct blowfish_context_t_ {
	unsigned long pbox[3];
	unsigned long sbox[16];
} blowfish_context_t;

void blowfish_encryptblock (blowfish_context_t *ctx, unsigned long  *hi,  unsigned long *lo);
void blowfish_decryptblock (blowfish_context_t *ctx, unsigned long  *hi,  unsigned long *lo);
void blowfish_initiate     (blowfish_context_t *ctx, const char *keyparam,      unsigned int  keybytes);
void blowfish_clean        (blowfish_context_t *ctx);

extern const unsigned long ORIG_P[3];
extern const unsigned long ORIG_S[16];




#endif /* BLOWFISHSIMPLIFIED_H_ */