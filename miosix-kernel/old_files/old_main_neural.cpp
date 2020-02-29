

#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include "miosix.h"
#include "i2c_helper.h"
#include "circular_queue.h"
#include "NN.h"
#include "network_data.h"
#include "LSM6DSLSensor.h"
#include "LSM303AGRAccSensor.h"
#include "LSM303AGRMagSensor.h"
#include "XNUCLEO_IKS01A2.h"
#include "nn_info.h"


#define READ_1B					1
#define READ_2B					2
#define READ_4B					4

#define VECTOR_SIZE             12
#define WINDOW_SIZE				50

#define NUM_CLASSES 4
#define MEAN		0
#define SIGMA		1


using namespace std;
using namespace miosix;

sensors sens;
axis ax;

bool startflag = false;
typedef Gpio<GPIOC_BASE, 13> usrbtn;

char run[] = "RUNNING";
char stand[] = "STANDING";
char walk[] = "WALKING";
char jump[] = "JUMPING";

char* activities[4] = { run, stand, walk, jump };

/*
void printing2(int32_t * int_vec) {
	for (int i = 0; i < 12; i++) {
		printf("%6ld", int_vec[i]);
	}
	printf("\n");
}
*/


void printing2(int32_t* int_vec, char* sensor, int i) {

	printf("%s:     ", sensor);
	printf("X: %f  ", (float)int_vec[0 + 3 * i]);
	printf("Y: %f  ", (float)int_vec[1 + 3 * i]);
	printf("Z: %f  ", (float)int_vec[2 + 3 * i]);
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

	// network variable declaration
	ai_handle network = AI_HANDLE_NULL;
	ai_buffer ai_input = AI_NETWORK_IN;
	ai_buffer ai_output = AI_NETWORK_OUT;
	ai_u8* activations = new ai_u8[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
	NN* neural_net = new NN();

	float* in_data = nullptr;
	float* out_data = (float*)malloc(NUM_CLASSES * sizeof(float));
	//float* sgm_presum = (float*)malloc(VECTOR_SIZE * sizeof(float));
	//float* mn_presum = (float*)malloc(VECTOR_SIZE * sizeof(float));
	int32_t* int_vec = new int32_t[VECTOR_SIZE];

	LSM6DSLSensor* acc_gyr = new LSM6DSLSensor(LSM6DSL_I2C_ADDR);
	LSM303AGRAccSensor* acc = new LSM303AGRAccSensor(LSM303AGRAcc_I2C_ADDR);  //++
	LSM303AGRMagSensor* mag = new LSM303AGRMagSensor(LSM303AGRMag_I2C_ADDR);  //++

	// struct declaration to use for input segment preprocessing	

	if (neural_net->nnCreate(&network)) {
		if (neural_net->nnInit(network, (ai_network_params*)AI_NETWORK_DATA_CONFIG, activations)) {

			circularQueue queue;
			dataset_preproc ds_pp;

			uint32_t tmp_val = 0;

			int count = 0;

			//enable board sensors

			int en1 = acc_gyr->enable_x();
			int en2 = acc_gyr->enable_g();
			int en3 = acc->enable();
			int en4 = mag->enable();

			printf("\r\n[LOG]: Sensors enabled!\r\n");

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

					//Clear terminal screen
					char ESC = 27;
					printf("%c[H", ESC);
					printf("%c[2J", ESC);
					//Thread::sleep(50);


					if (!acc_gyr->get_x_axes(buf_reader)) {
						int_vec[0] = buf_reader[0];
						int_vec[1] = buf_reader[1];
						int_vec[2] = buf_reader[2];
						ds_pp.update_min_max(int_vec, 0);
					}
					Thread::sleep(50);

					if (!acc_gyr->get_g_axes(buf_reader)) {
						int_vec[3] = buf_reader[0];// >> 6;
						int_vec[4] = buf_reader[1];// >> 6;
						int_vec[5] = buf_reader[2];// >> 6;
						ds_pp.update_min_max(int_vec, 1);
					}

					Thread::sleep(50);

					if (!acc->get_x_axes(buf_reader)) {
						int_vec[6] = buf_reader[0];
						int_vec[7] = buf_reader[1];
						int_vec[8] = buf_reader[2];
						ds_pp.update_min_max(int_vec, 2);
					}

					Thread::sleep(50);

					if (!mag->get_m_axes(buf_reader)) {
						int_vec[9] = buf_reader[0];
						int_vec[10] = buf_reader[1];
						int_vec[11] = buf_reader[2];
						ds_pp.update_min_max(int_vec, 3);
					}


					queue.insert(int_vec);
					/*
					printing2(int_vec, "\n\nLSM6DSL_ACCELEROMETER", 0);
					printing2(int_vec, "LSM6DSL_GYROSCOPE____", 1);
					printing2(int_vec, "LSM303A_ACCELEROMETER", 2);
					printing2(int_vec, "LSM303A_MAGNETOMETER", 3);
					*/
					//Clear terminal screen
					printf("%c[H", ESC);
					printf("%c[2J", ESC);
					Thread::sleep(100);


					count++;

					if (count == WINDOW_SIZE) {
						count = 0;
						printf("\r\n[LOG]: FINISH, Batch collected!\r\n");
						printf("\r\n[LOG]: MEAN PRESUM, %.2f %2.f!\r\n", ds_pp.get_min(1, 0), ds_pp.get_min(3, 0));
						printf("\r\n[LOG]: Processing the collected samples...\r\n");
						neural_net->prepareData(&ds_pp, in_data, out_data, &ai_input, &ai_output, 1);
						ds_pp.print_min();
						ds_pp.print_max();
						ds_pp.print_dataset(in_data);
						printf("\r\n[LOG]: Running the Neural Network...\r\n");
						int n_b = neural_net->nnRun(network, &ai_input, &ai_output, 1);
						printf("\r\n****************	AI NN NETWORK RESULT	*********************\r\n");
						printf("\r\n[LOG]: OUTPUT N_BATCHES: %d\r\n", n_b);
						for (int i = 0; i < NUM_CLASSES; i++)
							printf("[OUTPUT]: Class %s (acc): %.2f %\r\n", activities[i], out_data[i]);

						printf("\r\n[LOG]: Destroying the Neural Network...\r\n");
						neural_net->nnDestroy(network);
						while (startflag == false) {
							if (usrbtn::value() == 0) {
								startflag = true;
								Thread::sleep(500);
							}
						}
						startflag = false;


					}

				}


			}
		}
	}


}




