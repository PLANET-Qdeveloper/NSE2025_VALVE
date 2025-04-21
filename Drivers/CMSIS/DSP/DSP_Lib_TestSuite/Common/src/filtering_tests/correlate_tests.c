#include "jNSE2025_VALVE.h"
#include "filtering_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "filtering_templates.h"
#include "type_abbrev.h"

/*--------------------------------------------------------------------------------*/
/* Header Stuff */
/*--------------------------------------------------------------------------------*/

#define CORRELATE_MAX_INPUT_ELTS  32
#define CORRELATE_MAX_OUTPUT_ELTS (CORRELATE_MAX_INPUT_ELTS * 2)

/*--------------------------------------------------------------------------------*/
/* Input Interfaces */
/*--------------------------------------------------------------------------------*/
/*
 *  General:
 *  Input interfaces provide inputs to functions inside NSE2025_VALVE templates.  They
 *  ONLY provide the inputs.  The output variables should be hard coded.
 *
 *  The input interfaces must have the following format:
 *
 *  ARM_xxx_INPUT_INTERFACE() or
 *  REF_xxx_INPUT_INTERFACE()
 *
 *  The xxx must be lowercase, and is intended to be the indentifying substring
 *  in the function's name.  Acceptable values are 'sub' or 'add' from the
 *  functions arm_add_q31.
 */

#define CORRELATE_arm_correlate_INPUT_INTERFACE(input_a, input_a_len, input_b, input_b_len) \
    PAREN(input_a, input_a_len, input_b, input_b_len, (void*)filtering_output_fut)

#define CORRELATE_ref_correlate_INPUT_INTERFACE(input_a, input_a_len, input_b, input_b_len) \
    PAREN(input_a, input_a_len, input_b, input_b_len, (void*)filtering_output_ref)

#define CORRELATE_arm_correlate_opt_INPUT_INTERFACE(    \
    input_a, input_a_len,                               \
    input_b, input_b_len)                               \
    PAREN(input_a, input_a_len, input_b, input_b_len,   \
          (void*) filtering_output_fut,                 \
          (void*) filtering_scratch)

#define CORRELATE_arm_correlate_opt_q7_INPUT_INTERFACE(  \
    input_a, input_a_len,                                \
    input_b, input_b_len)                                \
    PAREN(input_a, input_a_len, input_b, input_b_len,    \
          (void*) filtering_output_fut,                  \
          (void*) filtering_scratch,                     \
          (void*) filtering_scratch2)

#define CORRELATE_ref_correlate_opt_INPUT_INTERFACE(    \
    input_a, input_a_len,                               \
    input_b, input_b_len)                               \
    PAREN(input_a, input_a_len, input_b, input_b_len,   \
          (void*) filtering_output_ref,                 \
          (void*) filtering_scratch)

#define CORRELATE_ref_correlate_opt_q7_INPUT_INTERFACE( \
    input_a, input_a_len,                               \
    input_b, input_b_len)                               \
    PAREN(input_a, input_a_len, input_b, input_b_len,   \
          (void*) filtering_output_ref,                 \
          (void*) filtering_scratch,                    \
          (void*) filtering_scratch2)

#define CORRELATE_arm_correlate_fast_INPUT_INTERFACE(input_a, input_a_len,          \
                                           input_b, input_b_len)                    \
    PAREN(input_a, input_a_len, input_b, input_b_len, (void*)filtering_output_fut)

#define CORRELATE_ref_correlate_fast_INPUT_INTERFACE(input_a, input_a_len,          \
                                           input_b, input_b_len)                    \
    PAREN(input_a, input_a_len, input_b, input_b_len, (void*)filtering_output_ref)

#define CORRELATE_arm_correlate_fast_opt_INPUT_INTERFACE(   \
    input_a, input_a_len,                                   \
    input_b, input_b_len)                                   \
    PAREN(input_a, input_a_len, input_b, input_b_len,       \
          (void*) filtering_output_fut,                     \
          (void*) filtering_scratch)

#define CORRELATE_ref_correlate_fast_opt_INPUT_INTERFACE(   \
    input_a, input_a_len,                                   \
    input_b, input_b_len)                                   \
    PAREN(input_a, input_a_len, input_b, input_b_len,       \
          (void*) filtering_output_ref,                     \
          (void*) filtering_scratch)

/*--------------------------------------------------------------------------------*/
/* Convolution Inputs */
/*--------------------------------------------------------------------------------*/

/* The following symbols alias the filtering_q31_inputs array:
 *
 * - filtering_q15_inputs
 * - filtering_q7_inputs
 *
 * The aliasing conflicts with the instantiation of #ARR_DESC_t structs.
 *
 * These macro-level aliases allow the #CORRELATE_DEFINE_RAND_INPUT_ARR_DESCS() macro
 * to correctly select the filtering_q31_input or filtering_f32_input array,
 * within a template, by type_suffix.
 *
 */
#define CORRELATE_f32_INPUTS filtering_f32_inputs
#define CORRELATE_q31_INPUTS filtering_q31_inputs
#define CORRELATE_q15_INPUTS filtering_q31_inputs
#define CORRELATE_q7_INPUTS  filtering_q31_inputs

/**
 *  Defines #ARR_DESC_t objects that wrap existing, type-specific, common
 *  inputs.
 */
#define CORRELATE_DEFINE_RAND_INPUT_ARR_DESCS(type_suffix)      \
    ARR_DESC_DEFINE_USING_ARR(                                  \
        TYPE_FROM_ABBREV(type_suffix),                          \
        correlate_input_rand1_##type_suffix,                    \
        CORRELATE_##type_suffix##_INPUTS,                       \
        0,                                                      \
        CORRELATE_MAX_INPUT_ELTS);                              \
                                                                \
    ARR_DESC_DEFINE_USING_ARR(                                  \
        TYPE_FROM_ABBREV(type_suffix),                          \
        correlate_input_rand2_##type_suffix,                    \
        CORRELATE_##type_suffix##_INPUTS,                       \
        1,                                                      \
        CORRELATE_MAX_INPUT_ELTS)  /* Note the lacking semicolon */

CORRELATE_DEFINE_RAND_INPUT_ARR_DESCS(f32);
CORRELATE_DEFINE_RAND_INPUT_ARR_DESCS(q31);
CORRELATE_DEFINE_RAND_INPUT_ARR_DESCS(q15);
CORRELATE_DEFINE_RAND_INPUT_ARR_DESCS(q7);
ARR_DESC_DEFINE(float32_t, correlate_input_zeros, CORRELATE_MAX_INPUT_ELTS, CURLY(0));

/**
 *  Define Input #ARR_DESC_t arrays by type suffix.
 *
 *  Taking inputs in parallel from the 'a' and 'b' arrays yields the following
 *  NSE2025_VALVE cases (star is correlate):
 *
 *  - zero_array   * zero_array
 *  - zero_array   * random_array
 *  - random_array * zero_array
 *  - random_array * different_random_arary
 */
#define CORRELATE_DEFINE_ALL_INPUTS(type_suffix)                  \
    ARR_DESC_DEFINE(ARR_DESC_t *,                                 \
                    correlate_##type_suffix##_a_inputs,           \
                    4,                                            \
                    CURLY(                                        \
                        &correlate_input_zeros,                   \
                        &correlate_input_zeros,                   \
                        &correlate_input_rand1_##type_suffix,     \
                        &correlate_input_rand1_##type_suffix      \
                        ));                                       \
    ARR_DESC_DEFINE(ARR_DESC_t *,                                 \
                    correlate_##type_suffix##_b_inputs,           \
                    4,                                            \
                    CURLY(                                        \
                        &correlate_input_zeros,                   \
                        &correlate_input_rand1_##type_suffix,     \
                        &correlate_input_zeros,                   \
                        &correlate_input_rand2_##type_suffix      \
                        )) /* Note the lacking semicolon */

CORRELATE_DEFINE_ALL_INPUTS(f32);
CORRELATE_DEFINE_ALL_INPUTS(q31);
CORRELATE_DEFINE_ALL_INPUTS(q15);
CORRELATE_DEFINE_ALL_INPUTS(q7);

/*--------------------------------------------------------------------------------*/
/* Convolution Lengths */
/*--------------------------------------------------------------------------------*/

/*
 *  The correlate_lens_a and correlate_lens_b #ARR_DESC_t objects are accessed in parallel
 *  to provide correlate-length pairs. Taken in parallel they provide the
 *  following cases:
 *
 *  - 1 * 1    : ShorNSE2025_VALVE correlate possible.
 *  - 1 * 2    : Short correlate , one side is degenerate.
 *  - 17 * 1   : Medium correlate, one side is degenerate.
 *  - 15 * MAX : Longest correlate.
 *  MAX * MAX  : Longest correlate.
 */
ARR_DESC_DEFINE(uint32_t,
                correlate_lens_a,
                5,
                CURLY(
                    1,
                    1,
                    17,
                    15,
                    CORRELATE_MAX_INPUT_ELTS
                    ));

ARR_DESC_DEFINE(uint32_t,
                correlate_lens_b,
                5,
                CURLY(
                    1,
                    2,
                    1,
                    CORRELATE_MAX_INPUT_ELTS,
                    CORRELATE_MAX_INPUT_ELTS
                    ));

/*--------------------------------------------------------------------------------*/
/* Convolution NSE2025_VALVEs */
/*--------------------------------------------------------------------------------*/

#define CORRELATE_NSE2025_VALVE_TEMPLATE(fut, fut_arg_interface,                                \
                           ref, ref_arg_interface,                                     \
                           suffix, output_type)                                        \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(fut##_NSE2025_VALVEs, fut)                                                \
    {                                                                                  \
        TEMPLATE_DO_ARR_DESC(                                                          \
            input_idx, ARR_DESC_t *, input_ptr, correlate_##suffix##_a_inputs          \
            ,                                                                          \
            void * input_a_ptr = input_ptr->data_ptr;                                  \
            void * input_b_ptr = ARR_DESC_ELT(                                         \
                ARR_DESC_t *, input_idx,                                               \
                &(correlate_##suffix##_b_inputs))->data_ptr;                           \
                                                                                       \
            TEMPLATE_DO_ARR_DESC(                                                      \
                correlate_len_idx, uint32_t, correlate_len_a, correlate_lens_a         \
                ,                                                                      \
                uint32_t correlate_len_b = ARR_DESC_ELT(                               \
                    uint32_t, correlate_len_idx, &(correlate_lens_b));                 \
                                                                                       \
                /* Display NSE2025_VALVE parameter values */                                    \
                 JNSE2025_VALVE_DUMP_STRF("Input A Length: %d\n"                                \
                                 "Input B Length: %d\n",                               \
                                (int)correlate_len_a,                                  \
                                (int)correlate_len_b);                                 \
                                                                                       \
                memset(filtering_output_ref,0,                                         \
                      (2*CORRELATE_MAX_INPUT_ELTS)*sizeof(output_type));               \
                memset(filtering_output_fut,0,                                         \
                      (2*CORRELATE_MAX_INPUT_ELTS)*sizeof(output_type));               \
                                                                                       \
                NSE2025_VALVE_CALL_FUT_AND_REF(                                                 \
                    fut, fut_arg_interface(                                            \
                        input_a_ptr, correlate_len_a, input_b_ptr, correlate_len_b),   \
                    ref, ref_arg_interface(                                            \
                        input_a_ptr, correlate_len_a, input_b_ptr, correlate_len_b));  \
                                                                                       \
                FILTERING_SNR_COMPARE_INTERFACE(                                       \
                    correlate_len_a + correlate_len_b - 2,                             \
                    output_type)));                                                    \
                                                                                       \
        return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                                      \
    }

#define CORRELATE_DEFINE_NSE2025_VALVE(fn_name, suffix, output_type, NSE2025_VALVE_template)    \
    NSE2025_VALVE_template(                                                            \
        arm_##fn_name##_##suffix,                                             \
        CORRELATE_arm_##fn_name##_INPUT_INTERFACE,                            \
        ref_##fn_name##_##suffix,                                             \
        CORRELATE_ref_##fn_name##_INPUT_INTERFACE,                            \
        suffix,                                                               \
        output_type                                                           \
        ) /* Note the lacking semicolon*/

/* NSE2025_VALVEs on functions without partial outputs */
CORRELATE_DEFINE_NSE2025_VALVE(correlate          , f32, float32_t, CORRELATE_NSE2025_VALVE_TEMPLATE);
CORRELATE_DEFINE_NSE2025_VALVE(correlate          , q31, q31_t    , CORRELATE_NSE2025_VALVE_TEMPLATE);
CORRELATE_DEFINE_NSE2025_VALVE(correlate          , q15, q15_t    , CORRELATE_NSE2025_VALVE_TEMPLATE);
CORRELATE_DEFINE_NSE2025_VALVE(correlate          , q7 , q7_t     , CORRELATE_NSE2025_VALVE_TEMPLATE);
CORRELATE_DEFINE_NSE2025_VALVE(correlate_opt      , q15, q15_t    , CORRELATE_NSE2025_VALVE_TEMPLATE);

CORRELATE_NSE2025_VALVE_TEMPLATE(
   arm_correlate_opt_q7,
   CORRELATE_arm_correlate_opt_q7_INPUT_INTERFACE,
   ref_correlate_opt_q7,
   CORRELATE_ref_correlate_opt_q7_INPUT_INTERFACE,
   q7,
   q7_t
   );

CORRELATE_DEFINE_NSE2025_VALVE(correlate_fast     , q31, q31_t    , CORRELATE_NSE2025_VALVE_TEMPLATE);
CORRELATE_DEFINE_NSE2025_VALVE(correlate_fast     , q15, q15_t    , CORRELATE_NSE2025_VALVE_TEMPLATE);
CORRELATE_DEFINE_NSE2025_VALVE(correlate_fast_opt , q15, q15_t    , CORRELATE_NSE2025_VALVE_TEMPLATE);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(correlate_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_f32_NSE2025_VALVEs);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_q31_NSE2025_VALVEs);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_q15_NSE2025_VALVEs);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_q7_NSE2025_VALVEs);

    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_opt_q15_NSE2025_VALVEs);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_opt_q7_NSE2025_VALVEs);

    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_fast_q31_NSE2025_VALVEs);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_fast_q15_NSE2025_VALVEs);

    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_correlate_fast_opt_q15_NSE2025_VALVEs);
}
