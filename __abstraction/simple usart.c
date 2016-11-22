#include "simple_usart/uart.h"

void configUsart()
{
	//Initialize UART
	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;
	//use --> printf("ABC");
}
