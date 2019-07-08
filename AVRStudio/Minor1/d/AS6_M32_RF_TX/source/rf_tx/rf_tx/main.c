/******************************************************************************

Reads the status of whole 8 bit PORT C and sends this 8 bit data over serial
port at 2400 bits per second speed. The PORTC data is encapsulated in a simple
packet to discriminate from a junk data. This type of packet is good for sending
over radio frequency using low cost ASK RF module.

Written By
 Avinash Gupta
Contact
 gmail@avinashgupta.com

For more interesting microcontroller tutorials and projects. Please visit
http://www.extremeelectronics.co.in

NOTICE:
PROGRAM SAMPLE PROVIDED FOR SELF LEARNING PURPOSE ONLY!
NO PART OF THIS WORK SHOULD BE USED IN ANY COMMERCIAL PROJECTS OR IN ANY 
TEACHING INSTITUTES FOR TEACHING THEIR STUDENTS
NO PART OF THIS WORK SHOULD BE PUBLISHED IN ANY FORM LIKE PRINTED OR ELECTRONIC
MEDIA

COPYRIGHT (C) 2008-2015 EXTREME ELECTRONICS, INDIA
******************************************************************************/


#include <avr/io.h>
#include <util/delay.h>

#include "lib/usart/usart_avr.h"

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
	sent over AIR will be latched on its local PORTC
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
		UWriteData('J');	//J for junk

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