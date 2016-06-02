
#include "stdafx.h"
#include "../libutils/logging.h"
#include "../libutils/Exception.h"
#include "../librf/spidev_lib++.h"
#include "../librf/RFM69OOKregisters.h"
#include "../librf/RFM69OOK.h"


//SPI *mySPI = NULL;
/*
void writeReg(unsigned char reg, unsigned char val)
{
	unsigned char data[2], tmp;
	data[0] = reg | 0x80;
	data[1] = val;
	int res = mySPI->xfer2(data, 2, &tmp, 0);
	//printf("write res = %d", res);
}

unsigned char readReg(unsigned char reg)
{
	unsigned char val, tmp;
	tmp = reg & 0x7F;
	int res = mySPI->xfer2(&tmp, 1, &val, 1);
	//printf("xfer2 res = %d", res);
	return val;
}
*/
void dumpRegs(RFM69OOK *rfm)
{
	for (unsigned char reg=1;reg<0xF;reg++)
	{
		unsigned char val = rfm->readReg(reg);
		printf("Reg %x = %x (%d)\n", reg, val, val);
	}
}

void Rfm69Test()
{

    printf("Rfm69Test\n");

	spi_config_t spi_config;
  spi_config.mode=0;
  spi_config.speed=500000;
  spi_config.delay=0;
  spi_config.bits_per_word=8;

	uint8_t tx_buffer[32];
	uint8_t rx_buffer[32];



  SPI mySPI("/dev/spidev32766.0",&spi_config);

  if (mySPI.begin())
  {
  	    printf("mySPI->begin()\n");

  	/*
    memset(tx_buffer,0,32);
    memset(rx_buffer,0,32);


	//writeReg(REG_OPMODE, (self.readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER)
	//	self.spi.write_then_read([addr | 0x80, value], 0)

    sprintf((char*)tx_buffer,"hello world");
    printf("sending %s, to spidev2.0 in full duplex \n ",(char*)tx_buffer); 
    mySPI->xfer(tx_buffer,strlen((char*)tx_buffer),rx_buffer,strlen((char*)tx_buffer));
    printf("rx_buffer=%s\n",(char *)rx_buffer);
    //mySPI->end();
    */
    //writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);

    RFM69OOK rfm(&mySPI);
    printf("Reg REG_OPMODE = %x\n", rfm.readReg(REG_OPMODE));
    printf("Reg REG_IRQFLAGS1 = %x\n", rfm.readReg(REG_IRQFLAGS1));
    rfm.writeReg(REG_OPMODE, (rfm.readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
    printf("Reg REG_OPMODE = %x\n", rfm.readReg(REG_OPMODE));

    rfm.initialize();
    rfm.receiveBegin();

/*    printf("Reg REG_SYNCVALUE1 = %x\n", readReg(REG_SYNCVALUE1));
    writeReg(REG_SYNCVALUE1, 0xAA);
    printf("Reg REG_SYNCVALUE1 = %x\n", readReg(REG_SYNCVALUE1));
    writeReg(REG_SYNCVALUE1, 0xBB);
    printf("Reg REG_SYNCVALUE1 = %x\n", readReg(REG_SYNCVALUE1));
*/
    dumpRegs(&rfm);

  }
}