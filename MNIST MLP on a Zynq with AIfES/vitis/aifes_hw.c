// Dense layer of AIfES where each multiplication is done by mul_ip (PL)
// reg0 = A, reg1 = B, reg2 = A*B

#include "xil_io.h"
#include "xparameters.h"
#include "aifes.h"

#define MUL_BASE  0x43C00000   // mul_ip_0 address (Vivado Address Editor)

typedef union { float f; u32 u; } float_u32;

float mul_hw(float a, float b)
{
    float_u32 x, y, r;
    x.f = a;
    y.f = b;
    Xil_Out32(MUL_BASE + 0, x.u);
    Xil_Out32(MUL_BASE + 4, y.u);
    r.u = Xil_In32(MUL_BASE + 8);
    return r.f;
}

// same code as aimath_f32_default_linear(), with mul_hw() instead of *
void linear_hw(const aitensor_t *a, const aitensor_t *b, const aitensor_t *c, aitensor_t *result)
{
    float *x = (float *) a->data;
    float *w = (float *) b->data;
    float *y = (float *) result->data;

    for (int i = 0; i < a->shape[0]; i++) {
        for (int j = 0; j < b->shape[1]; j++) {
            float sum = 0;
            for (int k = 0; k < a->shape[1]; k++)
                sum += mul_hw(x[i * a->shape[1] + k], w[k * b->shape[1] + j]);
            if (c != 0)
                sum += ((float *) c->data)[j];
            y[i * result->shape[1] + j] = sum;
        }
    }
}
