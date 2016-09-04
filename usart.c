/*
 * usart.c
 *
 * Created: 8/28/2016 3:39:39 PM
 * Author: Erik Ratcliffe
 *
 * USART code, mostly based on code from Atmel ATTiny104 datasheet.
 * See the ATTiny104 datasheet for license information about Atmel's code.
 */ 

#include "usart.h"

// Initialize the USART.
void USART_Init( unsigned int ubrr )
{
   // Set baud rate.
   // UBRR is a 12 bit value, the 4 most significant bits being "high" (H)
   // and the 8 least significant bits being "low" (L). For UBBRH, shift the 12 
   // bit value 8 bits to the right to get only the 4 most significant bits,
   // then for UBRRL cast the 12 bit value to an unsigned char to snip only the 8 
   // least significant bits.
   UBRRH = (unsigned char)(ubrr>>8);
   UBRRL = (unsigned char)ubrr;

   // Enable receiver and transmitter
   UCSRB = (1<<RXEN)|(1<<TXEN);

   // Set frame format: sync (0<<USBS), 8data, 2stop bits (8N1 in term
   // app? 8N2? Same output) 
   UCSRC = (1<<USBS)|(3<<UCSZ0);
 }

// Transmit a single byte.
void USART_Transmit( unsigned char data )
{
   // Wait for empty transmit buffer.
   while ( !( UCSRA & (1<<UDRE)) );

   //Put data into buffer, sends the data.
   UDR = data;
}

// Receive a single byte.
unsigned char USART_Receive( void )
{
   // Wait for data to be received.
   while ( !(UCSRA & (1<<RXC)) );

   // Get and return received data from buffer.
   return UDR;
}

// Transmit an entire string.
// This function is not used, but I played with it and it worked so I am
// leaving it in. Use it, forget it, whatever. This is not Atmel's, BTW.
// Public domain, take it and use it without guilt.
void USART_SendStr( char str[] )
{
   for(int i = 0; i < strlen(str); i++)
   {
      USART_Transmit(str[i]);
   }

   USART_Transmit('\n');
   USART_Transmit('\r');
}
