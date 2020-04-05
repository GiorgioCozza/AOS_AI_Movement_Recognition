/**
 *******************************************************************************
  *   @file LSM303AGR.h
  *   @author Cozza Giorgio
  *   @date 25/03/20
  *   @version 1.0
  *   @brief Header definition of class LSM303AGRAccMag.
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


#ifndef _LSM303AGR_H
#define _LSM303AGR_H


#include "i2c_helper.h"
#include <stdint.h>

#include "LSM303AGR_accelerometer.h"
#include "LSM303AGR_magnetometer.h"
#include "IKS01A2_config.h"


class LSM303AGRAccMag {

public:
    LSM303AGRAccMag(){}
    virtual ~LSM303AGRAccMag(){}

    bool init(void);

    bool read_acc_id(uint8_t* id);
    bool get_acc_axes(int32_t* aData);
    bool get_acc_sensitivity(float* aSens);
    bool get_acc_odr(float* aOdr);
    bool get_acc_fs(float* aFs);

    bool set_acc_fs(float aFs);
    bool set_acc_odr(float aOdr);


    bool read_mag_id(uint8_t* id);
    bool get_mag_axes(int32_t* mData);
    bool get_mag_sensitivity(float* mSens);
    bool get_mag_fs(float* mFs);
    bool get_mag_odr(float* mOdr);

    bool set_mag_odr(float mOdr);


protected:


    bool io_read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead, uint8_t dev_address , uint8_t reg_mask = 0xFF)
    {
        uint8_t i = 0;
        for (i = 0; i < NumByteToRead; i++) {
            if (!((I2CHelper::getInstance())->read(pBuffer + i, dev_address, RegisterAddr + i, 1)))
                return false;
            *(pBuffer + i) &= reg_mask;
        }
        return true;
    }


    bool io_write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite, uint8_t dev_address , uint8_t reg_mask = 0xFF)
    {
        uint8_t i = 0;
        uint8_t * tmp_buf = new uint8_t[NumByteToWrite];
        for (i = 0; i < NumByteToWrite; i++) {
            if (!((I2CHelper::getInstance())->read((tmp_buf + i), dev_address, RegisterAddr + i, 1)))
                return false;
            else {
                *(tmp_buf + i) &= ~reg_mask;
                *(tmp_buf + i) |= *(pBuffer + i);

                if (!((I2CHelper::getInstance())->write((tmp_buf + i), dev_address, RegisterAddr + i, 1, false)))
                    return false;
            }
        }
        delete[] tmp_buf;
        return true;
    }



    private:

};


#endif