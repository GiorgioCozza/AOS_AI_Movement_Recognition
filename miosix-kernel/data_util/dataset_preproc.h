
/**
 *******************************************************************************
  *   @file dataset_preprocessor.h
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

#ifndef DATASET_PREPROCESSOR_H
#define DATASET_PREPROCESSOR_H

 /*----------------------------   INCLUDES	----------------------------------*/
#include <stdio.h>
#include "network.h"
#include "network_data.h"
#include "ai_math_helpers.h"
#include "XNUCLEO_IKS01A2.h"
#include <stdint.h>
#include "prog_config.h"


class dataset_preproc {

private:
	float min[VECTOR_SIZE];
	float max[VECTOR_SIZE];
	uint8_t fst_read[VECTOR_SIZE];

protected:

public:
	dataset_preproc(void) {
		for (int i = 0; i < VECTOR_SIZE; i++) {
			fst_read[i] = 1;
			min[i] = 0;
			max[i] = 0;
		}
	}

	~dataset_preproc(void) {
		delete[] min;
		delete[] max;
	}

	float get_min(uint8_t sens, uint8_t ax);
	float get_max(uint8_t sens, uint8_t ax);
	float* get_min_ptr() { return min; };
	float* get_max_ptr() { return max; };
	void update_min_max(int32_t* sen_read, int sen);
	void normalize(float* in_vect);

	void print_min(void);
	void print_max(void);
	void print_dataset(float* in_vect);

};





#endif