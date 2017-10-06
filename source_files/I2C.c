/*
 * I2C.c
 *
 *  Created on: Oct 4, 2017
 *      Author: subhr
 */



#include "I2C.h"

uint32_t global_temperature = DEFAULT_TEMPERATURE;

void I2Cinit()
{
	GPIO_PinOutSet(SI7021ENABLE_port, SI7021ENABLE_pin);
	unsigned long i = 0;
	for(i=1; i<100000;i++);

	I2C_Init_TypeDef i2cInit =
	{
		.enable = false,
		.master = true,
		.refFreq = 0,
		.freq = I2C_FREQ_STANDARD_MAX,
		.clhr = i2cClockHLRStandard
	};


	I2C0->ROUTELOC0 |= (I2C_ROUTELOC0_SDALOC_LOC16 | I2C_ROUTELOC0_SCLLOC_LOC14);
	I2C0->ROUTEPEN |= (I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN);
	I2C_Init(I2C0, &i2cInit);

	/*Toggle SCL 9 times to reset a slave*/
	for (int i=0;i<9;i++)
	{
	GPIO_PinOutClear(SCL_port, SCL_pin);
	GPIO_PinOutSet(SCL_port, SCL_pin);
	}

	/*Reset the I2C bus*/
	if (I2C0->STATE & I2C_STATE_BUSY)
	{
		I2C0->CMD = I2C_CMD_ABORT;
	}

	I2C_IntClear(I2C0, (I2C_IFC_START | I2C_IFC_RSTART | I2C_IFC_ADDR | I2C_IFC_TXC | I2C_IFC_ACK | \
			I2C_IFC_NACK | I2C_IFC_MSTOP | I2C_IFC_ARBLOST | I2C_IFC_BUSERR | I2C_IFC_BUSHOLD |\
			I2C_IFC_TXOF | I2C_IFC_RXUF | I2C_IFC_BITO | I2C_IFC_CLTO | I2C_IFC_SSTOP | I2C_IFC_RXFULL | \
			I2C_IFC_CLERR));
	I2C_IntDisable(I2C0, (I2C_IEN_START | I2C_IEN_RSTART | I2C_IEN_ADDR | I2C_IEN_TXC | I2C_IEN_ACK | \
			I2C_IEN_NACK | I2C_IEN_MSTOP | I2C_IEN_ARBLOST | I2C_IEN_BUSERR | I2C_IEN_BUSHOLD |\
			I2C_IEN_TXOF | I2C_IEN_RXUF | I2C_IEN_BITO | I2C_IEN_CLTO | I2C_IEN_SSTOP | I2C_IEN_RXFULL | \
			I2C_IEN_CLERR));
	//I2C_IntEnable(I2C0, (I2C_IEN_ACK | I2C_IEN_NACK));
	//NVIC_EnableIRQ(I2C0_IRQn);

}

uint16_t I2C_read(uint8_t register_address)
{
	uint16_t read_data;
	I2C0->TXDATA = ((SI7021_ADDRESS << 1)| WRITE_COMMAND);
	I2C_start();
	while((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = register_address;
	I2C_start();
	while((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = ((SI7021_ADDRESS << 1)| READ_COMMAND);
	I2C_start();
	while((I2C0->IF & I2C_IF_ACK) == 0);
	I2C0->IFC = I2C_IFC_ACK;

	while((I2C0->IF & I2C_IF_RXDATAV ) == 0);
	read_data = I2C0->RXDATA;

	read_data = read_data << 8;

	I2C0->CMD = I2C_CMD_ACK;
	while((I2C0->IF & I2C_IF_RXDATAV ) == 0);
	read_data |= (I2C0->RXDATA)>>2;

	I2C0->CMD = I2C_CMD_NACK;
	I2C_stop();
	return (read_data);


}

inline void I2C_start()
{
	I2C0->CMD = I2C_CMD_START;
}

inline void I2C_stop()
{
	I2C0->CMD = I2C_CMD_STOP;
}


