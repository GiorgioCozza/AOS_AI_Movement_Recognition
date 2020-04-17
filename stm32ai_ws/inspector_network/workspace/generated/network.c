/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Thu Apr 16 15:52:24 2020
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
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
#define AI_TOOLS_VERSION_MAJOR 5
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
#define AI_NETWORK_MODEL_SIGNATURE     "fc8f821a7f3a5c7c278e772d8331cb75"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     "(rev-)"
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Thu Apr 16 15:52:24 2020"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

/**  Forward network declaration section  *************************************/
AI_STATIC ai_network AI_NET_OBJ_INSTANCE;


/**  Forward network array declarations  **************************************/
AI_STATIC ai_array FCN3_bias_array;   /* Array #0 */
AI_STATIC ai_array FCN3_weights_array;   /* Array #1 */
AI_STATIC ai_array FCN2_bias_array;   /* Array #2 */
AI_STATIC ai_array FCN2_weights_array;   /* Array #3 */
AI_STATIC ai_array FCN1_bias_array;   /* Array #4 */
AI_STATIC ai_array FCN1_weights_array;   /* Array #5 */
AI_STATIC ai_array LSTM1_bias_array;   /* Array #6 */
AI_STATIC ai_array LSTM1_peephole_array;   /* Array #7 */
AI_STATIC ai_array LSTM1_recurrent_array;   /* Array #8 */
AI_STATIC ai_array LSTM1_kernel_array;   /* Array #9 */
AI_STATIC ai_array batch_normalization_1_bias_array;   /* Array #10 */
AI_STATIC ai_array batch_normalization_1_scale_array;   /* Array #11 */
AI_STATIC ai_array input_0_output_array;   /* Array #12 */
AI_STATIC ai_array batch_normalization_1_output_array;   /* Array #13 */
AI_STATIC ai_array LSTM1_output_array;   /* Array #14 */
AI_STATIC ai_array FCN1_output_array;   /* Array #15 */
AI_STATIC ai_array FCN1_nl_output_array;   /* Array #16 */
AI_STATIC ai_array FCN2_output_array;   /* Array #17 */
AI_STATIC ai_array FCN2_nl_output_array;   /* Array #18 */
AI_STATIC ai_array FCN3_output_array;   /* Array #19 */
AI_STATIC ai_array FCN3_nl_output_array;   /* Array #20 */


/**  Forward network tensor declarations  *************************************/
AI_STATIC ai_tensor FCN3_bias;   /* Tensor #0 */
AI_STATIC ai_tensor FCN3_weights;   /* Tensor #1 */
AI_STATIC ai_tensor FCN2_bias;   /* Tensor #2 */
AI_STATIC ai_tensor FCN2_weights;   /* Tensor #3 */
AI_STATIC ai_tensor FCN1_bias;   /* Tensor #4 */
AI_STATIC ai_tensor FCN1_weights;   /* Tensor #5 */
AI_STATIC ai_tensor LSTM1_bias;   /* Tensor #6 */
AI_STATIC ai_tensor LSTM1_peephole;   /* Tensor #7 */
AI_STATIC ai_tensor LSTM1_recurrent;   /* Tensor #8 */
AI_STATIC ai_tensor LSTM1_kernel;   /* Tensor #9 */
AI_STATIC ai_tensor batch_normalization_1_bias;   /* Tensor #10 */
AI_STATIC ai_tensor batch_normalization_1_scale;   /* Tensor #11 */
AI_STATIC ai_tensor input_0_output;   /* Tensor #12 */
AI_STATIC ai_tensor batch_normalization_1_output;   /* Tensor #13 */
AI_STATIC ai_tensor LSTM1_output;   /* Tensor #14 */
AI_STATIC ai_tensor FCN1_output;   /* Tensor #15 */
AI_STATIC ai_tensor FCN1_nl_output;   /* Tensor #16 */
AI_STATIC ai_tensor FCN2_output;   /* Tensor #17 */
AI_STATIC ai_tensor FCN2_nl_output;   /* Tensor #18 */
AI_STATIC ai_tensor FCN3_output;   /* Tensor #19 */
AI_STATIC ai_tensor FCN3_nl_output;   /* Tensor #20 */


/**  Forward network tensor chain declarations  *******************************/
AI_STATIC_CONST ai_tensor_chain batch_normalization_1_chain;   /* Chain #0 */
AI_STATIC_CONST ai_tensor_chain LSTM1_chain;   /* Chain #1 */
AI_STATIC_CONST ai_tensor_chain FCN1_chain;   /* Chain #2 */
AI_STATIC_CONST ai_tensor_chain FCN1_nl_chain;   /* Chain #3 */
AI_STATIC_CONST ai_tensor_chain FCN2_chain;   /* Chain #4 */
AI_STATIC_CONST ai_tensor_chain FCN2_nl_chain;   /* Chain #5 */
AI_STATIC_CONST ai_tensor_chain FCN3_chain;   /* Chain #6 */
AI_STATIC_CONST ai_tensor_chain FCN3_nl_chain;   /* Chain #7 */


/**  Forward network layer declarations  **************************************/
AI_STATIC ai_layer_bn batch_normalization_1_layer; /* Layer #0 */
AI_STATIC ai_layer_lstm LSTM1_layer; /* Layer #1 */
AI_STATIC ai_layer_dense FCN1_layer; /* Layer #2 */
AI_STATIC ai_layer_nl FCN1_nl_layer; /* Layer #3 */
AI_STATIC ai_layer_dense FCN2_layer; /* Layer #4 */
AI_STATIC ai_layer_nl FCN2_nl_layer; /* Layer #5 */
AI_STATIC ai_layer_dense FCN3_layer; /* Layer #6 */
AI_STATIC ai_layer_nl FCN3_nl_layer; /* Layer #7 */


/**  Array declarations section  **********************************************/
AI_ARRAY_OBJ_DECLARE(
    FCN3_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 7,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN3_weights_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 224,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN2_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN2_weights_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 1024,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN1_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN1_weights_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 2048,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    LSTM1_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 256,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    LSTM1_peephole_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 192,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    LSTM1_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 16384,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    LSTM1_kernel_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 3072,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    batch_normalization_1_bias_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 12,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    batch_normalization_1_scale_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 12,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    input_0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
    NULL, NULL, 360,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    batch_normalization_1_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 360,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    LSTM1_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 64,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN1_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN1_nl_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN2_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN2_nl_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN3_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 7,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    FCN3_nl_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
    NULL, NULL, 7,
     AI_STATIC)




/**  Tensor declarations section  *********************************************/
AI_TENSOR_OBJ_DECLARE(
  FCN3_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 7, 1, 1), AI_STRIDE_INIT(4, 4, 4, 28, 28),
  1, &FCN3_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN3_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 32, 7, 1, 1), AI_STRIDE_INIT(4, 4, 128, 896, 896),
  1, &FCN3_weights_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN2_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &FCN2_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN2_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 32, 32, 1, 1), AI_STRIDE_INIT(4, 4, 128, 4096, 4096),
  1, &FCN2_weights_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN1_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &FCN1_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN1_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 64, 32, 1, 1), AI_STRIDE_INIT(4, 4, 256, 8192, 8192),
  1, &FCN1_weights_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &LSTM1_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_peephole, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 192, 1, 1), AI_STRIDE_INIT(4, 4, 4, 768, 768),
  1, &LSTM1_peephole_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_recurrent, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 64, 1, 1, 256), AI_STRIDE_INIT(4, 4, 256, 256, 256),
  1, &LSTM1_recurrent_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_kernel, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 12, 1, 1, 256), AI_STRIDE_INIT(4, 4, 48, 48, 48),
  1, &LSTM1_kernel_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  batch_normalization_1_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 12, 1, 1), AI_STRIDE_INIT(4, 4, 4, 48, 48),
  1, &batch_normalization_1_bias_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  batch_normalization_1_scale, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 12, 1, 1), AI_STRIDE_INIT(4, 4, 4, 48, 48),
  1, &batch_normalization_1_scale_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  input_0_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 12, 1, 30), AI_STRIDE_INIT(4, 4, 4, 48, 48),
  1, &input_0_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  batch_normalization_1_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 12, 1, 30), AI_STRIDE_INIT(4, 4, 4, 48, 48),
  1, &batch_normalization_1_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  LSTM1_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &LSTM1_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN1_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &FCN1_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN1_nl_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &FCN1_nl_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN2_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &FCN2_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN2_nl_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &FCN2_nl_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN3_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 7, 1, 1), AI_STRIDE_INIT(4, 4, 4, 28, 28),
  1, &FCN3_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  FCN3_nl_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 7, 1, 1), AI_STRIDE_INIT(4, 4, 4, 28, 28),
  1, &FCN3_nl_output_array, NULL)


/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  batch_normalization_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&input_0_output),
  AI_TENSOR_LIST_ENTRY(&batch_normalization_1_output),
  AI_TENSOR_LIST_ENTRY(&batch_normalization_1_scale, &batch_normalization_1_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  batch_normalization_1_layer, 0,
  BN_TYPE,
  bn, forward_bn,
  &AI_NET_OBJ_INSTANCE, &LSTM1_layer, AI_STATIC,
  .tensors = &batch_normalization_1_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  LSTM1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&batch_normalization_1_output),
  AI_TENSOR_LIST_ENTRY(&LSTM1_output),
  AI_TENSOR_LIST_ENTRY(&LSTM1_kernel, &LSTM1_recurrent, &LSTM1_peephole, &LSTM1_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  LSTM1_layer, 1,
  LSTM_TYPE,
  lstm, forward_lstm,
  &AI_NET_OBJ_INSTANCE, &FCN1_layer, AI_STATIC,
  .tensors = &LSTM1_chain, 
  .n_units = 64, 
  .activation_nl = ai_math_tanh, 
  .recurrent_nl = ai_math_sigmoid, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .out_nl = ai_math_tanh, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  FCN1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&LSTM1_output),
  AI_TENSOR_LIST_ENTRY(&FCN1_output),
  AI_TENSOR_LIST_ENTRY(&FCN1_weights, &FCN1_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  FCN1_layer, 3,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &FCN1_nl_layer, AI_STATIC,
  .tensors = &FCN1_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  FCN1_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&FCN1_output),
  AI_TENSOR_LIST_ENTRY(&FCN1_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  FCN1_nl_layer, 3,
  NL_TYPE,
  nl, forward_relu,
  &AI_NET_OBJ_INSTANCE, &FCN2_layer, AI_STATIC,
  .tensors = &FCN1_nl_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  FCN2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&FCN1_nl_output),
  AI_TENSOR_LIST_ENTRY(&FCN2_output),
  AI_TENSOR_LIST_ENTRY(&FCN2_weights, &FCN2_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  FCN2_layer, 5,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &FCN2_nl_layer, AI_STATIC,
  .tensors = &FCN2_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  FCN2_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&FCN2_output),
  AI_TENSOR_LIST_ENTRY(&FCN2_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  FCN2_nl_layer, 5,
  NL_TYPE,
  nl, forward_relu,
  &AI_NET_OBJ_INSTANCE, &FCN3_layer, AI_STATIC,
  .tensors = &FCN2_nl_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  FCN3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&FCN2_nl_output),
  AI_TENSOR_LIST_ENTRY(&FCN3_output),
  AI_TENSOR_LIST_ENTRY(&FCN3_weights, &FCN3_bias),
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  FCN3_layer, 7,
  DENSE_TYPE,
  dense, forward_dense,
  &AI_NET_OBJ_INSTANCE, &FCN3_nl_layer, AI_STATIC,
  .tensors = &FCN3_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  FCN3_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&FCN3_output),
  AI_TENSOR_LIST_ENTRY(&FCN3_nl_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  FCN3_nl_layer, 7,
  NL_TYPE,
  nl, forward_sm,
  &AI_NET_OBJ_INSTANCE, &FCN3_nl_layer, AI_STATIC,
  .tensors = &FCN3_nl_chain, 
)


AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 93180, 1,
                     NULL),
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 1696, 1,
                     NULL),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &FCN3_nl_output),
  &batch_normalization_1_layer, 0, NULL)



AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_buffer* activation_buffer)
{
  AI_ASSERT(net_ctx &&  activation_buffer && activation_buffer->data)

  ai_ptr activations = AI_PTR(AI_PTR_ALIGN(activation_buffer->data, 4));
  AI_ASSERT(activations)
  AI_UNUSED(net_ctx)

  {
    /* Updating activations (byte) offsets */
    input_0_output_array.data = AI_PTR(NULL);
    input_0_output_array.data_start = AI_PTR(NULL);
    batch_normalization_1_output_array.data = AI_PTR(activations + 0);
    batch_normalization_1_output_array.data_start = AI_PTR(activations + 0);
    LSTM1_output_array.data = AI_PTR(activations + 1440);
    LSTM1_output_array.data_start = AI_PTR(activations + 1440);
    FCN1_output_array.data = AI_PTR(activations + 0);
    FCN1_output_array.data_start = AI_PTR(activations + 0);
    FCN1_nl_output_array.data = AI_PTR(activations + 0);
    FCN1_nl_output_array.data_start = AI_PTR(activations + 0);
    FCN2_output_array.data = AI_PTR(activations + 128);
    FCN2_output_array.data_start = AI_PTR(activations + 128);
    FCN2_nl_output_array.data = AI_PTR(activations + 128);
    FCN2_nl_output_array.data_start = AI_PTR(activations + 128);
    FCN3_output_array.data = AI_PTR(activations + 0);
    FCN3_output_array.data_start = AI_PTR(activations + 0);
    FCN3_nl_output_array.data = AI_PTR(NULL);
    FCN3_nl_output_array.data_start = AI_PTR(NULL);
    
  }
  return true;
}



AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_buffer* weights_buffer)
{
  AI_ASSERT(net_ctx &&  weights_buffer && weights_buffer->data)

  ai_ptr weights = AI_PTR(weights_buffer->data);
  AI_ASSERT(weights)
  AI_UNUSED(net_ctx)

  {
    /* Updating weights (byte) offsets */
    
    FCN3_bias_array.format |= AI_FMT_FLAG_CONST;
    FCN3_bias_array.data = AI_PTR(weights + 93152);
    FCN3_bias_array.data_start = AI_PTR(weights + 93152);
    FCN3_weights_array.format |= AI_FMT_FLAG_CONST;
    FCN3_weights_array.data = AI_PTR(weights + 92256);
    FCN3_weights_array.data_start = AI_PTR(weights + 92256);
    FCN2_bias_array.format |= AI_FMT_FLAG_CONST;
    FCN2_bias_array.data = AI_PTR(weights + 92128);
    FCN2_bias_array.data_start = AI_PTR(weights + 92128);
    FCN2_weights_array.format |= AI_FMT_FLAG_CONST;
    FCN2_weights_array.data = AI_PTR(weights + 88032);
    FCN2_weights_array.data_start = AI_PTR(weights + 88032);
    FCN1_bias_array.format |= AI_FMT_FLAG_CONST;
    FCN1_bias_array.data = AI_PTR(weights + 87904);
    FCN1_bias_array.data_start = AI_PTR(weights + 87904);
    FCN1_weights_array.format |= AI_FMT_FLAG_CONST;
    FCN1_weights_array.data = AI_PTR(weights + 79712);
    FCN1_weights_array.data_start = AI_PTR(weights + 79712);
    LSTM1_bias_array.format |= AI_FMT_FLAG_CONST;
    LSTM1_bias_array.data = AI_PTR(weights + 78688);
    LSTM1_bias_array.data_start = AI_PTR(weights + 78688);
    LSTM1_peephole_array.format |= AI_FMT_FLAG_CONST;
    LSTM1_peephole_array.data = AI_PTR(weights + 77920);
    LSTM1_peephole_array.data_start = AI_PTR(weights + 77920);
    LSTM1_recurrent_array.format |= AI_FMT_FLAG_CONST;
    LSTM1_recurrent_array.data = AI_PTR(weights + 12384);
    LSTM1_recurrent_array.data_start = AI_PTR(weights + 12384);
    LSTM1_kernel_array.format |= AI_FMT_FLAG_CONST;
    LSTM1_kernel_array.data = AI_PTR(weights + 96);
    LSTM1_kernel_array.data_start = AI_PTR(weights + 96);
    batch_normalization_1_bias_array.format |= AI_FMT_FLAG_CONST;
    batch_normalization_1_bias_array.data = AI_PTR(weights + 48);
    batch_normalization_1_bias_array.data_start = AI_PTR(weights + 48);
    batch_normalization_1_scale_array.format |= AI_FMT_FLAG_CONST;
    batch_normalization_1_scale_array.data = AI_PTR(weights + 0);
    batch_normalization_1_scale_array.data_start = AI_PTR(weights + 0);
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
      
      .n_macc            = 597465,
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

