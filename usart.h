/*
 * usart.h
 *
 * Created: 8/28/2016 3:43:55 PM
 * Author: Erik Ratcliffe
 *
 * USART functions, mostly based on code from Atmel ATTiny104 datasheet.
 * See the ATTiny104 datasheet for information about licensing of Atmel
 * code.
 */ 


#ifndef USART_H_
#define USART_H_


#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

// Initialize the USART. 
void USART_Init( unsigned int ubrr );

// Transmit a single byte.
void USART_Transmit( unsigned char data );

// Receive a single byte.
unsigned char USART_Receive( void );

// Send a string of characters. Not used, not Atmel's. Public domain.
void USART_SendStr( char str[] );


#endif /* USART_H_ */
