
/**
 *******************************************************************************
  *   @file dataset_processing.c
  *   @author Cozza Giorgio
  *   @date 13/06/19
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

#include "dataset_processing.h"
#include <stddef.h>
/*
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_t thr[MAX_AV_THREADS];
ai_bool busy_thr[MAX_AV_THREADS] = { false, false };
*/
char lsm6dsl_acc[] = "LSM6DSL_ACCELEROMETER";
char lsm6dsl_gyr[] = "LSM6DSL_GYROSCOPE";
char lsm303a_acc[] = "LSM303A_ACCELEROMETER";
char lsm303a_mag[] = "LSM303A_MAGNETOMETER";
char* sensors[4] = { lsm6dsl_acc, lsm6dsl_gyr, lsm303a_acc, lsm303a_mag };
char axis[3] = { 'X','Y','Z' };

ai_float* process_vector_dataset(proc_utils* nu_t, void* (*proc_rtn)(void*)) {

	if ((nu_t->matrix != NULL || nu_t->inp_vec != NULL) && proc_rtn != NULL) {
		printf("\r\n[LOG]: Waiting for data processing...\r\n");
		ai_float* in_data;
		/*process the input segment for all the columns*/
		nu_t->counter = 0;
		while (nu_t->counter < 12) {
			/* scan the thread array to search an available one*/
			//int i;
			//for (i = 0; i < MAX_AV_THREADS; i++) {
				//if (busy_thr[i] == false) {
					//busy_thr[i] = true;
					//printf("\r\n[LOG]: The thread for the axis %d has been created\r\n", nu_t->counter);
			//printf("\r\n[LOG]: Thread n.%d created!\r\n", nu_t->counter);
			
			normalize(nu_t);

			//nu_t->idx = i;
			//int res = pthread_create((thr + i), NULL, proc_rtn(nu_t), (void*)& nu_t);
			//if (res == 0) {
			//}
	//	}
	//}
		}
		printf("\r\n[LOG]: RESULTS on the given DATASET\r\n");
		int k;
		for (k = 0; k < 4; k++) {
			printf("\r\n[LOG]: MEAN		%s		%c: %.2f,	%c: %.2f,	%c: %.2f\r\n", sensors[k], axis[0], nu_t->mean[3*k+0], axis[1], nu_t->mean[3*k+1], axis[2], nu_t->mean[3*k+2]);
			printf("\r[LOG]: SIGMA		%s		%c: %.2f,	%c: %.2f,	%c: %.2f\r\n", sensors[k], axis[0], nu_t->sigma[3 * k + 0], axis[1], nu_t->sigma[3 * k + 1], axis[2], nu_t->sigma[3 * k + 2]);
		}
		return nu_t->inp_vec;
	}
	else {
		return NULL;
	}
}


void* normalize(void* nor_ut) {

	uint8_t j, val_count;

	//pthread_mutex_lock(&mut);
	proc_utils* n_ut = (proc_utils*)nor_ut;
	//pthread_mutex_unlock(&mut);

	uint8_t* cnt = &(n_ut->counter);

	ai_float val, mn = 0, sgm = 0;
	val_count = 0;
	/*compute the mean of the current column*/
	for (j = 0; j < n_ut->n_rows; j++) {
		val = *(n_ut->inp_vec + DEFAULT_COLUMNS_N*j + *cnt);
		//printf("[LOG]: VALUE = %f\r\n", val);
		val_count++;
		mn += val;
	}
	mn = mn / (float)val_count;
	//pthread_mutex_lock(&mut);
	*(n_ut->mean + *cnt) = mn;
	//pthread_mutex_unlock(&mut);
	//printf("\r\n[LOG]: MEAN = %f\r\n", mn);

	val_count = 0;
	/* compute the standard deviation of the current column*/
	for (j = 0; j < n_ut->n_rows; j++) {
		val = *(n_ut->inp_vec + DEFAULT_COLUMNS_N * j + *cnt);

		sgm += AI_MATH_POW(val, 2);
		//printf("[LOG]: VALUE = %f\r\n", val);
		val_count++;
	}
	sgm = AI_MATH_SQRT(sgm / (float)val_count);

	//pthread_mutex_lock(&mut);
	*(n_ut->sigma + *cnt) = sgm;
	//pthread_mutex_unlock(&mut);
	//printf("\r\n[LOG]: SIGMA = %f\r\n", sgm);


	val_count = 1;
	/* process each single sample using the previous computed values*/
	for (j = 0; j < n_ut->n_rows; j++) {
		//pthread_mutex_lock(&mut);
		*(n_ut->inp_vec + DEFAULT_COLUMNS_N * j + *cnt) = (*(n_ut->inp_vec + DEFAULT_COLUMNS_N * j + *cnt) - *(n_ut->mean + *cnt)) / *(n_ut->sigma + *cnt);
		//pthread_mutex_unlock(&mut);
		//printf("[LOG]:  VALUE = %f\r\n", *(*(n_ut->matrix + j) + *cnt));
	}

	//pthread_mutex_lock(&mut);
	n_ut->counter++;
	//busy_thr[n_ut->idx] = false;
	//pthread_mutex_unlock(&mut);
}

