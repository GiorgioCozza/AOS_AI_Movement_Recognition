#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include "miosix.h"
#include "prog_config.h"
#include "nn_config.h"
#include "i2c_helper.h"
#include "circular_queue.h"
#include "LSM6DSL.h"
#include "LSM303AGR.h"
#include "XNUCLEO_IKS01A2.h"
#include "NN.h"


using namespace std;
using namespace miosix;


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

void print_config(LSM6DSLAccGyr * acc_gyr_ptr, LSM303AGRAccMag * acc_mag_ptr){

    uint8_t id = 0;
    float odr = 0.0, fs = 0.0, sens = 0.0;

    if (acc_gyr_ptr->read_id(&id))
        printf("\r\n[LOG] %s ACCELEROMETER (id: %d) CONFIG: \n", LSM6DSL_ACC, id);
    if (acc_gyr_ptr->get_acc_odr(&odr))
        printf("\n\t- Output Data Rate: %.0f Hz", odr);
    if (acc_gyr_ptr->get_acc_fs(&fs))
        printf("\n\t- Full Scale Value: %.1f g", fs);
    if (acc_gyr_ptr->get_acc_sensitivity(&sens))
        printf("\n\t- Sensitivity: %.3f", sens);

    printf("\n");

    if (acc_gyr_ptr->read_id(&id))
        printf("\r\n[LOG] %s GYROSCOPE (id: %d) CONFIG: \n", LSM6DSL_GYR, id);
    if (acc_gyr_ptr->get_gyr_odr(&odr))
        printf("\n\t- Output Data Rate: %.0f Hz", odr);
    if (acc_gyr_ptr->get_gyr_fs(&fs))
        printf("\n\t- Full Scale Value: %.1f g", fs);
    if (acc_gyr_ptr->get_gyr_sensitivity(&sens))
        printf("\n\t- Sensitivity: %.3f", sens);

    printf("\n");

    if (acc_mag_ptr->read_acc_id(&id))
        printf("\r\n[LOG] %s ACCELEROMETER (id: %d) CONFIG: \n", LSM303AGR_ACC, id);
    if (acc_mag_ptr->get_acc_odr(&odr))
        printf("\n\t- Output Data Rate: %.0f Hz", odr);
    if (acc_mag_ptr->get_acc_fs(&fs))
        printf("\n\t- Full Scale Value: %.1f g", fs);
    if (acc_mag_ptr->get_acc_sensitivity(&sens))
        printf("\n\t- Sensitivity: %.3f", sens);

    printf("\n");

    if (acc_mag_ptr->read_mag_id(&id))
        printf("\r\n[LOG] %s ACCELEROMETER (id: %d) CONFIG: \n", LSM303AGR_MAG, id);
    if (acc_mag_ptr->get_mag_odr(&odr))
        printf("\n\t- Output Data Rate: %.0f Hz", odr);
    if (acc_mag_ptr->get_mag_fs(&fs))
        printf("\n\t- Full Scale Value: %.1f g", fs);
    if (acc_mag_ptr->get_mag_sensitivity(&sens))
        printf("\n\t- Sensitivity: %.3f", sens);

    printf("\n");

}


uint8_t argmax(float * vec, uint16_t vec_sz){

    uint16_t idx_max=0;
    float _max = 0;
    for(int i = 0; i < vec_sz; i++)
        if (vec[i] > _max){
            idx_max = i;
            _max = vec[i];
        }

    return idx_max;
}



int main() {

    {
        FastInterruptDisableLock a;
        RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
        CRC->CR = CRC_CR_RESET;
    }
    printf("#######################   |AOS: Neural Network on STM32 with STM32CubeAI|	#####################################\r\n");

    int32_t buf_reader[3];

    float *in_data = nullptr;
    float *out_data = (float *) malloc(NUM_CLASSES * sizeof(float));
    float *int_vec = new float[VECTOR_SIZE];
    uint16_t vec_sz = WINDOW_SIZE * VECTOR_SIZE;

    LSM6DSLAccGyr *acc_gyr = new LSM6DSLAccGyr(LSM6DSL_I2C_ADDRESS_HIGH);
    LSM303AGRAccMag *acc_mag = new LSM303AGRAccMag(LSM303AGRMag_I2C_ADDR, LSM303AGRAcc_I2C_ADDR);  //++

    // struct declaration to use for input segment preprocessing

    circularQueue<float> queue(VECTOR_SIZE * WINDOW_SIZE);
    dataset_preproc ds_pp;

    // network variable declaration
    ai_handle network = AI_HANDLE_NULL;
    ai_buffer ai_input = AI_NETWORK_IN;
    ai_buffer ai_output = AI_NETWORK_OUT;
    ai_u8 *activations = new ai_u8[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
    NN *neural_net = new NN();


    uint32_t tmp_val = 0;
    uint32_t sample_cnt = 0;
    int count = 0;

    startflag = false;
    usrbtn::mode(Mode::INPUT_PULL_UP);
    //enable board sensors

    bool en1 = acc_gyr->init();
    bool en2 = acc_mag->init();

    print_config(acc_gyr, acc_mag);

    in_data = queue.getCircBuf();

    if (neural_net->nnCreate(&network)) {
        if (neural_net->nnInit(network, (ai_network_params*)AI_NETWORK_DATA_CONFIG, activations)) {


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

                char ESC = 27;
                printf("%c[H", ESC);
                printf("%c[2J", ESC);

                if (acc_gyr->get_acc_axes(buf_reader)) {
                    int_vec[0] = (float) buf_reader[0];
                    int_vec[1] = (float) buf_reader[1];
                    int_vec[2] = (float) buf_reader[2];
                }

                if (acc_gyr->get_gyr_axes(buf_reader)) {
                    int_vec[3] = (float) buf_reader[0];//
                    int_vec[4] = (float) buf_reader[1];//
                    int_vec[5] = (float) buf_reader[2];//
                }

                if (acc_mag->get_acc_axes(buf_reader)) {
                    int_vec[6] = (float) buf_reader[0];
                    int_vec[7] = (float) buf_reader[1];
                    int_vec[8] = (float) buf_reader[2];
                }

                if (acc_mag->get_mag_axes(buf_reader)) {
                    int_vec[9] = (float) buf_reader[0];
                    int_vec[10] = (float) buf_reader[1];
                    int_vec[11] = (float) buf_reader[2];
                }


                queue.insert(int_vec, VECTOR_SIZE);

                //Clear terminal screen
                printf("%c[H", ESC);
                printf("%c[2J", ESC);
                //Thread::sleep(100);

                count++;

                if (count == WINDOW_SIZE) {
                    count = 0;

                    neural_net->prepareData(&ds_pp, in_data, out_data, &ai_input, &ai_output, 1);

                    //printf("\r\n[LOG]: Running the Neural Network...\r\n");
                    int n_b = neural_net->nnRun(network, &ai_input, &ai_output, 1);

                    printf("\r\n****************	AI NN NETWORK RESULT	*********************\r\n");
                    printf("\n[LOG]: You are %s\n\n", movements[argmax(out_data, NUM_CLASSES)]);
                    Thread::sleep(50);
                    /*
                    printf("\r\n****************	AI NN NETWORK RESULT	*********************\r\n");
                    printf("\r\n[LOG]: OUTPUT N_BATCHES: %d\r\n", n_b);
                    for (int i = 0; i < NUM_CLASSES; i++)
                        printf("[OUTPUT]: Class %s (acc): %.2f %\r\n", movements[i], out_data[i]);
    */
                }

            }
        }


    }

}

