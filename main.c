/*
 * Tiny104_LED_USART
 *
 * Created: 8/20/2016 12:05:27 PM
 * Author : Erik Ratcliffe
 * License: Public domain? Sure, public domain.
 *
 * Send a command byte from serial port to turn LEDs or relays on/off.
 *
 *
 * BUILDING BLOCKS:
 *
 * Written for an Atmel Xplained Nano ATTiny104 eval board. This board
 * includes a TPI programmer (no debugger), USB connectivity, one button,
 * and one LED. It also has 1k of flash and 32 bytes of RAM, which is
 * tight. All communication happens through the single USART built into
 * the ATTiny104 MCU. The pins for this USART are also routed through the
 * USB connector so connecting via a micro USB cable works fine (no jumper
 * wires or USB/TTL devices are required).
 *
 * If you are not using that board, you will need to adjust ports, pins,
 * etc.
 *
 *
 * CONTROLLING THE BOARD:
 *
 * Because of the TPI programmer that is required for programming
 * ATTiny104 MCUs, you will have no choice (for now) but to use Atmel
 * Studio on Windows to program this MCU. On Windows, I highly recommend
 * you use RealTerm as your terminal program. It offers significantly more
 * granular functionality than basic serial terminals. Programmers will
 * love it.
 *
 * RealTerm's "Send" tab can send byte strings (enter the hex or 8bit
 * numeric value in the form field and click "Send Numbers"). If you're
 * not on Windows, find a terminal program that allows sending bytes.
 * SENDING A BYTE WITH SET BITS IS THE ONLY WAY TO TURN ANYTHING ON/OFF.
 *
 * Component bits currently look like this:
 *
 *    LED 0   (built-in, on PA5) is bit 0
 *    LED 1   (external, on PA7) is bit 1
 *    RELAY 0 (external, on PA6) is bit 2
 * 
 * The external LED is connected inline with a 330 ohm (I think) resistor.
 *
 * The RELAY used in testing was a simple OMR-106H reed relay:
 *
 * http://www.digikey.com/product-detail/en/te-connectivity-potter-brumfield-relays/OMR-106H,V000/PB886-ND/1095211
 *
 * I suspect these are no longer available, but reed relays are pretty
 * common and usually cheap so don't be scared to use a replacement. Just
 * be sure it takes 5v on the coil part, which is what the MCU will
 * interact with. This particular relay can switch power up to 125VAC
 * which is creepy, and I believe up to 29VDC, but it worked just fine
 * with a boring LED which is more my speed.
 *
 * Setting a bit for a component and then sending the resulting byte to
 * the board will turn on that component only. Unset the bit and send the
 * resulting byte to turn it off.
 *
 * NOTE: The on/off status of ALL COMPONENTS must be sent in each byte.
 * State is not saved between received bytes! Whatever is in the byte that
 * is received will determine what is on and what is off.
 *
 * So, in RealTerm's "Send" tab:
 *
 * Sending 0x03 (or 00000011) will turn on both LEDs.
 * Sending 0x02 (or 00000010) will turn on LED 1 and turn off LED 0 if it's on.
 * Sending 0x01 (or 00000001) will turn on LED 0 and turn off LED 1 if it's on.
 * Sending 0x04 (or 00000100) will turn on RELAY 0 and turn off all LEDs if any are on.
 * Sending 0x05 (or 00000101) will turn on RELAY 0 and LED 0 and turn off LED 1 if it's on.
 * Sending 0x00 (or 00000000) will turn off all components.
 *
 * You get the idea.
 *
 * Uses interrupts instead of polling. It's better that way.
 *
 * Serial port in the terminal program needs to be set to 4800/8N1 to communicate.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

// USART Baud Rate Register value (UBRR)
#define FOSC 1000000    // Clock Speed (8MHz/8, 8 = default divider
#define BAUD 4800       // Anything higher = corrupted chars. Sync no better than async.
#define MYUBRR (((( FOSC / 16) + ( BAUD / 2) ) / ( BAUD )) - 1) 

// Component bit positions in incoming command byte
#define C_LED0   0 
#define C_LED1   1
#define C_RELAY0 2 

// Component bit positions in AVR port(s)
#define LED0   1<<PORTA5
#define LED1   1<<PORTA7
#define RELAY0 1<<PORTA6

int main(void)
{
  // Initialize the USART
  USART_Init(MYUBRR); 
  
  // Configure the Data Direction Register for Port A (DDRA).
  // Set LED0, LED1, and RELAY0 pins as output (PA5, PA6, & PA7).
  DDRA |= 1<<DDRA5 | 1<<DDRA6 | 1<<DDRA7;

  // Now that data direction is established, enable the pins.
  // Start with the LEDs off (built-in LED 0 is off if true, on if false.
  // External LED 1 is the opposite: off if false, on if true.
  PORTA |= 1<<LED0 & ~(LED1) & ~(RELAY0);
  
  // USART Receive Complete interrupt stuff
  UCSRB |= (1 << RXCIE ); // Enable Receive Complete Interrupt Enable interrupt
  sei(); // Enable Global Interrupt Enable flag

  // Loop forever, do nothing. Interrupts handle everything now.
  // See the ISR() interrupt handler, elsewere in this file.
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

  // Turn LED 0 on/off
  if(cmd & 1<<C_LED0) 
    PORTA &= ~(LED0);   // Turn on LED 0
  else 
    PORTA |= LED0;      // Turn off LED 0

  // Turn LED 1 on/off
  if(cmd & 1<<C_LED1) 
    PORTA |= LED1;      // Turn on LED 1
  else 
    PORTA &= ~(LED1);   // Turn off LED 1

  // Turn RELAY 0 on/off
  if(cmd & 1<<C_RELAY0) 
    PORTA |= RELAY0;    // Turn on RELAY 0
  else 
    PORTA &= ~(RELAY0); // Turn off RELAY 0
}



