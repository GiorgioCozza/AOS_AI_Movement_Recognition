#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "miosix.h"
#include "i2c_helper.h"
#include "circular_queue.h"
#include "HTS221Sensor.h"
#include "LSM6DSLSensor.h"
#include "LSM303AGRAccSensor.h"
#include "LSM303AGRMagSensor.h"


#define LSM6DSL_I2C_ADDR		LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH
#define HTS221_I2C_ADDR			HTS221_I2C_ADDRESS
#define LSM303AGR_I2C_ADDR		LSM303AGR_MAG_I2C_ADDRESS

#include "NN.h"
#include "network_data.h"

#define LSM6DSL_I2C_ADDR		LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH
#define HTS221_I2C_ADDR			HTS221_I2C_ADDRESS
#define LSM303AGRAcc_I2C_ADDR	0x32
#define LSM303AGRMag_I2C_ADDR	LSM303AGR_MAG_I2C_ADDRESS        
#define ARDUINO_MKR1000_ADDR	0x04

#define READ_1B					1
#define READ_2B					2
#define READ_4B					4

#define VECTOR_SIZE             12
#define WINDOW_SIZE				50

using namespace std;
using namespace miosix;


//bool startflag = false;
//typedef Gpio<GPIOC_BASE, 13> usrbtn;

/* void printing2(int32_t* vector, char* sensor) {

	printf("%s:     ", sensor);
	printf("X: %6ld  ", vector[0]);
	printf("Y: %6ld  ", vector[1]);
	printf("Z: %6ld  ", vector[2]);
	printf("\n");
} */

int main()
{
	{
		FastInterruptDisableLock a;
		RCC->AHB1ENR = RCC_AHB1ENR_CRCEN;
		CRC->CR = CRC_CR_RESET;
	}
	printf("#######################   HELLO WORLD!	#####################################\r\n");
	printf("Hello world\n");
	for(;;) ;
}