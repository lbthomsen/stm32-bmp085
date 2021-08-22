/*
 * bmp085.c
 *
 *  Created on: Aug 21, 2021
 *      Author: Lars Boegild Thomsen <lbthomsen@gmail.com>
 */

#include "stdio.h"
#include "main.h"
#include "BMP085.h"

uint32_t state = 0;
uint32_t state_cnt = 0;

HAL_StatusTypeDef BMP085_init(BMP085_HandleTypeDef *dev,
		I2C_HandleTypeDef *i2cHandle) {

	dev->i2cHandle = i2cHandle;

	uint8_t buf[22] = { 0 };
	buf[0] = BMP085_CALIBRATION_DATA_ADDRESS;

	HAL_I2C_Master_Transmit(dev->i2cHandle, BMP085_I2C_ADDR, &buf[0], 1, 10);
	HAL_I2C_Master_Receive(dev->i2cHandle, BMP085_I2C_ADDR, &buf[0], sizeof(buf), 10); // Fetch all 11 words in one go

	// Gotta do this manually to flip the bytes
	dev->calibration_data.ac1 = (buf[0] << 8) + buf[1];
	dev->calibration_data.ac2 = (buf[2] << 8) + buf[3];
	dev->calibration_data.ac3 = (buf[4] << 8) + buf[5];
	dev->calibration_data.ac4 = (buf[6] << 8) + buf[7];
	dev->calibration_data.ac5 = (buf[8] << 8) + buf[9];
	dev->calibration_data.ac6 = (buf[10] << 8) + buf[11];
	dev->calibration_data.b1 = (buf[12] << 8) + buf[13];
	dev->calibration_data.b2 = (buf[14] << 8) + buf[15];
	dev->calibration_data.mb = (buf[16] << 8) + buf[17];
	dev->calibration_data.mc = (buf[18] << 8) + buf[19];
	dev->calibration_data.md = (buf[20] << 8) + buf[21];

	dev->delay = 100;

	return HAL_OK;
}

HAL_StatusTypeDef BMP085_tick(BMP085_HandleTypeDef *dev) {

	uint8_t buf[4] = {0};

	// Run the sequential state machine on each "tick"
	switch (state) {
	case BMP085_STATE_START:
		//DBG("BMP085 state machine start");
		state_cnt = 0;
		state = BMP085_STATE_REQUEST_TEMP;
		break;
	case BMP085_STATE_REQUEST_TEMP:
		buf[0] = BMP085_REQUEST_ADDRESS;
		buf[1] = BMP095_REQUEST_TEMP;
		HAL_I2C_Master_Transmit(dev->i2cHandle, BMP085_I2C_ADDR, (uint8_t *)&buf, 2, 10);
		state_cnt = 0;
		state = BMP085_STATE_WAIT_TEMP;
		break;
	case BMP085_STATE_WAIT_TEMP:
		state_cnt++;
		if (state_cnt >= 5) {
			state_cnt = 0;
			state = BMP085_STATE_GET_TEMP;
		}
		break;
	case BMP085_STATE_GET_TEMP:
		buf[0] = BMP085_GET_ADDRESS;
		HAL_I2C_Master_Transmit(dev->i2cHandle, BMP085_I2C_ADDR, *&buf, 1, 10);
		HAL_I2C_Master_Receive(dev->i2cHandle, BMP085_I2C_ADDR, (uint8_t*) &buf, 2, 10);

		int32_t ut = (buf[0] << 8) + buf[1];
		int32_t x1 = ((ut - dev->calibration_data.ac6) * dev->calibration_data.ac5 )  >> 15;
		int32_t x2 =  ((int32_t)dev->calibration_data.mc << 11) / (x1 + dev->calibration_data.md);
		int32_t b5 = x1 + x2;

		dev->temperature = (b5 + 8) / (2^4);

		state_cnt = 0;
		state = BMP085_STATE_REQUEST_PRESSURE;
		break;
	case BMP085_STATE_REQUEST_PRESSURE:
		buf[0] = BMP085_REQUEST_ADDRESS;
		buf[1] = BMP085_REQUEST_PRESSURE;
		HAL_I2C_Master_Transmit(dev->i2cHandle, BMP085_I2C_ADDR, (uint8_t *)&buf, 2, 10);
		state_cnt = 0;
		state = BMP085_STATE_WAIT_PRESSURE;
		break;
	case BMP085_STATE_WAIT_PRESSURE:
		state_cnt++;
		if (state_cnt >= 15) {
			state_cnt = 0;
			state = BMP085_STATE_GET_PRESSURE;
		}
		break;
	case BMP085_STATE_GET_PRESSURE:
		buf[0] = BMP085_GET_ADDRESS;
		HAL_I2C_Master_Transmit(dev->i2cHandle, BMP085_I2C_ADDR, *&buf, 1, 10);
		HAL_I2C_Master_Receive(dev->i2cHandle, BMP085_I2C_ADDR, (uint8_t*) &buf, 3, 10);

		int32_t UP = (buf[0] << 16) + (buf[1] << 8) + buf[2];

		state_cnt = 0;
		state = BMP085_STATE_SLEEP;
		break;
	case BMP085_STATE_SLEEP:
		state_cnt++;
		if (state_cnt > dev->delay) {
			state_cnt = 0;
			state = BMP085_STATE_START;
		}
		break;
	default:
		DBG("State machine gone FUBAR");
		state = BMP085_STATE_START;
	}

	return HAL_OK;
}
