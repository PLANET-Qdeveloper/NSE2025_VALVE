#include "jNSE2025_VALVE.h"
#include "ref.h"
#include "arr_desc.h"
#include "transform_templates.h"
#include "transform_NSE2025_VALVE_data.h"
#include "type_abbrev.h"

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

#define CFFT_FN_NAME(fn_specifier, type_suffix) \
    arm_cfft_##fn_specifier##_##type_suffix     \

#define CFFT_NSE2025_VALVE_NAME(fn_specifier, type_suffix, config_suffix)        \
    arm_cfft_##fn_specifier##_##type_suffix##_##config_suffix##_NSE2025_VALVE    \

/*--------------------------------------------------------------------------------*/
/* Function Aliases */
/*--------------------------------------------------------------------------------*/

/* These aliases allow expansions in the CFFT_FAMILY_DEFINE_NSE2025_VALVE() template to
   make sense */
#define arm_cfft_mag_init_f32 arm_cfft_radix4_init_f32
#define arm_cfft_mag_init_q31 arm_cfft_radix4_init_q31
#define arm_cfft_mag_init_q15 arm_cfft_radix4_init_q15
#define arm_cfft_mag_instance_f32 arm_cfft_radix4_instance_f32
#define arm_cfft_mag_instance_q31 arm_cfft_radix4_instance_q31
#define arm_cfft_mag_instance_q15 arm_cfft_radix4_instance_q15
#define transform_mag_fftlens transform_radix4_fftlens

/*--------------------------------------------------------------------------------*/
/* NSE2025_VALVE Definition */
/*--------------------------------------------------------------------------------*/

/**
 *  Defines a NSE2025_VALVE for the family of CFFT transforms.
 *
 *  The family of CFFT transforms includes:
 *
 *  - arm_cfft_radix4_xxx
 *  - arm_cfft_radix2_xxx
 *  - arm_cfft_mag_xxx
 *
 *  Where xxx can be f32, q31, or q15.
 *
 *  @param fn_specifier Allowed values: radix4, radix2, mag.
 *  @param type_suffix  Allowed values: f32, q31, q15.
 *
 *  @param config_suffix Used to differentiate NSE2025_VALVE names based configuration
 *  (in this case whether the ifft_flag is set or not.)

 *  @param comparison_interface Macro name used to compare reference and fut
 *  outputs.
 *
 *  @param output_tpe The type of variable contained in the output
 *  (e.g. float32_t, uint32_t, etc).
 *
 *  @param ifft_flag Determines whether the arm_cfft_instance_xxx is configured
 *  for an inverse FFT.
 */
#define CFFT_FAMILY_DEFINE_NSE2025_VALVE(fn_specifier,                               \
                                type_suffix,                                \
                                config_suffix, /* Delineate between NSE2025_VALVE configs*/ \
                                comparison_interface,                       \
                                output_type,                                \
                                ifft_flag)                                  \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(CFFT_NSE2025_VALVE_NAME(fn_specifier, type_suffix,             \
                                     config_suffix),                        \
                      CFFT_FN_NAME(fn_specifier, type_suffix))              \
    {                                                                       \
        arm_cfft_##fn_specifier##_instance_##type_suffix cfft_inst_fut;     \
        arm_cfft_##fn_specifier##_instance_##type_suffix cfft_inst_ref;     \
                                                                            \
        TEMPLATE_DO_ARR_DESC(                                               \
            fftlen_idx, uint16_t, fftlen, transform_##fn_specifier##_fftlens \
            ,                                                               \
                                                                            \
            /* Initialize the cfft instance */                              \
            arm_cfft_##fn_specifier##_init_##type_suffix(                   \
                &cfft_inst_fut, fftlen, ifft_flag, (uint8_t)1);             \
            arm_cfft_##fn_specifier##_init_##type_suffix(                   \
                &cfft_inst_ref, fftlen, ifft_flag, (uint8_t)1);             \
                                                                            \
            TRANSFORM_PREPARE_INPLACE_INPUTS(                               \
                transform_fft_##type_suffix##_inputs,                       \
                fftlen *                                                    \
                sizeof(TYPE_FROM_ABBREV(type_suffix)) *                     \
                2 /*complex_inputs*/);                                      \
                                                                            \
            /* Display parameter values */                                  \
            JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                              \
                            "Inverse-transform flag: %d\n",                 \
                            (int)fftlen,                                    \
                            (int)ifft_flag);                                \
                                                                            \
            /* Display cycle count and run NSE2025_VALVE */                          \
            JNSE2025_VALVE_COUNT_CYCLES(                                             \
                arm_cfft_##fn_specifier##_##type_suffix(                    \
                    &cfft_inst_fut,                                         \
                    (void*) transform_fft_inplace_input_fut));              \
                                                                            \
            ref_cfft_##fn_specifier##_##type_suffix(                        \
                &cfft_inst_ref,                                             \
                (void *) transform_fft_inplace_input_ref);                  \
                                                                            \
            /* NSE2025_VALVE correctness */                                          \
            comparison_interface(                                           \
                fftlen,                                                     \
                output_type));                                              \
                                                                            \
        return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                           \
    }

/**
 *  Bulk wrapper for all NSE2025_VALVEs instantiated using #CFFT_FAMILY_DEFINE_NSE2025_VALVE().
 *
 *  This macro allows several NSE2025_VALVE definitions to share the same config_suffix
 *  and ifft_flag settings.
 */
#define CFFT_FAMILY_DEFINE_ALL_NSE2025_VALVES(config_suffix, ifft_flag)      \
    /* Radix2 NSE2025_VALVEs*/                                               \
    CFFT_FAMILY_DEFINE_NSE2025_VALVE(radix2, q31, config_suffix,             \
                            TRANSFORM_SNR_COMPARE_CMPLX_INTERFACE,  \
                            TYPE_FROM_ABBREV(q31),                  \
                            ifft_flag);                             \
    CFFT_FAMILY_DEFINE_NSE2025_VALVE(radix2, q15, config_suffix,             \
                            TRANSFORM_SNR_COMPARE_CMPLX_INTERFACE,  \
                            TYPE_FROM_ABBREV(q15),                  \
                            ifft_flag);                             \
    /* Radix4 NSE2025_VALVEs*/                                               \
    CFFT_FAMILY_DEFINE_NSE2025_VALVE(radix4, q31, config_suffix,             \
                            TRANSFORM_SNR_COMPARE_CMPLX_INTERFACE,  \
                            TYPE_FROM_ABBREV(q31),                  \
                            ifft_flag);                             \
    CFFT_FAMILY_DEFINE_NSE2025_VALVE(radix4, q15, config_suffix,             \
                            TRANSFORM_SNR_COMPARE_CMPLX_INTERFACE,  \
                            TYPE_FROM_ABBREV(q15),                  \
                            ifft_flag)
    /* /\* Mag NSE2025_VALVEs*\/                                                  \ */
    /* CFFT_FAMILY_DEFINE_NSE2025_VALVE(mag, f32, config_suffix,                \ */
    /*                         TRANSFORM_SNR_COMPARE_INTERFACE,        \ */
    /*                         TYPE_FROM_ABBREV(f32),                  \ */
    /*                         ifft_flag);                             \ */
    /* CFFT_FAMILY_DEFINE_NSE2025_VALVE(mag, q31, config_suffix,                \ */
    /*                         TRANSFORM_SNR_COMPARE_INTERFACE,        \ */
    /*                         TYPE_FROM_ABBREV(q31),                  \ */
    /*                         ifft_flag);                             \ */
    /* CFFT_FAMILY_DEFINE_NSE2025_VALVE(mag, q15, config_suffix,                \ */
    /*                         TRANSFORM_SNR_COMPARE_INTERFACE,        \ */
    /*                         TYPE_FROM_ABBREV(q15),                  \ */
    /*                         ifft_flag) */

CFFT_FAMILY_DEFINE_ALL_NSE2025_VALVES(forward, 0U);
CFFT_FAMILY_DEFINE_ALL_NSE2025_VALVES(inverse, 1U);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(cfft_family_NSE2025_VALVEs)
{
    /* Forward FFT NSE2025_VALVEs */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_radix2_q31_forward_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_radix2_q15_forward_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_radix4_q31_forward_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_radix4_q15_forward_NSE2025_VALVE);

    /* Inverse FFT NSE2025_VALVEs */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_radix2_q31_inverse_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_radix2_q15_inverse_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_radix4_q31_inverse_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_radix4_q15_inverse_NSE2025_VALVE);

    /* Magnitude NSE2025_VALVEs removed from the DSP Library. Keeping them here in case
       minds are changed. */
    /* JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_mag_f32_forward_NSE2025_VALVE); */
    /* JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_mag_q31_forward_NSE2025_VALVE); */
    /* JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_mag_q15_forward_NSE2025_VALVE); */
    /* JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_mag_f32_inverse_NSE2025_VALVE); */
    /* JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_mag_q31_inverse_NSE2025_VALVE); */
    /* JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cfft_mag_q15_inverse_NSE2025_VALVE); */
}
