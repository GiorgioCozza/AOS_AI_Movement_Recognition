
/**
 *******************************************************************************
  *   @file NNManager.h
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


#ifndef NNMANAGER_H
#define NNMANAGER_H


 /*-----------------  INCLUDES  --------------------------------*/
#include <stdio.h>
#include "network.h"
#include "network_data.h"
#include "miosix.h"
#include "dataset_preproc.h"
#include "nn_config.h"
 /*-----------------  DEFINE  --------------------------------*/


/*-----------------  CLASS  ---------------------------------*/

/**
  *	NN class to manage a single neural network operations
  *	\author Giorgio Cozza, Liu Jiaqi
  *
  */
class NN {

public:
	NN() {};
	~NN() {

	};
	/**
	 * Method that creates the neural network.
	 * \param net ai_handle reference.
	 * \return the result of the operation.
	*/
	ai_bool nnCreate(ai_handle * net);
	/**
	 * Method that destroys the neural network.
	 * \param net ai_handle reference.
	 * \return the reference of the deleted network.
	*/
	ai_handle nnDestroy(ai_handle net);
	/**
	 * Method that runs the neural network.
	 * \param net ai_handle reference.
	 * \param ai_input an ai_buffer reference that contains processed input segment
	 * \param ai_output ai_buffer reference to store results
	 * \return the result of the operation.
	*/
	ai_i32 nnRun(ai_handle net, ai_buffer* ai_input, ai_buffer* ai_output, const ai_u16 batch_size);
	/**
	 * Method that initializes input and output datasets of the neural network.
	 * \param net ai_handle reference.
	 * \param ai_input an ai_buffer reference in which processed input will be stored
	 * \param ai_output ai_buffer reference to perform initialization
	 * \param in_data ai_float reference
	 * \param out_data ai_float reference
	 * \return the result of the operation.
	*/
	ai_bool prepareData(dataset_preproc* ds_pp, ai_float* in_data, ai_float* out_data, ai_buffer* ai_input, ai_buffer* ai_output, const ai_u16 batches);
	/**
	 * Show neural network info.
	 * \param net ai_handle reference.
	*/
	void showNNInfo(ai_handle net);
	/**
	 * Method that initializes the neural network.
	 * \param net ai_handle reference.
	 * \param ai_input reference that contains network parameters
	 * \param activations array reference
	 * \return the result of the operation.
	*/
	ai_bool nnInit(ai_handle net, ai_network_params* net_par, ai_u8 * activations);

protected:

private:
	ai_network_params* params=nullptr;
	ai_error last_error_rep/*=AI_ERROR_INIT(0,0)*/;
	ai_network_report* report=nullptr;


};


#endif
