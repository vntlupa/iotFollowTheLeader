/* bons sites:
http://www.engblaze.com/microcontroller-tutorial-avr-and-arduino-timer-interrupts/
https://arduino-info.wikispaces.com/Timers-Arduino
*/

/*
TIMER0
Timer0 is an 8-bit timer, meaning its counter register can record a maximum value of 255 (the same as an unsigned 8-bit byte). Timer0 is used by native Arduino timing functions such as delay() and millis(), so you Arduino users shouldn’t mess with it unless you’re comfortable with the consequences.

TIMER1
Timer1 is a 16-bit timer, with a maximum counter value of 65535 (an unsigned 16-bit integer). The Arduino Servo library uses this timer, so be aware if you use it in your projects.

TIMER2
Timer2 is an 8-bit timer that is very similar to Timer0. It is utilized by the Arduino tone() function.
*/

/*T = timer period, f = clock frequency

T = 1 / f
T = 1 / 1 MHz = 1 / 10^6 Hz
T = (1 * 10^-6) s
*/

//timerNum
#define TIMER0 0 //8 bits
#define TIMER1 1 //16 bits
#define TIMER2 2 //8 bits

//prescale
#define PRESCALE1 1
#define PRESCALE8 8
#define PRESCALE64 64
#define PRESCALE256 256
#define PRESCALE1024 2014

//prescale config for calculations
volatile int timer0Prescale = 1;
volatile int timer1Prescale = 1;
volatile int timer2Prescale = 1;




/* Code to blink LED when the TIMER0 counter overflows. */
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
ISR(TIMER0_OVF_vect)
{
PORTB=PORTB ^ 0xFF;// Toggle PORTB output pins
}
int main( void )
{
DDRB = 0xFF; // Configure PORTB as output
PORTB = 0xFF; // Set output pins as HIGH (LED ON)
TIMSK0=(1<<TOIE0); // Enable the interrupt on overflow for Timer0
TCNT0=0x00; // Set timer0 counter initial value to 0
TCCR0B = (1<<CS02) | (1<<CS00); // Set timer0 with /1024 prescaler
sei(); // Set the Global Interrupt Enable bit
while(true) { /* do nothing forever. */ }
}


void beginConfig()
{
	//cli();
	TCCR1A = 0;
	TCCR1B = 0;
	//sei();
}

void setTimerPrescale(char timer, int prescale)
{
	switch(prescale)
	{
		case PRESCALE1: //(1 << CS12) | (1 << CS11) | (1 << CS10);
		case PRESCALE8:
		case PRESCALE64:
		case PRESCALE256: timer1Prescale = prescale; TCCR1B = (1 << CS12); 
		case PRESCALE1024:
	}
}

void setTimerMode(char timerNum, char mode)
{
	if (timerNum == TIMER1)
	{
		TIMSK1 = (1 << TOIE1);
}

-----------------------------


// USE THIS BELOW -- quick config
// OK WORKING -- testar com o restante dos trecos de timer
-->   TIMER COM MYMIlLIS
{
	
	volatile unsigned long myMicros = 0;
	volatile unsigned long myMillis = 0;
	void setTimer1pre256( int hz)
	{
		cli();
		TCCR1A = 0;
		TCCR1B = 0;
		//TCNT1 = 34286;            // preload timer 65536-(16MHz/256/2Hz)
		TCNT1 = 65536 - (62500 / hz) ;
		TCCR1B |= (1 << CS12);    // 256 prescaler 
		TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt	
		//use --> ISR(TIMER1_OVF_vect) 
		sei();
	}
	
	ISR (TIMER1_OVF_vect) 	
	{
		//for 256 prescale each tick means: 3 milliSec + 906.25 microSec
		// for all 256 ticks --> 999 millisec + 936 microsec --> possivel corrigir a precisao
		myMicros += 936;
		if (myMicros >= 1000)
		{
			myMicros -= 1000;
			myMillis += 999 + 1;
		}
		else
		{
			myMillis += 999;
		}
     
      TCNT1 = 65536-62500;

	}
	
	unsigned long getMillis()
	{
		unsigned long ticks = TCNT1/256 ; // preload timer 65536-16MHz/256
		unsigned long elapsedFromReg = (ticks * 3906 ) / 1000;
		return myMillis + elapsedFromReg;
	}
	
	void myDelayMillis(unsigned long delayMillis)
	{
		unsigned long endMilli = delayMillis + getMillis();
		while (getMillis() < endMilli);
	}
}


// OK WORKING
void setTimer1ForMillis()
{
  	cli();
	//initialize timer
	// Enter Watchdog Configuration mode:
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    OCR1A = 64;            // compare match register 16MHz/256 -> 64 ->1024 microsegundos
    TCCR1B |= (1 << WGM12);   // CTC mode
    TCCR1B |= (1 << CS12);    // 256 prescaler 
    TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

    milliseconds = 0;
    microseconds = 0;
	//Enable global interrupts
	sei();

/*
	funcao para calcular os millis,, nao testei ainda,, mas teoricamente estah funcionando:

volatile unsigned long milliseconds;
volatile unsigned int microseconds;

ISR(TIMER1_COMPA_vect)
{
  microseconds += 24 - 1;
  milliseconds += 1;
  
  if (microseconds >= 1000) 
  {
    microseconds -= 1000;
    milliseconds += 1;
  }
  
}	

*/
}
