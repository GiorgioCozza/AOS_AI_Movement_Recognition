/**
 *******************************************************************************
  *   @file LSM303AGR_accelerometer.h
  *   @author Cozza Giorgio
  *   @date 25/03/20
  *   @version 1.0
  *   @brief Header file with info about the LSM303AGR's accelerometer.
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

#ifndef _LSM303AGR_ACCELEROMETER_H
#define _LSM303AGR_ACCELEROMETER_H

/**************************    I2C   *****************************/
#define LSM303AGR_ACC_I2C_ADDRESS                               0x32


/**********************  info registers  *************************/
#define LSM303AGR_ACC_WHO_AM_I_ADDR                             0x0F
#define LSM303AGR_ACC_WHO_AM_I                                  0x33


/************************  DATA REGISTERS  ***********************/
#define LSM303AGR_ACC_OUT_X_L                                   0x28
#define LSM303AGR_ACC_OUT_X_H                                   0x29
#define LSM303AGR_ACC_OUT_Y_L                                   0x2A
#define LSM303AGR_ACC_OUT_Y_H                                   0x2B
#define LSM303AGR_ACC_OUT_Z_L                                   0x2C
#define LSM303AGR_ACC_OUT_Z_H                                   0x2D


/***********************  CONTROL REGISTERS  ********************/
#define LSM303AGR_ACC_CTRL1                                     0x20
#define LSM303AGR_ACC_CTRL2                                     0x21
#define LSM303AGR_ACC_CTRL3                                     0x22
#define LSM303AGR_ACC_CTRL4                                     0x23
#define LSM303AGR_ACC_CTRL5                                     0x24
#define LSM303AGR_ACC_CTRL6                                     0x25
#define LSM303AGR_ACC_FIFO_CTRL                                 0x2E



#define LSM303AGR_ACC_BDU_DISABLED                              0x00
#define LSM303AGR_ACC_BDU_ENABLED                               0x80

#define LSM303AGR_ACC_BDU_MASK                                  0x80



#define LSM303AGR_ACC_BYPASS                                    0x00
#define LSM303AGR_ACC_FIFO                                      0x40
#define LSM303AGR_ACC_STREAM                                    0x80

#define LSM303AGR_ACC_FIFO_MASK                                 0xC0


/****************************  ODR  *****************************/
#define LSM303AGR_ACC_ODR_POWER_DOWN                            0x00
#define LSM303AGR_ACC_ODR_1HZ                                   0x10
#define LSM303AGR_ACC_ODR_10HZ                                  0x20
#define LSM303AGR_ACC_ODR_25HZ                                  0x30
#define LSM303AGR_ACC_ODR_50HZ                                  0x40
#define LSM303AGR_ACC_ODR_100HZ                                 0x50
#define LSM303AGR_ACC_ODR_200HZ                                 0x60
#define LSM303AGR_ACC_ODR_400HZ                                 0x70
#define LSM303AGR_ACC_ODR_1620HZ                                0x80
#define LSM303AGR_ACC_ODR_1344HZ                                0x90

#define LSM303AGR_ACC_ODR_MASK                                  0xF0


#define LSM303AGR_CTRL1_XEN_ENABLED                             0x01
#define LSM303AGR_CTRL1_XEN_DISABLED                            0x00
#define LSM303AGR_CTRL1_YEN_ENABLED                             0x02
#define LSM303AGR_CTRL1_YEN_DISABLED                            0x00
#define LSM303AGR_CTRL1_ZEN_ENABLED                             0x04
#define LSM303AGR_CTRL1_ZEN_DISABLED                            0x00
#define LSM303AGR_CTRL1_XEN_YEN_ZEN_ENABLED                     0x07

#define LSM303AGR_CTRL1_XEN_MASK                                0x01
#define LSM303AGR_CTRL1_YEN_MASK                                0x02
#define LSM303AGR_CTRL1_ZEN_MASK                                0x04
#define LSM303AGR_CTRL1_XEN_YEN_ZEN_MASK                        0x07


#define LSM303AGR_ACC_LP_DISABLED                               0x00
#define LSM303AGR_ACC_LP_ENABLED                                0x08

#define LSM303AGR_ACC_LP_MASK                                   0x08


#define LSM303AGR_ACC_HR_DISABLED                               0x00
#define LSM303AGR_ACC_HR_ENABLED                                0x08

#define LSM303AGR_ACC_HR_MASK                                   0x08


/*************************  FULL SCALE  ************************/
#define LSM303AGR_ACC_FS_2G                                     0x00
#define LSM303AGR_ACC_FS_4G                                     0x10
#define LSM303AGR_ACC_FS_8G                                     0x20
#define LSM303AGR_ACC_FS_16G                                    0x30

#define LSM303AGR_ACC_FS_MASK                                   0x30


/************************  SENSITIVITY  **************************/
#define LSM303AGR_X_SENSITIVITY_FS_2G_NORMAL                   3.900f
#define LSM303AGR_X_SENSITIVITY_FS_2G_HIGH_RES                 0.980f
#define LSM303AGR_X_SENSITIVITY_FS_2G_LOW_POWER                15.630f
#define LSM303AGR_X_SENSITIVITY_FS_4G_NORMAL                   7.820f
#define LSM303AGR_X_SENSITIVITY_FS_4G_HIGH_RES                 1.950f
#define LSM303AGR_X_SENSITIVITY_FS_4G_LOW_POWER                31.260f
#define LSM303AGR_X_SENSITIVITY_FS_8G_NORMAL                   15.630f
#define LSM303AGR_X_SENSITIVITY_FS_8G_HIGH_RES                 3.900f
#define LSM303AGR_X_SENSITIVITY_FS_8G_LOW_POWER                62.520f
#define LSM303AGR_X_SENSITIVITY_FS_16G_NORMAL                  46.900f
#define LSM303AGR_X_SENSITIVITY_FS_16G_HIGH_RES                11.720f
#define LSM303AGR_X_SENSITIVITY_FS_16G_LOW_POWER               187.580f

#endif