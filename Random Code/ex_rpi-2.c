/* Simple example which puts text on lcd display 		*/
/* written for RaspberryPi and makes use of     		*/
/* wiringPI routines detailed here:-             		*/
/* https://projects.drogon.net/raspberry-pi/wiringpi/  		*/
/*								*/
/* NOTE: before running make sure SPI code is laoded with	*/
/* > gpio load spi						*/
/*								*/
/* ALSO NOTE: No support if offered or implied with this code.	*/
/* Use at your own risk and don't blame me if it doesn't work..	*/
/*								*/
/* Display should be wired to PI board as follows		*/
/*								*/
/* Module pin			Raspberry pin	wiringPI#	*/
/* 1 VDD                        1				*/
/* 2 VSS                        6				*/
/* 3 PS (low for serial)        6				*/
/* 4 DB7/SID                    19		12		*/
/* 5 DB6/SCLK                   23		14		*/
/* 6-11 DB5-DB0                 N/C				*/
/* 12 E_RD                      N/C				*/
/* 13 RW_W                      N/C				*/
/* 14 RS (l=ctrl, 1=data        5		9		*/
/* 15 /RES reset input          3		8		*/
/* 16 /CS                       6				*/
/* 17 Led+      requires external bias 				*/
/* 18 Led-                      6				*/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "font5x7int.h"

int main (void)
{
uint8_t spiData [10] ;

// set interface speed
if (wiringPiSPISetup (0,100000) < 0)
	{
	fprintf (stderr, "Unable to open SPI device 0: %s\n", strerror (errno)) ;
	exit (1) ;
	}

wiringPiSetupSys() ;
wiringPiSetup() ;

/* setup output ports */
pinMode(8, OUTPUT); /* reset */
pinMode(9, OUTPUT); /* rs */

/* reset LCD display */
digitalWrite(8,0);
delayMicroseconds (100) ;
digitalWrite(8,1);

void write_cmd(int c)
	{
	digitalWrite(9,0); // set rs bit for cmd
	spiData[0]=c;
	wiringPiSPIDataRW (0, spiData, 1) ;
	}
void write_char(char Char)
	{
	int c;
	digitalWrite(9,1); // set rs bit for cmd
	for(c=0;c<5;c++)	// font set is 5 bits wide
		{
		spiData[c]=(font5x7int[Char*5+c]);
		}	
	spiData[5]=0;
	wiringPiSPIDataRW (0, spiData, 6) ;
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
	}

/* write controls to LCD display */
write_cmd(175);	//display on
//write_cmd(167);	// reverse
write_cmd(226);	// reset display
write_cmd(47); 	// power control
write_cmd(179); 	// set row (180-row)
write_cmd(163); 	// LCD bias select
delayMicroseconds (100) ;

//22-char"1234567890123456789012"
write_cmd(179); 	// set row 1
write_cmd(16);		// column 0
write_str("For datasheets and");
write_cmd(178); 	// set row 2
write_cmd(16);		//column 0
write_str("examples please see");
write_cmd(177); 	// set row 3
write_cmd(16);	//column 0
write_str("www.retrosemi.co.uk/");
write_cmd(176); 	// set row (180-row)
write_cmd(16);	//column 0
write_str("lcd132x32 ");
return 0 ;
}
