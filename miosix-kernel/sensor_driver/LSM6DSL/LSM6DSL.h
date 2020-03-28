




#ifndef __LSM6DSL_H
#define __LSM6DSL_H

#include <stdint.h>
#include "i2c_helper.h"


/***********************   REGISTERS  ****************************/

/**********************  info registers  *************************/

#define LSM6DSL_WHO_AM_I 					        0x6A
#define LSM6DSL_WHO_AM_I_REG                        0X0F


/************************  I2C ADDRESSES  ************************/

#define LSM6DSL_I2C_ADDRESS_LOW				        0xD4
#define LSM6DSL_I2C_ADDRESS_HIGH			        0xD6

/************************  CONTROL REGISTERS  ********************/

#define LSM6DSL_FIFO_CTRL1                          0X06
#define LSM6DSL_FIFO_CTRL2                          0X07
#define LSM6DSL_FIFO_CTRL3                          0X08
#define LSM6DSL_FIFO_CTRL4                          0X09
#define LSM6DSL_FIFO_CTRL5                          0X0A

#define LSM6DSL_CTRL10_C                            0X19
#define LSM6DSL_CTRL3_C                             0X12
#define LSM6DSL_CTRL4_C                             0X13
#define LSM6DSL_CTRL5_C                             0X14

#define LSM6DSL_FIFO_MODE_BYPASS                    0x00
#define LSM6DSL_FIFO_MODE_FIFO                      0x01
#define LSM6DSL_FIFO_MODE_STREAM                    0x02
#define LSM6DSL_FIFO_MODE_STF                       0x03
#define LSM6DSL_FIFO_MODE_BTS                       0x04
#define LSM6DSL_FIFO_MODE_DYN_STREAM                0x05
#define LSM6DSL_FIFO_MODE_DYN_STREAM_2              0x06
#define LSM6DSL_FIFO_MODE_BTF                       0x07

#define LSM6DSL_FIFO_MODE_MASK                      0x07



#define LSM6DSL_BDU_CONTINUOS                       0x00
#define LSM6DSL_BDU_BLOCK_UPDATE                    0x40

#define LSM6DSL_BDU_MASK                            0x40



#define LSM6DSL_IF_INC_DISABLED                     0x00
#define LSM6DSL_IF_INC_ENABLED                      0x04

#define LSM6DSL_IF_INC_MASK                         0x04


#include "LSM6DSL_accelerometer.h"
#include "LSM6DSL_gyroscope.h"


class LSM6DSLAccGyr
{
	
	public:
		LSM6DSLAccGyr(uint8_t addr = LSM6DSL_I2C_ADDRESS_HIGH) { address = addr; }
		virtual ~LSM6DSLAccGyr() {}
		bool init(void);
		bool read_id(uint8_t* id);
		
		bool get_acc_axes(int32_t* aData);
		bool get_acc_sensitivity(float* aSens);
		bool get_acc_odr(float* aodr);
		bool get_acc_fs(float* afs);
		
		bool set_acc_fs(float afs);
		bool set_acc_odr(float aodr);
		
		
		bool get_gyr_axes(int32_t* gData);
		bool get_gyr_sensitivity(float* gSens);
		bool get_gyr_fs(float* gfs);
		bool get_gyr_odr(float* godr);

		bool set_gyr_fs(float gfs);
		bool set_gyr_odr(float godr);
		
		
	protected:

    /**
 * @brief Utility function to read data.
 * @param  pBuffer: pointer to data to be read.
 * @param  RegisterAddr: specifies internal address register to be read.
 * @param  NumByteToRead: number of bytes to be read.
 * @retval true if ok, false otherwise.
 */
    uint8_t io_read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead, uint8_t reg_mask = 0xFF)
    {
        uint8_t i = 0;
        for (i = 0; i < NumByteToRead; i++) {
            if (!((I2CHelper::getInstance())->read(pBuffer + i, address, RegisterAddr + i, 1)))
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
    uint8_t io_write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite, uint8_t reg_mask = 0xFF) {
        uint8_t i = 0;
        uint8_t *tmp_buf = new uint8_t[NumByteToWrite];
        for (i = 0; i < NumByteToWrite; i++) {
            if (!((I2CHelper::getInstance())->read(tmp_buf + i, address, RegisterAddr + i, 1)))
                return false;
            else {
                *(tmp_buf + i) &= ~reg_mask;
                *(tmp_buf + i) |= *(pBuffer + i);
                if (!((I2CHelper::getInstance())->write(tmp_buf + i, address, RegisterAddr + i, 1, false)))
                    return false;
            }
        }
        delete[] tmp_buf;
        return true;
    }



        private:
		uint8_t address;
};


#endif