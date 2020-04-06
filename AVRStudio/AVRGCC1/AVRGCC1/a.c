#include <avr/io.h>
#include <util/delay.h>

#include "usart_avr.h"

void main()
{
   //Initialize the USART with Baud rate = 2400bps
   USARTInit(416);

   //Enable Internal Pullups on PORTC
   PORTC=0xFF;

   /* 

   Keep transmitting the Value of Local PORTC
   to the Remote Station.

   On Remote RX station the Value of PORTC
   sent on AIR will be latched on its local PORTC
   */

   uint8_t data;
   while(1)
   {
      data=PINC;

      /* 
      Now send a Packet
      Packet Format is AA<data><data inverse>Z

      total Packet size if 5 bytes.
      */

      //Stabilize the Tx Module By Sending JUNK data
      UWriteData('J');  //J for junk

      //Send 'A'
      UWriteData('A');

      //Send Another 'A'
      UWriteData('A');

      //Send the data;
      UWriteData(data);

      //Send inverse of data for error detection purpose

      UWriteData(~data);

      //End the packet by writing 'Z'
      UWriteData('Z');

      //Wait for some time
      _delay_loop_2(0);
      _delay_loop_2(0);
      _delay_loop_2(0);
      _delay_loop_2(0);
   }
}
