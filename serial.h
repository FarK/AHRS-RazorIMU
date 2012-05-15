#ifndef SERIAL_H
#define SERIAL_H

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
void SPI_SlaveInit(void);
char SPI_SlaveReceive(void);

#endif
