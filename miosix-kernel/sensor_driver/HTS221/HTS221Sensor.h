/**
 ******************************************************************************
 * @file    HTS221Sensor.h
 * @author  CLab
 * @version V1.0.0
 * @date    5 August 2016
 * @brief   Abstract class of an HTS221 Humidity and Temperature sensor.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */


#ifndef _HTS221_H
#define _HTS221_H

#include "HTS221_driver.h"
#include "i2c_helper.h"


 /*---------  CLASS DECLARATION  -----------------------------------------------*/
class HTS221Sensor {

public:
	HTS221Sensor(uint8_t addr = HTS221_I2C_ADDRESS) { address = addr; }
	virtual ~HTS221Sensor() {}
	int readId(uint8_t* id);
	int	getTemperature(float* dat);
	int getHumidity(float* dat);
	int getOdr(float* odr);
	int setOdr(float odr);
	int enable(void);
	int disable(void);
	int reset(void* init);
	int init(void* handle);
	int readReg(uint8_t regAddr, uint8_t* dat);
	int writeReg(uint8_t regAddr, uint8_t dat);

	/**
	 * @brief Utility function to read data.
	 * @param  pBuffer: pointer to data to be read.
	 * @param  RegisterAddr: specifies internal address register to be read.
	 * @param  NumByteToRead: number of bytes to be read.
	 * @retval 0 if ok, an error code otherwise.
	 */
	bool i2c_read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead)
	{
		if (!((I2CHelper::getInstance())->read(pBuffer, address, RegisterAddr, NumByteToRead)))
			return true;
		else
			return false;

	}

	/**
	 * @brief Utility function to write data.
	 * @param  pBuffer: pointer to data to be written.
	 * @param  RegisterAddr: specifies internal address register to be written.
	 * @param  NumByteToWrite: number of bytes to write.
	 * @retval 0 if ok, an error code otherwise.
	 */
	bool i2c_write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite)
	{
		if (!((I2CHelper::getInstance())->write(pBuffer, address, RegisterAddr, NumByteToWrite, false)))
			return true;
		else
			return false;
	}

protected:

private:
	uint8_t address;
};


#ifdef __cplusplus
extern "C" {
#endif
	uint8_t HTS221_io_read(void* handle, uint8_t writeAddr, uint8_t* buf, uint16_t numBytes);
	uint8_t HTS221_io_write(void* handle, uint8_t writeAddr, uint8_t* buf, uint16_t numBytes);
#ifdef __cplusplus
}
#endif

#endif // !_HTS221_SENSOR_H



