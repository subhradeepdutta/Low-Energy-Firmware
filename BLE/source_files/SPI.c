/********************************************************************************
* File ​Name 	  : SPI.c
* File Description: This file includes the function to initialize the USART1 for
* 					SPI transmission to communicate with the BMA280 and read as
* 					well as write registers from the BMA280 over the SPI bus
* Author		  : Subhradeep Dutta
* Date			  : 09/29/2017
* Compiler : arm-none-eabi-gcc
* Linker   : arm-none-eabi-ld
* Debugger : gdb
********************************************************************************/
#include "SPI.h"

void SPI_init()
{
	USART_InitSync_TypeDef spiInit =
	{
			  /** Specifies whether TX and/or RX shall be enabled when init completed. */
			  .enable = usartDisable,
			  /**
			   * USART/UART reference clock assumed when configuring baudrate setup. Set
			   * it to 0 if currently configurated reference clock shall be used.
			   */
			  .refFreq = 0,
			  /** Desired baudrate. */
			  .baudrate = 500000,
			  /** Number of databits in frame. */
			  .databits = usartDatabits8,
			  /** Select if to operate in master or slave mode. */
			  .master = true,
			  /** Select if to send most or least significant bit first. */
			  .msbf =  true,
			  /** Clock polarity/phase mode. */
			  .clockMode = usartClockMode0,
			  .autoTx = false,
			   /** Auto CS enabling */
			  .autoCsEnable = true,
			  .autoCsHold = 1,
			  .autoCsSetup = 1,
	};
	USART_InitSync(USART1, &spiInit);

	 USART1->ROUTELOC0 |= USART_ROUTELOC0_CLKLOC_LOC11 | USART_ROUTELOC0_TXLOC_LOC11 | USART_ROUTELOC0_RXLOC_LOC11 | USART_ROUTELOC0_CSLOC_LOC11;
	 USART1->ROUTEPEN |= USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_CSPEN;
	 USART1->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;
	 USART_Enable(USART1, usartEnable);
}



uint8_t BMA280_RegisterRead(USART_TypeDef *usart, uint8_t address)
{
	uint16_t data;
	uint32_t tmp;

	data = 0x0080;

	data = data | address;

  while (!(usart->STATUS & USART_STATUS_TXBL));
  usart->TXDOUBLE = (uint32_t)data;
  while (!(usart->STATUS & USART_STATUS_TXC));
  tmp = usart->RXDOUBLE;

  tmp = tmp >> 8;
  return (uint8_t)tmp;

}

void BMA280_RegisterWrite(USART_TypeDef *usart, uint8_t address, uint8_t data)
{
  uint16_t tmp;

  tmp = (((uint16_t) data) << 8) | ((uint16_t) address);

  while (!(usart->STATUS & USART_STATUS_TXBL)) ;

  usart->TXDOUBLE = (uint32_t) tmp;

  while (!(usart->STATUS & USART_STATUS_TXC)) ;

  while ( (usart->STATUS & USART_STATUS_RXDATAV))
  tmp = usart->RXDOUBLE;

}
