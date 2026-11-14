/*
 * AIfES MNIST MLP (784-512-256-128-64-10) on a Zynq-7020 (ARM Cortex-A9, standalone)
 * Vitis "Hello World" application: this file replaces helloworld.c.
 *
 * F32 by default. Uncomment USE_Q7 to run the quantized model.
 */
//#define USE_Q7

#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "xil_printf.h"
#include "xiltimer.h"       // XTime_GetTime / COUNTS_PER_SECOND (A9 global timer)

#include "aifes.h"

// Print functions used by the generated AIfES headers
// (in AIfES_for_Arduino they are implemented in aifes_config.cpp, for Arduino only)
void aiprint(const char *str)                          { printf("%s", str); }
void aiprint_uint(const char *format, unsigned int v)  { printf(format, v); }

#ifdef USE_Q7
  #include "aifes_q7_fnn.h"
  #define CREATE_MODEL()       aifes_q7_fnn_create_model()
  #define INFERENCE(in, out)   aifes_q7_fnn_inference((in), (out))
  #define FMT_NAME "Q7"
#else
  #include "aifes_f32_fnn.h"
  #define CREATE_MODEL()       aifes_f32_fnn_create_model()
  #define INFERENCE(in, out)   aifes_f32_fnn_inference((in), (out))
  #define FMT_NAME "F32"
#endif

#include "test_data.h"      // N_TEST, test_images, test_labels, keras_pred

static int argmax(const float *v, int n)
{
    int best = 0;
    for (int i = 1; i < n; i++) if (v[i] > v[best]) best = i;
    return best;
}

int main(void)
{
    init_platform();
    printf("\r\n=== AIfES MNIST " FMT_NAME " on Zynq ===\r\n");

    uint8_t error = CREATE_MODEL();
    if (error != 0) {
        printf("create_model error (%d)\r\n", error);
        return error;
    }

    float output_data[10];
    int correct = 0, same_as_keras = 0;
    XTime t0, t1;
    uint64_t total_ticks = 0;

    for (int i = 0; i < N_TEST; i++) {
        XTime_GetTime(&t0);
        INFERENCE((float *)test_images[i], output_data);
        XTime_GetTime(&t1);
        total_ticks += (t1 - t0);

        int pred = argmax(output_data, 10);
        if (pred == test_labels[i]) correct++;
        if (pred == keras_pred[i])  same_as_keras++;

        if (i < 5) {   // details for the first 5 images
            printf("img %d  label=%d  pred=%d  keras=%d  | ", i, test_labels[i], pred, keras_pred[i]);
            for (int k = 0; k < 10; k++) printf("%.3f ", output_data[k]);
            printf("\r\n");
        }
    }

    double us = (double)total_ticks * 1e6 / COUNTS_PER_SECOND / N_TEST;
    printf("\r\nAccuracy (vs labels)     : %d / %d\r\n", correct, N_TEST);
    printf("Agreement with Keras     : %d / %d\r\n", same_as_keras, N_TEST);
    printf("Mean time per inference  : %.1f us\r\n", us);

    cleanup_platform();
    return 0;
}
