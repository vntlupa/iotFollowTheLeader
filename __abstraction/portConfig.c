/*
PORTB 0x25 Port B Data Register
DDRB 0x24 Port B Data Direction Register
PINB 0x23 Port B Input Pins Address
PORTC 0x28 Port C Data Register
DDRC 0x27 Port C Data Direction Register
PINC 0x26 Port C Input Pins Address
PORTD 0x2B Port D Data Register
DDRD 0x2A Port D Data Direction Register
PIND 0x29 Port D Input Pins Address
*/

// portNum
#define PORT13 13 //PB5
#define PORT12 12 //PB4
#define PORT11 11 //PB3
#define PORT10 10 //PB2
#define PORT09 9  //PB1
#define PORT08 8  //PB0
#define PORT03 3  //PD3 INT1
#define PORT02 2  //PD2 INT0


//mode
#define INPUT 0
#define OUTPUT 1

//state
#define HIGH 1
#define LOW 0

//registers
volatile char * ddrB = (char *) 0x24; // configura
volatile char * pinB = (char *) 0x23; // le
volatile char * portB = (char *) 0x25; // grava
volatile char * ddrC = (char *) 0x27; // configura
volatile char * pinC = (char *) 0x26; // le
volatile char * portC = (char *) 0x28; // grava
volatile char * ddrD = (char *) 0x2A; // configura
volatile char * pinD = (char *) 0x29; // le
volatile char * portD = (char *) 0x2B; // grava



void configPort( char portNum, char mode )
{
	if (mode == INPUT) // 0
	{
		if (portNum <= PORT13 && portNum >= PORT08)
		{
			*ddrB &= ~ ( 1 << (portNum - 8) );
		}
	}
	else // OUTPUT    // 1
	{
		if (portNum <= PORT13 && portNum >= PORT08)
		{
			*ddrB |= ( 1 << (portNum - 8) );
		}		
	}
}

void setPortState(char portNum, char state)
{
	if (state == LOW)
	{
		if (portNum <= PORT13 && portNum >= PORT08)
		{
			*portB &= ~ ( 1 << (portNum - 8) ); 
		}
	}
	else // HIGH
	{
		if (portNum <= PORT13 && portNum >= PORT08)
		{
			*portB |= ( 1 << (portNum - 8) ); 
		}		
	}
}


char getPortState(char portNum)
{
	char state = -1;
	if (portNum <= PORT13 && portNum >= PORT08)
	{
		state = *pinB & ( 1 << (portNum - 8) ) ;
	}
	if (state == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void configInt0() // PORT02 ,, quick config
{
	cli();
	DDRD &= ~(1 << DDD2); // PD2 (PCINT0 pin) is now an input
	PORTD |= (1 << PORTD2); // PD2 is now with pull-up enabled
	EICRA &= ~(1 << ISC00); // set INT0 to trigger on
	EICRA |= (1 << ISC01); // FALLING edge (ISC0 = 01)
	EIMSK |= (1 << INT0); // Allow INT0 interrupts
	sei();
	//use --> ISR (INT0_vect)
}


/////////////////////////////////////////////////////////
// --> quickTeste

void setup() {
	configPort(PORT13, OUTPUT);
	configPort(PORT12, INPUT);
	configInt0();
}

ISR (INT0_vect)
{
	if (getPortState(PORT13) == HIGH)
	{
		setPortState(PORT13, LOW);
	}
	else //LOW
	{
		setPortState(PORT13, HIGH);
	}
}

void loop() {
/*
	if (getPortState(PORT12) == HIGH)
	{
		setPortState(PORT13, HIGH);
	}
	else //LOW
	{
		setPortState(PORT13, LOW);
	}
	*/
}

