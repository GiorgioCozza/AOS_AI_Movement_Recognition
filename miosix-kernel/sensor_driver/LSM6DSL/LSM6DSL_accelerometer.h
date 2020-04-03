/**
 *******************************************************************************
  *   @file LSM6DSL_accelerometer.h
  *   @author Cozza Giorgio
  *   @date 25/03/20
  *   @version 1.0
  *   @brief Header file with info about the LSM6DSL's accelerometer.
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

#ifndef __LSM6DSL_ACCELEROMETER_H
#define __LSM6DSL_ACCELEROMETER_H



/**********************  info registers  *************************/
#define LSM6DSL_WHO_AM_I 					0x6A
#define LSM6DSL_WHO_AM_I_REG   			    0X0F



/**************************    I2C   *****************************/
#define LSM6DSL_I2C_ADDRESS_LOW			    0xD4
#define LSM6DSL_I2C_ADDRESS_HIGH			0xd6


/***********************  CONTROL REGISTERS  *********************/
#define LSM6DSL_CTRL1_XL                    0X10
#define LSM6DSL_CTRL8_XL                    0X17
#define LSM6DSL_CTRL9_XL                    0X18


/************************  DATA REGISTERS  ***********************/
#define LSM6DSL_OUTX_L_XL      			    0X28
#define LSM6DSL_OUTX_H_XL      			    0X29
#define LSM6DSL_OUTY_L_XL      		    	0X2A
#define LSM6DSL_OUTY_H_XL      			    0X2B
#define LSM6DSL_OUTZ_L_XL      			    0X2C
#define LSM6DSL_OUTZ_H_XL      			    0X2D


/*************************  FULL SCALE  ************************/
#define LSM6DSL_FS_XL_2g				    0x00
#define LSM6DSL_FS_XL_16g				    0x04
#define LSM6DSL_FS_XL_4g				    0x08
#define LSM6DSL_FS_XL_8g				    0x0C

#define LSM6DSL_FS_XL_MASK		            0x0C


/***************************  ODR  ******************************/
#define LSM6DSL_ODR_XL_POWER_DOWN           0x00
#define LSM6DSL_ODR_XL_13Hz         	    0x10
#define LSM6DSL_ODR_XL_26Hz         	    0x20
#define LSM6DSL_ODR_XL_52Hz         	    0x30
#define LSM6DSL_ODR_XL_104Hz        	    0x40
#define LSM6DSL_ODR_XL_208Hz        	    0x50
#define LSM6DSL_ODR_XL_416Hz       	        0x60
#define LSM6DSL_ODR_XL_833Hz        	    0x70
#define LSM6DSL_ODR_XL_1660Hz       	    0x80
#define LSM6DSL_ODR_XL_3330Hz       	    0x90
#define LSM6DSL_ODR_XL_6660Hz      	 	    0xA0

#define LSM6DSL_ODR_XL_MASK                 0xF0


/************************  SENSITIVITY  **************************/
#define LSM6DSL_ACC_SENSITIVITY_FOR_FS_2G   0.061
#define LSM6DSL_ACC_SENSITIVITY_FOR_FS_4G   0.122
#define LSM6DSL_ACC_SENSITIVITY_FOR_FS_8G   0.244
#define LSM6DSL_ACC_SENSITIVITY_FOR_FS_16G  0.488

#endif