#Atmel ATTiny104 USART experiment

Control two LEDs and one reed relay with an ATTiny104 MCU via USART.

##BUILDING BLOCKS:

Written for an Atmel Xplained Nano ATTiny104 eval board. This board
includes a TPI programmer (no debugger), USB connectivity, one button, and
one LED. It also has 1k of flash and 32 bytes of RAM, which is tight. All
communication happens through the single USART built into the ATTiny104
MCU. The pins for this USART are also routed through the USB connector so
connecting via a micro USB cable works fine (no jumper wires or USB/TTL
devices are required).

If you are not using this board, you will need to adjust ports, pins, etc.

##CONTROLLING THE BOARD:

Because of the TPI programmer that is required for programming ATTiny104
MCUs, you will have no choice (for now) but to use Atmel Studio on Windows
to program this MCU. On Windows, I highly recommend you use RealTerm as
your terminal program. It offers significantly more granular functionality
than basic serial terminals. Programmers will love it.

RealTerm's "Send" tab can send byte strings (enter the hex or 8bit numeric
value in the form field and click "Send Numbers"). If you're not on
Windows, find a terminal program that allows sending bytes. SENDING A BYTE
WITH SET BITS IS THE ONLY WAY TO TURN ANYTHING ON/OFF.

Component bits looked like this during my experimentation:

- LED 0   (built-in, connected to pin PA5) is bit 0
- LED 1   (external, connected to pin PA7) is bit 1
- RELAY 1 (external, the coil part connected to pin PA6) is bit 2

The external LED was connected inline with a 330 ohm (I think) resistor.

The RELAY used in testing was a simple OMR-106H reed relay:

http://www.digikey.com/product-detail/en/te-connectivity-potter-brumfield-relays/OMR-106H,V000/PB886-ND/1095211

I suspect these are no longer available, but reed relays are pretty common
and usually cheap so don't be scared to use a replacement. Just be sure it
takes 5v on the coil part, which is what the MCU will interact with. This
particular relay can switch power up to 125VAC which is creepy, but it
worked just fine with a boring LED which is more my speed.

Setting a bit for a component and then sending the resulting byte to the
board will turn on that component only. Unset the bit and send the
resulting byte to turn it off.

NOTE: The on/off status of ALL COMPONENTS must be sent in each byte. State
is not saved between received bytes! Whatever is in the byte that is
received will determine what is on and what is off.

So, in RealTerm's "Send" tab:

- Sending 0x03 (or 00000011) will turn on both LEDs.
- Sending 0x02 (or 00000010) will turn on LED 1 and turn off LED 0 if it's on.
- Sending 0x01 (or 00000001) will turn on LED 0 and turn off LED 1 if it's on.
- Sending 0x04 (or 00000100) will turn on RELAY 1 and turn off all LEDs if any are on.
- Sending 0x05 (or 00000101) will turn on RELAY 1 and LED 0 and turn off LED 1 if it's on.
- Sending 0x00 (or 00000000) will turn off all components.

You get the idea.

##MISC:

This program uses interrupts instead of polling. It's better that way.

The serial port in the terminal program needs to be set to 4800/8N1 to
communicate.  I had problems with higher baud rates. This is most likely
the result of the default clock divider on the board (8MHz divided by 8 by
default = 1MHz, but serial communication is only solid at divisions of
something like 1.8345, not 1. The ATTiny104 datasheet explains this, so
look there for an explanation).

##CAVEATS:

I have tested this locally using basic hardware and it has worked
flawlessly. The component I attached to the reed relay was another LED (I
know, yawn). The relay is rated up to something like 29V DC and 125V AC,
so there isn't much that can't be operated by a similar relay, at least in
the USA. 

That said, I will add for my own peace of mind that you should not mess
with AC appliances and relays unless you have a background in electronics
or electrical engineering. In other words, don't take your AC torch-lamp
and splice the power cord through the relay! Bad! Only work with DC if
possible. I'm fairly sure (but not positive) that appliances that have
power bricks in their cords (sometimes called "wall warts") are DC, so
maybe they're safer?  I don't know. Check with someone that knows, don't
take my word for it. And don't sue me if you ignore my advice and die.
Thanks.

##LICENSE:

My parts are public domain. Some code was borrowed almost but not quite
verbatim from Atmel's ATTiny104 datasheet (USART stuff), and I can't make
their code public domain, so whatever license their code uses is applied
to their code.

