

/**
 *******************************************************************************
  *   @file I2C_helper.cpp
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


#include "i2c_helper.h"


/*------ static variables ----------------------------------------------------*/
I2CHelper* I2CHelper::instance = NULL;



/*------ methods impl --------------------------------------------------------*/
I2CHelper* I2CHelper::getInstance() {

	if (instance == NULL) {
		instance = new I2CHelper();
	}
	return instance;
}

bool I2CHelper::read(uint8_t* buf, uint8_t devAddr, uint8_t regAddr, uint16_t numbByte) {


	_i2c_dev::init();

	for (int j = 0; j < numbByte; j++) {
		_i2c_dev::sendStart();
		if (_i2c_dev::send((unsigned char)devAddr)) {
			if (_i2c_dev::send((unsigned char)(regAddr + j))) {
				_i2c_dev::sendRepeatedStart();
				unsigned char sl_addrr = (unsigned char)(devAddr + 1);
				if (_i2c_dev::send((unsigned char)sl_addrr)) {
					*(buf + j) = _i2c_dev::recvWithNack();
					_i2c_dev::sendStop();
					delayUs(10);
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}


	}
	return true;
}


bool I2CHelper::write(uint8_t * buf, uint8_t devAddr, uint8_t regAddr, uint16_t numByte, bool arduinoRx) {

	if (arduinoRx) {

		for (int i = 0; i < numByte; i++)
		{
			_i2c_dev::init();
			_i2c_dev::sendStart();

			if (_i2c_dev::send((unsigned char)devAddr)) {
				delayUs(10);
				if (!_i2c_dev::send((unsigned char) * (buf + i))) { return false; }
			}
			else { return true; }

			_i2c_dev::sendStop();
		}
		return true;

	}
	else {

        _i2c_dev::init();
        _i2c_dev::sendStart();

        for (int j = 0; j < numByte; j++) {

			if (_i2c_dev::send((unsigned char)(devAddr))) {
                if (_i2c_dev::send((unsigned char)(regAddr + j))) {
                    if( !_i2c_dev::send((unsigned char)buf[j]) )
					    return false;

                }
				else {
					return false;
				}
			}
			else {
				return false;
			}
            _i2c_dev::sendStop();
            delayUs(10);
        }

		return true;
	}
}




