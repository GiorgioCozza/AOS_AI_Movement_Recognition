


#ifndef _LSM303AGR_MAGNETOMETER_H
#define _LSM303AGR_MAGNETOMETER_H


#define LSM303AGR_MAG_I2C_ADDRESS                               0x3C
#define LSM303AGR_MAGNETOMETER_I2C_ADDRESS                      0x3C

/*############################  ID REGISTERS  ################################*/

#define LSM303AGR_MAG_WHO_AM_I                                  0x40
#define LSM303AGR_MAG_WHO_AM_I_ADDR                             0x4F

#define LSM303AGR_MAG_WHO_AM_I_MASK                             0xFF

/*##########################  AXES REGISTERS  ################################*/

#define LSM303AGR_MAG_OUTX_L                                    0x68
#define LSM303AGR_MAG_OUTX_H                                    0x69
#define LSM303AGR_MAG_OUTY_L                                    0x6A
#define LSM303AGR_MAG_OUTY_H                                    0x6B
#define LSM303AGR_MAG_OUTZ_L                                    0x6C
#define LSM303AGR_MAG_OUTZ_H                                    0x6D


/*########################  CONTROL REGISTERS  ##############################*/

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


/*##########################  ODR REGISTERS  ################################*/

#define LSM303AGR_MAG_ODR_10HZ                                  0x00
#define LSM303AGR_MAG_ODR_20HZ                                  0x04
#define LSM303AGR_MAG_ODR_50HZ                                  0x08
#define LSM303AGR_MAG_ODR_100HZ                                 0x0C

#define LSM303AGR_MAG_ODR_MASK                                  0x0C


/*#######################  SENSITIVITY values  #############################*/

#define LSM303AGR_MAG_SENSITIVITY_VAL                           1.5f
#define LSM303AGR_MAG_FS_VAL                                    49.152f




#endif