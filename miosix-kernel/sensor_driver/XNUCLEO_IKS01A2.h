
/**
 *******************************************************************************
  *   @file X-NUCLEO-IKS01A2.h
  *   @author Cozza Giorgio
  *   @date 13/06/19
  *   @version 1.0
  *   @brief Header file with info about the sensor board IKS01A2.
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


#ifndef _X_NUCLEO_IKS01A2_H
#define _X_NUCLEO_IKS01A2_H


#define LSM6DSL_I2C_ADDR		LSM6DSL_I2C_ADDRESS_HIGH
#define HTS221_I2C_ADDR			HTS221_I2C_ADDRESS
#define LSM303AGR_I2C_ADDR		LSM303AGR_MAG_I2C_ADDRESS
#define LSM6DSL_I2C_ADDR		LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH
#define HTS221_I2C_ADDR			HTS221_I2C_ADDRESS
#define LSM303AGRAcc_I2C_ADDR	0x32
#define LSM303AGRMag_I2C_ADDR	LSM303AGR_MAG_I2C_ADDRESS 

#define SENSOR_NUM				4
#define LSM6DSL					"LSM6DSL_ACC_GYR"
#define LSM6DSL_ACC				"LSM6DSL_ACCELEROMETER"
#define LSM6DSL_GYR				"LSM6DSL_GYROSCOPE"
#define LSM303AGR_ACC			"LSM303AGR_ACCELEROMETER"
#define LSM303AGR_MAG			"LSM303AGR_MAGNETOMETER"



typedef enum axis {X=0, Y, Z};
typedef enum sensors{ lsm6dsl_acc = 0, lsm6dsl_gyr, lsm303agr_acc, lsm303agr_mag};

#endif