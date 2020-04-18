#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include "miosix.h"
#include "main_config.h"
#include "i2c_helper.h"
#include "circular_queue.h"
#include "LSM6DSL.h"
#include "LSM303AGR.h"
#include "NN.h"


using namespace std;
using namespace miosix;


bool startflag = false;
typedef Gpio<GPIOC_BASE, 13> usrbtn;
typedef Gpio<GPIOA_BASE, 5> usrled;

static const char* movements[7] = { "RUNNING", "WALKING", "JUMPING", "STANDING", "SITTING", "SUPINE", "LYING ON SIDE"};


void print_on_serial(float* int_vec, uint32_t smp_cnt) {

    printf(" %lu, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f", smp_cnt, (float)int_vec[0], (float)int_vec[1], (float)int_vec[2], \
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

    printf("\r\n");

}

void clear_s(void){

    char ESC = 27;
    printf("%c[H", ESC);
    printf("%c[2J", ESC);
}


int main() {

    {
        FastInterruptDisableLock a;
        RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
        CRC->CR = CRC_CR_RESET;
    }


    float *in_data = nullptr;
    float *out_data = (float *) malloc(NUM_CLASSES * sizeof(float));
    float *int_vec = new float[VECTOR_SIZE];
    int32_t buf_reader[3];
    uint8_t pred_cnt = 0;
    uint8_t pred_vec[PRED_SIZE];

    // sensor drivers vars
    LSM6DSLAccGyr *acc_gyr = new LSM6DSLAccGyr();
    LSM303AGRAccMag *acc_mag = new LSM303AGRAccMag();  //++

    // circular buffering vars
    circularQueue<float> data_queue(VECTOR_SIZE * WINDOW_SIZE);
    data_proc dt_proc;

    // neural network vars
    ai_handle network = AI_HANDLE_NULL;
    ai_buffer ai_input = AI_NETWORK_IN;
    ai_buffer ai_output = AI_NETWORK_OUT;
    ai_u8 *activations = new ai_u8[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
    NN *neural_net = new NN();

    uint32_t count = 0;

    // USR button vars
    startflag = false;
    usrbtn::mode(Mode::INPUT_PULL_UP);

    //enable board sensors
    bool ag_en = acc_gyr->init();
    bool am_en = acc_mag->init();

    print_config(acc_gyr, acc_mag);

    in_data = data_queue.getCircBuf();

    Timer time;

    printf("\r\n********************   AOS: Neural Network on STM32 with STM32CubeAI	 **********************\r\n");

    if ( ag_en && am_en ) {
        if (neural_net->nnCreate(&network)) {
            if (neural_net->nnInit(network, (ai_network_params *) AI_NETWORK_DATA_CONFIG, activations)) {

                neural_net->showNNInfo((const ai_handle) network);

                while (1) {

                    // check USR button to start
                    if (startflag == false)
                        printf("\r\nPress USR button to start...\r\n");
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


                    // read LSM6DSL accelerometer axes
                    if (acc_gyr->get_acc_axes(buf_reader)) {
                        int_vec[0] = (float) buf_reader[0];
                        int_vec[1] = (float) buf_reader[1];
                        int_vec[2] = (float) buf_reader[2];
                    }

                    // read LSM6DSL gyroscope axes
                    if (acc_gyr->get_gyr_axes(buf_reader)) {
                        int_vec[3] = (float) buf_reader[0];//
                        int_vec[4] = (float) buf_reader[1];//
                        int_vec[5] = (float) buf_reader[2];//
                    }

                    // read LSM303AGR accelerometer axes
                    if (acc_mag->get_acc_axes(buf_reader)) {
                        int_vec[6] = (float) buf_reader[0];
                        int_vec[7] = (float) buf_reader[1];
                        int_vec[8] = (float) buf_reader[2];
                    }

                    // read LSM303AGR magnetometer axes
                    if (acc_mag->get_mag_axes(buf_reader)) {
                        int_vec[9] = (float) buf_reader[0];
                        int_vec[10] = (float) buf_reader[1];
                        int_vec[11] = (float) buf_reader[2];
                    }


                    data_queue.insert(int_vec, VECTOR_SIZE);

                    count++;
                    if (count >= WINDOW_SIZE) {
                        count = 0;

                        neural_net->prepareData(&dt_proc, in_data, out_data, &ai_input, &ai_output, 1);

                        if (neural_net->nnRun(network, &ai_input, &ai_output, 1) > 0) {
                            pred_vec[pred_cnt] = dt_proc.get_argmax((const float *) out_data,
                                                                    (const uint8_t) NUM_CLASSES);

                            if (PRED_SIZE == 0) {

                                // real-time predictions
                                clear_s();

                                printf("\n[RESULT] You are %s\n\n", movements[pred_vec[pred_cnt]]);
                                pred_cnt = 0;

                            } else if (pred_cnt >= PRED_SIZE) {

                                // final prediction as mode of several observations
                                clear_s();

                                uint8_t pred_mode = dt_proc.get_mode((const uint8_t *) pred_vec,
                                                                (const uint16_t) PRED_SIZE,
                                                                (const uint8_t) NUM_CLASSES);

                                printf("\n[RESULT] You are %s\n\n", movements[pred_mode]);

                                pred_cnt = 0;
                                startflag = false;

                            }
                            pred_cnt++;


                        }
                    }

                }
            }


        }
    }


}

