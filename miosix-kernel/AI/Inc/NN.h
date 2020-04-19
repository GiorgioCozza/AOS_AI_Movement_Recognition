
/**
 *******************************************************************************
  *   @file NN.h
  *   @author Cozza Giorgio, Liu Jiaqi
  *   @date 08/04/19
  *   @version 1.0
  *   @brief Class in charge of managing a neural network instance.
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


#ifndef NN_H
#define NN_H


 /*-----------------  INCLUDES  --------------------------------*/
#include <stdio.h>
#include "network.h"
#include "network_data.h"
#include "miosix.h"
#include "data_proc.h"
#include "main_config.h"


class NN {

public:
	NN() {};
	~NN() {

	};
	/**
	 * Method that creates the neural network.
	 * \param net ai_handle reference.
	 * \return bool.
	*/
	bool nnCreate(ai_handle * net);

    /**
     * Method that initializes the neural network.
     * \param net ai_handle reference.
     * \param ai_input reference that contains network parameters
     * \param activations array reference
     * \return the result of the operation.
    */
    bool nnInit(ai_handle net, ai_network_params* net_par, ai_u8 * activations);

    /**
     * Initializes input and output of the neural network.
     * \param net ai_handle reference.
     * \param ai_input an ai_buffer to store processed input
     * \param ai_output ai_buffer to store prediction results
     * \param in_data ai_float input pointer
     * \param out_data ai_float output pointer
     * \return the result of the operation.
    */
    bool prepareData(data_proc* ds_pp, ai_float* in_data, ai_float* out_data,
                     ai_buffer* ai_input, ai_buffer* ai_output, const ai_u16 batches);

    /**
     * Runs the neural network.
     * \param net ai_handle reference.
     * \param ai_input an ai_buffer reference that contains processed input segment
     * \param ai_output ai_buffer reference to store results
     * \return the result of the operation.
    */
    int nnRun(ai_handle net, const ai_buffer* ai_input, ai_buffer* ai_output, const ai_u16 batch_size);

	/**
	 * Method that destroys the neural network.
	 * \param net ai_handle reference.
	 * \return the reference of the deleted network.
	*/
	bool nnDestroy(ai_handle net);

	/**
	 * Show neural network info.
	 * \param net ai_handle reference.
	*/
	void showNNInfo(const ai_handle net);



protected:

private:
	ai_network_params* params=nullptr;
	ai_error last_error_rep;
	ai_network_report* report=nullptr;


};


#endif
