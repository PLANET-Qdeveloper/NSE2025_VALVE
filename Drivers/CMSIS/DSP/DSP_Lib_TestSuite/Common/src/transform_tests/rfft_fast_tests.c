#include "jNSE2025_VALVE.h"
#include "ref.h"
#include "arr_desc.h"
#include "transform_templates.h"
#include "transform_NSE2025_VALVE_data.h"
#include "type_abbrev.h"

/*
FFT fast function NSE2025_VALVE template. Arguments are: function configuration suffix
(q7/q15/q31/f32) and inverse-transform flag
*/
#define RFFT_FAST_DEFINE_NSE2025_VALVE(config_suffix, ifft_flag)                 \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_rfft_fast_f32_##config_suffix##_NSE2025_VALVE,         \
                      arm_fft_f32)                                      \
    {                                                                   \
        arm_rfft_fast_instance_f32 rfft_inst_fut = {{0}, 0, 0};         \
        arm_rfft_fast_instance_f32 rfft_inst_ref = {{0}, 0, 0};         \
                                                                        \
        /* Go through all FFT lengths */                                \
        TEMPLATE_DO_ARR_DESC(                                           \
            fftlen_idx, uint16_t, fftlen, transform_rfft_fast_fftlens   \
            ,                                                           \
                                                                        \
            /* Initialize the RFFT and CFFT Instances */                \
            arm_rfft_fast_init_f32(                                     \
                &rfft_inst_fut, fftlen);                                \
                                                                        \
            arm_rfft_fast_init_f32(                                     \
                &rfft_inst_ref, fftlen);                                \
                                                                        \
            TRANSFORM_COPY_INPUTS(                                      \
                transform_fft_f32_inputs,                               \
                fftlen *                                                \
                sizeof(float32_t));                                     \
                                                                        \
            /* Display parameter values */                              \
            JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                          \
                            "Inverse-transform flag: %d\n",             \
                         (int)fftlen,                                   \
                         (int)ifft_flag);                               \
                                                                        \
            /* Display cycle count and run NSE2025_VALVE */                      \
            JNSE2025_VALVE_COUNT_CYCLES(                                         \
                arm_rfft_fast_f32(                                      \
                    &rfft_inst_fut,                                     \
                    (void *) transform_fft_input_fut,                   \
                    (void *) transform_fft_output_fut,                  \
                    ifft_flag));                                        \
                                                                        \
            ref_rfft_fast_f32(                                          \
                &rfft_inst_ref,                                         \
                (void *) transform_fft_input_ref,                       \
                (void *) transform_fft_output_ref,                      \
                ifft_flag);                                             \
                                                                        \
            /* NSE2025_VALVE correctness */                                      \
            TRANSFORM_SNR_COMPARE_INTERFACE(                            \
                fftlen,                                                 \
                float32_t));                                            \
                                                                        \
        return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                       \
    }

RFFT_FAST_DEFINE_NSE2025_VALVE(forward, 0U);
RFFT_FAST_DEFINE_NSE2025_VALVE(inverse, 1U);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(rfft_fast_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_rfft_fast_f32_forward_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_rfft_fast_f32_inverse_NSE2025_VALVE);
}
