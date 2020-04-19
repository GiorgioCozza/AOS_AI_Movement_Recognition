/**
 *******************************************************************************
  *   @file LSM6DSL_gyroscope.h
  *   @author Cozza Giorgio
  *   @date 25/03/20
  *   @version 1.0
  *   @brief Header file with info about the LSM6DSL's gyroscope.
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

#ifndef __LSM6DSL_GYROSCOPE_H
#define __LSM6DSL_GYROSCOPE_H



/************************  CONTROL REGISTERS  ********************/
#define LSM6DSL_CTRL2_G				        	0x11
#define LSM6DSL_CTRL6_G  				        0X15
#define LSM6DSL_CTRL7_G 				        0X16



/************************  DATA REGISTERS  ***********************/
#define LSM6DSL_OUTX_L_G   				        0X22
#define LSM6DSL_OUTX_H_G   				        0X23
#define LSM6DSL_OUTY_L_G   				        0X24
#define LSM6DSL_OUTY_H_G   				        0X25
#define LSM6DSL_OUTZ_L_G   			        	0X26
#define LSM6DSL_OUTZ_H_G   				        0X27



/*************************  FULL SCALE  ************************/
#define LSM6DSL_FS_G_245dps                     0x00
#define LSM6DSL_FS_G_500dps                     0x04
#define LSM6DSL_FS_G_1000dps                    0x08
#define LSM6DSL_FS_G_2000dps                    0x0C

#define LSM6DSL_FS_G_MASK                       0x0C

#define LSM6DSL_FS_125_DISABLED                 0x00
#define LSM6DSL_FS_125_ENABLED                  0x02

#define LSM6DSL_FS_125_MASK                     0x02



/****************************  ODR  *****************************/
#define LSM6DSL_ODR_G_POWER_DOWN                0x00
#define LSM6DSL_ODR_G_13Hz          	        0x10
#define LSM6DSL_ODR_G_26Hz          	        0x20
#define LSM6DSL_ODR_G_52Hz          	        0x30
#define LSM6DSL_ODR_G_104Hz         	        0x40
#define LSM6DSL_ODR_G_208Hz         	        0x50
#define LSM6DSL_ODR_G_416Hz         	        0x60
#define LSM6DSL_ODR_G_833Hz         	        0x70
#define LSM6DSL_ODR_G_1660Hz        	        0x80
#define LSM6DSL_ODR_G_3330Hz        	        0x90
#define LSM6DSL_ODR_G_6660Hz        	        0xA0

#define LSM6DSL_ODR_G_MASK                      0xF0


/************************  SENSITIVITY  **************************/
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_125DPS   04.375
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_245DPS   08.750
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_500DPS   17.500
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_1000DPS  35.000
#define LSM6DSL_GYRO_SENSITIVITY_FOR_FS_2000DPS  70.000



#endif