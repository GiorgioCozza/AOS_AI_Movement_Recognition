


/**
 *******************************************************************************
  *   @file I2C_helper.h
  *   @author Cozza Giorgio, Liu Qiaqi
  *   @date 08/04/19
  *   @version 1.0
  *   @brief I2C communication manager, mediates data transfer from the sensors
			 to the STM32F401 using Miosix i2c_software interface
 *******************************************************************************

  This is a free software released into public domain. Anyone is free to copy,
  modify, publish, use, compile or distribute this software, either in source
  code form or as compiled binary

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
********************************************************************************
 */


#ifndef _I2C_HELPER_H
#define _I2C_HELPER_H

#include <stdio.h>
#include <stdint.h>
#include "miosix.h"

using namespace miosix;

typedef Gpio<GPIOB_BASE, 8> scl_pin;
typedef Gpio<GPIOB_BASE, 9> sda_pin;
typedef SoftwareI2C<sda_pin, scl_pin, 100, false> _i2c_dev;

class I2CHelper {

public:

	static I2CHelper* getInstance();
	~I2CHelper(void) {
	}

	bool read(uint8_t* buf, uint8_t devAddr, uint8_t regAddr, uint16_t numByte);
	bool write(uint8_t* buf, uint8_t devAddr, uint8_t regAddr, uint16_t numByte, bool arduinoRx);
protected:
	I2CHelper() {}


private:
	static I2CHelper* instance;

};

#endif // !_I2C_HELPER_H




