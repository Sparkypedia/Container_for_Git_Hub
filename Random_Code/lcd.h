
#ifndef LCD_H
#define LCD_H

//function prototypes..
extern int revb (int n);
extern void initlcd (void);
extern void clearlcd (void);
extern void write_char (char Char);
extern void write_str (char * Str);
extern void changelcdrow (int row);
extern void SPI_MasterInit (void);
extern void SPI_MasterTransmitdata (char cData);
extern void SPI_MasterTransmitcontrol (char cData);

#endif
