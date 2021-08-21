/*
 * bmp085.h
 *
 *  Created on: Aug 21, 2021
 *      Author: lth
 */

#ifndef BMP085_H_
#define BMP085_H_

#define BMP085_I2C_ADDR 0xee // Device write address
#define BMP085_CALIBRATION_DATA_ADDRESS 0xaa

typedef struct {
    int16_t ac1;
	int16_t ac2;
	int16_t ac3;
	uint16_t ac4;
	uint16_t ac5;
	uint16_t ac6;
	int16_t b1;
	int16_t b2;
	int16_t mb;
	int16_t mc;
	int16_t md;
} BMP085_calibration_data_s;

typedef struct {
	I2C_HandleTypeDef *i2cHandle;
	BMP085_calibration_data_s calibration_data;
	HAL_StatusTypeDef status;
	int32_t temperature;
	int32_t pressure;
} BMP085_HandleTypeDef;

HAL_StatusTypeDef BMP085_init(BMP085_HandleTypeDef *dev, I2C_HandleTypeDef *i2cHandle);

HAL_StatusTypeDef BMP085_tick(); // Call every ms if possible

#endif /* BMP085_H_ */
