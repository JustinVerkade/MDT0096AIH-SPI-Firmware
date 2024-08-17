/*
 * MDT0096AIH.c
 *
 *  Created on: 27 jan. 2023
 *      Author: J.Verkade
 */

#include "MDT0096AIH.h"
#include <stdlib.h>

//#################//
//- Macro defines -//
//#################//

#define MDT0096AIH_ActiveRS(me) HAL_GPIO_WritePin(me->pin_bank_rs, me->pin_num_rs, GPIO_PIN_SET)
#define MDT0096AIH_InactiveRS(me) HAL_GPIO_WritePin(me->pin_bank_rs, me->pin_num_rs, GPIO_PIN_RESET)
#define MDT0096AIH_ActiveRST(me) HAL_GPIO_WritePin(me->pin_bank_rst, me->pin_num_rst, GPIO_PIN_RESET)
#define MDT0096AIH_InactiveRST(me) HAL_GPIO_WritePin(me->pin_bank_rst, me->pin_num_rst, GPIO_PIN_SET)
#define MDT0096AIH_ActiveCS(me) HAL_GPIO_WritePin(me->pin_bank_cs, me->pin_num_cs, GPIO_PIN_RESET)
#define MDT0096AIH_InactiveCS(me) HAL_GPIO_WritePin(me->pin_bank_cs, me->pin_num_cs, GPIO_PIN_SET)

//#################//
//- Data transmit -//
//#################//

static void MDT0096AIH_TransmitData(MDT0096AIH_t *me, uint8_t data)
{
	while(!(me->spih->Instance->SR & (1<<1)));
	me->spih->Instance->DR = data;
}

//####################//
//- Command transmit -//
//####################//

static void MDT0096AIH_TransmitCmd(MDT0096AIH_t *me, uint8_t cmd)
{
	// init port
	MDT0096AIH_InactiveRS(me);
	MDT0096AIH_ActiveCS(me);

	// transfer data
	uint8_t buffer[] = {cmd};
	HAL_SPI_Transmit(me->spih, buffer, 1, 1000);

	// deactivate port
	MDT0096AIH_InactiveCS(me);
	MDT0096AIH_ActiveRS(me);
}

//#########################//
//- Command byte transmit -//
//#########################//

static void MDT0096AIH_TransmitCmdByte(MDT0096AIH_t *me, uint8_t cmd, uint8_t *data, uint8_t len)
{
	// init port
	MDT0096AIH_InactiveRS(me);
	MDT0096AIH_ActiveCS(me);

	// transfer data
	uint8_t buffer[] = {cmd};
	HAL_SPI_Transmit(me->spih, buffer, 1, 1000);

	// set transmit data
	MDT0096AIH_ActiveRS(me);

	// transmit data
	HAL_SPI_Transmit(me->spih, data, len, 1000);

	// deactivate port
	MDT0096AIH_InactiveCS(me);
}

//########//
//- Init -//
//########//

void MDT0096AIH_Init(MDT0096AIH_t *me,
		SPI_HandleTypeDef *spih,
		GPIO_TypeDef *pin_bank_rs,
		uint16_t pin_num_rs,
		GPIO_TypeDef *pin_bank_rst,
		uint16_t pin_num_rst,
		GPIO_TypeDef *pin_bank_cs,
		uint16_t pin_num_cs)
{
	// link struct data

	// SPI
	me->spih = spih;

	// GPIO
	me->pin_bank_rs = pin_bank_rs;
	me->pin_num_rs = pin_num_rs;
	me->pin_bank_rst = pin_bank_rst;
	me->pin_num_rst = pin_num_rst;
	me->pin_bank_cs = pin_bank_cs;
	me->pin_num_cs = pin_num_cs;

	// set prefixes
	MDT0096AIH_ActiveRS(me);
	MDT0096AIH_InactiveCS(me);
	MDT0096AIH_InactiveRST(me);

	// hardware reset
	MDT0096AIH_Reset(me);

	// init display
	MDT0096AIH_TransmitCmd(me, MDT0096AIH_SLPOUT); // get out of sleep

	HAL_Delay(100);

	MDT0096AIH_TransmitCmd(me, MDT0096AIH_NORON);  // normal display mode on

	HAL_Delay(100);

	MDT0096AIH_TransmitCmd(me, MDT0096AIH_DISPON); // display on

	HAL_Delay(100);

	MDT0096AIH_TransmitCmd(me, MDT0096AIH_INVON); // invert buffer

	HAL_Delay(100);

	// set BGR to RGB color code
	// invert column index
	uint8_t madctl_buffer[] = {0b00001000};
	MDT0096AIH_TransmitCmdByte(me, MDT0096AIH_MADCTL, madctl_buffer, 1);

	HAL_Delay(100);

	// define row window 0 -> 160
	uint8_t raset_buffer[] = {0, 1, 0, 160};
	MDT0096AIH_TransmitCmdByte(me, MDT0096AIH_RASET, raset_buffer, 4);

	HAL_Delay(100);

	// define column window
	uint8_t caset_buffer[] = {0, 26, 0, 25+80};
	MDT0096AIH_TransmitCmdByte(me, MDT0096AIH_CASET, caset_buffer, 4);

	HAL_Delay(100);

	MDT0096AIH_TransmitCmd(me, MDT0096AIH_RAMWR);  // display write

	MDT0096AIH_ActiveCS(me);
	for(uint16_t i=0; i<160*80; i++)
	{
		MDT0096AIH_TransmitData(me, 0xFF);
		MDT0096AIH_TransmitData(me, 0xFF);
		MDT0096AIH_TransmitData(me, 0xFF);
	}
	MDT0096AIH_InactiveCS(me);

	HAL_Delay(100);

	MDT0096AIH_ActiveCS(me);
}

//#########//
//- Reset -//
//#########//

void MDT0096AIH_Reset(MDT0096AIH_t *me)
{
	MDT0096AIH_ActiveRST(me);
	HAL_Delay(100);
	MDT0096AIH_InactiveRST(me);
	HAL_Delay(200);
}

// write pixel
void MDT0096AIH_WritePixel(MDT0096AIH_t *me, uint8_t *buffer)
{
	MDT0096AIH_TransmitData(me, buffer[0]);
	MDT0096AIH_TransmitData(me, buffer[1]);
	MDT0096AIH_TransmitData(me, buffer[2]);
}


