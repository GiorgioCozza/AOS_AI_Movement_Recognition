

#ifndef __LSM6DSL_ACCELEROMETER_H
#define __LSM6DSL_ACCELEROMETER_H


/***********************  READ REGISTERS INFO  *******************/

/************************  SENSITIVITY INFO  *********************/

#define LSM6DSL_ACC_SENSITIVITY_FOR_FS_2G   0.061  /**< Sensitivity value for 2 g full scale [mg/LSB] */
#define LSM6DSL_ACC_SENSITIVITY_FOR_FS_4G   0.122  /**< Sensitivity value for 4 g full scale [mg/LSB] */
#define LSM6DSL_ACC_SENSITIVITY_FOR_FS_8G   0.244  /**< Sensitivity value for 8 g full scale [mg/LSB] */
#define LSM6DSL_ACC_SENSITIVITY_FOR_FS_16G  0.488  /**< Sensitivity value for 16 g full scale [mg/LSB] */


/***********************   REGISTERS  ****************************/

/**********************  info registers  *************************/

#define LSM6DSL_WHO_AM_I 					0x6A
#define LSM6DSL_WHO_AM_I_REG   			0X0F


/************************  I2C ADDRESSES  ************************/

#define LSM6DSL_I2C_ADDRESS_LOW			0xD4
#define LSM6DSL_I2C_ADDRESS_HIGH			0xd6


/************************  CONTROL REGISTERS  ********************/

#define LSM6DSL_CTRL1_XL   0X10
#define LSM6DSL_CTRL8_XL   0X17
#define LSM6DSL_CTRL9_XL   0X18

/************************  DATA REGISTERS  ***********************/

#define LSM6DSL_OUTX_L_XL      			0X28
#define LSM6DSL_OUTX_H_XL      			0X29
#define LSM6DSL_OUTY_L_XL      			0X2A
#define LSM6DSL_OUTY_H_XL      			0X2B
#define LSM6DSL_OUTZ_L_XL      			0X2C
#define LSM6DSL_OUTZ_H_XL      			0X2D


typedef enum {
	
	LSM6DSL_FS_XL_2g				=0x00,
	LSM6DSL_FS_XL_16g				=0x04,
	LSM6DSL_FS_XL_4g				=0x08,
	LSM6DSL_FS_XL_8g				=0x0C,
	
} LSM6DSL_FS_XL_t;
#define 	LSM6DSL_FS_XL_MASK		0x0C


typedef enum {
    LSM6DSL_ODR_XL_POWER_DOWN       =0x00,
    LSM6DSL_ODR_XL_13Hz         	=0x10,
    LSM6DSL_ODR_XL_26Hz         	=0x20,
    LSM6DSL_ODR_XL_52Hz         	=0x30,
    LSM6DSL_ODR_XL_104Hz        	=0x40,
    LSM6DSL_ODR_XL_208Hz        	=0x50,
    LSM6DSL_ODR_XL_416Hz       	    =0x60,
    LSM6DSL_ODR_XL_833Hz        	=0x70,
    LSM6DSL_ODR_XL_1660Hz       	=0x80,
    LSM6DSL_ODR_XL_3330Hz       	=0x90,
    LSM6DSL_ODR_XL_6660Hz      	 	=0xA0,
} LSM6DSL_ODR_XL_t;

#define       LSM6DSL_ODR_XL_MASK    0xF0



#endif