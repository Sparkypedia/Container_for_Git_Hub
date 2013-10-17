/* test using spi protocol for text on lcd display */
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>


char mydata[] = {15,17,19,22,24,26,27,28,29,29,29,29,28,27,25,23,21,19,17,14,12,9,7,
5,3,2,1,0,0,0,0,1,2,4,6,8,10,13,15,18,20,22,24,26,28,29,29,29,29,29,28,26,25,23,21,18,
16,13,11,9,6,4,3,1,0,0,0,0,0,1,3,4,6,9,11,14,16,18,21,23,25,27,28,29,29,29,29,29,27,26,
24,22,20,18,15,13,10,8,6,4,2,1,0,0,0,0,1,2,3,5,7,9,12,14,17,19,21,24,25,27,28,29,29,29,
29,28,27,26,24,22,19,17};

#define DD_LED		PINA0
#define DD_RS		PINA1
#define DD_RESET	PINA2

#define DD_CS		PINB4
#define DD_SCLK		PINB7
#define DD_SID		PINB5

#define DDR_SPI		PORTA

int data[132];

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

int main(void)
	{
	int p;
	int c;
	SPI_MasterInit();
	SPI_MasterTransmitcontrol(175); // Display ON
	//SPI_MasterTransmitcontrol(167); // Reverse ON
	SPI_MasterTransmitcontrol(226); // Reset Display
	SPI_MasterTransmitcontrol(47); // power control
	clearlcd();
	while(1)
		{
		for(c=0;c<4;c++)
		{
		SPI_MasterTransmitcontrol(16);
		SPI_MasterTransmitcontrol(0);
		SPI_MasterTransmitcontrol(176+c); //page
		SPCR = (1<<SPE) | (1<<MSTR) | (1<<DORD) ;
		for (p=0;p<132;p++)
			{
			if((mydata[p] -8*c)>0) SPI_MasterTransmitdata(0x80 >> (mydata[p] - 8*c));
			else SPI_MasterTransmitdata(0);
			//SPI_MasterTransmitdata(fontdata[1067+'k'+(c*11*12)]);
			}
		SPCR = (1<<SPE) | (1<<MSTR) & ~(1<<DORD);
		}
		}
	}
