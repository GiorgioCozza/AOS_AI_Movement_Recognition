


#ifndef _LSM303AGR_ACCELEROMETER_H
#define _LSM303AGR_ACCELEROMETER_H


#define LSM303AGR_ACC_WHO_AM_I_ADDR                             0x0F
#define LSM303AGR_ACC_WHO_AM_I                                  0x33


#define LSM303AGR_ACC_OUT_X_L                                   0x28
#define LSM303AGR_ACC_OUT_X_H                                   0x29
#define LSM303AGR_ACC_OUT_Y_L                                   0x2A
#define LSM303AGR_ACC_OUT_Y_H                                   0x2B
#define LSM303AGR_ACC_OUT_Z_L                                   0x2C
#define LSM303AGR_ACC_OUT_Z_H                                   0x2D


#define LSM303AGR_ACC_FIFO_CTRL                                 0x2F

#define LSM303AGR_ACC_CTRL1                                     0x20
#define LSM303AGR_ACC_CTRL2                                     0x21
#define LSM303AGR_ACC_CTRL3                                     0x22
#define LSM303AGR_ACC_CTRL4                                     0x23
#define LSM303AGR_ACC_CTRL5                                     0x24
#define LSM303AGR_ACC_CTRL6                                     0x25

#define LSM303AGR_ACC_BDU_DISABLED                              0x00
#define LSM303AGR_ACC_BDU_ENABLED                               0x80

#define LSM303AGR_ACC_BDU_MASK                                  0x80



#define LSM303AGR_ACC_BYPASS                                    0x00
#define LSM303AGR_ACC_FIFO                                      0x40
#define LSM303AGR_ACC_STREAM                                    0x80

#define LSM303AGR_ACC_FIFO_MASK                                 0xC0



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




#endif