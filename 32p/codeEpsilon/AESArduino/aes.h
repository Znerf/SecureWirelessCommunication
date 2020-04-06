#ifndef AES
#define AES

// AES block size in bytes
#define AES_BLOCK_SIZE 16

// Total number of rounds for AES-128
#define AES_NUM_OF_ROUNDS 10

// Key schedule size
#define AES_KEY_SCHEDULE_SIZE ((AES_BLOCK_SIZE) * (AES_NUM_OF_ROUNDS + 1))

/* Status values */
typedef enum { STATUS_OKAY = 0, ERR_NO_KEY } aes_ret_status_t;


/*! \brief Performs key expansion to generate key schedule
 * from initial set of Key of 4 words.
 * \param round_key  Holds the pointer to store key schedule
 * \param key        Initial Key received from the user
 */
void KeyExpansion(uint8_t *round_key, uint8_t *key);

/*! \brief xtime calculation
 * The value is multiplied by 0x02. This
 * is implemented at the byte level as a
 * left shift and XOR with 0x1b if the MSB
 * of the value (before) shifting is 1.
 * \param value  Value for which multiplication to be done
 */
uint8_t xtime(uint8_t value);

/*! \brief Performs Sub Byte and Shift rows operation.
 * \param state  variable to store the intermediate result of the algorithm
 */
void SubBytes_ShiftRows(uint8_t *state);

/*! \brief Performs Mix Columns Operation
 * \param state  variable to store the intermediate result of the algorithm
 */
void MixColumnns(uint8_t *state);

/*! \brief Performs Add RoundKey  operation.
 * \param state  variable to store the intermediate result of the algorithm
 */
void AddRoundKey(uint8_t *state, uint8_t round);

/*! \brief Performs Inverse of sub bytes and shift rows operation
 * \param state  variable to store the intermediate result of the algorithm
 */
void Inv_ShiftRows_SubBytes(uint8_t *state);

/*! \brief Performs AddRound Key
 * \param state  variable to store the intermediate result of the algorithm
 * \note inverse of Add round key and Add round key are same.Two separate
 * functions are defined for ease of understanding the encryption and
 * decryption process.
 */
void Inv_AddRoundKey(uint8_t *state, uint8_t round);

/*! \brief Performs Inverse  Mix Columns operation
 * \param state  variable to store the intermediate result of the algorithm
 * \param Number of current ongoing round in AES algorithm
 */
void Inv_MixColumns(uint8_t *state, uint8_t round);

/*! \brief Performs Key Expansion
 * \param key  Initial Key vectors received from user
 */
void aes_init(uint8_t *key);

/*! \brief Performs AES-128 Encryption
 * \param plainText  Input plain text block
 * \param state Variable to store the intermediate result
 * \retval ERR_NO_KEY if key expansion is not done vis aes_init()
 * \retval STATUS_OKAY if aes_init() is called before this function.
 */
aes_ret_status_t aes_cipher(uint8_t *plainText, uint8_t *state);

/*! \brief Performs AES-128 Decryption
 * \param cipherText  Input cipher text block
 * \param state Variable to store the intermediate result
 * \retval ERR_NO_KEY if key expansion is not done vis aes_init()
 * \retval STATUS_OKAY if aes_init() is called before this function.
 */
aes_ret_status_t aes_inverse_cipher(uint8_t *cipherText, uint8_t *state);

/*! \brief This function performs CBC encryption on input plain text blocks.
 * \param plainText   pointer to the array containing the plain text bytes
 * \param cipherText  pointer to the array where the encrypted cipher output has to be stored
 * \param init_vector pointer to the array containing the unique initialization vector
 * \param size        size of the input plain text
 */
void cbc_encrypt(uint8_t *plainText, uint8_t *cipherText, uint8_t *init_vector, uint32_t size);

/*! \brief This function performs CBC decryption on input cipher text blocks.
 * \param cipherText  pointer to the array containing the cipher text bytes
 * \param plainText   pointer to the array where the decrypted plain text has to be stored
 * \param init_vector pointer to the array containing the unique initialization vector
 * \param size        size of the input plain text
 */
void cbc_decrypt(uint8_t *cipherText, uint8_t *plainText, uint8_t *init_vector, uint32_t size);

#endif
