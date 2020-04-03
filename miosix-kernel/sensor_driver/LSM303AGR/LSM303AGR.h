


#ifndef _LSM303AGR_H
#define _LSM303AGR_H


#include "i2c_helper.h"
#include <stdint.h>

#include "LSM303AGR_accelerometer.h"
#include "LSM303AGR_magnetometer.h"
#include "XNUCLEO_IKS01A2.h"


class LSM303AGRAccMag {

public:
    LSM303AGRAccMag(uint8_t mag_addr = LSM303AGR_MAG_I2C_ADDRESS,
                    uint8_t acc_addr = LSM303AGR_ACC_I2C_ADDRESS)
                    { mag_addr = mag_addr; acc_addr = acc_addr; }

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

    /**
	 * @brief Utility function to read data.
	 * @param  pBuffer: pointer to data to be read.
	 * @param  RegisterAddr: specifies internal address register to be read.
	 * @param  NumByteToRead: number of bytes to be read.
	 * @retval true if ok, false otherwise.
	 */
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

    /**
     * @brief Utility function to write data.
     * @param  pBuffer: pointer to data to be written.
     * @param  RegisterAddr: specifies internal address register to be written.
     * @param  NumByteToWrite: number of bytes to write.
     * @retval true if ok, false otherwise.
     */
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
    uint8_t mag_addr;
    uint8_t acc_addr;

};


#endif