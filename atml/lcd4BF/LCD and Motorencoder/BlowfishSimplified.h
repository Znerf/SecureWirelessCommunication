/*
 * BlowfishSimplified.h
 *
 * Created: 9/07/2019 1:17:17 PM
 *  Author: Rajil
 */ 


#ifndef BLOWFISHSIMPLIFIED_H_
#define BLOWFISHSIMPLIFIED_H_

typedef struct blowfish_context_t_ {
	unsigned long pbox[256];
	unsigned long sbox[256];
} blowfish_context_t;

void blowfish_encryptblock (blowfish_context_t *ctx, unsigned long  *hi,  unsigned long *lo);
void blowfish_decryptblock (blowfish_context_t *ctx, unsigned long  *hi,  unsigned long *lo);
void blowfish_initiate     (blowfish_context_t *ctx, const char *keyparam,      unsigned int  keybytes);
void blowfish_clean        (blowfish_context_t *ctx);

extern const unsigned long ORIG_P[18];
extern const unsigned long ORIG_S[256];




#endif /* BLOWFISHSIMPLIFIED_H_ */