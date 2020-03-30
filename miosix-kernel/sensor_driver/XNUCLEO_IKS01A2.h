
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

#include "LSM6DSL.h"
#include "LSM303AGR.h"

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




/***************  LSM6DSL Accelerometer Output Data Rates  ************/
/*                                                                    */
/*          - LSM6DSL_ODR_XL_POWER_DOWN                               */
/*          - LSM6DSL_ODR_XL_13Hz      	                              */
/*          - LSM6DSL_ODR_XL_26Hz          	                          */
/*          - LSM6DSL_ODR_XL_52Hz          	                          */
/*          - LSM6DSL_ODR_XL_104Hz         	                          */
/*          - LSM6DSL_ODR_XL_208Hz         	                          */
/*          - LSM6DSL_ODR_XL_416Hz         	                          */
/*          - LSM6DSL_ODR_XL_833Hz         	                          */
/*          - LSM6DSL_ODR_XL_1660Hz        	                          */
/*          - LSM6DSL_ODR_XL_3330Hz        	                          */
/*          - LSM6DSL_ODR_XL_6660Hz        	                          */
/*                                                                    */
/**********************************************************************/

#define C_LSM6DSL_ACC_ODR                          208.0f

/***********  LSM6DSL Accelerometer Full Scale Values  ****************/
/*                                                                    */
/*          - LSM6DSL_FS_XL_2g                                        */
/*          - LSM6DSL_FS_XL_4g          	                          */
/*          - LSM6DSL_FS_XL_8g          	                          */
/*          - LSM6DSL_FS_XL_16g         	                          */
/*                                                                    */
/**********************************************************************/

#define C_LSM6DSL_ACC_FS                           2.0f


/*****************  LSM6DSL Gyroscope Output Data Rates  **************/
/*                                                                    */
/*          - LSM6DSL_ODR_G_POWER_DOWN                                */
/*          - LSM6DSL_ODR_G_13Hz      	                              */
/*          - LSM6DSL_ODR_G_26Hz          	                          */
/*          - LSM6DSL_ODR_G_52Hz          	                          */
/*          - LSM6DSL_ODR_G_104Hz         	                          */
/*          - LSM6DSL_ODR_G_208Hz         	                          */
/*          - LSM6DSL_ODR_G_416Hz         	                          */
/*          - LSM6DSL_ODR_G_833Hz         	                          */
/*          - LSM6DSL_ODR_G_1660Hz        	                          */
/*          - LSM6DSL_ODR_G_3330Hz        	                          */
/*          - LSM6DSL_ODR_G_6660Hz        	                          */
/*                                                                    */
/**********************************************************************/

#define C_LSM6DSL_GYRO_ODR                         208.0f


/************  LSM6DSL Gyroscope Full Scale Values  *******************/
/*                                                                    */
/*          - LSM6DSL_FS_G_245dps                                     */
/*          - LSM6DSL_FS_G_245dps                                     */
/*          - LSM6DSL_FS_G_500dps          	                          */
/*          - LSM6DSL_FS_G_1000dps          	                      */
/*          - LSM6DSL_FS_G_2000dps         	                          */
/*                                                                    */
/**********************************************************************/

#define C_LSM6DSL_GYRO_FS                          2000.0f


/***********  LSM303AGR Magnetometer Output Data Rates  ***************/
/*                                                                    */
/*          - LSM303AGR_MAG_ODR_10HZ                                  */
/*          - LSM303AGR_MAG_ODR_20HZ                                  */
/*          - LSM303AGR_MAG_ODR_50HZ          	                      */
/*          - LSM303AGR_MAG_ODR_100HZ          	                      */
/*                                                                    */
/**********************************************************************/

#define C_LSM303AGR_MAG_ODR                        100.0f


/***********  LSM303AGR Accelerometer Output Data Rates  **************/
/*                                                                    */
/*          - LSM303AGR_ACC_ODR_POWER_DOWN                            */
/*          - LSM303AGR_ACC_ODR_1HZ      	                          */
/*          - LSM303AGR_ACC_ODR_10HZ          	                      */
/*          - LSM303AGR_ACC_ODR_25HZ          	                      */
/*          - LSM303AGR_ACC_ODR_50HZ         	                      */
/*          - LSM303AGR_ACC_ODR_100HZ         	                      */
/*          - LSM303AGR_ACC_ODR_200HZ         	                      */
/*          - LSM303AGR_ACC_ODR_400HZ         	                      */
/*          - LSM303AGR_ACC_ODR_1620HZ        	                      */
/*          - LSM303AGR_ACC_ODR_1344HZ        	                      */
/*          - LSM6DSL_ODR_G_6660Hz        	                          */
/*                                                                    */
/**********************************************************************/

#define C_LSM303AGR_ACC_ODR                         100.0f


/***********  LSM303AGR Accelerometer Full Scale Values  **************/
/*                                                                    */
/*          - LSM303AGR_ACC_FS_2G                                     */
/*          - LSM303AGR_ACC_FS_4G          	                          */
/*          - LSM303AGR_ACC_FS_8G          	                          */
/*          - LSM303AGR_ACC_FS_16G         	                          */
/*                                                                    */
/**********************************************************************/

#define C_LSM303AGR_ACC_FS                           2.0f

#endif