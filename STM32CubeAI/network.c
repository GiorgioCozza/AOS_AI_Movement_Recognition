/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Fri Apr  3 18:28:37 2020
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */



#include "network.h"

#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "layers.h"

#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#define AI_TOOLS_VERSION_MAJOR 4
#define AI_TOOLS_VERSION_MINOR 0
#define AI_TOOLS_VERSION_MICRO 0

#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#define AI_TOOLS_API_VERSION_MAJOR 1
#define AI_TOOLS_API_VERSION_MINOR 3
#define AI_TOOLS_API_VERSION_MICRO 0

#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "9a318b55ace4f0e13625fc4c0edee66c"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     "(rev-)"
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Fri Apr  3 18:28:37 2020"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

/**  Forward network declaration section  *************************************/
AI_STATIC ai_network AI_NET_OBJ_INSTANCE;


/**  Forward network array declarations  **************************************/
AI_STATIC ai_array FCN1_bias_array;   /* Array #0 */
AI_STATIC ai_array FCN1_weights_array;   /* Array #1 */
AI_STATIC ai_array LSTM4_bias_array;   /* Array #2 */
AI_STATIC ai_array LSTM4_peephole_array;   /* Array #3 */
AI_STATIC ai_array LSTM4_recurrent_array;   /* Array #4 */
AI_STATIC ai_array LSTM4_kernel_array;   /* Array #5 */
AI_STATIC ai_array LSTM3_bias_array;   /* Array #6 */
AI_STATIC ai_array LSTM3_peephole_array;   /* Array #7 */
AI_STATIC ai_array LSTM3_recurrent_array;   /* Array #8 */
AI_STATIC ai_array LSTM3_kernel_array;   /* Array #9 */
AI_STATIC ai_array LSTM2_bias_array;   /* Array #10 */
AI_STATIC ai_array LSTM2_peephole_array;   /* Array #11 */
AI_STATIC ai_array LSTM2_recurrent_array;   /* Array #12 */
AI_STATIC ai_array LSTM2_kernel_array;   /* Array #13 */
AI_STATIC ai_array LSTM1_bias_array;   /* Array #14 */
AI_STATIC ai_array LSTM1_peephole_array;   /* Array #15 */
AI_STATIC ai_array LSTM1_recurrent_array;   /* Array #16 */
AI_STATIC ai_array LSTM1_kernel_array;   /* Array #17 */
AI_STATIC ai_array batch_normalization_2_bias_array;   /* Array #18 */
AI_STATIC ai_array batch_normalization_2_scale_array;   /* Array #19 */
AI_STATIC ai_array input_0_output_array;   /* Array #20 */
AI_STATIC ai_array batch_normalization_2_output_array;   /* Array #21 */
AI_STATIC ai_array LSTM1_output_array;   /* Array #22 */
AI_STATIC ai_array LSTM2_output_array;   /* Array #23 */
AI_STATIC ai_array LSTM3_output_array;   /* Array #24 */
AI_STATIC ai_array LSTM4_output_array;   /* Array #25 */
AI_STATIC ai_array FCN1_output_array;   /* Array #26 */
AI_STATIC ai_array FCN1_nl_output_array;   /* Array #27 */


/**  Forward network tensor declarations  *************************************/
AI_STATIC ai_tensor FCN1_bias;   /* Tensor #0 */
AI_STATIC ai_tensor FCN1_weights;   /* Tensor #1 */
AI_STATIC ai_tensor LSTM4_bias;   /* Tensor #2 */
AI_STATIC ai_tensor LSTM4_peephole;   /* Tensor #3 */
AI_STATIC ai_tensor LSTM4_recurrent;   /* Tensor #4 */
AI_STATIC ai_tensor LSTM4_kernel;   /* Tensor #5 */
AI_STATIC ai_tensor LSTM3_bias;   /* Tensor #6 */
AI_STATIC ai_tensor LSTM3_peephole;   /* Tensor #7 */
AI_STATIC ai_tensor LSTM3_recurrent;   /* Tensor #8 */
AI_STATIC ai_tensor LSTM3_kernel;   /* Tensor #9 */
AI_STATIC ai_tensor LSTM2_bias;   /* Tensor #10 */
AI_STATIC ai_tensor LSTM2_peephole;   /* Tensor #11 */
AI_STATIC ai_tensor LSTM2_recurrent;   /* Tensor #12 */
AI_STATIC ai_tensor LSTM2_kernel;   /* Tensor #13 */
AI_STATIC ai_tensor LSTM1_bias;   /* Tensor #14 */
AI_STATIC ai_tensor LSTM1_peephole;   /* Tensor #15 */
AI_STATIC ai_tensor LSTM1_recurrent;   /* Tensor #16 */
AI_STATIC ai_tensor LSTM1_kernel;   /* Tensor #17 */
AI_STATIC ai_tensor batch_normalization_2_bias;   /* Tensor #18 */
AI_STATIC ai_tensor batch_normalization_2_scale;   /* Tensor #19 */
AI_STATIC ai_tensor input_0_output;   /* Tensor #20 */
AI_STATIC ai_tensor batch_normalization_2_output;   /* Tensor #21 */
AI_STATIC ai_tensor LSTM1_output;   /* Tensor #22 */
AI_STATIC ai_tensor LSTM2_output;   /* Tensor #23 */
AI_STATIC ai_tensor LSTM3_output;   /* Tensor #24 */
AI_STATIC ai_tensor LSTM4_output;   /* Tensor #25 */
AI_STATIC ai_tensor FCN1_output;   /* Tensor #26 */
AI_STATIC ai_tensor FCN1_nl_output;   /* Tensor #27 */


/**  Forward network tensor chain declarations  *******************************/
AI_STATIC_CONST ai_tensor_chain batch_normalization_2_chain;   /* Chain #0 */
AI_STATIC_CONST ai_tensor_chain LSTM1_chain;   /* Chain #1 */
AI_STATIC_CONST ai_tensor_chain LSTM2_chain;   /* Chain #2 */
AI_STATIC_CONST ai_tensor_chain LSTM3_chain;   /* Chain #3 */
AI_STATIC_CONST ai_tensor_chain LSTM4_chain;   /* Chain #4 */
AI_STATIC_CONST ai_tensor_chain FCN1_chain;   /* Chain #5 */
AI_STATIC_CONST ai_tensor_chain FCN1_nl_chain;   /* Chain #6 */


/**  Subgraph network operator tensor chain declarations  *********************/


/**  Subgraph network operator declarations  *********************************/


/**  Forward network layer declarations  **************************************/
AI_STATIC ai_layer_bn batch_normalization_2_layer; /* Layer #0 */
AI_STATIC ai_layer_lstm LSTM1_layer; /* Layer #1 */
AI_STATIC ai_layer_lstm LSTM2_layer; /* Layer #2 */
AI_STATIC ai_layer_lstm LSTM3_layer; /* Layer #3 */
AI_STATIC ai_layer_lstm LSTM4_layer; /* Layer #4 */
AI_STATIC ai_layer_dense FCN1_layer; /* Layer #5 */
AI_STATIC ai_layer_nl FCN1_nl_layer; /* Layer #6 */


/**  Array declarations section  **********************************************/
AI_ARRAY_OBJ_DECLARE(
  FCN1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 7,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  FCN1_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 224,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM4_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM4_peephole_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 96,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM4_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM4_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM3_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM3_peephole_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 96,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM3_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM3_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM2_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM2_peephole_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 96,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM2_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM2_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 128,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM1_peephole_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 96,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM1_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4096,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM1_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1536,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  batch_normalization_2_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  batch_normalization_2_scale_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 12,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  input_0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 360,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  batch_normalization_2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 360,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 960,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM2_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 960,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM3_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 960,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  LSTM4_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  FCN1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 7,
  AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
  FCN1_nl_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 7,
  AI_STATIC)


/**  Tensor declarations section  *********************************************/
AI_TENSOR_OBJ_DECLARE(
  FCN1_bias,
  AI_SHAPE_INIT(1, 1, 7, 1),
  AI_STRIDE_INIT(28, 28, 4, 4),
  &FCN1_bias_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  FCN1_weights,
  AI_SHAPE_INIT(1, 1, 7, 32),
  AI_STRIDE_INIT(896, 896, 128, 4),
  &FCN1_weights_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM4_bias,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &LSTM4_bias_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM4_peephole,
  AI_SHAPE_INIT(1, 1, 96, 1),
  AI_STRIDE_INIT(384, 384, 4, 4),
  &LSTM4_peephole_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM4_recurrent,
  AI_SHAPE_INIT(128, 1, 1, 32),
  AI_STRIDE_INIT(128, 128, 128, 4),
  &LSTM4_recurrent_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM4_kernel,
  AI_SHAPE_INIT(128, 1, 1, 32),
  AI_STRIDE_INIT(128, 128, 128, 4),
  &LSTM4_kernel_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM3_bias,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &LSTM3_bias_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM3_peephole,
  AI_SHAPE_INIT(1, 1, 96, 1),
  AI_STRIDE_INIT(384, 384, 4, 4),
  &LSTM3_peephole_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM3_recurrent,
  AI_SHAPE_INIT(128, 1, 1, 32),
  AI_STRIDE_INIT(128, 128, 128, 4),
  &LSTM3_recurrent_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM3_kernel,
  AI_SHAPE_INIT(128, 1, 1, 32),
  AI_STRIDE_INIT(128, 128, 128, 4),
  &LSTM3_kernel_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM2_bias,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &LSTM2_bias_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM2_peephole,
  AI_SHAPE_INIT(1, 1, 96, 1),
  AI_STRIDE_INIT(384, 384, 4, 4),
  &LSTM2_peephole_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM2_recurrent,
  AI_SHAPE_INIT(128, 1, 1, 32),
  AI_STRIDE_INIT(128, 128, 128, 4),
  &LSTM2_recurrent_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM2_kernel,
  AI_SHAPE_INIT(128, 1, 1, 32),
  AI_STRIDE_INIT(128, 128, 128, 4),
  &LSTM2_kernel_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_bias,
  AI_SHAPE_INIT(1, 1, 128, 1),
  AI_STRIDE_INIT(512, 512, 4, 4),
  &LSTM1_bias_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_peephole,
  AI_SHAPE_INIT(1, 1, 96, 1),
  AI_STRIDE_INIT(384, 384, 4, 4),
  &LSTM1_peephole_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_recurrent,
  AI_SHAPE_INIT(128, 1, 1, 32),
  AI_STRIDE_INIT(128, 128, 128, 4),
  &LSTM1_recurrent_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_kernel,
  AI_SHAPE_INIT(128, 1, 1, 12),
  AI_STRIDE_INIT(48, 48, 48, 4),
  &LSTM1_kernel_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  batch_normalization_2_bias,
  AI_SHAPE_INIT(1, 1, 12, 1),
  AI_STRIDE_INIT(48, 48, 4, 4),
  &batch_normalization_2_bias_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  batch_normalization_2_scale,
  AI_SHAPE_INIT(1, 1, 12, 1),
  AI_STRIDE_INIT(48, 48, 4, 4),
  &batch_normalization_2_scale_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  input_0_output,
  AI_SHAPE_INIT(30, 1, 12, 1),
  AI_STRIDE_INIT(48, 48, 4, 4),
  &input_0_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  batch_normalization_2_output,
  AI_SHAPE_INIT(30, 1, 12, 1),
  AI_STRIDE_INIT(48, 48, 4, 4),
  &batch_normalization_2_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_output,
  AI_SHAPE_INIT(30, 1, 32, 1),
  AI_STRIDE_INIT(128, 128, 4, 4),
  &LSTM1_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM2_output,
  AI_SHAPE_INIT(30, 1, 32, 1),
  AI_STRIDE_INIT(128, 128, 4, 4),
  &LSTM2_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM3_output,
  AI_SHAPE_INIT(30, 1, 32, 1),
  AI_STRIDE_INIT(128, 128, 4, 4),
  &LSTM3_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  LSTM4_output,
  AI_SHAPE_INIT(1, 1, 32, 1),
  AI_STRIDE_INIT(128, 128, 4, 4),
  &LSTM4_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  FCN1_output,
  AI_SHAPE_INIT(1, 1, 7, 1),
  AI_STRIDE_INIT(28, 28, 4, 4),
  &FCN1_output_array,
  AI_STATIC)
AI_TENSOR_OBJ_DECLARE(
  FCN1_nl_output,
  AI_SHAPE_INIT(1, 1, 7, 1),
  AI_STRIDE_INIT(28, 28, 4, 4),
  &FCN1_nl_output_array,
  AI_STATIC)


/**  Layer declarations section  **********************************************/



AI_TENSOR_CHAIN_OBJ_DECLARE(
  batch_normalization_2_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&input_0_output),
  AI_TENSOR_LIST_ENTRY(&batch_normalization_2_output),
  AI_TENSOR_LIST_ENTRY(&batch_normalization_2_scale, &batch_normalization_2_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  batch_normalization_2_layer, 0,
  BN_TYPE,
  bn, forward_bn,
  &AI_NET_OBJ_INSTANCE, &LSTM1_layer, AI_STATIC,
  .tensors = &batch_normalization_2_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  LSTM1_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&batch_normalization_2_output),
  AI_TENSOR_LIST_ENTRY(&LSTM1_output),
  AI_TENSOR_LIST_ENTRY(&LSTM1_kernel, &LSTM1_recurrent, &LSTM1_peephole, &LSTM1_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  LSTM1_layer, 1,
  LSTM_TYPE,
  lstm, forward_lstm,
  &AI_NET_OBJ_INSTANCE, &LSTM2_layer, AI_STATIC,
  .tensors = &LSTM1_chain, 
  .n_units = 32, 
  .activation_nl = ai_math_tanh, 
  .recurrent_nl = ai_math_hard_sigmoid, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .out_nl = ai_math_tanh, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  LSTM2_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&LSTM1_output),
  AI_TENSOR_LIST_ENTRY(&LSTM2_output),
  AI_TENSOR_LIST_ENTRY(&LSTM2_kernel, &LSTM2_recurrent, &LSTM2_peephole, &LSTM2_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  LSTM2_layer, 3,
  LSTM_TYPE,
  lstm, forward_lstm,
  &AI_NET_OBJ_INSTANCE, &LSTM3_layer, AI_STATIC,
  .tensors = &LSTM2_chain, 
  .n_units = 32, 
  .activation_nl = ai_math_tanh, 
  .recurrent_nl = ai_math_hard_sigmoid, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .out_nl = ai_math_tanh, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  LSTM3_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&LSTM2_output),
  AI_TENSOR_LIST_ENTRY(&LSTM3_output),
  AI_TENSOR_LIST_ENTRY(&LSTM3_kernel, &LSTM3_recurrent, &LSTM3_peephole, &LSTM3_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  LSTM3_layer, 5,
  LSTM_TYPE,
  lstm, forward_lstm,
  &AI_NET_OBJ_INSTANCE, &LSTM4_layer, AI_STATIC,
  .tensors = &LSTM3_chain, 
  .n_units = 32, 
  .activation_nl = ai_math_tanh, 
  .recurrent_nl = ai_math_hard_sigmoid, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .out_nl = ai_math_tanh, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  LSTM4_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&LSTM3_output),
  AI_TENSOR_LIST_ENTRY(&LSTM4_output),
  AI_TENSOR_LIST_ENTRY(&LSTM4_kernel, &LSTM4_recurrent, &LSTM4_peephole, &LSTM4_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  LSTM4_layer, 7,
  LSTM_TYPE,
  lstm, forward_lstm,
  &AI_NET_OBJ_INSTANCE, &FCN1_layer, AI_STATIC,
  .tensors = &LSTM4_chain, 
  .n_units = 32, 
  .activation_nl = ai_math_tanh, 
  .recurrent_nl = ai_math_hard_sigmoid, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .out_nl = ai_math_tanh, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  FCN1_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&LSTM4_output),
  AI_TENSOR_LIST_ENTRY(&FCN1_output),
  AI_TENSOR_LIST_ENTRY(&FCN1_weights, &FCN1_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  FCN1_layer, 9,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &FCN1_nl_layer, AI_STATIC,
  .tensors = &FCN1_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  FCN1_nl_chain, AI_STATIC_CONST, 
  AI_TENSOR_LIST_ENTRY(&FCN1_output),
  AI_TENSOR_LIST_ENTRY(&FCN1_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  FCN1_nl_layer, 9,
  NL_TYPE,
  nl, forward_sm,
  &AI_NET_OBJ_INSTANCE, &FCN1_nl_layer, AI_STATIC,
  .tensors = &FCN1_nl_chain, 
)


AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE,
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 125436, 1,
                     NULL),
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 7680, 1,
                     NULL),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &FCN1_nl_output),
  &batch_normalization_2_layer, 0)



AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_buffer* activation_buffer)
{
  AI_ASSERT(net_ctx &&  activation_buffer && activation_buffer->data)

  ai_ptr activations = AI_PTR(AI_PTR_ALIGN(activation_buffer->data, 4));
  AI_ASSERT( activations )

  {
    /* Updating activations (byte) offsets */
    input_0_output_array.data = AI_PTR(NULL);
    input_0_output_array.data_start = AI_PTR(NULL);
    batch_normalization_2_output_array.data = AI_PTR(activations + 0);
    batch_normalization_2_output_array.data_start = AI_PTR(activations + 0);
    LSTM1_output_array.data = AI_PTR(activations + 3840);
    LSTM1_output_array.data_start = AI_PTR(activations + 3840);
    LSTM2_output_array.data = AI_PTR(activations + 0);
    LSTM2_output_array.data_start = AI_PTR(activations + 0);
    LSTM3_output_array.data = AI_PTR(activations + 3840);
    LSTM3_output_array.data_start = AI_PTR(activations + 3840);
    LSTM4_output_array.data = AI_PTR(activations + 0);
    LSTM4_output_array.data_start = AI_PTR(activations + 0);
    FCN1_output_array.data = AI_PTR(activations + 128);
    FCN1_output_array.data_start = AI_PTR(activations + 128);
    FCN1_nl_output_array.data = AI_PTR(NULL);
    FCN1_nl_output_array.data_start = AI_PTR(NULL);
    
  }
  return true;
}



AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_buffer* weights_buffer)
{
  AI_ASSERT(net_ctx &&  weights_buffer && weights_buffer->data)

  ai_ptr weights = AI_PTR(weights_buffer->data);
  AI_ASSERT( weights )

  {
    /* Updating weights (byte) offsets */
    FCN1_bias_array.format |= AI_FMT_FLAG_CONST;
    FCN1_bias_array.data = AI_PTR(weights + 125408);
    FCN1_bias_array.data_start = AI_PTR(weights + 125408);
  FCN1_weights_array.format |= AI_FMT_FLAG_CONST;
    FCN1_weights_array.data = AI_PTR(weights + 124512);
    FCN1_weights_array.data_start = AI_PTR(weights + 124512);
  LSTM4_bias_array.format |= AI_FMT_FLAG_CONST;
    LSTM4_bias_array.data = AI_PTR(weights + 124000);
    LSTM4_bias_array.data_start = AI_PTR(weights + 124000);
  LSTM4_peephole_array.format |= AI_FMT_FLAG_CONST;
    LSTM4_peephole_array.data = AI_PTR(weights + 123616);
    LSTM4_peephole_array.data_start = AI_PTR(weights + 123616);
  LSTM4_recurrent_array.format |= AI_FMT_FLAG_CONST;
    LSTM4_recurrent_array.data = AI_PTR(weights + 107232);
    LSTM4_recurrent_array.data_start = AI_PTR(weights + 107232);
  LSTM4_kernel_array.format |= AI_FMT_FLAG_CONST;
    LSTM4_kernel_array.data = AI_PTR(weights + 90848);
    LSTM4_kernel_array.data_start = AI_PTR(weights + 90848);
  LSTM3_bias_array.format |= AI_FMT_FLAG_CONST;
    LSTM3_bias_array.data = AI_PTR(weights + 90336);
    LSTM3_bias_array.data_start = AI_PTR(weights + 90336);
  LSTM3_peephole_array.format |= AI_FMT_FLAG_CONST;
    LSTM3_peephole_array.data = AI_PTR(weights + 89952);
    LSTM3_peephole_array.data_start = AI_PTR(weights + 89952);
  LSTM3_recurrent_array.format |= AI_FMT_FLAG_CONST;
    LSTM3_recurrent_array.data = AI_PTR(weights + 73568);
    LSTM3_recurrent_array.data_start = AI_PTR(weights + 73568);
  LSTM3_kernel_array.format |= AI_FMT_FLAG_CONST;
    LSTM3_kernel_array.data = AI_PTR(weights + 57184);
    LSTM3_kernel_array.data_start = AI_PTR(weights + 57184);
  LSTM2_bias_array.format |= AI_FMT_FLAG_CONST;
    LSTM2_bias_array.data = AI_PTR(weights + 56672);
    LSTM2_bias_array.data_start = AI_PTR(weights + 56672);
  LSTM2_peephole_array.format |= AI_FMT_FLAG_CONST;
    LSTM2_peephole_array.data = AI_PTR(weights + 56288);
    LSTM2_peephole_array.data_start = AI_PTR(weights + 56288);
  LSTM2_recurrent_array.format |= AI_FMT_FLAG_CONST;
    LSTM2_recurrent_array.data = AI_PTR(weights + 39904);
    LSTM2_recurrent_array.data_start = AI_PTR(weights + 39904);
  LSTM2_kernel_array.format |= AI_FMT_FLAG_CONST;
    LSTM2_kernel_array.data = AI_PTR(weights + 23520);
    LSTM2_kernel_array.data_start = AI_PTR(weights + 23520);
  LSTM1_bias_array.format |= AI_FMT_FLAG_CONST;
    LSTM1_bias_array.data = AI_PTR(weights + 23008);
    LSTM1_bias_array.data_start = AI_PTR(weights + 23008);
  LSTM1_peephole_array.format |= AI_FMT_FLAG_CONST;
    LSTM1_peephole_array.data = AI_PTR(weights + 22624);
    LSTM1_peephole_array.data_start = AI_PTR(weights + 22624);
  LSTM1_recurrent_array.format |= AI_FMT_FLAG_CONST;
    LSTM1_recurrent_array.data = AI_PTR(weights + 6240);
    LSTM1_recurrent_array.data_start = AI_PTR(weights + 6240);
  LSTM1_kernel_array.format |= AI_FMT_FLAG_CONST;
    LSTM1_kernel_array.data = AI_PTR(weights + 96);
    LSTM1_kernel_array.data_start = AI_PTR(weights + 96);
  batch_normalization_2_bias_array.format |= AI_FMT_FLAG_CONST;
    batch_normalization_2_bias_array.data = AI_PTR(weights + 48);
    batch_normalization_2_bias_array.data_start = AI_PTR(weights + 48);
  batch_normalization_2_scale_array.format |= AI_FMT_FLAG_CONST;
    batch_normalization_2_scale_array.data = AI_PTR(weights + 0);
    batch_normalization_2_scale_array.data_start = AI_PTR(weights + 0);
  
  }

  return true;
}


/**  PUBLIC APIs SECTION  *****************************************************/

AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if ( report && net_ctx )
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = {AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR,
                            AI_TOOLS_API_VERSION_MICRO, 0x0},

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 926489,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .activations       = AI_STRUCT_INIT,
      .params            = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if ( !ai_platform_api_get_network_report(network, &r) ) return false;

    *report = r;
    return true;
  }

  return false;
}

AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if ( !net_ctx ) return false;

  ai_bool ok = true;
  ok &= network_configure_weights(net_ctx, &params->params);
  ok &= network_configure_activations(net_ctx, &params->activations);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}

#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

