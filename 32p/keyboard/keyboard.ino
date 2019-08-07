/*  PS2Keyboard library, International Keyboard Layout Example
    http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html

    keyboard.begin() accepts an optional 3rd parameter to
    configure the PS2 keyboard layout.  Uncomment the line for
    your keyboard.  If it doesn't exist, you can create it in
    PS2Keyboard.cpp and email paul@pjrc.com to have it included
    in future versions of this library.
*/
   
//#include <PS2Keyboard.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h" // for attachInterrupt, FALLING
#else
#include "WProgram.h"
#endif

//#include "utility/int_pins.h"

// Every call to read() returns a single byte for each
// keystroke.  These configure what byte will be returned
// for each "special" key.  To ignore a key, use zero.
#define PS2_TAB        9
#define PS2_ENTER     13
#define PS2_BACKSPACE     127
#define PS2_ESC       27
#define PS2_INSERT      0
#define PS2_DELETE      127
#define PS2_HOME      0
#define PS2_END       0
#define PS2_PAGEUP      25
#define PS2_PAGEDOWN      26
#define PS2_UPARROW     11
#define PS2_LEFTARROW     8
#define PS2_DOWNARROW     10
#define PS2_RIGHTARROW      21
#define PS2_F1        0
#define PS2_F2        0
#define PS2_F3        0
#define PS2_F4        0
#define PS2_F5        0
#define PS2_F6        0
#define PS2_F7        0
#define PS2_F8        0
#define PS2_F9        0
#define PS2_F10       0
#define PS2_F11       0
#define PS2_F12       0
#define PS2_SCROLL      0
#define PS2_EURO_SIGN     0

#define PS2_INVERTED_EXCLAMATION  161 // ¡
#define PS2_CENT_SIGN     162 // ¢
#define PS2_POUND_SIGN      163 // £
#define PS2_CURRENCY_SIGN   164 // ¤
#define PS2_YEN_SIGN      165 // ¥
#define PS2_BROKEN_BAR                  166 // ¦
#define PS2_SECTION_SIGN    167 // §
#define PS2_DIAERESIS     168 // ¨
#define PS2_COPYRIGHT_SIGN    169 // ©
#define PS2_FEMININE_ORDINAL    170 // ª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE 171 // «
#define PS2_NOT_SIGN      172 // ¬
#define PS2_HYPHEN      173
#define PS2_REGISTERED_SIGN   174 // ®
#define PS2_MACRON      175 // ¯
#define PS2_DEGREE_SIGN     176 // °
#define PS2_PLUS_MINUS_SIGN   177 // ±
#define PS2_SUPERSCRIPT_TWO   178 // ²
#define PS2_SUPERSCRIPT_THREE   179 // ³
#define PS2_ACUTE_ACCENT    180 // ´
#define PS2_MICRO_SIGN      181 // µ
#define PS2_PILCROW_SIGN    182 // ¶
#define PS2_MIDDLE_DOT      183 // ·
#define PS2_CEDILLA     184 // ¸
#define PS2_SUPERSCRIPT_ONE   185 // ¹
#define PS2_MASCULINE_ORDINAL   186 // º
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE  187 // »
#define PS2_FRACTION_ONE_QUARTER  188 // ¼
#define PS2_FRACTION_ONE_HALF   189 // ½
#define PS2_FRACTION_THREE_QUARTERS 190 // ¾
#define PS2_INVERTED_QUESTION_MARK  191 // ¿
#define PS2_A_GRAVE     192 // À
#define PS2_A_ACUTE     193 // Á
#define PS2_A_CIRCUMFLEX    194 // Â
#define PS2_A_TILDE     195 // Ã
#define PS2_A_DIAERESIS     196 // Ä
#define PS2_A_RING_ABOVE    197 // Å
#define PS2_AE        198 // Æ
#define PS2_C_CEDILLA     199 // Ç
#define PS2_E_GRAVE     200 // È
#define PS2_E_ACUTE     201 // É
#define PS2_E_CIRCUMFLEX    202 // Ê
#define PS2_E_DIAERESIS     203 // Ë
#define PS2_I_GRAVE     204 // Ì
#define PS2_I_ACUTE     205 // Í
#define PS2_I_CIRCUMFLEX    206 // Î
#define PS2_I_DIAERESIS     207 // Ï
#define PS2_ETH       208 // Ð
#define PS2_N_TILDE     209 // Ñ
#define PS2_O_GRAVE     210 // Ò
#define PS2_O_ACUTE     211 // Ó
#define PS2_O_CIRCUMFLEX    212 // Ô
#define PS2_O_TILDE     213 // Õ
#define PS2_O_DIAERESIS     214 // Ö
#define PS2_MULTIPLICATION    215 // ×
#define PS2_O_STROKE      216 // Ø
#define PS2_U_GRAVE     217 // Ù
#define PS2_U_ACUTE     218 // Ú
#define PS2_U_CIRCUMFLEX    219 // Û
#define PS2_U_DIAERESIS     220 // Ü
#define PS2_Y_ACUTE     221 // Ý
#define PS2_THORN     222 // Þ
#define PS2_SHARP_S     223 // ß
#define PS2_a_GRAVE     224 // à
#define PS2_a_ACUTE     225 // á
#define PS2_a_CIRCUMFLEX    226 // â
#define PS2_a_TILDE     227 // ã
#define PS2_a_DIAERESIS     228 // ä
#define PS2_a_RING_ABOVE    229 // å
#define PS2_ae        230 // æ
#define PS2_c_CEDILLA     231 // ç
#define PS2_e_GRAVE     232 // è
#define PS2_e_ACUTE     233 // é
#define PS2_e_CIRCUMFLEX    234 // ê
#define PS2_e_DIAERESIS     235 // ë
#define PS2_i_GRAVE     236 // ì
#define PS2_i_ACUTE     237 // í
#define PS2_i_CIRCUMFLEX    238 // î
#define PS2_i_DIAERESIS     239 // ï
#define PS2_eth       240 // ð
#define PS2_n_TILDE     241 // ñ
#define PS2_o_GRAVE     242 // ò
#define PS2_o_ACUTE     243 // ó
#define PS2_o_CIRCUMFLEX    244 // ô
#define PS2_o_TILDE     245 // õ
#define PS2_o_DIAERESIS     246 // ö
#define PS2_DIVISION      247 // ÷
#define PS2_o_STROKE      248 // ø
#define PS2_u_GRAVE     249 // ù
#define PS2_u_ACUTE     250 // ú
#define PS2_u_CIRCUMFLEX    251 // û
#define PS2_u_DIAERESIS     252 // ü
#define PS2_y_ACUTE     253 // ý
#define PS2_thorn     254 // þ
#define PS2_y_DIAERESIS     255 // ÿ


#define PS2_KEYMAP_SIZE 136

typedef struct {
  uint8_t noshift[PS2_KEYMAP_SIZE];
  uint8_t shift[PS2_KEYMAP_SIZE];
  unsigned int uses_altgr;
    /*
     * "uint8_t uses_altgr;" makes the ESP8266 - NodeMCU modules crash.
     * So, I replaced it with an int and... It works!
     * I think it's because of the 32-bit architecture of the ESP8266
     * and the use of the flash memory to store the keymaps.
     * Maybe I'm wrong, it remains a hypothesis.
     */
  uint8_t altgr[PS2_KEYMAP_SIZE];
} PS2Keymap_t;


extern const PROGMEM PS2Keymap_t PS2Keymap_US;

class PS2Keyboard {
  public:
    PS2Keyboard();
    static void begin(uint8_t dataPin, uint8_t irq_pin, const PS2Keymap_t &map = PS2Keymap_US);
    static bool available();
    static void clear();
    static uint8_t readScanCode(void);
    static int read();
    static int readUnicode();
};

//inrpin
#if !defined(CORE_INT0_PIN) && !defined(CORE_INT1_PIN) && !defined(CORE_INT2_PIN)&& !defined(CORE_INT3_PIN)

// Arduino Mega
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // Arduino Mega
  #define CORE_INT0_PIN  2
  #define CORE_INT1_PIN  3
  #define CORE_INT2_PIN  21
  #define CORE_INT3_PIN  20
  #define CORE_INT4_PIN  19
  #define CORE_INT5_PIN  18

// Arduino Due (untested)
#elif defined(__SAM3X8E__) 
  #define CORE_INT_EVERY_PIN
  #ifndef PROGMEM
  #define PROGMEM
  #endif
  #ifndef pgm_read_byte
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #endif

// Arduino Leonardo (untested)
#elif defined(__AVR_ATmega32U4__) && !defined(CORE_TEENSY)
  #define CORE_INT0_PIN  3
  #define CORE_INT1_PIN  2
  #define CORE_INT2_PIN  0
  #define CORE_INT3_PIN  1

// Sanguino (untested)
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) // Sanguino
  #define CORE_INT0_PIN  10
  #define CORE_INT1_PIN  11
  #define CORE_INT2_PIN  2

// Chipkit Uno32 (untested)
#elif defined(__PIC32MX__) && defined(_BOARD_UNO_)
  #define CORE_INT0_PIN  38
  #define CORE_INT1_PIN  2
  #define CORE_INT2_PIN  7
  #define CORE_INT3_PIN  8
  #define CORE_INT4_PIN  35

// Chipkit Mega32 (untested)
#elif defined(__PIC32MX__) && defined(_BOARD_MEGA_)
  #define CORE_INT0_PIN  3
  #define CORE_INT1_PIN  2
  #define CORE_INT2_PIN  7
  #define CORE_INT3_PIN  21
  #define CORE_INT4_PIN  20

// http://hlt.media.mit.edu/?p=1229
#elif defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define CORE_INT0_PIN  2

#elif defined(__AVR_ATtiny84__)
  #define CORE_INT0_PIN  8

//ESP8266 - NodeMCU 0.9 (ESP-12 Module) / NodeMCU 1.0 (ESP-12E Module)
#elif defined(ESP8266)
  #define CORE_INT_EVERY_PIN

//ESP32 support
#elif defined(ESP32)
  #define CORE_INT_EVERY_PIN

//STM32F1 support
#elif defined(__STM32F1__)
  #define CORE_INT_EVERY_PIN

// Arduino Uno, Duemilanove, LilyPad, Mini, Fio, etc...
#else
  #define CORE_INT0_PIN  2
  #define CORE_INT1_PIN  3

#endif
#endif
//dfsfd

#define BUFFER_SIZE 45
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;
static uint8_t DataPin;
static uint8_t CharBuffer=0;
static uint8_t UTF8next=0;
static const PS2Keymap_t *keymap=NULL;

// The ISR for the external interrupt
void ps2interrupt(void)
{
  static uint8_t bitcount=0;
  static uint8_t incoming=0;
  static uint32_t prev_ms=0;
  uint32_t now_ms;
  uint8_t n, val;

  val = digitalRead(DataPin);
  now_ms = millis();
  if (now_ms - prev_ms > 250) {
    bitcount = 0;
    incoming = 0;
  }
  prev_ms = now_ms;
  n = bitcount - 1;
  if (n <= 7) {
    incoming |= (val << n);
  }
  bitcount++;
  if (bitcount == 11) {
    uint8_t i = head + 1;
    if (i >= BUFFER_SIZE) i = 0;
    if (i != tail) {
      buffer[i] = incoming;
      head = i;
    }
    bitcount = 0;
    incoming = 0;
  }
}

static inline uint8_t get_scan_code(void)
{
  uint8_t c, i;

  i = tail;
  if (i == head) return 0;
  i++;
  if (i >= BUFFER_SIZE) i = 0;
  c = buffer[i];
  tail = i;
  return c;
}

// These arrays provide a simple key map, to turn scan codes into ISO8859
// output.  If a non-US keyboard is used, these may need to be modified
// for the desired output.
//

const PROGMEM PS2Keymap_t PS2Keymap_US = {
  // without shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
  0, 0, 'z', 's', 'a', 'w', '2', 0,
  0, 'c', 'x', 'd', 'e', '4', '3', 0,
  0, ' ', 'v', 'f', 't', 'r', '5', 0,
  0, 'n', 'b', 'h', 'g', 'y', '6', 0,
  0, 0, 'm', 'j', 'u', '7', '8', 0,
  0, ',', 'k', 'i', 'o', '0', '9', 0,
  0, '.', '/', 'l', ';', 'p', '-', 0,
  0, 0, '\'', 0, '[', '=', 0, 0,
  0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '\\', 0, 0,
  0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  // with shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '~', 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
  0, 0, 'Z', 'S', 'A', 'W', '@', 0,
  0, 'C', 'X', 'D', 'E', '$', '#', 0,
  0, ' ', 'V', 'F', 'T', 'R', '%', 0,
  0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
  0, 0, 'M', 'J', 'U', '&', '*', 0,
  0, '<', 'K', 'I', 'O', ')', '(', 0,
  0, '>', '?', 'L', ':', 'P', '_', 0,
  0, 0, '"', 0, '{', '+', 0, 0,
  0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '|', 0, 0,
  0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  0
};



const PROGMEM PS2Keymap_t PS2Keymap_UK = {
// https://github.com/PaulStoffregen/PS2Keyboard/issues/15
  // without shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
  0, 0, 'z', 's', 'a', 'w', '2', 0,
  0, 'c', 'x', 'd', 'e', '4', '3', 0,
  0, ' ', 'v', 'f', 't', 'r', '5', 0,
  0, 'n', 'b', 'h', 'g', 'y', '6', 0,
  0, 0, 'm', 'j', 'u', '7', '8', 0,
  0, ',', 'k', 'i', 'o', '0', '9', 0,
  0, '.', '/', 'l', ';', 'p', '-', 0,
  0, 0, '\'', 0, '[', '=', 0, 0,
  0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '#', 0, 0,
  0, '\\', 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  // with shift
  {0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
  0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, 172 /* ¬ */, 0,
  0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
  0, 0, 'Z', 'S', 'A', 'W', '"', 0,
  0, 'C', 'X', 'D', 'E', '$', 163 /* £ */, 0,
  0, ' ', 'V', 'F', 'T', 'R', '%', 0,
  0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
  0, 0, 'M', 'J', 'U', '&', '*', 0,
  0, '<', 'K', 'I', 'O', ')', '(', 0,
  0, '>', '?', 'L', ':', 'P', '_', 0,
  0, 0, '@', 0, '{', '+', 0, 0,
  0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '~', 0, 0,
  0, '|', 0, 0, 0, 0, PS2_BACKSPACE, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
  PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
  0, 0, 0, PS2_F7 },
  0
};

#define BREAK     0x01
#define MODIFIER  0x02
#define SHIFT_L   0x04
#define SHIFT_R   0x08
#define ALTGR     0x10

static char get_iso8859_code(void)
{
  static uint8_t state=0;
  uint8_t s;
  char c;

  while (1) {
    s = get_scan_code();
    if (!s) return 0;
    if (s == 0xF0) {
      state |= BREAK;
    } else if (s == 0xE0) {
      state |= MODIFIER;
    } else {
      if (state & BREAK) {
        if (s == 0x12) {
          state &= ~SHIFT_L;
        } else if (s == 0x59) {
          state &= ~SHIFT_R;
        } else if (s == 0x11 && (state & MODIFIER)) {
          state &= ~ALTGR;
        }
        // CTRL, ALT & WIN keys could be added
        // but is that really worth the overhead?
        state &= ~(BREAK | MODIFIER);
        continue;
      }
      if (s == 0x12) {
        state |= SHIFT_L;
        continue;
      } else if (s == 0x59) {
        state |= SHIFT_R;
        continue;
      } else if (s == 0x11 && (state & MODIFIER)) {
        state |= ALTGR;
      }
      c = 0;
      if (state & MODIFIER) {
        switch (s) {
          case 0x70: c = PS2_INSERT;      break;
          case 0x6C: c = PS2_HOME;        break;
          case 0x7D: c = PS2_PAGEUP;      break;
          case 0x71: c = PS2_DELETE;      break;
          case 0x69: c = PS2_END;         break;
          case 0x7A: c = PS2_PAGEDOWN;    break;
          case 0x75: c = PS2_UPARROW;     break;
          case 0x6B: c = PS2_LEFTARROW;   break;
          case 0x72: c = PS2_DOWNARROW;   break;
          case 0x74: c = PS2_RIGHTARROW;  break;
          case 0x4A: c = '/';             break;
          case 0x5A: c = PS2_ENTER;       break;
          default: break;
        }
      } else if ((state & ALTGR) && keymap->uses_altgr) {
        if (s < PS2_KEYMAP_SIZE)
          c = pgm_read_byte(keymap->altgr + s);
      } else if (state & (SHIFT_L | SHIFT_R)) {
        if (s < PS2_KEYMAP_SIZE)
          c = pgm_read_byte(keymap->shift + s);
      } else {
        if (s < PS2_KEYMAP_SIZE)
          c = pgm_read_byte(keymap->noshift + s);
      }
      state &= ~(BREAK | MODIFIER);
      if (c) return c;
    }
  }
}

bool PS2Keyboard::available() {
  if (CharBuffer || UTF8next) return true;
  CharBuffer = get_iso8859_code();
  if (CharBuffer) return true;
  return false;
}

void PS2Keyboard::clear() {
  CharBuffer = 0;
  UTF8next = 0;
}

uint8_t PS2Keyboard::readScanCode(void)
{
  return get_scan_code();
}

int PS2Keyboard::read() {
  uint8_t result;

  result = UTF8next;
  if (result) {
    UTF8next = 0;
  } else {
    result = CharBuffer;
    if (result) {
      CharBuffer = 0;
    } else {
      result = get_iso8859_code();
    }
    if (result >= 128) {
      UTF8next = (result & 0x3F) | 0x80;
      result = ((result >> 6) & 0x1F) | 0xC0;
    }
  }
  if (!result) return -1;
  return result;
}

int PS2Keyboard::readUnicode() {
  int result;

  result = CharBuffer;
  if (!result) result = get_iso8859_code();
  if (!result) return -1;
  UTF8next = 0;
  CharBuffer = 0;
  return result;
}

PS2Keyboard::PS2Keyboard() {
  // nothing to do here, begin() does it all
}

void PS2Keyboard::begin(uint8_t data_pin, uint8_t irq_pin, const PS2Keymap_t &map) {
  uint8_t irq_num=255;

  DataPin = data_pin;
  keymap = &map;

  // initialize the pins
#ifdef INPUT_PULLUP
  pinMode(irq_pin, INPUT_PULLUP);
  pinMode(data_pin, INPUT_PULLUP);
#else
  pinMode(irq_pin, INPUT);
  digitalWrite(irq_pin, HIGH);
  pinMode(data_pin, INPUT);
  digitalWrite(data_pin, HIGH);
#endif

#ifdef CORE_INT_EVERY_PIN
  irq_num = irq_pin;

#else
  switch(irq_pin) {
    #ifdef CORE_INT0_PIN
    case CORE_INT0_PIN:
      irq_num = 0;
      break;
    #endif
    #ifdef CORE_INT1_PIN
    case CORE_INT1_PIN:
      irq_num = 1;
      break;
    #endif
    #ifdef CORE_INT2_PIN
    case CORE_INT2_PIN:
      irq_num = 2;
      break;
    #endif
    #ifdef CORE_INT3_PIN
    case CORE_INT3_PIN:
      irq_num = 3;
      break;
    #endif
    #ifdef CORE_INT4_PIN
    case CORE_INT4_PIN:
      irq_num = 4;
      break;
    #endif
    #ifdef CORE_INT5_PIN
    case CORE_INT5_PIN:
      irq_num = 5;
      break;
    #endif
    #ifdef CORE_INT6_PIN
    case CORE_INT6_PIN:
      irq_num = 6;
      break;
    #endif
    #ifdef CORE_INT7_PIN
    case CORE_INT7_PIN:
      irq_num = 7;
      break;
    #endif
    #ifdef CORE_INT8_PIN
    case CORE_INT8_PIN:
      irq_num = 8;
      break;
    #endif
    #ifdef CORE_INT9_PIN
    case CORE_INT9_PIN:
      irq_num = 9;
      break;
    #endif
    #ifdef CORE_INT10_PIN
    case CORE_INT10_PIN:
      irq_num = 10;
      break;
    #endif
    #ifdef CORE_INT11_PIN
    case CORE_INT11_PIN:
      irq_num = 11;
      break;
    #endif
    #ifdef CORE_INT12_PIN
    case CORE_INT12_PIN:
      irq_num = 12;
      break;
    #endif
    #ifdef CORE_INT13_PIN
    case CORE_INT13_PIN:
      irq_num = 13;
      break;
    #endif
    #ifdef CORE_INT14_PIN
    case CORE_INT14_PIN:
      irq_num = 14;
      break;
    #endif
    #ifdef CORE_INT15_PIN
    case CORE_INT15_PIN:
      irq_num = 15;
      break;
    #endif
    #ifdef CORE_INT16_PIN
    case CORE_INT16_PIN:
      irq_num = 16;
      break;
    #endif
    #ifdef CORE_INT17_PIN
    case CORE_INT17_PIN:
      irq_num = 17;
      break;
    #endif
    #ifdef CORE_INT18_PIN
    case CORE_INT18_PIN:
      irq_num = 18;
      break;
    #endif
    #ifdef CORE_INT19_PIN
    case CORE_INT19_PIN:
      irq_num = 19;
      break;
    #endif
    #ifdef CORE_INT20_PIN
    case CORE_INT20_PIN:
      irq_num = 20;
      break;
    #endif
    #ifdef CORE_INT21_PIN
    case CORE_INT21_PIN:
      irq_num = 21;
      break;
    #endif
    #ifdef CORE_INT22_PIN
    case CORE_INT22_PIN:
      irq_num = 22;
      break;
    #endif
    #ifdef CORE_INT23_PIN
    case CORE_INT23_PIN:
      irq_num = 23;
      break;
    #endif
  }
#endif

  head = 0;
  tail = 0;
  if (irq_num < 255) {
    attachInterrupt(irq_num, ps2interrupt, FALLING);
  }
}
const int kDataPin = 3;
const int kIRQpin =  2;

PS2Keyboard keyboard;

void setup() {
  keyboard.begin(kDataPin, kIRQpin, PS2Keymap_US);
//  keyboard.begin(kDataPin, IRQpin, PS2Keymap_German);
  //keyboard.begin(DataPin, IRQpin, PS2Keymap_French);
  //keyboard.begin(DataPin, IRQpin, PS2Keymap_Spanish);
  //keyboard.begin(DataPin, IRQpin, PS2Keymap_Italian);
  //keyboard.begin(DataPin, IRQpin, PS2Keymap_UK);
  Serial.begin(9600);
  Serial.println("International Keyboard Test:");
  delay(1000);
}
void loop() {
  if (keyboard.available()) {
    char c = keyboard.read();
    Serial.print(c);
  }
}
