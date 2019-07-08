#include <avr/io.h>

#include "usart_avr.h"

void main()
{

   uint8_t i; //Clasical loop varriable

   uint8_t packet[5],data=0;

   DDRC|=0xFF; //All Output

   //Initialize the USART with Baud rate = 2400bps
   USARTInit(416);

   /*
   Get data from the remote Tx Station
   The data is the value of PORTC on Remote Tx Board
   So we will copy it to the PORTC of this board.

   */

   while(1)
   {
      //Wait for a packet
      while(!UDataAvailable());
      if(UReadData()!='A') continue;
      while(!UDataAvailable());
      if(UReadData()!='A') continue;

      while(UDataAvailable()!=3);

      //Get the packet

      for(i=2;i<5;i++)
      {
         packet[i]=UReadData();
      }

      //Is it ok?
      if(packet[2]!=((uint8_t)~packet[3])) continue;

      if(packet[4]!='Z') continue;

      //The packet is ok

      data=packet[2];

      //Now we have data put it to PORTC
      PORTC=data;
   }


}