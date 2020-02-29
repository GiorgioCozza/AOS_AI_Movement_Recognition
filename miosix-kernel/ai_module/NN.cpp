
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


ai_bool NN::nnCreate(ai_handle* net) {

	printf("\r\n[LOG]: AI Network (AI platform API %d.%d.%d)...\r\n",
		AI_PLATFORM_API_MAJOR,
		AI_PLATFORM_API_MINOR,
		AI_PLATFORM_API_MICRO);

	//if (NN::params == AI_HANDLE_NULL) {
		//create the neural network
	NN::last_error_rep = ai_network_create(net, (const ai_buffer*)AI_NETWORK_DATA_CONFIG);
	printf("\r\n[LOG]: creating a new neural network...\r\n");
	return true;
	if (last_error_rep.type != AI_ERROR_NONE) {
		printf("\r\n[ERROR]: an error occurred during creation!\r\n[CODE]: -type: %lu code: %lu\r\n", last_error_rep.type, last_error_rep.code);

	}
	//}
	return false;
}

ai_bool NN::nnInit(ai_handle net, ai_network_params* net_par, ai_u8* activations) {

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
	printf("\r\n[LOG]: Network correctly initialized!\r\n");
	return true;
}


ai_i32 NN::nnRun(ai_handle net, ai_buffer* ai_input, ai_buffer* ai_output, const ai_u16 batch_size) {

	ai_i32 nbatch;
	float result;
	if (ai_input->data != nullptr) {
		//printf("\r\n[OUTPUT]: %.2f %.2f %.2f\r\n", (float)(*(ai_input->data + 0)), (float*)(*(ai_input->data + 1)), (float*)(*(ai_input->data + 1)));
		// if the input dataset is not empty run the neural network
		nbatch = ai_network_run(net, (const ai_buffer*)ai_input, ai_output);

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


ai_bool NN::prepareData(dataset_preproc* ds_pp, ai_float* in_data, ai_float* out_data, ai_buffer* ai_input, ai_buffer* ai_output, const ai_u16 batches) {

	uint16_t req_out_sz = sizeof(ai_float) * batches;
	printf("\r\n[LOG]: Preparing the neural network datasets...\r\n");
	// input segment preprocessing

	//ds_pp->normalize(in_data);

	printf("\r\n[TEST] in_data: %f\r\n", *(in_data + 3));

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


ai_handle NN::nnDestroy(ai_handle net) {

	printf("\r\n[LOG]: Removing neural network...\r\n");

	net = ai_network_destroy(net);

	if (net == nullptr) {
		printf("\r\n[LOG]: Neural network successfully deleted!\r\n");
		return net;
	}
	else
		printf("\r\n[ERROR]: Removing process failed!\r\n");

	return AI_HANDLE_NULL;
}



void NN::showNNInfo(ai_handle net) {


	ai_network_get_info(net, NN::report);
	printf("\r\n ##########################   NEURAL NETWORK INFO ##################################\r\n");
	printf("\n");
	printf("Network info: \n");
	printf("\n\t[NAME]: %s", report->model_name);
	printf("\n\t[MODEL_DATETIME]: %s ", report->model_datetime);
	printf("\n\t[COMPILE_DATETIME]: %s", report->compile_datetime);
	printf("\n\t[API_VERSION]: %s", report->api_version);

}