/**
 *******************************************************************************
  *   @file LSM303AGR_magnetometer.h
  *   @author Cozza Giorgio
  *   @date 25/03/20
  *   @version 1.0
  *   @brief Header file with info about the LSM303AGR's magnetometer.
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

#ifndef _LSM303AGR_MAGNETOMETER_H
#define _LSM303AGR_MAGNETOMETER_H

/**************************    I2C   *****************************/
#define LSM303AGR_MAG_I2C_ADDRESS                               0x3C
#define LSM303AGR_MAGNETOMETER_I2C_ADDRESS                      0x3C

/**********************  info registers  *************************/
#define LSM303AGR_MAG_WHO_AM_I                                  0x40
#define LSM303AGR_MAG_WHO_AM_I_ADDR                             0x4F

#define LSM303AGR_MAG_WHO_AM_I_MASK                             0xFF

/************************  DATA REGISTERS  ***********************/
#define LSM303AGR_MAG_OUTX_L                                    0x68
#define LSM303AGR_MAG_OUTX_H                                    0x69
#define LSM303AGR_MAG_OUTY_L                                    0x6A
#define LSM303AGR_MAG_OUTY_H                                    0x6B
#define LSM303AGR_MAG_OUTZ_L                                    0x6C
#define LSM303AGR_MAG_OUTZ_H                                    0x6D


/************************  CONTROL REGISTERS  ********************/
#define LSM303AGR_MAG_CFG_A                                     0X60
#define LSM303AGR_MAG_CFG_B                                     0X61
#define LSM303AGR_MAG_CFG_C                                     0X62



#define LSM303AGR_MAG_MD_CONTINUOS                              0x00
#define LSM303AGR_MAG_MD_SINGLE                                 0x01
#define LSM303AGR_MAG_MD_IDLE1                                  0x02
#define LSM303AGR_MAG_MD_IDLE2                                  0x03

#define LSM303AGR_MAG_MD_MASK                                   0x03


#define LSM303AGR_MAG_BDU_DISABLED                              0x00
#define LSM303AGR_MAG_BDU_ENABLED                               0x10

#define LSM303AGR_MAG_BDU_MASK                                  0x10


/****************************  ODR  *****************************/
#define LSM303AGR_MAG_ODR_10HZ                                  0x00
#define LSM303AGR_MAG_ODR_20HZ                                  0x04
#define LSM303AGR_MAG_ODR_50HZ                                  0x08
#define LSM303AGR_MAG_ODR_100HZ                                 0x0C

#define LSM303AGR_MAG_ODR_MASK                                  0x0C



#define LSM303AGR_MAG_SENSITIVITY_VAL                           1.5f

/*************************  FULL SCALE  ************************/
#define LSM303AGR_MAG_FS_VAL                                    49.152f




#endif