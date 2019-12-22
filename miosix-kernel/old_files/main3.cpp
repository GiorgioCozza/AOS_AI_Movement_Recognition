


#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include "miosix.h"
#include "i2c_helper.h"
#include "circular_queue.h"
#include "sensor_driver/HTS221/HTS221Sensor.h"
#include "sensor_driver/LSM6DSL/LSM6DSLSensor.h"
#include "sensor_driver/LSM303AGR/LSM303AGRAccSensor.h"
#include "sensor_driver/LSM303AGR/LSM303AGRMagSensor.h"


#define LSM6DSL_I2C_ADDR		LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH
#define HTS221_I2C_ADDR			HTS221_I2C_ADDRESS
#define LSM303AGR_I2C_ADDR		LSM303AGR_MAG_I2C_ADDRESS


#include "AI/AIinclude/network.h"
#include "AI/data/network_data.h"

#define LSM6DSL_I2C_ADDR		LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH
#define HTS221_I2C_ADDR			HTS221_I2C_ADDRESS
#define LSM303AGRAcc_I2C_ADDR	0x32
#define LSM303AGRMag_I2C_ADDR	LSM303AGR_MAG_I2C_ADDRESS        
#define ARDUINO_MKR1000_ADDR	0x04

#define READ_1B					1
#define READ_2B					2
#define READ_4B					4

#define vec_SIZE                120

using namespace std;
using namespace miosix;


/* Global handle to reference the instantiated NN */
//static ai_handle network = AI_HANDLE_NULL;
/* Global buffer to handle the activations data buffer - R/W data */
/*AI_ALIGNED(4)
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
static ai_buffer ai_input[120] = { AI_NETWORK_IN_1 };
static ai_buffer ai_output[1] = { AI_NETWORK_OUT_1 };
ai_float vec_data[vec_SIZE];


int aiInit(void) {
	ai_error err;
	//ai_network_report report;
	//ai_bool res;

	err = ai_network_create(&network, (const ai_buffer *)AI_NETWORK_DATA_CONFIG);
	if (err.type != AI_ERROR_NONE) {
		printf("ai error -type=%d code = %d\r\n", err.type, err.code);
	}

	//res = ai_network_get_info(network, &report)
	//if(res){
		//printf("report\n");
	//}

	// initialize network
	const ai_network_params params = {
	AI_NETWORK_DATA_WEIGHTS(ai_network_data_weights_get()),
	AI_NETWORK_DATA_ACTIVATIONS(activations) };
	if (!ai_network_init(network, &params)) {
		err = ai_network_get_error(network);
	}
	return 0;
}

int aiRun(const ai_float* in_data, ai_float* out_data,
	const ai_u16 batch_size)
{
	ai_i32 nbatch;
	ai_error error2;

	// initialize input/output buffer handlers
	ai_input[0].n_batches = batch_size;
	ai_input[0].data = AI_HANDLE_PTR(in_data);
	ai_output[0].n_batches = batch_size;
	ai_output[0].data = AI_HANDLE_PTR(out_data);

	nbatch = ai_network_run(network, &ai_input[0], &ai_output[0]);
	if (nbatch != batch_size) {
		error2 = ai_network_get_error(network);
		// manage the error

	}
	return 0;
}


int main()
{

	uint8_t arduinoAddr = 4;
	uint8_t buf_sender[4];
	uint8_t devId;
	float tem;
	int32_t buf_reader[3];
	HTS221Sensor* temp_hum = new HTS221Sensor(HTS221_I2C_ADDR);
	LSM6DSLSensor* acc_gyr = new LSM6DSLSensor(LSM6DSL_I2C_ADDR);


	int en1 = acc_gyr->enable_x();
	int en2 = acc_gyr->enable_g();

	if (!en1 && !en2) {
		uint8_t ard_addr = (ARDUINO_MKR1000_ADDR << 1) | 0;

		if (!acc_gyr->read_id(&devId)) {
			//(I2CHelper::getInstance())->write(buf, ard_addr, 0, 1, true);
			delayUs(1000);
			while (1) {

				//Arduino Communication

				if (!acc_gyr->read_id(&devId))
				{
					printf("\nDEV ID: %d\n", devId);
				}
				if (!acc_gyr->get_x_axes(buf_reader)) {

					printf("\n********* Accelerometer *********\n");
					printf("\nX: %6ld\n", buf_reader[0]);
					printf("Y: %6ld\n", buf_reader[1]);
					printf("Z: %6ld\n", buf_reader[2]);

					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < READ_4B; j++)
						{
							buf_sender[(READ_4B - (j + 1))] = (uint8_t)(buf_reader[i] >> 8 * j);
						}
						printf("\nbuf_sender[0]: %d\n", buf_sender[0]);
						printf("buf_sender[1]: %d\n", buf_sender[1]);
						printf("buf_sender[2]: %d\n", buf_sender[2]);
						printf("buf_sender[3]: %d\n", buf_sender[3]);
						(I2CHelper::getInstance())->write(buf_sender, ard_addr, 0, 4, true);
						delayUs(1000);
					}

					Thread::sleep(5000);

					if (!acc_gyr->get_g_axes(buf_reader)) {

						printf("\n********* Gyroscope *********\n");
						printf("\nX: %6ld\n", buf_reader[0]);
						printf("Y: %6ld\n", buf_reader[1]);
						printf("Z: %6ld\n", buf_reader[2]);

						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < READ_4B; j++)
							{
								buf_sender[(READ_4B - (j + 1))] = (uint8_t)(buf_reader[i] >> 8 * j);
							}
							printf("\nbuf_sender[0]: %d\n", buf_sender[0]);
							printf("buf_sender[1]: %d\n", buf_sender[1]);
							printf("buf_sender[2]: %d\n", buf_sender[2]);
							printf("buf_sender[3]: %d\n", buf_sender[3]);
							int32_t tmp_val = (uint32_t)(((uint32_t)buf_sender[0] << 24) \
								| ((uint32_t)buf_sender[1] << 16) \
								| ((uint16_t)buf_sender[2] << 8) \
								| ((uint8_t)buf_sender[3]));
							printf("%6ld\n", tmp_val);
							(I2CHelper::getInstance())->write(buf_sender, ard_addr, 0, 4, true);
							delayUs(1000);
						}

					}


				}

				if (!temp_hum->getTemperature(&tem)) {

				}

				Thread::sleep(5000);
			}

		}
	}

	if (!temp_hum->getTemperature(&tem)) {	}


	//uint8_t* c2 = *(buf[1]);
	//(I2CHelper::getInstance())->write(*c2, ard_addr, 0, 1, true);

}
*/

/*
void printing2(int32_t * vector) {
	for (int i = 0; i < 12; i++) {
		printf("%6ld", vector[i]);
	}
	printf("\n");
}
*/

void printing2(int32_t* vector, char* sensor) {

	printf("%s:     ", sensor);
	printf("X: %6ld  ", vector[0]);
	printf("Y: %6ld  ", vector[1]);
	printf("Z: %6ld  ", vector[2]);
	printf("\n");
}

int main() {

	uint8_t arduinoAddr = 4;
	uint8_t buf_sender[4];
	uint8_t devId;  // modify with 3 ID
	uint8_t devId2;
	uint8_t devId3;
	float tem;
	int32_t buf_reader[3];


	//aiInit();

	int32_t* vector;
	vector = (int32_t*)malloc(VECTOR_SIZE * sizeof(int32_t));

	int32_t** matrix;
	matrix = (int32_t * *)malloc(WINDOW_SIZE * sizeof(int32_t*));
	for (int i = 0; i < WINDOW_SIZE; i++)
		matrix[i] = (int32_t*)malloc(VECTOR_SIZE * sizeof(int32_t));

	HTS221Sensor * temp_hum = new HTS221Sensor(HTS221_I2C_ADDR);
	LSM6DSLSensor * acc_gyr = new LSM6DSLSensor(LSM6DSL_I2C_ADDR);

	LSM303AGRAccSensor * acc = new LSM303AGRAccSensor(LSM303AGRAcc_I2C_ADDR);  //++
	LSM303AGRMagSensor * mag = new LSM303AGRMagSensor(LSM303AGR_MAG_I2C_ADDRESS);  //++

	circularQueue queue;

	uint32_t tmp_val = 0;

	int count = 0;

	int en1 = acc_gyr->enable_x();
	int en2 = acc_gyr->enable_g();
	int en3 = acc->enable();
	int en4 = mag->enable();

	if (!acc_gyr->read_id(&devId))
	{
		printf("\nDEV ID: %d\n", devId);
	}
	if (!acc->read_id(&devId2))
	{
		printf("\nDEV ID: %d\n", devId2);
	}
	if (!mag->read_id(&devId3))
	{
		printf("\nDEV ID: %d\n", devId3);
	}

	if (!en1 && !en2 && !en3 && !en4) {

		if (!acc_gyr->read_id(&devId) && !acc->read_id(&devId2) && !mag->read_id(&devId3)) {  //++	      
			while (1) {

				//count++;

				if (!acc_gyr->get_x_axes(buf_reader)) {
					vector[0] = buf_reader[0];
					vector[1] = buf_reader[1];
					vector[2] = buf_reader[2];
				}

				Thread::sleep(50);
				printing2(buf_reader, "\n\nLSM6DSL_ACCELEROMETER");


				if (!acc_gyr->get_g_axes(buf_reader)) {
					vector[3] = buf_reader[0] >> 6;
					vector[4] = buf_reader[1] >> 6;
					vector[5] = buf_reader[2] >> 6;
				}

				Thread::sleep(50);
				printing2(buf_reader, "LSM6DSL_GYROSCOPE____");

				if (!acc->get_x_axes(buf_reader)) {
					vector[6] = buf_reader[0];
					vector[7] = buf_reader[1];
					vector[8] = buf_reader[2];
				}
				else {
					vector[6] = buf_reader[0];
					vector[7] = buf_reader[1];
					vector[8] = buf_reader[2];
				}

				Thread::sleep(50);
				printing2(buf_reader, "LSM303A_ACCELEROMETER");

				if (!mag->get_m_axes(buf_reader)) {
					vector[9] = buf_reader[0];
					vector[10] = buf_reader[1];
					vector[11] = buf_reader[2];
				}

				Thread::sleep(50);
				printing2(buf_reader, "LSM303A_MAGNETOMETER_");

				Thread::sleep(500);


				/*queue.insert(vector);

				if(count ==20){
					count =0;
					queue.printing();
				}
				*/
				//printing2(buf_reader);

				Thread::sleep(50);
			}

		}

	}
}
