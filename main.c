/*
 * Tiny104_USART_SWITCHES
 *
 * Created: 9/6/2016 12:05:27 PM
 * Author : Erik Ratcliffe
 * License: Public domain? Sure, public domain.
 *
 * Send a command byte from serial port to turn pins on/off
 * on an Atmel Xplained Nano board w/ an ATTiny104 MCU.
 *
 * Serial terminal settings: 4800 8N1
 *
 * Pins PA0-PA7 on the board are handled via bits 0-7 in the 
 * byte that you'll send to the board via serial terminal.
 * Except for the built-in LED which is attached to PA5, setting
 * a bit to 0 turns off the associated pin and setting a bit to 1
 * turns it on.
 *
 * This board is 5v, so output pins are 5V. Don't connect anything
 * that can't handle 5V to any of the pins.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

// USART Baud Rate Register value (UBRR)
#define FOSC 1000000 // Clock Speed (8MHz/8, 8 = default divider)
#define BAUD 4800    // Anything higher = corruption. FOSC?
#define MYUBRR (((( FOSC / 16) + ( BAUD / 2) ) / ( BAUD )) - 1) 

int main(void)
{
  // Initialize the USART
  USART_Init(MYUBRR); 
  
  // Configure the Data Direction Register for Port A (DDRA).
  // Set all pins in PORTA as output (1111 1111, a.k.a. 0xFF)
  DDRA = 0xFF;

  // Now that data direction is established, enable the pins.
  // Turn off all pins in PORTA by default.
  // NOTE: On Xplained Nano board, the built-in LED on PA5 follows
  // reverse logic: setting the bit to 0 turns ON the LED and setting
  // it to 1 turns OFF the LED. I have no earthly idea why. So, the 
  // built-in LED on the Xplained Nano board will be lit by default
  // after all pins in the port are set to "off" (0000 0000, a.k.a. 0x00).
  PORTA = 0x00;
  
  // USART Receive Complete interrupt stuff
  UCSRB |= (1 << RXCIE ); // Enable Receive Complete Interrupt Enable interrupt
  sei();                  // Enable Global Interrupt Enable flag

  // Loop forever, do nothing. Interrupts handle everything now.
  // See the ISR() interrupt handler below for the actions taken when
  // the interrupt is triggered.
  for(;;)
  {}
}


// Handler for the USART Receive Complete interrupt.
// The interrupt we want here is USART_RXC. Slap "_vect" at the end of the
// interrupt name, and specify ISR_BLOCK just because you need to.
ISR(USART_RXC_vect, ISR_BLOCK)
{
  // User input byte
  unsigned char cmd;

  // Read input from USART
  cmd = USART_Receive();

  // Turn components on/off based on bits in the received byte.
  // Whatever we get from the user, push that into PORTA. The 8 bits
  // in the byte map perfectly with the 8 bits in PORTA. Components
  // connected with the PORTA pins (if any) should turn on/off based on 
  // how the bits in the cmd byte were set.
  PORTA = cmd;
}



