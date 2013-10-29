/* test using spi protocol for text on lcd display */
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "font5x7int.h"


#define DD_LED		PINA0
#define DD_RS		PINA1
#define DD_RESET	PINA2

#define DD_CS		PINB4
#define DD_SCLK		PINB7
#define DD_SID		PINB5

#define DDR_SPI		PORTA

//following needed as fonts are upside down
int revb(int n) {
n = ((n >> 1) & 0x55) | ((n << 1) & 0xaa);
n = ((n >> 2) & 0x33) | ((n << 2) & 0xcc);
n = ((n >> 4) & 0x0f) | ((n << 4) & 0xf0);
return n;
}

void SPI_MasterInit(void)
	{
	// set up port directions.
	DDRA = (1<<DD_RS) | (1<<DD_RESET);
	//DDRA = (1<<DD_LED) | (1<<DD_RS) | (1<<DD_RESET);
	DDRB = (1<<DD_CS) | (1<<DD_SCLK) | (1<<DD_SID);

	SPCR = (1<<SPE) | (1<<MSTR) ;
	//SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1);

	PORTB &= ~(1<<DD_CS);	// enable lcd
	//toggle reset X->L->H
	PORTA &= ~(1<<DD_RESET); _delay_ms(300); PORTA |=  (1<<DD_RESET);
	}

void SPI_MasterTransmitdata(char cData)
	{
	PORTA |=  (1<<DD_RS); // High for Data
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)));
	}

void SPI_MasterTransmitcontrol(char cData)
	{
	PORTA &= ~(1<<DD_RS); // Low for control
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)));
	}
void initlcd()
	{
	SPI_MasterTransmitcontrol(175); // Display ON
	SPI_MasterTransmitcontrol(226); // Reset Display
	SPI_MasterTransmitcontrol(47); // power control
	}
void clearlcd()
	{
	int p;
	int c;
	for(p=0;p<4;p++)
		{
		SPI_MasterTransmitcontrol(176+p); //page
		SPI_MasterTransmitcontrol(16);
		SPI_MasterTransmitcontrol(0);
		for(c=0;c<132;c++)
			SPI_MasterTransmitdata(0);
		}
	}
void write_char(char Char)
	{
	int c;
	for(c=0;c<5;c++)
		{
		SPI_MasterTransmitdata(revb(font5x7int[Char*5+c]));
		}	
		SPI_MasterTransmitdata(0);
	}
void write_str(char *Str)
	{
	int c;
	for(c=0;c<strlen(Str);c++)
		{
		write_char(Str[c]);
		}
	while(c<23) {
		write_char(' '); // fill with white space
		c++;
		}
	SPCR = (1<<SPE) | (1<<MSTR) & ~(1<<DORD);
	}
void changelcdrow(int row)
	{
	SPI_MasterTransmitcontrol(16);
	SPI_MasterTransmitcontrol(0);
	SPI_MasterTransmitcontrol(176+4-row); //rows 1,2,3,4 are valid
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<DORD) ;
	}

int main(void)
	{
	int p;
	int c;
	SPI_MasterInit();
	initlcd();
	clearlcd();

	changelcdrow(1);
	//write_char('K');
	write_str("This is a test....");
	changelcdrow(2);
	write_str("abcdefghijklmnopqrstuvwxyz");
	//SPCR = (1<<SPE) | (1<<MSTR) & ~(1<<DORD);
	}
