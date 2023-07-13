/*
 * MDT0096AIH.h
 *
 *  Created on: 27 jan. 2023
 *      Author: J.Verkade
 */

#ifndef INC_DRIVERS_MDT0096AIH_H_
#define INC_DRIVERS_MDT0096AIH_H_

#include <main.h>


// register defines
#define MDT0096AIH_NOP		0x00
#define MDT0096AIH_SLPIN 	0x10
#define MDT0096AIH_SLPOUT 	0x11
#define MDT0096AIH_PTLON	0x12
#define MDT0096AIH_NORON	0x13
#define MDT0096AIH_INVOFF	0x20
#define MDT0096AIH_INVON	0x21
#define MDT0096AIH_GAMSET	0x26
#define MDT0096AIH_DISPOFF	0x28
#define MDT0096AIH_DISPON	0x29
#define MDT0096AIH_CASET	0x2A
#define MDT0096AIH_RASET	0x2B
#define MDT0096AIH_RAMWR	0x2C
#define MDT0096AIH_RGBSET	0x2D
#define MDT0096AIH_RAMRD	0x2E
#define MDT0096AIH_PTLAR	0x30
#define MDT0096AIH_SCRLAR	0x33
#define MDT0096AIH_TEOFF	0x34
#define MDT0096AIH_TEON		0x35
#define MDT0096AIH_MADCTL	0x36
#define MDT0096AIH_VSCSAD	0x37
#define MDT0096AIH_IDMOFF	0x38
#define MDT0096AIH_IDMON	0x39
#define MDT0096AIH_COLMOD	0x3A
#define MDT0096AIH_RDID1	0xDA
#define MDT0096AIH_RDID2	0xDB
#define MDT0096AIH_RDID3	0xDC

// class struct
typedef struct MDT0096AIH_t MDT0096AIH_t;
struct MDT0096AIH_t
{
	// spi bus
	SPI_HandleTypeDef *spih;

	// gpio pins
	GPIO_TypeDef *pin_bank_rs;
	uint16_t pin_num_rs;
	GPIO_TypeDef *pin_bank_rst;
	uint16_t pin_num_rst;
	GPIO_TypeDef *pin_bank_cs;
	uint16_t pin_num_cs;
};

// init
void MDT0096AIH_Init(MDT0096AIH_t *me,
		SPI_HandleTypeDef *spih,
		GPIO_TypeDef *pin_bank_rs,
		uint16_t pin_num_rs,
		GPIO_TypeDef *pin_bank_rst,
		uint16_t pin_num_rst,
		GPIO_TypeDef *pin_bank_cs,
		uint16_t pin_num_cs);

// actions
void MDT0096AIH_Reset(MDT0096AIH_t *me);

// write pixel
void MDT0096AIH_WritePixel(MDT0096AIH_t *me, uint8_t *buffer);

#endif /* INC_DRIVERS_MDT0096AIH_H_ */
