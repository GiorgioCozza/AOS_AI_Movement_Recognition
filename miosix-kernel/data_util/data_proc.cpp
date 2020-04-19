
/**
 *******************************************************************************
  *   @file data_proc.cpp
  *   @author Cozza Giorgio
  *   @date 08/10/19
  *   @version 1.0
  *   @brief Function toolkit to process data.
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



#include "data_proc.h"


float data_proc::get_min(uint8_t sens, uint8_t ax) {
	return data_proc::min[3 * sens + ax];
}

float data_proc::get_max(uint8_t sens, uint8_t ax) {
	return data_proc::max[3 * sens + ax];
}

void data_proc::normalize(float* in_vect) {

	uint16_t vec_cntr = 0;
	while (vec_cntr < VECTOR_SIZE) {
		for (int j = 0; j < WINDOW_SIZE; j++)
			*(in_vect + VECTOR_SIZE * j + vec_cntr) = (*(in_vect + VECTOR_SIZE * j + vec_cntr) - *(data_proc::min + vec_cntr))\
													/ ((*(data_proc::max + vec_cntr) - *(data_proc::min + vec_cntr))+0.001);

		vec_cntr++;
	}

}


void data_proc::update_min_max(int32_t *sen_read, int sen) {

	float tmp = 0;
	for (int i = 0; i < 3; i++) {
		tmp = (float)sen_read[i + 3 * sen];
		if (fst_read[i + 3 * sen]) {
			data_proc::min[i + 3 * sen] = tmp;
			data_proc::max[i + 3 * sen] = tmp;
			fst_read[i + 3 * sen] = 0;
		}
		else {
			if (tmp < data_proc::min[i + 3 * sen])
				data_proc::min[i + 3 * sen] = tmp;

			if (tmp > data_proc::max[i + 3 * sen])
				data_proc::max[i + 3 * sen] = tmp;
		}
	}
	
}


uint8_t data_proc::get_argmax(const float * vec, const uint16_t vec_sz){

    uint8_t idx_max=0;
    float _max = 0;
    for(int i = 0; i < vec_sz; i++)
        if (vec[i] > _max){
            idx_max = i;
            _max = vec[i];
        }

    return idx_max;
}


uint8_t data_proc::get_mode(const uint8_t * vec, const uint16_t vec_sz, const uint8_t cat_num){

    uint16_t * cat_vec = new uint16_t[cat_num];
    uint8_t _max = 0, mode_idx = 0;

    for(int i = 0; i < vec_sz; i++){
        cat_vec[vec[i]] += 1;
        if (cat_vec[vec[i]] > _max ) {
            _max = cat_vec[vec[i]];
            mode_idx = vec[i];
        }
    }
    delete[] cat_vec;
    return mode_idx;
}


void data_proc::print_min(void) {

	printf("\r\n[LOG]: MIN	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM6DSL_ACC, "X", data_proc::min[0], "Y", data_proc::min[1], "Z", data_proc::min[2]);
	printf("\r[LOG]: MIN	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM6DSL_GYR, "X", data_proc::min[3], "Y", data_proc::min[4], "Z", data_proc::min[5]);
	printf("\r[LOG]: MIN	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM303AGR_ACC, "X", data_proc::min[6], "Y", data_proc::min[7], "Z", data_proc::min[8]);
	printf("\r[LOG]: MIN	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM303AGR_MAG, "X", data_proc::min[9], "Y", data_proc::min[10], "Z", data_proc::min[11]);

}

void data_proc::print_max(void) {
	printf("\r\n[LOG]: MAX	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM6DSL_ACC, "X", data_proc::max[0], "Y", data_proc::max[1], "Z", data_proc::max[2]);
	printf("\r[LOG]: MAX	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM6DSL_GYR, "X", data_proc::max[3], "Y", data_proc::max[4], "Z", data_proc::max[5]);
	printf("\r[LOG]: MAX	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM303AGR_ACC, "X", data_proc::max[6], "Y", data_proc::max[7], "Z", data_proc::max[8]);
	printf("\r[LOG]: MAX	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM303AGR_MAG, "X", data_proc::max[9], "Y", data_proc::max[10], "Z", data_proc::max[11]);

}

void data_proc::print_dataset(float* in_v) {

	printf("\r\n|***** LSM6DSL_ACCELEROMETER *****||***** LSM6DSL_GYROSCOPE *****||***** LSM303A_ACCELEROMETER ****||****** LSM303A_MAGNETOMETER *****|\r\n");
	printf("    |                                 ||                             ||                                ||                                 |    ");
	for (int i = 0; i < WINDOW_SIZE; i++) {
		printf("\r\n|   X: %f   Y:%f   Z:%f   ||   X: %f   Y:%f   Z:%f   ||   X: %f   Y:%f   Z:%f   ||   X: %f   Y:%f   Z:%f   ||\r\n",
			*(float*)(in_v + VECTOR_SIZE * i), *(float*)(in_v + VECTOR_SIZE * i + 1), *(float*)(in_v + VECTOR_SIZE * i + 2), *(float*)(in_v + VECTOR_SIZE * i + 3), *(float*)(in_v + VECTOR_SIZE * i + 4),
			*(float*)(in_v + VECTOR_SIZE * i + 5), *(float*)(in_v + VECTOR_SIZE * i + 6), *(float*)(in_v + VECTOR_SIZE * i + 7), *(float*)(in_v + VECTOR_SIZE * i + 8),
			*(float*)(in_v + VECTOR_SIZE * i + 9), *(float*)(in_v + VECTOR_SIZE * i + 10), *(float*)(in_v + VECTOR_SIZE * i + 11));
	}
}