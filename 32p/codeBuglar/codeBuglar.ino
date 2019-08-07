

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#define LCD_EN_PORT PORTC
#define LCD_EN_PIN  PINC0
  
#define LCD_RS_PORT PORTC   //R/S line
#define LCD_RS_PIN  PINC1

#define LCD_D7_PORT PORTC
#define LCD_D7_PIN  PINC2
#define LCD_D6_PORT PORTC
#define LCD_D6_PIN  PINC3
#define LCD_D5_PORT PORTC
#define LCD_D5_PIN  PINC4
#define LCD_D4_PORT PORTC
#define LCD_D4_PIN  PINC5



/*************************************************/
/*****DO NOT MODIFY ANYTHING BELOW THIS POINT*****/
/*************************************************/


#include <util/delay.h>

#include <avr/pgmspace.h>

#include <stdarg.h>
#include <stdlib.h> 


#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */

/* instruction register bit positions, see HD44780U data sheet */
#define LCD_CLR               0      /* DB0: clear display                  */

#define LCD_HOME              1      /* DB1: return to home position        */

#define LCD_ENTRY_MODE        2      /* DB2: set entry mode                 */
#define LCD_ENTRY_INC         1      /*   DB1: 1=increment, 0=decrement     */
#define LCD_ENTRY_SHIFT       0      /*   DB2: 1=display shift on           */

#define LCD_ON                3      /* DB3: turn lcd/cursor on             */
#define LCD_ON_DISPLAY        2      /*   DB2: turn display on              */
#define LCD_ON_CURSOR         1      /*   DB1: turn cursor on               */
#define LCD_ON_BLINK          0      /*     DB0: blinking cursor ?          */

#define LCD_MOVE              4      /* DB4: move cursor/display            */
#define LCD_MOVE_DISP         3      /*   DB3: move display (0-> cursor) ?  */
#define LCD_MOVE_RIGHT        2      /*   DB2: move right (0-> left) ?      */

#define LCD_FUNCTION          5      /* DB5: function set                   */
#define LCD_FUNCTION_8BIT     4      /*   DB4: set 8BIT mode (0->4BIT mode) */
#define LCD_FUNCTION_2LINES   3      /*   DB3: two lines (0->one line)      */
#define LCD_FUNCTION_10DOTS   2      /*   DB2: 5x10 font (0->5x7 font)      */

#define LCD_CGRAM             6      /* DB6: set CG RAM address             */

#define LCD_DDRAM             7      /* DB7: set DD RAM address             */

#define LCD_BUSY              7      /* DB7: LCD is busy                    */

/* set entry mode: display shift on/off, dec/inc cursor move direction */
#define LCD_ENTRY_DEC            0x04   /* display shift off, dec cursor move dir */ //((1<<LCD_ENTRY_MODE) | (0<<LCD_ENTRY_INC) | (0<<LCD_ENTRY_SHIFT))
#define LCD_ENTRY_DEC_SHIFT      0x05   /* display shift on,  dec cursor move dir */ //((1<<LCD_ENTRY_MODE) | (0<<LCD_ENTRY_INC) | (1<<LCD_ENTRY_SHIFT))
#define LCD_ENTRY_INC_           0x06   /* display shift off, inc cursor move dir */ //((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) | (0<<LCD_ENTRY_SHIFT))
#define LCD_ENTRY_INC_SHIFT      0x07   /* display shift on,  inc cursor move dir */ //((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) | (1<<LCD_ENTRY_SHIFT))

/* display on/off, cursor on/off, blinking char at cursor position */
#define LCD_DISP_OFF             0x08   /* display off                            */ //((1<<LCD_ON) | (0<<LCD_ON_DISPLAY) | (0<<LCD_ON_CURSOR) | (0<<LCD_ON_BLINK))
#define LCD_DISP_ON              0x0C   /* display on, cursor off                 */ //((1<<LCD_ON) | (1<<LCD_ON_DISPLAY) | (0<<LCD_ON_CURSOR) | (0<<LCD_ON_BLINK))
#define LCD_DISP_ON_BLINK        0x0D   /* display on, cursor off, blink char     */ //((1<<LCD_ON) | (1<<LCD_ON_DISPLAY) | (0<<LCD_ON_CURSOR) | (1<<LCD_ON_BLINK))
#define LCD_DISP_ON_CURSOR       0x0E   /* display on, cursor on                  */ //((1<<LCD_ON) | (1<<LCD_ON_DISPLAY) | (1<<LCD_ON_CURSOR) | (0<<LCD_ON_BLINK))
#define LCD_DISP_ON_CURSOR_BLINK 0x0F   /* display on, cursor on, blink char      */ //((1<<LCD_ON) | (1<<LCD_ON_DISPLAY) | (1<<LCD_ON_CURSOR) | (1<<LCD_ON_BLINK))

/* move cursor/shift display */
#define LCD_MOVE_CURSOR_LEFT     0x10   /* move cursor left  (decrement)          */ //((1<<LCD_MOVE) | (0<<LCD_MOVE_DISP) | (0<<LCD_MOVE_RIGHT))
#define LCD_MOVE_CURSOR_RIGHT    0x14   /* move cursor right (increment)          */ //((1<<LCD_MOVE) | (0<<LCD_MOVE_DISP) | (1<<LCD_MOVE_RIGHT))
#define LCD_MOVE_DISP_LEFT       0x18   /* shift display left                     */ //((1<<LCD_MOVE) | (1<<LCD_MOVE_DISP) | (0<<LCD_MOVE_RIGHT))
#define LCD_MOVE_DISP_RIGHT      0x1C   /* shift display right                    */ //((1<<LCD_MOVE) | (1<<LCD_MOVE_DISP) | (1<<LCD_MOVE_RIGHT))

/* function set: set interface data length and number of display lines */
#define LCD_FUNCTION_4BIT_1LINE  0x20   /* 4-bit interface, single line, 5x7 dots */ //((1<<LCD_FUNCTION) | (0<<LCD_FUNCTION_8BIT) | (0<<LCD_FUNCTION_2LINES) | (0<<LCD_FUNCTION_10DOTS))
#define LCD_FUNCTION_4BIT_2LINES 0x28   /* 4-bit interface, dual line,   5x7 dots */ //((1<<LCD_FUNCTION) | (0<<LCD_FUNCTION_8BIT) | (1<<LCD_FUNCTION_2LINES) | (0<<LCD_FUNCTION_10DOTS))
#define LCD_FUNCTION_8BIT_1LINE  0x30   /* 8-bit interface, single line, 5x7 dots */ //((1<<LCD_FUNCTION) | (1<<LCD_FUNCTION_8BIT) | (0<<LCD_FUNCTION_2LINES) | (0<<LCD_FUNCTION_10DOTS))
#define LCD_FUNCTION_8BIT_2LINES 0x38   /* 8-bit interface, dual line,   5x7 dots */ //((1<<LCD_FUNCTION) | (0<<LCD_FUNCTION_8BIT) | (1<<LCD_FUNCTION_2LINES) | (0<<LCD_FUNCTION_10DOTS))


#define LCD_MODE_DEFAULT     ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )


#define LCD_STROBE()  ((LCD_EN_PORT |= (1 << LCD_EN_PIN)),(LCD_EN_PORT &= ~(1 << LCD_EN_PIN)))



static void lcd_write(uint8_t c)
{
  _delay_us(40);
  //MS nibble
  if(c & 0x80)
    LCD_D7_PORT |=  (1 << LCD_D7_PIN);
  else
    LCD_D7_PORT &= ~(1 << LCD_D7_PIN);
  
  if(c & 0x40)
    LCD_D6_PORT |=  (1 << LCD_D6_PIN);
  else
    LCD_D6_PORT &= ~(1 << LCD_D6_PIN);
  
  if(c & 0x20)
    LCD_D5_PORT |=  (1 << LCD_D5_PIN);
  else
    LCD_D5_PORT &= ~(1 << LCD_D5_PIN);
    
  if(c & 0x10)
    LCD_D4_PORT |=  (1 << LCD_D4_PIN);
  else
    LCD_D4_PORT &= ~(1 << LCD_D4_PIN);
  
  LCD_STROBE();
  
  //LS nibble
  if(c & 0x08)
    LCD_D7_PORT |=  (1 << LCD_D7_PIN);
  else
    LCD_D7_PORT &= ~(1 << LCD_D7_PIN);
    
  if(c & 0x04)
    LCD_D6_PORT |=  (1 << LCD_D6_PIN);
  else
    LCD_D6_PORT &= ~(1 << LCD_D6_PIN);
    
  if(c & 0x02)
    LCD_D5_PORT |=  (1 << LCD_D5_PIN);
  else
    LCD_D5_PORT &= ~(1 << LCD_D5_PIN);
    
  if(c & 0x01)
    LCD_D4_PORT |=  (1 << LCD_D4_PIN);
  else
    LCD_D4_PORT &= ~(1 << LCD_D4_PIN);
  
  LCD_STROBE();
}

void lcd_clear(void)
{
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  lcd_write(1<<LCD_CLR);
  _delay_ms(2);
}


void lcd_puts(const char * s)
{
  LCD_RS_PORT |= (1 << LCD_RS_PIN); // write characters
  while(*s)
    lcd_write(*s++);
}

void lcd_goto(unsigned char pos)
{
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  lcd_write(0x80|pos);
}




void lcd_init()
{
  DDR(LCD_RS_PORT) |= (1 << LCD_RS_PIN);
  DDR(LCD_EN_PORT) |= (1 << LCD_EN_PIN);
  
  DDR(LCD_D7_PORT) |= (1 << LCD_D7_PIN);
  DDR(LCD_D6_PORT) |= (1 << LCD_D6_PIN);
  DDR(LCD_D5_PORT) |= (1 << LCD_D5_PIN);
  DDR(LCD_D4_PORT) |= (1 << LCD_D4_PIN);
  
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  LCD_EN_PORT &= ~(1 << LCD_EN_PIN);
  
  _delay_ms(15);  // wait 15mSec after power applied,

  LCD_D4_PORT |= (1 << LCD_D4_PIN);//0x3 & 0x01;        //bit0 000X
  LCD_D5_PORT |= (1 << LCD_D5_PIN);//(0x3>>1) & 0x01;   //bit1 00XY -> 000X
  LCD_D6_PORT &= ~(1 << LCD_D6_PIN);//(0x3>>2) & 0x01;    //bit2 0XYZ -> 000X
  LCD_D7_PORT &= ~(1 << LCD_D7_PIN);//(0x3>>3) & 0x01;    //bit3 XYZW -> 000X
  
  LCD_STROBE();
  _delay_ms(5);
  LCD_STROBE();
  _delay_us(200);
  LCD_STROBE();
  _delay_us(200);
  
  // Four bit mode 
  LCD_D4_PORT &= ~(1 << LCD_D4_PIN);  //2 & 0x01
  LCD_D5_PORT |=  (1 << LCD_D5_PIN);  //(2>>1) & 0x01
  LCD_D6_PORT &= ~(1 << LCD_D6_PIN);  //(2>>2) & 0x01
  LCD_D7_PORT &= ~(1 << LCD_D7_PIN);  //(2>>3) & 0x01
  
  LCD_STROBE();

  lcd_write(0x28);    // Set interface length: nibblemode, 2line, 5x7dot
  lcd_write(0b00001100);  // Display On, Cursor Off, Cursor Blink off
  lcd_clear();      // Clear screen
  lcd_write(0x6);     // Set entry Mode : increment, displayShiftOff
}

void lcd_gotoxy( unsigned char x, unsigned char y )
{
  lcd_goto( y*64+x );
}



//set desired baud rate
#define BAUDRATE 9600
//calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)
//define receive parameters
#define SYNC 0XAA// synchro signal
#define RADDR 0x44

void USART_Init(void)
{
  UBRR0L=(uint8_t)UBRRVAL;     //low byte
  UBRR0H=(uint8_t)(UBRRVAL>>8);   //high byte
//  //Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
 // UCSR0C=(1<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|  (0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
//  //Enable Transmitter and Receiver and Interrupt on receive complete
  
// UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
UCSR0C = 0
    | (0<<UMSEL01) | (0<<UMSEL00)   // Asynchronous USART
    | (0<<USBS0)|(1 << UCSZ01)|(1<<UCSZ00);// Set frame format: 8data, 1 stop bit.
}
void USART_vSendByte(uint8_t u8Data)
{
  // Wait if a byte is being transmitted
  while((UCSR0A&(1<<UDRE0)) == 0);
  // Transmit data
  UDR0 = u8Data;
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

uint8_t USART_vReceiveByte(void)
{
  // Wait until a byte has been received
  while((UCSR0A&(1<<RXC0)) == 0);
  // Return received data
  return UDR0;
}
char strre[50];
ISR(USART_RX_vect)
{
  //define variables
  static int count=0;
  uint8_t raddress, data, chk;//transmitter address
//  //receive destination address
  raddress=USART_vReceiveByte();
////  //receive data
 data=USART_vReceiveByte();
//  //receive checksum
  chk=USART_vReceiveByte();
//  //compare received checksum with calculated
//  //PORTB=0xff;
  static bool start=0;
  lcd_clear();
  lcd_gotoxy(0,0);
//  a++;
  lcd_puts("done");
  delayms(10);
  
//  static int count;
  if(chk==(raddress+data))//if match perform operations
  {
    
//    //if transmitter address match
    if(raddress==RADDR)
    {
        strre[count]=data;
        count++;
        strre[count]='\0';
      //  lcd_gotoxy(0,0);
      //  lcd_puts(strre);
    
      
    }
  }
//      if(data==SYNC)
//      {
//        if (!start){
//          PORTB=(1<<0);//LED ON
//          count=0;
//          start=1;
//        }else{
//          PORTB=(0b00000000);
//          start=0;
//          strre[count]='\0';
//          lcd_gotoxy(0,0);
//          lcd_puts(strre);
//          
//        }
//        
//      }
//      else
//      {
//        //blink led as error
//        strre[count]=data;
//        count++;
//        strre[count]='\0';
//        lcd_gotoxy(0,0);
//        lcd_puts(strre);
//      }
//    }
//  }
  USART_interrupt_init();
}

void USART_interrupt_init(void)
{
    cli();
    // Macro to determine the baud prescale rate see table 22.1 in the Mega datasheet
  
    UBRR0 = UBRRVAL;                 // Set the baud rate prescale rate register
    UCSR0B = ((1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0));       // Enable receiver and transmitter and Rx interrupt
    UCSR0C = ((0<<USBS0)|(1 << UCSZ01)|(1<<UCSZ00));  // Set frame format: 8data, 1 stop bit. See Table 22-7 for details
    sei();
}


int main(){
  lcd_init();
  USART_interrupt_init();
  
  char str1[50] = "Defender?";
  
  lcd_gotoxy(0,0);
  lcd_puts(str1);
  //while(1);
  pinMode(11,INPUT);
  delayms(100);
  
   while(digitalRead(11)!=1);
  lcd_clear();
  
    //send command to switch led ON
    int i=0;
    while(str1[i] != '?'){
      Send_Packet(RADDR, str1[i]);
      delayms(100);
      Send_Packet(RADDR, str1[i]);
      delayms(100);
      Send_Packet(RADDR, str1[i]);
      delayms(100);
      i++;
    }
    
  while(1){
      
  }
}
