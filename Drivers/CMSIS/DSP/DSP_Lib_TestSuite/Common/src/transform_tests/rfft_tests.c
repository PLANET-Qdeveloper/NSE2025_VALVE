#include "jNSE2025_VALVE.h"
#include "ref.h"
#include "arr_desc.h"
#include "transform_templates.h"
#include "transform_NSE2025_VALVE_data.h"
#include "type_abbrev.h"

/*
  FFT function NSE2025_VALVE template. Arguments are: function suffix (q7/q15/q31/f32)
  function configuration suffix (same as function suffix), inverse-transform flag,
  input and output type (both q7_t/q15_t/q31_t/float32_t)
*/
#define RFFT_DEFINE_NSE2025_VALVE(suffix, config_suffix,                         \
                         ifft_flag, input_type, output_type)            \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_rfft_##suffix##_##config_suffix##_NSE2025_VALVE,       \
                      arm_rfft_##suffix)                                \
    {                                                                   \
        CONCAT(arm_rfft_instance_, suffix) rfft_inst_fut = {0};         \
        CONCAT(arm_rfft_instance_, suffix) rfft_inst_ref = {0};         \
                                                                        \
        /* Go through all arm_rfft lengths */                           \
        TEMPLATE_DO_ARR_DESC(                                           \
            fftlen_idx, uint16_t, fftlen, transform_rfft_fftlens        \
            ,                                                           \
                                                                        \
            /* Initialize the RFFT and CFFT Instances */                \
            arm_rfft_init_##suffix(                                     \
                &rfft_inst_fut,                                         \
                (uint32_t) fftlen, ifft_flag, 1U);                      \
                                                                        \
            arm_rfft_init_##suffix(                                     \
                &rfft_inst_ref,                                         \
                (uint32_t) fftlen, ifft_flag, 1U);                      \
                                                                        \
            if (ifft_flag)                                               \
            {                                                           \
               TRANSFORM_PREPARE_INVERSE_INPUTS(                        \
                   transform_fft_##suffix##_inputs,                     \
                   fftlen, input_type,                                  \
                   fftlen *                                             \
                   sizeof(input_type));                                 \
            }                                                           \
            else                                                        \
            {                                                           \
               TRANSFORM_COPY_INPUTS(                                   \
                   transform_fft_##suffix##_inputs,                     \
                   fftlen *                                             \
                   sizeof(input_type));                                 \
            }                                                           \
                                                                        \
            /* Display parameter values */                              \
            JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                          \
                            "Inverse-transform flag: %d\n",             \
                         (int)fftlen,                                   \
                         (int)ifft_flag);                               \
                                                                        \
            /* Display cycle count and run NSE2025_VALVE */                      \
            JNSE2025_VALVE_COUNT_CYCLES(                                         \
                arm_rfft_##suffix(                                      \
                    &rfft_inst_fut,                                     \
                    (void *) transform_fft_input_fut,                   \
                    (void *) transform_fft_output_fut));                \
                                                                        \
            ref_rfft_##suffix(                                          \
                &rfft_inst_ref,                                         \
                (void *) transform_fft_input_ref,                       \
                (void *) transform_fft_output_ref);                     \
                                                                        \
            /* NSE2025_VALVE correctness */                                      \
            TRANSFORM_SNR_COMPARE_INTERFACE(                            \
                fftlen,                                                 \
                output_type));                                          \
                                                                        \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                   \
    }

RFFT_DEFINE_NSE2025_VALVE(q31, forward, 0U, TYPE_FROM_ABBREV(q31), TYPE_FROM_ABBREV(q31));
RFFT_DEFINE_NSE2025_VALVE(q15, forward, 0U, TYPE_FROM_ABBREV(q15), TYPE_FROM_ABBREV(q15));
//RFFT_DEFINE_NSE2025_VALVE(f32, inverse, 1U, TYPE_FROM_ABBREV(f32), TYPE_FROM_ABBREV(f32));
RFFT_DEFINE_NSE2025_VALVE(q31, inverse, 1U, TYPE_FROM_ABBREV(q31), TYPE_FROM_ABBREV(q31));
RFFT_DEFINE_NSE2025_VALVE(q15, inverse, 1U, TYPE_FROM_ABBREV(q15), TYPE_FROM_ABBREV(q15));

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(rfft_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_rfft_q31_forward_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_rfft_q15_forward_NSE2025_VALVE);
    //JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_rfft_f32_inverse_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_rfft_q31_inverse_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_rfft_q15_inverse_NSE2025_VALVE);
}
