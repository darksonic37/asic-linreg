#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "mb_interface.h"

#define OPCODE_RESET   4
#define OPCODE_STORE_X 0
#define OPCODE_STORE_Y 1
#define OPCODE_STORE_T 2
#define OPCODE_STORE_A 5
#define OPCODE_COMPUTE 3

#define CONSTANT_M 10
#define CONSTANT_N 2
#define SCALING_FACTOR 2048

unsigned int reset()
{
	unsigned int inst = 0; // don't care
	inst = ((OPCODE_RESET << 29) | inst);
	return inst;
}

unsigned int store_x(int x, unsigned int i, unsigned int j)
{
	unsigned int inst = (x & 0x007fffff);
	inst = ((j << 23) & 0x03800000) | inst;
	inst = ((i << 26) & 0x1C000000) | inst;
	inst = (OPCODE_STORE_X << 29) | inst;
	return inst;
}

unsigned int store_y(int y, unsigned int i)
{
	unsigned int inst = (y & 0x03ffffff);
	inst = ((i << 26) & 0x1C000000) | inst;
	inst = (OPCODE_STORE_Y << 29) | inst;
	return inst;
}

unsigned int store_t(int t, unsigned int i)
{
	unsigned int inst = (t & 0x03ffffff);
	inst = ((i << 26) & 0x1C000000) | inst;
	inst = (OPCODE_STORE_T << 29) | inst;
	return inst;
}

unsigned int store_a(int a)
{
	unsigned int inst = (a & 0x03ffffff);
	inst = (OPCODE_STORE_A << 29) | inst;
	return inst;
}

unsigned int compute(unsigned int iter)
{
	unsigned int inst = iter;
	inst = ((OPCODE_COMPUTE << 29) | inst);
	return inst;
}

int main()
{
	init_platform();

    int X[CONSTANT_M][CONSTANT_N] = {
    		{1*SCALING_FACTOR, 2.34*SCALING_FACTOR},
			{1*SCALING_FACTOR, 3.77*SCALING_FACTOR},
			{1*SCALING_FACTOR, 4.54*SCALING_FACTOR},
    		{1*SCALING_FACTOR, 5.81*SCALING_FACTOR},
    		{1*SCALING_FACTOR, 6.12*SCALING_FACTOR},
    		{1*SCALING_FACTOR, 5.01*SCALING_FACTOR},
    		{1*SCALING_FACTOR, 3.09*SCALING_FACTOR},
    		{1*SCALING_FACTOR, 4.14*SCALING_FACTOR},
    		{1*SCALING_FACTOR, 1.48*SCALING_FACTOR},
    		{1*SCALING_FACTOR, 2.20*SCALING_FACTOR}
    };

    int Y[] = {
    		4.12*SCALING_FACTOR,
			3.04*SCALING_FACTOR,
			3.19*SCALING_FACTOR,
			6.35*SCALING_FACTOR,
			4.73*SCALING_FACTOR,
			6.77*SCALING_FACTOR,
			1.39*SCALING_FACTOR,
			5.76*SCALING_FACTOR,
			3.27*SCALING_FACTOR,
			2.82*SCALING_FACTOR
    };

    int T[] = {
    		1.01*SCALING_FACTOR,
			2.02*SCALING_FACTOR
    };

    int alpha = 0.11*SCALING_FACTOR;


//    int X[3][3] = {{1*SCALING_FACTOR,2*SCALING_FACTOR,5*SCALING_FACTOR}, {1*SCALING_FACTOR,3*SCALING_FACTOR,6*SCALING_FACTOR}, {1*SCALING_FACTOR,4*SCALING_FACTOR,7*SCALING_FACTOR}};
//    int Y[3] = {0*SCALING_FACTOR,1*SCALING_FACTOR,2*SCALING_FACTOR};
//    int T[3] = {1*SCALING_FACTOR,2*SCALING_FACTOR,2*SCALING_FACTOR};
//    int alpha = CONSTANT_M*SCALING_FACTOR;

    // Reset coprocessor
    putfsl(reset(), 0);

    // Store X matrix
    for (unsigned int i = 0; i < CONSTANT_M; i++) {
        for (unsigned int j = 0; j < CONSTANT_N; j++) {
        	unsigned int inst = store_x(X[i][j], i, j);
            putfsl(inst, 0);
    		xil_printf("\nStored X[%d][%d] = %d with instruction 0x%08x", i, j, X[i][j], inst);
        }
    }

    // Store Y vector
	for (unsigned int i = 0; i < CONSTANT_M; i++) {
		unsigned int inst = store_y(Y[i], i);
		putfsl(inst, 0);
		xil_printf("\nStored Y[%d] = %d with instruction 0x%08x", i, Y[i], inst);
	}

	// Store theta vector
	for (unsigned int i = 0; i < CONSTANT_N; i++) {
		unsigned int inst = store_t(T[i], i);
		putfsl(inst, 0);
		xil_printf("\nStored T[%d] = %d with instruction 0x%08x", i, T[i], inst);
	}

	// Store learning rate value
	unsigned int inst = store_a(alpha);
	xil_printf("\nStored alpha = %d with instruction 0x%08x", alpha, inst);
	putfsl(inst, 0);

    // Run gradient descent for linear regression until the algorithm converges
	int iter;
    for (iter = 0; iter < 5; iter++) {
		// Issue new iteration of gradient descent
    	unsigned int inst = compute(iter+1);
    	putfsl(inst, 0);
    	xil_printf("\n\nIssued iteration %d of gradient descent with instruction 0x%08x", iter+1, inst);

        // Retrieve new theta vector
    	for (int l = 0; l < CONSTANT_N; l++) {
    		unsigned int v;
    		getfsl(v, 0);
    		T[l] = (int) v;
    		xil_printf("\nRetrieved new T[%d] = %d", l, T[l]);
    	}
    }

    xil_printf("\n\nAlgorithm converged in %d iterations!", iter);
    xil_printf("\nLinear regression model is y = ");
    for (unsigned int i = 0; i < CONSTANT_N-1; i++) xil_printf("%d*x^%d + ", T[i], i);
    xil_printf("%d*x^%d", T[CONSTANT_N-1], CONSTANT_N-1);

    cleanup_platform();
    return 0;
}
