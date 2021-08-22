/*
 * bmp085.c
 *
 *  Created on: Aug 21, 2021
 *      Author: lth
 */

#include "main.h"
#include "BMP085.h"

uint8_t state = 0;

HAL_StatusTypeDef BMP085_init(BMP085_HandleTypeDef *dev, I2C_HandleTypeDef *i2cHandle) {

	dev->i2cHandle = i2cHandle;

	uint8_t address = BMP085_CALIBRATION_DATA_ADDRESS;
	uint8_t buf[22] = {0};

	HAL_I2C_Master_Transmit(dev->i2cHandle, BMP085_I2C_ADDR, &address, 1, 10);
	HAL_I2C_Master_Receive(dev->i2cHandle, BMP085_I2C_ADDR, (uint8_t*)&buf, sizeof(buf), 10); // Fetch all 11 values in one go

	// Gotta do this manually to flip the bytes
	dev->calibration_data.ac1 = (buf[0] << 8) + buf[1];
	dev->calibration_data.ac2 = (buf[2] << 8) + buf[3];
	dev->calibration_data.ac3 = (buf[4] << 8) + buf[5];
	dev->calibration_data.ac4 = (buf[6] << 8) + buf[7];
	dev->calibration_data.ac5 = (buf[8] << 8) + buf[9];
	dev->calibration_data.ac6 = (buf[10] << 8) + buf[11];
	dev->calibration_data.b1  = (buf[12] << 8) + buf[13];
	dev->calibration_data.b2  = (buf[14] << 8) + buf[15];
	dev->calibration_data.mb  = (buf[16] << 8) + buf[17];
	dev->calibration_data.mc  = (buf[18] << 8) + buf[19];
	dev->calibration_data.md  = (buf[20] << 8) + buf[21];

	return HAL_OK;
}

HAL_StatusTypeDef BMP085_tick(BMP085_HandleTypeDef *dev) {

	switch (state) {
	case BMP085_STATE_START:

		state = BMP085_STATE_REQUEST_TEMP;
		break;
	case BMP085_STATE_REQUEST_TEMP:

		break;
	}

	return HAL_OK;
}
