
/**
 *******************************************************************************
  *   @file NN.cpp
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


#include "NN.h"


bool NN::nnCreate(ai_handle* net) {

	printf("\r\n[LOG] AI Network (AI platform API %u.%u.%u)",
		AI_PLATFORM_API_MAJOR,
		AI_PLATFORM_API_MINOR,
		AI_PLATFORM_API_MICRO);

	NN::last_error_rep = ai_network_create(net, (const ai_buffer*)AI_NETWORK_DATA_CONFIG);
	printf("\r\n[LOG] Creating a new neural network...");

    if (NN::last_error_rep.type != AI_ERROR_NONE) {
        printf("\r\n[ERROR]: An error occurred during creation!\r\n[CODE]: -type: %lu code: %lu\r\n",
               last_error_rep.type, last_error_rep.code);
        return false;

    }else{
        printf("\r\n[LOG] New neural network created!\r\n");
        return true;
    }

}



bool NN::nnInit(ai_handle net, ai_network_params* net_par, ai_u8* activations) {

    printf("\r\n[LOG] Neural network initialization...");
	if (net_par == AI_NETWORK_DATA_CONFIG) {
		// initialize network using NN default params
        ai_network_params params = { AI_NETWORK_DATA_WEIGHTS(ai_network_data_weights_get()), AI_NETWORK_DATA_ACTIVATIONS(activations) };
        NN::params = &params;
    }
	else {
		NN::params = net_par;
	}
    if (!ai_network_init(net, (const ai_network_params*)NN::params)) {
        NN::last_error_rep = ai_network_get_error(net);
		printf("\r\n[ERROR]: Error during initialization!\r\n[CODE]: -type: %lu code: %lu\r\n", last_error_rep.type, last_error_rep.code);
		return false;
	}

	printf("\r\n[LOG] Network correctly initialized!\r\n");
	return true;
}



bool NN::prepareData(data_proc* ds_pp, ai_float* in_data, ai_float* out_data,\
                        ai_buffer* ai_input, ai_buffer* ai_output, const ai_u16 batches) {

    // input  preprocessing
    //ds_pp->normalize(in_data);

    if (in_data == NULL) {
        printf("\r\n[ERROR]: An error occurred during dataset processing\r\n");
        return false;
    }
    ai_input->n_batches = batches;
    ai_input->data = AI_HANDLE_PTR(in_data);
    ai_output->n_batches = batches;
    ai_output->data = AI_HANDLE_PTR(out_data);

    return true;
}



int NN::nnRun(ai_handle net, const ai_buffer* ai_input, ai_buffer* ai_output, const ai_u16 batch_size) {

	ai_i32 nbatch;

	if (ai_input->data != nullptr) {

		nbatch = ai_network_run(net, ai_input, ai_output);

		if (nbatch != batch_size) {
			NN::last_error_rep = ai_network_get_error(net);
			printf("\r\n[ERROR]: The neural network has not processed the expected number of batches!\r\n[CODE]: -type: %lu code: %lu\r\n", last_error_rep.type, last_error_rep.code);

			return -1;
		}
	}
	else {
		printf("\r\n[ERROR]: The I/O datasets are not still initialized!\r\n");
		return -1;
	}
	//free(ai_output);
	return nbatch;
}



bool NN::nnDestroy(ai_handle net) {

	printf("\r\n[LOG]: Removing neural network...");

	net = ai_network_destroy(net);

	if (net == nullptr) {

	    //if (NN::params != nullptr){ delete NN::params;}
	    //if (NN::report != nullptr){ delete NN::report;}
	    NN::last_error_rep = AI_ERROR_INIT(NONE, NONE);

		printf("\r\n[LOG]: Neural network successfully deleted!");
		return true;
	}
	else {
        printf("\r\n[ERROR]: Deleting process failed!\r\n");
        return false;
    }

}



void NN::showNNInfo(const ai_handle net) {

    ai_network_report rep;
	if(ai_network_get_info(net, &rep)){
	    NN::report = &rep;
        printf("\r\n*************************   NEURAL NETWORK INFO   ****************************\r\n");
        printf("\n");
        printf("Network info: \n");
        printf("\n\t[NAME]: %s", NN::report->model_name);
        printf("\n\t[MODEL_DATETIME]: %s ", NN::report->model_datetime);
        printf("\n\t[COMPILE_DATETIME]: %s", NN::report->compile_datetime);
        printf("\n\t[API_VERSION]: %u.%u.%u\r\n", AI_PLATFORM_API_MAJOR, AI_PLATFORM_API_MINOR, AI_PLATFORM_API_MICRO);
    }else{
	    printf("\r\n[ERROR] No network info!\r\n");
	}
}