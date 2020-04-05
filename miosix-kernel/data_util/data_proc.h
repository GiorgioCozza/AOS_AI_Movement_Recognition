
/**
 *******************************************************************************
  *   @file data_proc.h
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

#ifndef _DATA_PROC_H
#define _DATA_PROC_H

 /*----------------------------   INCLUDES	----------------------------------*/
#include <stdio.h>
#include "network.h"
#include "network_data.h"
#include "ai_math_helpers.h"
#include "IKS01A2_config.h"
#include <stdint.h>
#include "prog_config.h"


class data_proc {

private:
	float min[VECTOR_SIZE];
	float max[VECTOR_SIZE];
	uint8_t fst_read[VECTOR_SIZE];

protected:

public:
	data_proc(void) {
		for (int i = 0; i < VECTOR_SIZE; i++) {
			fst_read[i] = 1;
			min[i] = 0;
			max[i] = 0;
		}
	}

	~data_proc(void) {
		delete[] min;
		delete[] max;
	}

	float get_min(uint8_t sens, uint8_t ax);
	float get_max(uint8_t sens, uint8_t ax);
	float* get_min_ptr() { return min; };
	float* get_max_ptr() { return max; };
	void update_min_max(int32_t* sen_read, int sen);
	void normalize(float* in_vect);
    uint8_t get_argmax(const float * vec, const uint16_t vec_sz);
    uint8_t get_mode(const uint8_t * vec, const uint16_t vec_sz, const uint8_t cat_num);
    
	void print_min(void);
	void print_max(void);
	void print_dataset(float* in_vect);

};





#endif