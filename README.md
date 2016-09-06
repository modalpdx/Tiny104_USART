#Atmel Xplained Nano (ATTiny104) USART experiment

Control pins PA0-PA7 via USART using a single byte.

##BUILDING BLOCKS:

Written for an [Atmel Xplained Nano ATTiny104 eval board](http://www.atmel.com/tools/ATTINY104-XNANO.aspx).
This board includes a TPI programmer (no debugger), USB connectivity, one
button, and one LED. It also has 1k of flash and 32 bytes of RAM, which is
tight. All communication happens through the single USART built into the
ATTiny104 MCU. The pins for this USART are also routed through the USB
connector so connecting via a micro USB cable works fine (no jumper wires
or USB/TTL devices are required).

If you are not using this board, you will need to adjust ports, pins, etc.

##CONTROLLING THE BOARD:

Because of the TPI programmer that is required for programming ATTiny104
MCUs, you will have no choice (for now) but to use Atmel Studio on Windows
to program this MCU. On Windows, I highly recommend you use RealTerm as
your terminal program. It offers significantly more granular functionality
than basic serial terminals. Programmers will love it.

RealTerm's "Send" tab can send byte strings (enter the hex value in the
form field and click "Send Numbers"). If you're not on Windows, find a
terminal program that allows sending bytes. Moserial on Linux seems to
work, as do others I assume. 

The output pins used here are PA0 - PA7, mapped to bits 0 - 7 in the byte
you'll be sending to the MCU. 5V logic is used here, so don't plug in 3.3V
components without adding some kind of voltage regulation.

Setting a bit for a pin in a serial terminal and then sending the
resulting byte to the board will turn on the pin (and its attached
component) only.  Unset the bit and send the resulting byte to turn it
off.

NOTE: The on/off status of ALL COMPONENTS must be sent in each byte.
State is not saved between received bytes! Whatever is in the byte that is
received will determine what is on and what is off.

So, in RealTerm's "Send" tab (or whatever serial terminal you're using):

- Sending 0x03 (00000011) will turn on pins PA0 and PA1 and turn the rest off.
- Sending 0x42 (01000010) will turn on pins PA6 and PA1 and turn the rest off.
- Sending 0xFF (11111111) will turn on all pins.
- Sending 0x00 (00000000) will turn off all pins.

You get the idea.

##MISC:

This program uses interrupts instead of polling. It's better that way.

Due to the proprietary nature of TPI programming and mEDBG programmers
(both of which are required for programming ATTiny104 MCUs), you will need
Atmel Studio on Windows to do anything with any of this code. Sorry, blame
Atmel for not going to Eclipse route (grrr). Some day, some day...

The serial port in the terminal program needs to be set to 4800/8N1 to
communicate.  I had problems with higher baud rates. This is most likely
the result of the default clock divider on the Xplained Nanon board. 8MHz
divided by 8 by default = 1MHz, but serial communication is only solid at
divisions of something like 1.8345, not 1. The ATTiny104 datasheet
explains this, so look there for an explanation.

I admit that shoving an entire byte into a PORTx is not very glamorous. 
I'm trying to keep things really small which means no arrays, no enums,
no loops or switch statements, etc. This is a dumb device, and like many
dumb devices, it trusts you completely. Sending a byte and processing it 
immediately (and then forgetting about it) makes this all very compact.
On my system, Atmel Studio reports 162 bytes of storage and 0 bytes of
memory used, which fall well within the device's 1k of storage and 32 
bytes of memory.

##CAVEATS:

I have tested this using basic hardware and it has worked flawlessly. The
component I attached to the reed relay was another LED (I know, yawn). The
relay is rated up to something like 29V DC and 125V AC, so there isn't
much that can't be operated by a similar relay, at least in the USA.

That said, I will add for my own peace of mind that you should not mess
with AC appliances and relays unless you have a background in electronics
or electrical engineering. In other words, don't take your AC torch-lamp
and splice the power cord through the relay! Bad! Only work with DC if
possible. I'm fairly sure (but not positive) that appliances that have
power bricks in their cords (sometimes called "wall warts") are DC, so
maybe they're safer?  I don't know. Check with someone that knows, don't
take my word for it. And don't sue me if you ignore my advice and die.
Thanks.

Last bit: I enabled all 8 pins of PORTA as output, but I don't think it's
a good idea to actually enable all 8 pins. They're set as output and are
ready to go for convenience more than practicality. If you plan on
connecting 8 components to the 8 PORTA pins, please check the datasheet
for output limits and make sure you're not pushing the MCU too hard. You
may be fine, but be careful.

##LICENSE:

My parts are public domain. Some code was borrowed almost but not quite
verbatim from Atmel's ATTiny104 datasheet (USART stuff), and I can't make
their code public domain, so whatever license their code uses is applied
to their code.

