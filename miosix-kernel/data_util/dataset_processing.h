/**
 *******************************************************************************
  *   @file dataset_processing.c
  *   @author Cozza Giorgio, Liu Jiaqi
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

#ifndef _DATASET_PROCESSING_H_
#define _DATASET_PROCESSING_H_

#ifdef __cplusplus
extern "C" {
#endif

	/*----------------------------   INCLUDES	----------------------------------*/
#include "network.h"
#include "network_data.h"
#include "ai_math_helpers.h"
#include <stdint.h>
//#include <pthread.h>

/*--------------------------    DEFINITIONS	    ------------------------------*/
#define MAX_AV_THREADS			1
#define DEFAULT_ROWS_N			50
#define DEFAULT_COLUMNS_N		12
#define DEFAULT_MEAN_INIT		AI_MEM_ALLOC(DEFAULT_COLUMNS_N, float)
#define DEFAULT_SIGMA_INIT		AI_MEM_ALLOC(DEFAULT_COLUMNS_N, float)



typedef void* (*prc_rtn)(void*);

#define PROC_VEC_INIT(vec_, r_, c_)								\
		{	.mean = (float *)malloc(c_*sizeof(float)),				\
			.sigma = (float *)malloc(c_*sizeof(float)),				\			
			.counter = 0, \
			.matrix = nullptr, \
			.inp_vec = (vec_), \
			.n_rows = (r_), \
			.n_cols = (c_), \
			.idx = 0 }

#define PROC_MAT_INIT(mat_, r_, c_)						\
		{	.mean = (float *)malloc(c_*sizeof(float)),				\
			.sigma = (float *)malloc(c_*sizeof(float)),				\
			.counter = 0,										\
			.matrix = (mat_),									\
			.inp_vec = nullptr,									\
			.n_rows = (r_),										\
			.n_cols = (c_),										\
			.idx = 0 }


	/* struct that contains data and references to perform segment processing*/
	typedef struct {

		float* mean;
		float* sigma;
		uint8_t counter;
		float** matrix;
		float* inp_vec;
		uint16_t n_rows;
		uint16_t n_cols;
		uint8_t idx;
	} proc_utils;

	/*!
	 * @brief Normalize a segment of  collected sensor data samples.
	 * @ingroup data_processing
	 * @details The normalization is performed through multiple threads
	 * using a proc_util struct
	 * @param nu_t the struct pointer used to store segment references and
	 * a synchronization counter
	 * @return a single pointer reference to the processed segment.
	 */
	float* process_vector_dataset(proc_utils* nu_t, void* (*proc_rtn)(void*));

	/*!
	 * @brief Normalization routine.
	 * @ingroup data_processing
	 * @details Routine used by each thread of the process_vector_dataset
	 * function to normalize a column of the input matrix.
	 * @param nor_ut struct pointer containing the matrix reference to process
	 */
	void* normalize_wpresum(void* v);

#ifdef __cplusplus
}
#endif

#endif // include guard