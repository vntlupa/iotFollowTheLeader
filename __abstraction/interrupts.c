/* bons sites:
http://www.engblaze.com/microcontroller-tutorial-avr-and-arduino-timer-interrupts/
http://courses.cs.washington.edu/courses/csep567/10wi/lectures/Lecture7.pdf
http://courses.cs.washington.edu/courses/csep567/10wi/lectures/
*/

/*
Global interrupt enable
? Bit in SREG
? Allows all interrupts to be disabled with one bit
? sei() – set the bit
? cli() – clear the bit
*/

/*
1 0x0000(1) RESET External Pin, Power-on Reset, Brown-out Reset and Watchdog System
Reset
2 0x0002 INT0 External Interrupt Request 0
3 0x0004 INT1 External Interrupt Request 0
4 0x0006 PCINT0 Pin Change Interrupt Request 0
5 0x0008 PCINT1 Pin Change Interrupt Request 1
6 0x000A PCINT2 Pin Change Interrupt Request 2
7 0x000C WDT Watchdog Time-out Interrupt
8 0x000E TIMER2_COMPA Timer/Counter2 Compare Match A
9 0x0010 TIMER2_COMPB Timer/Coutner2 Compare Match B
10 0x0012 TIMER2_OVF Timer/Counter2 Overflow
11 0x0014 TIMER1_CAPT Timer/Counter1 Capture Event
12 0x0016 TIMER1_COMPA Timer/Counter1 Compare Match A
13 0x0018 TIMER1_COMPB Timer/Coutner1 Compare Match B
14 0x001A TIMER1_OVF Timer/Counter1 Overflow
15 0x001C TIMER0_COMPA Timer/Counter0 Compare Match A
16 0x001E TIMER0_COMPB Timer/Coutner0 Compare Match B
17 0x0020 TIMER0_OVF Timer/Counter0 Overflow
18 0x0022 SPI STC SPI Serial Transfer Complete
19 0x0024 USART_RX USART Rx Complete
20 0x0026 USART_UDRE USART Data Register Empty
21 0x0028 USART_TX USART Tx Complete
22 0x002A ADC ADC Conversion Complete
23 0x002C EE READY EEPROM Ready
24 0x002E ANALOG COMP Analog Comparator


ISR(TIMER0_OVF_vect)
{
	
}
*/

// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>
 
#define LEDPIN 2
 
void setup()
{
    pinMode(LEDPIN, OUTPUT);
 
    // initialize Timer1
    cli();             // disable global interrupts
    TCCR1A = 0;        // set entire TCCR1A register to 0
    TCCR1B = 0;
 
    // enable Timer1 overflow interrupt:
    TIMSK1 = (1 << TOIE1);
    // Set CS10 bit so timer runs at clock speed:
    TCCR1B |= (1 << CS10);
    // enable global interrupts:
    sei();
}