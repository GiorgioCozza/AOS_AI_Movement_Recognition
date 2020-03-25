

#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include "miosix.h"
#include "prog_config.h"
#include "nn_config.h"
#include "i2c_helper.h"
#include "circular_queue.h"
#include "LSM6DSLSensor.h"
#include "LSM303AGRAccSensor.h"
#include "LSM303AGRMagSensor.h"
#include "XNUCLEO_IKS01A2.h"


using namespace std;
using namespace miosix;

sensors sens;
axis ax;

bool startflag = false;
typedef Gpio<GPIOC_BASE, 13> usrbtn;
typedef Gpio<GPIOA_BASE, 5> usrled;



void print_on_serial(float* int_vec, uint32_t smp_cnt) {

	printf(" %d, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f", smp_cnt, (float)int_vec[0], (float)int_vec[1], (float)int_vec[2], \
																							(float)int_vec[3], (float)int_vec[4], (float)int_vec[5], \
																							(float)int_vec[6], (float)int_vec[7], (float)int_vec[8], \
																							(float)int_vec[9], (float)int_vec[10], (float)int_vec[11]);

	printf("\n");
}




int main() {

	{
		FastInterruptDisableLock a;
		RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
		CRC->CR = CRC_CR_RESET;
	}
	printf("#######################   |AOS: Neural Network on STM32 with STM32CubeAI|	#####################################\r\n");
	uint8_t buf_sender[4];
	uint8_t devId[SENSOR_NUM];
	//float tem;
	int32_t buf_reader[3];
	//char rep;

	float* in_data = nullptr;
	float* out_data = (float*)malloc(NUM_CLASSES * sizeof(float));
	float* int_vec = new float[VECTOR_SIZE];

	LSM6DSLAccGyr* acc_gyr = new LSM6DSLAccGyr(LSM6DSL_I2C_ADDRESS_HIGH);
	LSM303AGRAccSensor* acc = new LSM303AGRAccSensor(LSM303AGRAcc_I2C_ADDR);  //++
	LSM303AGRMagSensor* mag = new LSM303AGRMagSensor(LSM303AGRMag_I2C_ADDR);  //++

	// struct declaration to use for input segment preprocessing	

			circularQueue<float> queue(VECTOR_SIZE * WINDOW_SIZE);

			uint32_t tmp_val = 0;
			uint32_t sample_cnt = 0;
			int count = 0;
			
			startflag = false;
			usrbtn::mode(Mode::INPUT_PULL_UP);
			//enable board sensors
	
			
			int en1 = acc_gyr->init();
			int en2 = acc->enable();
			int en3 = mag->enable();
		

			float *c;
			acc_gyr->get_acc_odr(c);
			float cv = *c;
			printf("\r\n[LOG]: Sensors enabled! ODR: %f\r\n", cv);

			/* Print sensor IDs*/

			if (!acc_gyr->read_id(&(devId[lsm6dsl_acc])))
			{
				printf("\r\n[LOG]: %s, device enabled ID: %d\r\n", LSM6DSL, devId[lsm6dsl_acc]);
			}
			if (!acc->read_id(&(devId[lsm303agr_acc])))
			{
				printf("[LOG]: %s, device enabled ID: %d\r\n", LSM303AGR_ACC, devId[lsm303agr_acc]);
			}
			if (!mag->read_id(&(devId[lsm303agr_mag])))
			{
				printf("[LOG]: %s, device enabled ID: %d\r\n", LSM303AGR_MAG, devId[lsm303agr_mag]);
			}
			in_data = queue.getCircBuf();

			if (!en1 && !en2 && !en3 && !en4) {


				printf("\r\n[LOG]: START, Batch collection...\r\n");

				while (1) {
					
					
					while (startflag == false) {
						if (usrbtn::value() == 0) {
							startflag = true;
							printf("S");
							printf("\n");
							Thread::sleep(500);
						}
						usrled::high();
					}
					
					usrled::low();
					sample_cnt++;


					if (!acc_gyr->get_acc_axes(buf_reader)) {
						int_vec[0] = (float)buf_reader[0];
						int_vec[1] = (float)buf_reader[1];
						int_vec[2] = (float)buf_reader[2];
					}

					if (!acc_gyr->get_gyr_axes(buf_reader)) {
						int_vec[3] = (float)buf_reader[0];//
						int_vec[4] = (float)buf_reader[1];// 
						int_vec[5] = (float)buf_reader[2];//
					}

					if (!acc->get_x_axes(buf_reader)) {
						int_vec[6] = (float)buf_reader[0];
						int_vec[7] = (float)buf_reader[1];
						int_vec[8] = (float)buf_reader[2];
					}

					if (!mag->get_m_axes(buf_reader)) {
						int_vec[9] = (float)buf_reader[0];
						int_vec[10] = (float)buf_reader[1];
						int_vec[11] = (float)buf_reader[2];
					}

					
					queue.insert(int_vec, VECTOR_SIZE);
					
					print_on_serial(int_vec, sample_cnt);
					
					if (usrbtn::value() == 0) {
							startflag = false;
							Thread::sleep(500);
						}


					count++;
					

				}


			}


}




