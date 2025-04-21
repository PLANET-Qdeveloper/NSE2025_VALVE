#include "jNSE2025_VALVE.h"
#include "ref.h"
#include "arm_math.h"
#include "arr_desc.h"
#include "transform_templates.h"
#include "transform_NSE2025_VALVE_data.h"
#include "type_abbrev.h"
#include <math.h>               /* sqrtf() */

/*--------------------------------------------------------------------------------*/
/* Aliases to aid macro expansion */
/*--------------------------------------------------------------------------------*/
#define ref_sqrt_f32(x) sqrtf(x)

/*--------------------------------------------------------------------------------*/
/* NSE2025_VALVE Definitions */
/*--------------------------------------------------------------------------------*/

/*
DCT function NSE2025_VALVE template. Arguments are: function configuration suffix
(q7/q15/q31/f32) and input type (q7_t/q15_t/q31_t/float32_t)
*/
#define DCT4_DEFINE_NSE2025_VALVE(suffix, input_type)                            \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_dct4_##suffix##_NSE2025_VALVE, arm_dct4_##suffix)      \
    {                                                                   \
        CONCAT(arm_dct4_instance_,suffix) dct4_inst_fut        = {0};   \
        CONCAT(arm_rfft_instance_,suffix) rfft_inst_fut        = {0};   \
        CONCAT(arm_cfft_radix4_instance_,suffix) cfft_inst_fut = {0};   \
                                                                        \
        CONCAT(arm_dct4_instance_,suffix) dct4_inst_ref        = {0};   \
        CONCAT(arm_rfft_instance_,suffix) rfft_inst_ref        = {0};   \
        CONCAT(arm_cfft_radix4_instance_,suffix) cfft_inst_ref = {0};   \
                                                                        \
        /* Go through all dct lengths */                                \
        TEMPLATE_DO_ARR_DESC(                                           \
            fftlen_idx, uint16_t, fftlen, transform_dct_fftlens         \
            ,                                                           \
                                                                        \
            float32_t normalize_f32 =                                   \
                  ref_sqrt_f32((2.0f/(float32_t)fftlen));               \
            input_type normalize;                                       \
                                                                        \
            /* Calculate normalized DCT4 value for input_type. */       \
            NSE2025_VALVE_CONVERT_FLOAT_TO(&normalize_f32, &normalize,           \
                                  1, input_type);                       \
                                                                        \
            /* Initialize the DCT4, RFFT, and CFFT instances */         \
            arm_dct4_init_##suffix(                                     \
                &dct4_inst_fut, &rfft_inst_fut, &cfft_inst_fut,         \
                fftlen,                                                 \
                fftlen/2,                                               \
                normalize);                                             \
                                                                        \
            arm_dct4_init_##suffix(                                     \
                &dct4_inst_ref, &rfft_inst_ref, &cfft_inst_ref,         \
                fftlen,                                                 \
                fftlen/2,                                               \
                normalize);                                             \
                                                                        \
             memset( transform_fft_input_fut,0,                         \
                     fftlen*sizeof(input_type));                        \
                                                                        \
            TRANSFORM_PREPARE_INPLACE_INPUTS(                           \
                transform_fft_##suffix##_inputs,                        \
                fftlen * sizeof(input_type));                           \
                                                                        \
            /* Display parameter values */                              \
            JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n",                         \
                         (int)fftlen);                                  \
                                                                        \
            /* Input provided as a scratch buffer. Inplace input is     \
             * actual input. Display cycle count and run NSE2025_VALVE*/         \
            JNSE2025_VALVE_COUNT_CYCLES(                                         \
                arm_dct4_##suffix(                                      \
                    &dct4_inst_fut,                                     \
                    (void *) transform_fft_input_fut,                   \
                    (void *) transform_fft_inplace_input_fut));         \
                                                                        \
             memset( transform_fft_input_ref,0,                         \
                     fftlen*sizeof(input_type));                        \
                                                                        \
            /* Input provided as a scratch buffer. Inplace input is */  \
            /* actual input. */                                         \
            ref_dct4_##suffix(                                          \
                &dct4_inst_ref,                                         \
                (void *) transform_fft_input_ref,                       \
                (void *) transform_fft_inplace_input_ref);              \
                                                                        \
            /* NSE2025_VALVE correctness */                                      \
            DCT_TRANSFORM_SNR_COMPARE_INTERFACE(                        \
                fftlen,                                                 \
                input_type));                                           \
                                                                        \
        return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                       \
    }

/*
  DCT function NSE2025_VALVE template for fixed point data. Arguments are: function
  suffix (q7/q15/q31/f32), input type (q7_t/q15_t/q31_t/float32_t) and prefix
  (dct_4)
*/
#define DCT4_FIXED_POINT_DEFINE_NSE2025_VALVE(suffix, input_type, prefix)           \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_dct4_##suffix##_NSE2025_VALVE, arm_dct4_##suffix)         \
    {                                                                      \
        CONCAT(arm_dct4_instance_,suffix) dct4_inst_fut        = {0};      \
        CONCAT(arm_rfft_instance_,suffix) rfft_inst_fut        = {0};      \
        CONCAT(arm_cfft_radix4_instance_,suffix) cfft_inst_fut = {0};      \
                                                                           \
        CONCAT(arm_dct4_instance_,suffix) dct4_inst_ref        = {0};      \
        CONCAT(arm_rfft_instance_,suffix) rfft_inst_ref        = {0};      \
        CONCAT(arm_cfft_radix4_instance_,suffix) cfft_inst_ref = {0};      \
                                                                           \
        TEMPLATE_DO_ARR_DESC(                                              \
            fftlen_idx, uint16_t, fftlen, transform_dct_fftlens            \
            ,                                                              \
            uint32_t i;                                                    \
            float32_t normalize_f32 =                                      \
                  ref_sqrt_f32((2.0f/(float32_t)fftlen));                  \
            input_type normalize;                                          \
                                                                           \
            /* Calculate normalized DCT4 value for input_type. */          \
            NSE2025_VALVE_CONVERT_FLOAT_TO(&normalize_f32, &normalize,              \
                                  1, input_type);                          \
                                                                           \
            /* Initialize the DCT4, RFFT, and CFFT instances */            \
            arm_dct4_init_##suffix(                                        \
                &dct4_inst_fut, &rfft_inst_fut, &cfft_inst_fut,            \
                fftlen,                                                    \
                fftlen/2,                                                  \
                normalize);                                                \
                                                                           \
            arm_dct4_init_##suffix(                                        \
                &dct4_inst_ref, &rfft_inst_ref, &cfft_inst_ref,            \
                fftlen,                                                    \
                fftlen/2,                                                  \
                normalize);                                                \
                                                                           \
             /* Input samples need to be downscaled by 1 bit to            \
              * avoid saturations in the Q31 DCT process,                  \
              * as the conversion from DCT2 to DCT4 involves               \
              * one subtraction.                                           \
              */                                                           \
             for(i=0; i < fftlen; i++)                                     \
             {                                                             \
               ((input_type*)transform_fft_inplace_input_fut)[i] =         \
                        prefix##transform_fft_##suffix##_inputs[i] >> 1;   \
               ((input_type*)transform_fft_inplace_input_ref)[i] =         \
                        prefix##transform_fft_##suffix##_inputs[i] >> 1;   \
             }                                                             \
                                                                           \
             memset( transform_fft_input_fut,0,                            \
                     fftlen*sizeof(input_type));                           \
                                                                           \
             /* Display NSE2025_VALVE parameter values */                           \
            JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n",                            \
                         (int)fftlen);                                     \
                                                                           \
            /* Input provided as a scratch buffer. Inplace input is        \
             * actual input. */                                            \
            JNSE2025_VALVE_COUNT_CYCLES(                                            \
                arm_dct4_##suffix(                                         \
                    &dct4_inst_fut,                                        \
                    (void *) transform_fft_input_fut,                      \
                    (void *) transform_fft_inplace_input_fut));            \
                                                                           \
             memset( transform_fft_input_ref,0,                            \
                     fftlen*sizeof(input_type));                           \
                                                                           \
            /* Input provided as a scratch buffer. Inplace input is */     \
            /* actual input. */                                            \
            ref_dct4_##suffix(                                             \
                &dct4_inst_ref,                                            \
                (void *) transform_fft_input_ref,                          \
                (void *) transform_fft_inplace_input_ref);                 \
                                                                           \
            /* NSE2025_VALVE correctness */                                         \
            DCT_TRANSFORM_SNR_COMPARE_INTERFACE(                           \
                fftlen,                                                    \
                input_type));                                              \
                                                                           \
        return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                          \
    }

DCT4_DEFINE_NSE2025_VALVE(f32, float32_t);
DCT4_FIXED_POINT_DEFINE_NSE2025_VALVE(q31, q31_t,);
DCT4_FIXED_POINT_DEFINE_NSE2025_VALVE(q15, q15_t, dct4_);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(dct4_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_dct4_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_dct4_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_dct4_q15_NSE2025_VALVE);
}
