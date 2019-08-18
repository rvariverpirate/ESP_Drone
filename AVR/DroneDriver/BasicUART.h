#define BAUDPRESCALLER (((F_CPU/(BAUDRATE * 16UL)))) - 1

void USART_init(void)
{
	UBRR0H = (uint8_t)((BAUDPRESCALLER>>8));
	UBRR0L = (uint8_t)(BAUDPRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_send(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void USART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART_send(*StringPtr);
		StringPtr++;
	}
	//StringPtr ++;
}

unsigned char USART_receive(void)
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void USART_getstring(char readString[15]){
	int i = 0;
	if(USART_receive() == 's'){// start
		readString[i] = 's';
		i ++;
		while(1){
			readString[i] = USART_receive();
			if(readString[i] == 'e'){
				break;
			}
		i++;
		}
	}
}

