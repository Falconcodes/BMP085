
#include <mega328p.h>

// Declare your global variables here

// Standard Input/Output functions
#include <stdio.h>

// TWI functions
#include <twi.h>

#include <delay.h>

#define TWI_SLAVE_ADDR 0b1110111
  
#define TWI_SLAVE_READ 0xEF


void main(void)
{
// Declare your local variables here

char tx_buffer[3]; // transmission buffer
char rx_buffer[3]; // receive buffer

unsigned long press, msb, lsb, lsxb;

unsigned long  pressure_=0;
signed long  temperature_=0;

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: Off
// USART Transmitter: On
// USART0 Mode: Asynchronous
// USART Baud Rate: 9600
UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (0<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
UBRR0H=0x00;
UBRR0L=0x67;

#asm("sei") // Global enable interrupts

// TWI initialization
// Mode: TWI Master
// Bit Rate: 100 kHz
twi_master_init(100);


while (1)
 {
 tx_buffer[0]=0xF4; //register address                    
 tx_buffer[1]=0xF4; //precision setting (0x34 + oss)
 
 if (twi_master_trans( TWI_SLAVE_ADDR, &tx_buffer[0], 2, 0, 0 )) 
    {
     delay_ms(30);
     
     tx_buffer[0]=0xF6;
     twi_master_trans( TWI_SLAVE_ADDR, &tx_buffer[0], 1, &rx_buffer[0], 3 );
          
     msb  = rx_buffer[0];
     lsb  = rx_buffer[1];
     lsxb = rx_buffer[2];
     
     press = (msb<<11) + (lsb<<3) + (lsxb>>5);
     
     printf("Response: %li", press/3 );
    }
 
 else printf("Error");
 
 delay_ms(500);
 
 }
}
