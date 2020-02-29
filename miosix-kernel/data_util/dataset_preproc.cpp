
/**
 *******************************************************************************
  *   @file dataset_preprocessor.cpp
  *   @author Cozza Giorgio
  *   @date 08/10/19
  *   @version 1.0
  *   @brief Function toolkit to pre-process the NN input dataset.
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



#include "dataset_preproc.h"


float dataset_preproc::get_min(uint8_t sens, uint8_t ax) {
	return dataset_preproc::min[3 * sens + ax];
}

float dataset_preproc::get_max(uint8_t sens, uint8_t ax) {
	return dataset_preproc::max[3 * sens + ax];
}

void dataset_preproc::normalize(float* in_vect) {

	uint16_t vec_cntr = 0;
	while (vec_cntr < VECTOR_SIZE) {
		for (int j = 0; j < WINDOW_SIZE; j++)
			*(in_vect + VECTOR_SIZE * j + vec_cntr) = (*(in_vect + VECTOR_SIZE * j + vec_cntr) - *(dataset_preproc::min + vec_cntr))\
													/ ((*(dataset_preproc::max + vec_cntr) - *(dataset_preproc::min + vec_cntr))+0.000000001);

		vec_cntr++;
	}

}


void dataset_preproc::update_min_max(int32_t *sen_read, int sen) {

	float tmp = 0;
	for (int i = 0; i < 3; i++) {
		tmp = (float)sen_read[i + 3 * sen];
		if (fst_read[i + 3 * sen]) {
			dataset_preproc::min[i + 3 * sen] = tmp;
			dataset_preproc::max[i + 3 * sen] = tmp;
			fst_read[i + 3 * sen] = 0;
		}
		else {
			if (tmp < dataset_preproc::min[i + 3 * sen])
				dataset_preproc::min[i + 3 * sen] = tmp;

			if (tmp > dataset_preproc::max[i + 3 * sen])
				dataset_preproc::max[i + 3 * sen] = tmp;
		}
	}
	
}


void dataset_preproc::print_min(void) {

	printf("\r\n[LOG]: MIN	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM6DSL_ACC, "X", dataset_preproc::min[0], "Y", dataset_preproc::min[1], "Z", dataset_preproc::min[2]);
	printf("\r[LOG]: MIN	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM6DSL_GYR, "X", dataset_preproc::min[3], "Y", dataset_preproc::min[4], "Z", dataset_preproc::min[5]);
	printf("\r[LOG]: MIN	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM303AGR_ACC, "X", dataset_preproc::min[6], "Y", dataset_preproc::min[7], "Z", dataset_preproc::min[8]);
	printf("\r[LOG]: MIN	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM303AGR_MAG, "X", dataset_preproc::min[9], "Y", dataset_preproc::min[10], "Z", dataset_preproc::min[11]);

}

void dataset_preproc::print_max(void) {
	printf("\r\n[LOG]: MAX	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM6DSL_ACC, "X", dataset_preproc::max[0], "Y", dataset_preproc::max[1], "Z", dataset_preproc::max[2]);
	printf("\r[LOG]: MAX	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM6DSL_GYR, "X", dataset_preproc::max[3], "Y", dataset_preproc::max[4], "Z", dataset_preproc::max[5]);
	printf("\r[LOG]: MAX	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM303AGR_ACC, "X", dataset_preproc::max[6], "Y", dataset_preproc::max[7], "Z", dataset_preproc::max[8]);
	printf("\r[LOG]: MAX	%s		%s: %.2f,	%s: %.2f,	%s: %.2f\r\n", LSM303AGR_MAG, "X", dataset_preproc::max[9], "Y", dataset_preproc::max[10], "Z", dataset_preproc::max[11]);

}

void dataset_preproc::print_dataset(float* in_v) {

	printf("\r\n|***** LSM6DSL_ACCELEROMETER *****||***** LSM6DSL_GYROSCOPE *****||***** LSM303A_ACCELEROMETER ****||****** LSM303A_MAGNETOMETER *****|\r\n");
	printf("    |                                 ||                             ||                                ||                                 |    ");
	for (int i = 0; i < WINDOW_SIZE; i++) {
		printf("\r\n|   X: %f   Y:%f   Z:%f   ||   X: %f   Y:%f   Z:%f   ||   X: %f   Y:%f   Z:%f   ||   X: %f   Y:%f   Z:%f   ||\r\n",
			*(float*)(in_v + VECTOR_SIZE * i), *(float*)(in_v + VECTOR_SIZE * i + 1), *(float*)(in_v + VECTOR_SIZE * i + 2), *(float*)(in_v + VECTOR_SIZE * i + 3), *(float*)(in_v + VECTOR_SIZE * i + 4),
			*(float*)(in_v + VECTOR_SIZE * i + 5), *(float*)(in_v + VECTOR_SIZE * i + 6), *(float*)(in_v + VECTOR_SIZE * i + 7), *(float*)(in_v + VECTOR_SIZE * i + 8),
			*(float*)(in_v + VECTOR_SIZE * i + 9), *(float*)(in_v + VECTOR_SIZE * i + 10), *(float*)(in_v + VECTOR_SIZE * i + 11));
	}
}