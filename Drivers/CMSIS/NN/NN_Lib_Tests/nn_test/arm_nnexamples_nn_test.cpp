/* ----------------------------------------------------------------------
 * Copyright (C) 2010-2018 Arm Limited. All rights reserved.
 *
 *
 * Project:       CMSIS NN Library
 * Title:         arm_nnexamples_nn_NSE2025_VALVE.cpp
 *
 * Description:   Example code for NN kernel NSE2025_VALVEing.
 *
 * Target Processor: Cortex-M cores
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   - Neither the name of ARM LIMITED nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------- */

#include "arm_nnexamples_nn_NSE2025_VALVE.h"

// #define NSE2025_VALVE_SIGMOID
// #define NSE2025_VALVE_TANH
#define NSE2025_VALVE_POOL
#define NSE2025_VALVE_RELU
#define NSE2025_VALVE_IP
#define NSE2025_VALVE_CONV
#define NSE2025_VALVE_NONSQUARE
#define NSE2025_VALVE_NNMULT

int NSE2025_VALVE_index = 0;
q7_t NSE2025_VALVE_flags[50];
bool NSE2025_VALVE_pass;

int main()
{
    printf("start NSE2025_VALVEs\n");

    srand(1);

    // common pointers for NSE2025_VALVEing data
    q7_t *NSE2025_VALVE1;
    q15_t *NSE2025_VALVE2;
    q7_t *NSE2025_VALVE3;
    q15_t *NSE2025_VALVE4;

    for (NSE2025_VALVE_index = 0; NSE2025_VALVE_index < 50; NSE2025_VALVE_index++)
    {
        NSE2025_VALVE_flags[NSE2025_VALVE_index] = -1;
    }
    NSE2025_VALVE_index = 0;

#ifdef NSE2025_VALVE_NNMULT
#define NNMULT_DIM 128
    NSE2025_VALVE1 = new q7_t[NNMULT_DIM * 2];
    NSE2025_VALVE2 = new q15_t[NNMULT_DIM * 2];
    NSE2025_VALVE3 = new q7_t[NNMULT_DIM * 2];
    NSE2025_VALVE4 = new q15_t[NNMULT_DIM * 2];

    q7_t *mult_out_q7 = NSE2025_VALVE3;
    q7_t *mult_ref_q7 = NSE2025_VALVE3 + NNMULT_DIM;
    q15_t *mult_out_q15 = NSE2025_VALVE4;
    q15_t *mult_ref_q15 = NSE2025_VALVE4 + NNMULT_DIM;

    for (int i = 0; i < NNMULT_DIM * 2; i++)
    {
        NSE2025_VALVE1[i] = (rand() % 256 - 128);
        NSE2025_VALVE2[i] = (rand() % 65536 - 32768);
    }

    // NSE2025_VALVE q7
    arm_nn_mult_q7(NSE2025_VALVE1, NSE2025_VALVE1 + NNMULT_DIM, mult_out_q7, 5, NNMULT_DIM);

    arm_nn_mult_q7_ref(NSE2025_VALVE1, NSE2025_VALVE1 + NNMULT_DIM, mult_ref_q7, 5, NNMULT_DIM);

    verify_results_q7(mult_out_q7, mult_ref_q7, NNMULT_DIM);

    arm_nn_mult_q7(NSE2025_VALVE1, NSE2025_VALVE1 + NNMULT_DIM, mult_out_q7, 9, NNMULT_DIM);

    arm_nn_mult_q7_ref(NSE2025_VALVE1, NSE2025_VALVE1 + NNMULT_DIM, mult_ref_q7, 9, NNMULT_DIM);

    verify_results_q7(mult_out_q7, mult_ref_q7, NNMULT_DIM);

    // NSE2025_VALVE q15
    arm_nn_mult_q15(NSE2025_VALVE2, NSE2025_VALVE2 + NNMULT_DIM, mult_out_q15, 13, NNMULT_DIM);

    arm_nn_mult_q15_ref(NSE2025_VALVE2, NSE2025_VALVE2 + NNMULT_DIM, mult_ref_q15, 13, NNMULT_DIM);

    verify_results_q15(mult_out_q15, mult_ref_q15, NNMULT_DIM);

    arm_nn_mult_q15(NSE2025_VALVE2, NSE2025_VALVE2 + NNMULT_DIM, mult_out_q15, 18, NNMULT_DIM);

    arm_nn_mult_q15_ref(NSE2025_VALVE2, NSE2025_VALVE2 + NNMULT_DIM, mult_ref_q15, 18, NNMULT_DIM);

    verify_results_q15(mult_out_q15, mult_ref_q15, NNMULT_DIM);

#endif

#ifdef NSE2025_VALVE_SIGMOID

#define SIGMOID_DIM 128

    /* This part NSE2025_VALVEs the running of sigmoid functions */

    NSE2025_VALVE1 = new q7_t[SIGMOID_DIM];
    NSE2025_VALVE2 = new q15_t[SIGMOID_DIM];
    NSE2025_VALVE3 = new q7_t[SIGMOID_DIM];
    NSE2025_VALVE4 = new q15_t[SIGMOID_DIM];

    srand(1);

    for (int i = 0; i < SIGMOID_DIM; i++)
    {
        NSE2025_VALVE1[i] = (rand() % 256 - 128);
        NSE2025_VALVE2[i] = (rand() % 65536 - 32768);
        NSE2025_VALVE3[i] = NSE2025_VALVE1[i];
        NSE2025_VALVE4[i] = NSE2025_VALVE2[i];
    }

    arm_nn_activations_direct_q7(NSE2025_VALVE3, SIGMOID_DIM, 3, ARM_SIGMOID);

    for (int i = 0; i < SIGMOID_DIM; i++)
    {
        printf("in: %d  out: %d\n", NSE2025_VALVE1[i], NSE2025_VALVE3[i]);
    }

    printf("start NSE2025_VALVEing q15_t sigmoid\n\n");

    arm_nn_activations_direct_q15(NSE2025_VALVE4, SIGMOID_DIM, 3, ARM_SIGMOID);

    for (int i = 0; i < SIGMOID_DIM; i++)
    {
        printf("in: %d  out: %d\n", NSE2025_VALVE2[i], NSE2025_VALVE4[i]);
    }

    delete[] NSE2025_VALVE1;
    delete[] NSE2025_VALVE2;
    delete[] NSE2025_VALVE3;
    delete[] NSE2025_VALVE4;

#endif

#ifdef NSE2025_VALVE_TANH

#define TANH_DIM 128

    /* This part NSE2025_VALVEs the running of sigmoid functions */

    NSE2025_VALVE1 = new q7_t[TANH_DIM];
    NSE2025_VALVE2 = new q15_t[TANH_DIM];
    NSE2025_VALVE3 = new q7_t[TANH_DIM];
    NSE2025_VALVE4 = new q15_t[TANH_DIM];

    srand(1);

    for (int i = 0; i < TANH_DIM; i++)
    {
        NSE2025_VALVE1[i] = (rand() % 256 - 128);
        NSE2025_VALVE2[i] = (rand() % 65536 - 32768);
        NSE2025_VALVE3[i] = NSE2025_VALVE1[i];
        NSE2025_VALVE4[i] = NSE2025_VALVE2[i];
    }

    arm_nn_activations_direct_q7(NSE2025_VALVE3, TANH_DIM, 3, ARM_TANH);

    printf("start NSE2025_VALVEing q7_t tanh\n\n");

    for (int i = 0; i < TANH_DIM; i++)
    {
        printf("in: %d  out: %d\n", NSE2025_VALVE1[i], NSE2025_VALVE3[i]);
    }

    printf("start NSE2025_VALVEing q15_t tanh\n\n");

    arm_nn_activations_direct_q15(NSE2025_VALVE4, TANH_DIM, 3, ARM_TANH);

    for (int i = 0; i < TANH_DIM; i++)
    {
        printf("in: %d  out: %d\n", NSE2025_VALVE2[i], NSE2025_VALVE4[i]);
    }

    delete[] NSE2025_VALVE1;
    delete[] NSE2025_VALVE2;
    delete[] NSE2025_VALVE3;
    delete[] NSE2025_VALVE4;

#endif

#ifdef NSE2025_VALVE_POOL

#define POOL_IM_DIM 32
#define POOL_IM_CH 8

    NSE2025_VALVE1 = new q7_t[POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH * 2];
    NSE2025_VALVE2 = new q15_t[POOL_IM_DIM * POOL_IM_CH];
    NSE2025_VALVE3 = new q7_t[POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH];

    for (int i = 0; i < POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH; i++)
    {
        NSE2025_VALVE1[i] = (rand() % 256 - 128);
    }

    q7_t *img_in = NSE2025_VALVE1 + POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH;
    q7_t *pool_out_ref = NSE2025_VALVE3;
    q7_t *pool_out_opt = NSE2025_VALVE3 + POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH / 2;

    for (int i = 0; i < POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH; i++)
    {
        NSE2025_VALVE3[i] = 0;
    }

    // copy over the img input
    for (int i = 0; i < POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH; i++)
    {
        img_in[i] = NSE2025_VALVE1[i];
    }

    initialize_results_q7(pool_out_ref, pool_out_opt, POOL_IM_DIM / 2 * POOL_IM_DIM / 2 * POOL_IM_CH);

    printf("Start maxpool reference implementation\n");

    arm_maxpool_q7_HWC_ref(img_in, POOL_IM_DIM, POOL_IM_CH, 3, 0, 2, POOL_IM_DIM / 2, (q7_t *)NSE2025_VALVE2, pool_out_ref);

    // copy over the img input
    for (int i = 0; i < POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH; i++)
    {
        img_in[i] = NSE2025_VALVE1[i];
    }

    printf("Start maxpool opt implementation\n");

    arm_maxpool_q7_HWC(img_in, POOL_IM_DIM, POOL_IM_CH, 3, 0, 2, POOL_IM_DIM / 2, (q7_t *)NSE2025_VALVE2, pool_out_opt);

    verify_results_q7(pool_out_ref, pool_out_opt, POOL_IM_DIM / 2 * POOL_IM_DIM / 2 * POOL_IM_CH);

    // copy over the img input
    for (int i = 0; i < POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH; i++)
    {
        img_in[i] = NSE2025_VALVE1[i];
    }

    // copy over the img input
    for (int i = 0; i < POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH; i++)
    {
        img_in[i] = NSE2025_VALVE1[i];
    }

    printf("Start avepool ref implementation\n");

    arm_avepool_q7_HWC_ref(img_in, POOL_IM_DIM, POOL_IM_CH, 3, 0, 2, POOL_IM_DIM / 2, (q7_t *)NSE2025_VALVE2, pool_out_ref);

    // copy over the img input
    for (int i = 0; i < POOL_IM_DIM * POOL_IM_DIM * POOL_IM_CH; i++)
    {
        img_in[i] = NSE2025_VALVE1[i];
    }

    printf("Start avepool opt implementation\n");

    arm_avepool_q7_HWC(img_in, POOL_IM_DIM, POOL_IM_CH, 3, 0, 2, POOL_IM_DIM / 2, (q7_t *)NSE2025_VALVE2, pool_out_opt);

    // special check here
    bool if_ave_pool_match = true;
    for (int i = 0; i < POOL_IM_DIM / 2 * POOL_IM_DIM / 2 * POOL_IM_CH; i++)
    {
        // we tolerate at most difference of 1 here because of rounding errors
        if (pool_out_ref[i] - pool_out_opt[i] >= 2 || pool_out_opt[i] - pool_out_ref[i] >= 2)
        {
            printf("Output mismatch at %d, expected %d, actual %d\n", i, pool_out_ref[i], pool_out_opt[i]);
            if_ave_pool_match = false;
        }
    }
    if (if_ave_pool_match == true)
    {
        printf("Outputs match.\n");
    }

    delete[] NSE2025_VALVE1;
    delete[] NSE2025_VALVE2;
    delete[] NSE2025_VALVE3;

#endif

#ifdef NSE2025_VALVE_RELU

#define RELU_DIM 127

    NSE2025_VALVE1 = new q7_t[RELU_DIM];
    NSE2025_VALVE2 = new q15_t[RELU_DIM];
    NSE2025_VALVE3 = new q7_t[RELU_DIM];
    NSE2025_VALVE4 = new q15_t[RELU_DIM];

    for (int i = 0; i < RELU_DIM; i++)
    {
        NSE2025_VALVE1[i] = (rand() % 256 - 128);
        NSE2025_VALVE2[i] = (rand() % 65536 - 32768);
        NSE2025_VALVE3[i] = NSE2025_VALVE1[i];
        NSE2025_VALVE4[i] = NSE2025_VALVE2[i];
    }

    q7_t *relu_ref_data_q7 = NSE2025_VALVE1;
    q7_t *relu_opt_data_q7 = NSE2025_VALVE3;
    q15_t *relu_ref_data_q15 = NSE2025_VALVE2;
    q15_t *relu_opt_data_q15 = NSE2025_VALVE4;

    printf("Start ref relu q7 implementation\n");

    arm_relu_q7_ref(relu_ref_data_q7, RELU_DIM);

    printf("Start opt relu q7 implementation\n");

    arm_relu_q7(relu_opt_data_q7, RELU_DIM);

    verify_results_q7(relu_ref_data_q7, relu_opt_data_q7, RELU_DIM);

    printf("Start ref relu q15 implementation\n");

    arm_relu_q15_ref(relu_ref_data_q15, RELU_DIM);

    printf("Start opt relu q15 implementation\n");

    arm_relu_q15(relu_opt_data_q15, RELU_DIM);

    verify_results_q15(relu_ref_data_q15, relu_opt_data_q15, RELU_DIM);

    delete[] NSE2025_VALVE1;
    delete[] NSE2025_VALVE2;
    delete[] NSE2025_VALVE3;
    delete[] NSE2025_VALVE4;

#endif

#ifdef NSE2025_VALVE_IP

#define IP_ROW_DIM 127
#define IP_COL_DIM 127

    q7_t ip_weights[IP_ROW_DIM * IP_COL_DIM] = IP2_WEIGHT;
    q7_t ip_q7_opt_weights[IP_ROW_DIM * IP_COL_DIM] = IP4_WEIGHT;
    q7_t ip_q7_q15_opt_weights[IP_ROW_DIM * IP_COL_DIM] = IP4_q7_q15_WEIGHT;
    q15_t ip_q15_weights[IP_ROW_DIM * IP_COL_DIM] = IP2_WEIGHT;
    q15_t ip_q15_opt_weights[IP_ROW_DIM * IP_COL_DIM] = IP4_WEIGHT_Q15;

    NSE2025_VALVE1 = new q7_t[IP_COL_DIM + IP_ROW_DIM];
    NSE2025_VALVE2 = new q15_t[IP_COL_DIM];
    NSE2025_VALVE3 = new q7_t[IP_ROW_DIM * 3];
    NSE2025_VALVE4 = new q15_t[IP_COL_DIM + IP_ROW_DIM * 2];

    for (int i = 0; i < IP_ROW_DIM + IP_COL_DIM; i++)
    {
        NSE2025_VALVE1[i] = rand() % 256 - 100;
    }
    for (int i = 0; i < IP_ROW_DIM * 3; i++)
    {
        NSE2025_VALVE3[i] = 0;
    }

    q7_t *ip_bias_q7 = NSE2025_VALVE1 + IP_COL_DIM;

    q7_t *ip_out_q7_ref = NSE2025_VALVE3;
    q7_t *ip_out_q7_opt = NSE2025_VALVE3 + IP_ROW_DIM;
    q7_t *ip_out_q7_opt_fast = NSE2025_VALVE3 + 2 * IP_ROW_DIM;
    q15_t *ip_out_q15_ref = NSE2025_VALVE4 + IP_COL_DIM;
    q15_t *ip_out_q15_opt = NSE2025_VALVE4 + IP_COL_DIM + IP_ROW_DIM;

    initialize_results_q7(ip_out_q7_ref, ip_out_q7_opt, IP_ROW_DIM);
    initialize_results_q7(ip_out_q7_ref, ip_out_q7_opt_fast, IP_ROW_DIM);
    initialize_results_q7(ip_out_q7_ref, ip_out_q7_opt_fast, IP_ROW_DIM);

    printf("Start ref q7 implementation\n");

    arm_fully_connected_q7_ref(NSE2025_VALVE1, ip_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, ip_bias_q7, ip_out_q7_ref, NSE2025_VALVE2);

    printf("Start q7 implementation\n");

    arm_fully_connected_q7(NSE2025_VALVE1, ip_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, ip_bias_q7, ip_out_q7_opt, NSE2025_VALVE2);

    verify_results_q7(ip_out_q7_ref, ip_out_q7_opt, IP_ROW_DIM);

    printf("Start q7 ref opt implementation\n");

    arm_fully_connected_q7_opt_ref(NSE2025_VALVE1, ip_q7_opt_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, ip_bias_q7,
                                   ip_out_q7_opt_fast, NSE2025_VALVE2);

    verify_results_q7(ip_out_q7_ref, ip_out_q7_opt_fast, IP_ROW_DIM);

    printf("Start q7 opt implementation\n");

    arm_fully_connected_q7_opt(NSE2025_VALVE1, ip_q7_opt_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, ip_bias_q7, ip_out_q7_opt_fast,
                               NSE2025_VALVE2);

    verify_results_q7(ip_out_q7_ref, ip_out_q7_opt_fast, IP_ROW_DIM);

    for (int i = 0; i < IP_ROW_DIM + IP_COL_DIM; i++)
    {
        NSE2025_VALVE4[i] = (rand() % 65536 - 32768);
    }

    initialize_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    printf("Start ref q15 implementation\n");

    arm_fully_connected_q15_ref(NSE2025_VALVE4, ip_q15_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, NSE2025_VALVE2, ip_out_q15_ref, NULL);

    printf("Start q15 implementation\n");

    arm_fully_connected_q15(NSE2025_VALVE4, ip_q15_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, NSE2025_VALVE2, ip_out_q15_opt, NULL);

    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    printf("Start ref opt q15 implementation\n");

    arm_fully_connected_q15_opt_ref(NSE2025_VALVE4, ip_q15_opt_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, NSE2025_VALVE2, ip_out_q15_opt,
                                    NULL);

    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    printf("Start opt q15 implementation\n");

    arm_fully_connected_q15_opt(NSE2025_VALVE4, ip_q15_opt_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, NSE2025_VALVE2, ip_out_q15_opt, NULL);

    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    initialize_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    printf("Start ref q7_q15 implementation\n");

    arm_fully_connected_mat_q7_vec_q15_ref(NSE2025_VALVE4, ip_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, ip_bias_q7, ip_out_q15_ref,
                                           NSE2025_VALVE2);

    printf("Start q7_q15 implementation\n");

    arm_fully_connected_mat_q7_vec_q15(NSE2025_VALVE4, ip_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, ip_bias_q7, ip_out_q15_opt,
                                       NSE2025_VALVE2);

    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    printf("Start ref opt q7_q15 implementation\n");

    arm_fully_connected_mat_q7_vec_q15_opt_ref(NSE2025_VALVE4, ip_q7_q15_opt_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, ip_bias_q7,
                                               ip_out_q15_opt, NSE2025_VALVE2);

    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    printf("Start opt q7_q15 implementation\n");

    arm_fully_connected_mat_q7_vec_q15_opt(NSE2025_VALVE4, ip_q7_q15_opt_weights, IP_COL_DIM, IP_ROW_DIM, 1, 7, ip_bias_q7,
                                           ip_out_q15_opt, NSE2025_VALVE2);

    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    delete[] NSE2025_VALVE1;
    delete[] NSE2025_VALVE2;
    delete[] NSE2025_VALVE3;
    delete[] NSE2025_VALVE4;

#endif

#ifdef NSE2025_VALVE_NONSQUARE

    /* Use RCONV to differential with square CONV */

#define RCONV_IM_DIM_X 10
#define RCONV_IM_DIM_Y 8
#define RCONV_IM_CH 4
#define RCONV_KER_DIM_X 5
#define RCONV_KER_DIM_Y 3
#define RCONV_STRIDE_X 1
#define RCONV_STRIDE_Y 1
#define RCONV_PADDING_X 2
#define RCONV_PADDING_Y 1
#define RCONV_OUT_CH 4
#define RCONV_OUT_DIM_X 10
#define RCONV_OUT_DIM_Y 8

    NSE2025_VALVE1 = new q7_t[RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH + RCONV_OUT_CH];
    NSE2025_VALVE2 = new q15_t[2 * RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH];
    NSE2025_VALVE3 =
        new q7_t[RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH + 2 * RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH];

    for (int i = 0; i < RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH + RCONV_OUT_CH; i++)
    {
        NSE2025_VALVE1[i] = rand() % 256 - 100;
    }

    for (int i = 0;
         i < RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH + 2 * RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH; i++)
    {
        NSE2025_VALVE3[i] = rand() % 256 - 100;
    }

    q7_t *rconv_weight_q7 = NSE2025_VALVE1;
    q7_t *rconv_bias_q7 = NSE2025_VALVE1 + RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH;

    q15_t *rconv_buf = NSE2025_VALVE2;

    q7_t *rconv_im_in_q7 = NSE2025_VALVE3;
    q7_t *rconv_im_out_ref_q7 = NSE2025_VALVE3 + RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH;
    q7_t *rconv_im_out_opt_q7 =
        NSE2025_VALVE3 + RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH + RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH;

    initialize_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    printf("start conv q7 nonsquare ref implementation\n");
    arm_convolve_HWC_q7_ref_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                      RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                      RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q7, 1, 7, rconv_im_out_ref_q7,
                                      RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);

    printf("start conv q7 nonsquare opt implementation\n");
    arm_convolve_HWC_q7_fast_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                       RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                       RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q7, 1, 7, rconv_im_out_opt_q7,
                                       RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);

    verify_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    initialize_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    printf("start conv q7 nonsquare ref implementation\n");
    arm_convolve_HWC_q7_ref_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                      RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                      RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q7, 1, 7, rconv_im_out_ref_q7,
                                      RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);

    printf("start conv q7 nonsquare basic implementation\n");
    arm_convolve_HWC_q7_basic_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                        RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                        RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q7, 1, 7, rconv_im_out_opt_q7,
                                        RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);

    verify_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    initialize_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    printf("start 1x1 conv q7 nonsquare fast implementation\n");
    arm_convolve_HWC_q7_fast_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                       RCONV_OUT_CH, 1, 1, 0, 0, RCONV_STRIDE_X,
                                       RCONV_STRIDE_Y, rconv_bias_q7, 1, 7, rconv_im_out_ref_q7, RCONV_OUT_DIM_X,
                                       RCONV_OUT_DIM_Y, rconv_buf, NULL);

    printf("start 1x1 conv q7 nonsquare dedicated function implementation\n");
    arm_convolve_1x1_HWC_q7_fast_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                           RCONV_OUT_CH, 1, 1, 0, 0, RCONV_STRIDE_X,
                                           RCONV_STRIDE_Y, rconv_bias_q7, 1, 7, rconv_im_out_opt_q7, RCONV_OUT_DIM_X,
                                           RCONV_OUT_DIM_Y, rconv_buf, NULL);

    verify_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    printf("start depthwise separable conv q7 nonsquare ref implementation\n");
    arm_depthwise_separable_conv_HWC_q7_ref_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH,
                                                      rconv_weight_q7, RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y,
                                                      RCONV_PADDING_X, RCONV_PADDING_Y, RCONV_STRIDE_X, RCONV_STRIDE_Y,
                                                      rconv_bias_q7, 1, 7, rconv_im_out_ref_q7, RCONV_OUT_DIM_X,
                                                      RCONV_OUT_DIM_Y, rconv_buf, NULL);

    printf("start depthwise separable conv q7 nonsquare opt implementation\n");
    arm_depthwise_separable_conv_HWC_q7_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH,
                                                  rconv_weight_q7, RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y,
                                                  RCONV_PADDING_X, RCONV_PADDING_Y, RCONV_STRIDE_X, RCONV_STRIDE_Y,
                                                  rconv_bias_q7, 1, 7, rconv_im_out_opt_q7, RCONV_OUT_DIM_X,
                                                  RCONV_OUT_DIM_Y, rconv_buf, NULL);

    verify_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    delete[] NSE2025_VALVE1;
    delete[] NSE2025_VALVE2;
    delete[] NSE2025_VALVE3;

    NSE2025_VALVE2 = new q15_t[RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH + RCONV_OUT_CH];                          // weights + bias
    NSE2025_VALVE4 = new q15_t[2 * RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH                                                      // buffer
                               + RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH + 2 * RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH]; // i/o

    for (int i = 0; i < RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH + RCONV_OUT_CH; i++)
    {
        NSE2025_VALVE2[i] = rand() % 256 - 100;
    }

    for (int i = 0;
         i < 2 * RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH + RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH + 2 * RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH;
         i++)
    {
        NSE2025_VALVE4[i] = rand() % 256 - 100;
    }

    q15_t *rconv_weight_q15 = NSE2025_VALVE2;
    q15_t *rconv_bias_q15 = NSE2025_VALVE2 + RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH;

    rconv_buf = NSE2025_VALVE4;

    q15_t *rconv_im_in_q15 = NSE2025_VALVE4 + 2 * RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH;
    q15_t *rconv_im_out_ref_q15 = rconv_im_in_q15 + RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH;
    q15_t *rconv_im_out_opt_q15 = rconv_im_out_ref_q15 + RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH;

    initialize_results_q15(rconv_im_out_ref_q15, rconv_im_out_opt_q15, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    printf("start conv q15 nonsquare ref implementation\n");
    arm_convolve_HWC_q15_nonsquare_ref(rconv_im_in_q15, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q15,
                                       RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                       RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q15, 1, 7, rconv_im_out_ref_q15,
                                       RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);

    printf("start conv q5 nonsquare opt implementation\n");
    arm_convolve_HWC_q15_fast_nonsquare(rconv_im_in_q15, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q15,
                                        RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                        RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q15, 1, 7, rconv_im_out_opt_q15,
                                        RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);

    verify_results_q15(rconv_im_out_ref_q15, rconv_im_out_opt_q15, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    delete[] NSE2025_VALVE2;
    delete[] NSE2025_VALVE4;
#endif

#ifdef NSE2025_VALVE_CONV

#define CONV_IM_DIM 16
#define CONV_IM_CH 16
#define CONV_KER_DIM 5
#define CONV_OUT_CH 16
#define CONV_OUT_DIM 16

    NSE2025_VALVE1 = new q7_t[CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH + CONV_OUT_CH];
    NSE2025_VALVE2 =
        new q15_t[CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH +
                  2 * CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH + CONV_OUT_CH];
    NSE2025_VALVE3 = new q7_t[CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH + 2 * CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH];
    NSE2025_VALVE4 = new q15_t[CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH + 2 * CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH];

    for (int i = 0; i < CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH + CONV_OUT_CH; i++)
    {
        NSE2025_VALVE1[i] = rand() % 256 - 100;
    }

    for (int i = 0;
         i <
         CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH +
             2 * CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH + CONV_OUT_CH;
         i++)
    {
        NSE2025_VALVE2[i] = (rand() % 65536 - 32768);
    }

    for (int i = 0; i < CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH + 2 * CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH; i++)
    {
        NSE2025_VALVE3[i] = rand() % 256 - 100;
    }

    for (int i = 0; i < CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH + 2 * CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH; i++)
    {
        NSE2025_VALVE4[i] = (rand() % 65536 - 32768);
    }

    q7_t *conv_weight_q7 = NSE2025_VALVE1;
    q7_t *conv_bias_q7 = NSE2025_VALVE1 + CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH;

    q15_t *conv_weight_q15 = NSE2025_VALVE2;
    q15_t *conv_buf = NSE2025_VALVE2 + CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH;
    q15_t *conv_bias_q15 =
        NSE2025_VALVE2 + CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH +
        2 * CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH;

    q7_t *conv_im_in_q7 = NSE2025_VALVE3;
    q7_t *conv_im_out_ref_q7 = NSE2025_VALVE3 + CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH;
    q7_t *conv_im_out_opt_q7 =
        NSE2025_VALVE3 + CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH + CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH;

    q15_t *conv_im_in_q15 = NSE2025_VALVE4;
    q15_t *conv_im_out_ref_q15 = NSE2025_VALVE4 + CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH;
    q15_t *conv_im_out_opt_q15 =
        NSE2025_VALVE4 + CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH + CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH;

    initialize_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    printf("start q7 ref implementation\n");

    arm_convolve_HWC_q7_ref(conv_im_in_q7, CONV_IM_DIM, CONV_IM_CH, conv_weight_q7,
                            CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 7, conv_im_out_ref_q7,
                            CONV_OUT_DIM, conv_buf, NULL);

    printf("start q7 basic implementation\n");

    arm_convolve_HWC_q7_basic(conv_im_in_q7, CONV_IM_DIM, CONV_IM_CH, conv_weight_q7,
                              CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 7, conv_im_out_opt_q7,
                              CONV_OUT_DIM, conv_buf, NULL);

    verify_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    printf("start q7 fast implementation\n");

    arm_convolve_HWC_q7_fast(conv_im_in_q7, CONV_IM_DIM, CONV_IM_CH, conv_weight_q7,
                             CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 7, conv_im_out_opt_q7,
                             CONV_OUT_DIM, conv_buf, NULL);

    verify_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    // NSE2025_VALVEing with RGB
    printf("start q7 ref implementation for RGB\n");

    arm_convolve_HWC_q7_ref(conv_im_in_q7, CONV_IM_DIM, 3, conv_weight_q7,
                            CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 7, conv_im_out_ref_q7,
                            CONV_OUT_DIM, conv_buf, NULL);

    printf("start q7 basic implementation for RGB\n");

    arm_convolve_HWC_q7_basic(conv_im_in_q7, CONV_IM_DIM, 3, conv_weight_q7,
                              CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 7, conv_im_out_opt_q7,
                              CONV_OUT_DIM, conv_buf, NULL);

    verify_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    printf("start q7 RGB implementation for RGB\n");

    arm_convolve_HWC_q7_RGB(conv_im_in_q7, CONV_IM_DIM, 3, conv_weight_q7,
                            CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 7, conv_im_out_opt_q7,
                            CONV_OUT_DIM, conv_buf, NULL);

    verify_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    // NSE2025_VALVEing q15
    initialize_results_q15(conv_im_out_ref_q15, conv_im_out_opt_q15, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    printf("start q15 ref implementation\n");

    arm_convolve_HWC_q15_ref(conv_im_in_q15, CONV_IM_DIM, CONV_IM_CH, conv_weight_q15,
                             CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q15, 0, 15, conv_im_out_ref_q15,
                             CONV_OUT_DIM, conv_buf, NULL);

    printf("start q15 basic implementation\n");

    arm_convolve_HWC_q15_basic(conv_im_in_q15, CONV_IM_DIM, CONV_IM_CH, conv_weight_q15,
                               CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q15, 0, 15, conv_im_out_opt_q15,
                               CONV_OUT_DIM, conv_buf, NULL);

    verify_results_q15(conv_im_out_ref_q15, conv_im_out_opt_q15, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    printf("start q15 fast implementation\n");

    arm_convolve_HWC_q15_fast(conv_im_in_q15, CONV_IM_DIM, CONV_IM_CH, conv_weight_q15,
                              CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q15, 0, 15, conv_im_out_opt_q15,
                              CONV_OUT_DIM, conv_buf, NULL);

    verify_results_q15(conv_im_out_ref_q15, conv_im_out_opt_q15, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    // depthwise separable conv
    initialize_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    printf("start q7 depthwise_separable_conv ref implementation\n");

    arm_depthwise_separable_conv_HWC_q7_ref(conv_im_in_q7, CONV_IM_DIM, CONV_IM_CH, conv_weight_q7,
                                            CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 7, conv_im_out_ref_q7,
                                            CONV_OUT_DIM, conv_buf, NULL);

    printf("start q7 depthwise_separable_conv implementation\n");

    arm_depthwise_separable_conv_HWC_q7(conv_im_in_q7, CONV_IM_DIM, CONV_IM_CH, conv_weight_q7,
                                        CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 7, conv_im_out_opt_q7,
                                        CONV_OUT_DIM, conv_buf, NULL);

    verify_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    delete[] NSE2025_VALVE1;
    delete[] NSE2025_VALVE2;
    delete[] NSE2025_VALVE3;
    delete[] NSE2025_VALVE4;

#endif

    NSE2025_VALVE_pass = true;
    NSE2025_VALVE_index = 0;
    while (NSE2025_VALVE_flags[NSE2025_VALVE_index] != -1)
    {
        if (NSE2025_VALVE_flags[NSE2025_VALVE_index])
        {
            NSE2025_VALVE_pass = false;
        }
        NSE2025_VALVE_index++;
    }
    if (NSE2025_VALVE_pass)
    {
        printf("All NSE2025_VALVEs passed\n");
    }
    else
    {
        printf("NSE2025_VALVE failed passed\n");
    }

    return 0;
}
