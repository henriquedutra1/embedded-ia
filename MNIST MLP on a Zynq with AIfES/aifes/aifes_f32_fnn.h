/*
  www.aifes.ai
  https://github.com/Fraunhofer-IMS/AIfES_for_Arduino

  You can find more AIfES tutorials here:
  https://create.arduino.cc/projecthub/aifes_team

  AIfES: Configuration file automatically generated from AIfES-Converter
  https://github.com/Fraunhofer-IMS/AIfES-Converter

  You first need to initialize the model once with:
  ---------------------------------------------------------------------------
    uint8_t error;
    error = aifes_f32_fnn_create_model();
    if (error == 1)
    {
        //do something for error handling
        //e.g. while(true)
    }
  ---------------------------------------------------------------------------
  Please check the error. It is 1 if something inside the initialization failed. Check the output

  Paste the following code into your project and insert your inputs/features:
  ---------------------------------------------------------------------------
    float input_data[784]; // AIfES input data
    float output_data[10]; // AIfES output data

    aifes_f32_fnn_inference((float*) input_data, (float*) output_data);
  ---------------------------------------------------------------------------


  Copyright (C) 2022  Fraunhofer Institute for Microelectronic Circuits and Systems.
  All rights reserved.
  AIfES-Converter is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.
  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <aifes.h>
#include "aifes_f32_weights.h"

#ifndef AIFES_F32_FNN
#define AIFES_F32_FNN

#define DATASETS 1
#define INPUTS  784
#define OUTPUTS 10

aimodel_t model;

// Layer definition
uint16_t input_layer_shape[] = {DATASETS, INPUTS};

ailayer_input_f32_t		input_layer	= AILAYER_INPUT_F32_A(2, input_layer_shape);
ailayer_dense_f32_t		dense_layer_1	= AILAYER_DENSE_F32_A(512);
ailayer_relu_f32_t		relu_layer_1	= AILAYER_RELU_F32_A();
ailayer_dense_f32_t		dense_layer_2	= AILAYER_DENSE_F32_A(256);
ailayer_relu_f32_t		relu_layer_2	= AILAYER_RELU_F32_A();
ailayer_dense_f32_t		dense_layer_3	= AILAYER_DENSE_F32_A(128);
ailayer_relu_f32_t		relu_layer_3	= AILAYER_RELU_F32_A();
ailayer_dense_f32_t		dense_layer_4	= AILAYER_DENSE_F32_A(64);
ailayer_relu_f32_t		relu_layer_4	= AILAYER_RELU_F32_A();
ailayer_dense_f32_t		dense_layer_5	= AILAYER_DENSE_F32_A(10);
ailayer_softmax_f32_t		softmax_layer_5	= AILAYER_SOFTMAX_F32_A();


uint16_t output_layer_shape[] = {DATASETS, OUTPUTS};

uint8_t aifes_f32_fnn_create_model()
{
  ailayer_t *x;

  model.input_layer = ailayer_input_f32_default(&input_layer);
  x = ailayer_dense_f32_default(&dense_layer_1, model.input_layer);
  x = ailayer_relu_f32_default(&relu_layer_1, x);
  x = ailayer_dense_f32_default(&dense_layer_2, x);
  x = ailayer_relu_f32_default(&relu_layer_2, x);
  x = ailayer_dense_f32_default(&dense_layer_3, x);
  x = ailayer_relu_f32_default(&relu_layer_3, x);
  x = ailayer_dense_f32_default(&dense_layer_4, x);
  x = ailayer_relu_f32_default(&relu_layer_4, x);
  x = ailayer_dense_f32_default(&dense_layer_5, x);
  x = ailayer_softmax_f32_default(&softmax_layer_5, x);
  model.output_layer = x;

  aialgo_compile_model(&model); // Compile the AIfES model

  uint32_t parameter_memory_size = aialgo_sizeof_parameter_memory(&model);

  aialgo_distribute_parameter_memory(&model, (void*) aifes_f32_flat_weights, parameter_memory_size);

  uint32_t memory_size = aialgo_sizeof_inference_memory(&model);

  // Comment the following lines after the first run of AIfES, when you use the option with an array instead of malloc
  void *memory_ptr = malloc(memory_size);

  if(memory_ptr == NULL)
  {
    aiprint("Not enough memory available for inference of the neural network.");
    return 1;
  }

  aiprint("AIfES needs an inference memory of:");
  aiprint_uint("%u", memory_size);
  aiprint("\nYou can use this size and create an array to avoid using malloc.\n");

  // Uncomment the following line and add the output of the previous printf into SIZE_OF_PRINTF. Then comment the lines
  // above
  // uint8_t memory_array[SIZE_OF_PRINTF];
  // uint8_t *memory_ptr= memory_array

  aialgo_schedule_inference_memory(&model, memory_ptr, memory_size);
  return 0;

}

void aifes_f32_fnn_inference(float* input_data, float* output_data)
{

  aitensor_t input_tensor = AITENSOR_2D_F32(input_layer_shape, input_data);

  // Definition of the output shape
  aitensor_t output_tensor = AITENSOR_2D_F32(output_layer_shape, output_data);

  aialgo_inference_model(&model, &input_tensor, &output_tensor);

}

#endif // AIFES_F32_FNN