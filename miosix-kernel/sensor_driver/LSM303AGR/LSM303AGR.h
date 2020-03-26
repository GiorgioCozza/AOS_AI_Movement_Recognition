


#ifndef _LSM303AGR_H
#define _LSM303AGR_H


#include "i2c_helper.h"
#include <stdint.h>


#include "LSM303AGR_accelerometer.h"
#include "LSM303AGR_magnetometer.h"



class LSM303AGRAccMag {

public:
    LSM303AGRAccMag(uint8_t mag_addr = LSM303AGR_MAG_I2C_ADDRESS,
                    uint8_t acc_addr = LSM303AGR_ACC_I2C_ADDRESS)
                    { mag_addr = mag_addr; acc_addr = acc_addr; }

    virtual ~LSM303AGRAccMag(){}
    bool init(void);
    bool read_id(uint8_t* id);

    bool get_acc_axes(int32_t* aData);
    bool get_acc_sensitivity(float* aSens);
    bool get_acc_odr(float* aOdr);
    bool get_acc_fs(float* aFs);

    bool set_acc_fs(float aFs);
    bool set_acc_odr(float aOdr);


    bool get_mag_axes(int32_t* mData);
    bool get_mag_sensitivity(float* mSens);
    bool get_mag_fs(float* mFs);
    bool get_mag_odr(float* mOdr);

    bool set_mag_fs(float mFs);
    bool set_mag_odr(float mOdr);


protected:

    /**
	 * @brief Utility function to read data.
	 * @param  pBuffer: pointer to data to be read.
	 * @param  RegisterAddr: specifies internal address register to be read.
	 * @param  NumByteToRead: number of bytes to be read.
	 * @retval true if ok, false otherwise.
	 */
    uint8_t io_read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead)
    {
        if (!((I2CHelper::getInstance())->read(pBuffer, address, RegisterAddr, NumByteToRead)))
            return false;
        else
            return true;
    }

    /**
     * @brief Utility function to write data.
     * @param  pBuffer: pointer to data to be written.
     * @param  RegisterAddr: specifies internal address register to be written.
     * @param  NumByteToWrite: number of bytes to write.
     * @retval true if ok, false otherwise.
     */
    uint8_t io_write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite)
    {
        if (!((I2CHelper::getInstance())->write(pBuffer, address, RegisterAddr, NumByteToWrite, false)))
            return false;
        else
            return true;
    }


private:
    uint8_t mag_addr;
    uint8_t acc_addr;

};


#endif