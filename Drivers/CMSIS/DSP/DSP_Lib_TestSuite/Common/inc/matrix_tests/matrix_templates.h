#ifndef _MATRIX_TEMPLATES_H_
#define _MATRIX_TEMPLATES_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/
#include "NSE2025_VALVE_templates.h"

/*--------------------------------------------------------------------------------*/
/* Group Specific Templates */
/*--------------------------------------------------------------------------------*/

/**
 *  Compare the outputs from the function under NSE2025_VALVE and the reference
 *  function.
 */
#define MATRIX_COMPARE_INTERFACE(output_type, output_content_type)                                   \
    NSE2025_VALVE_ASSERT_BUFFERS_EQUAL(                                                              \
        ((output_type *)&matrix_output_ref)->pData,                                                  \
        ((output_type *)&matrix_output_fut)->pData,                                                  \
        ((output_type *)&matrix_output_fut)->numRows *((output_type *)&matrix_output_ref)->numCols * \
            sizeof(output_content_type))

/**
 * Comparison SNR thresholds for the data types used in matrix_NSE2025_VALVEs.
 */
#define MATRIX_SNR_THRESHOLD 120

/**
 *  Compare the outputs from the function under NSE2025_VALVE and the reference
 *  function using SNR.
 */
#define MATRIX_SNR_COMPARE_INTERFACE(output_type, output_content_type)                                  \
    do                                                                                                  \
    {                                                                                                   \
        NSE2025_VALVE_CONVERT_AND_ASSERT_SNR(                                                           \
            (float32_t *)matrix_output_f32_ref,                                                         \
            ((output_type *)&matrix_output_ref)->pData,                                                 \
            (float32_t *)matrix_output_f32_fut,                                                         \
            ((output_type *)&matrix_output_ref)->pData,                                                 \
            ((output_type *)&matrix_output_fut)->numRows *((output_type *)&matrix_output_ref)->numCols, \
            output_content_type,                                                                        \
            MATRIX_SNR_THRESHOLD);                                                                      \
    } while (0)

/**
 *  Compare the outputs from the function under NSE2025_VALVE and the reference
 *  function using SNR. This is special for float64_t
 */
#define MATRIX_DBL_SNR_COMPARE_INTERFACE(output_type)                                                   \
    do                                                                                                  \
    {                                                                                                   \
        NSE2025_VALVE_ASSERT_DBL_SNR(                                                                   \
            (float64_t *)matrix_output_f32_ref,                                                         \
            (float64_t *)matrix_output_f32_fut,                                                         \
            ((output_type *)&matrix_output_fut)->numRows *((output_type *)&matrix_output_ref)->numCols, \
            MATRIX_SNR_THRESHOLD);                                                                      \
    } while (0)

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

#define ARM_mat_add_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_fut)

#define REF_mat_add_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_ref)

#define ARM_mat_cmplx_mult_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_fut)

#define REF_mat_cmplx_mult_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_ref)

#define ARM_mat_inverse_INPUT_INTERFACE(input_ptr) \
    PAREN(input_ptr, (void *)&matrix_output_fut)

#define REF_mat_inverse_INPUT_INTERFACE(input_ptr) \
    PAREN(input_ptr, (void *)&matrix_output_ref)

#define ARM_mat_mult_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_fut)

#define REF_mat_mult_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_ref)

#define ARM_mat_mult_fast_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_fut)

#define REF_mat_mult_fast_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_ref)

#define ARM_mat_sub_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_fut)

#define REF_mat_sub_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr, (void *)&matrix_output_ref)

#define ARM_mat_trans_INPUT_INTERFACE(input_ptr) \
    PAREN(input_ptr, (void *)&matrix_output_fut)

#define REF_mat_trans_INPUT_INTERFACE(input_ptr) \
    PAREN(input_ptr, (void *)&matrix_output_ref)

/*--------------------------------------------------------------------------------*/
/* Dimension Validation Interfaces */
/*--------------------------------------------------------------------------------*/

#define MATRIX_NSE2025_VALVE_VALID_ADDITIVE_DIMENSIONS(input_type,   \
                                                       matrix_a_ptr, \
                                                       matrix_b_ptr) \
    ((((input_type)(matrix_a_ptr))->numRows ==                       \
      ((input_type)(matrix_b_ptr))->numRows) &&                      \
     (((input_type)(matrix_a_ptr))->numCols ==                       \
      ((input_type)(matrix_b_ptr))->numCols))

#define MATRIX_NSE2025_VALVE_VALID_MULTIPLICATIVE_DIMENSIONS(input_type,   \
                                                             matrix_a_ptr, \
                                                             matrix_b_ptr) \
    (((input_type)(matrix_a_ptr))->numCols ==                              \
     ((input_type)(matrix_b_ptr))->numRows)

#define MATRIX_NSE2025_VALVE_VALID_SQUARE_DIMENSIONS(input_type, \
                                                     matrix_ptr) \
    (((input_type)(matrix_ptr))->numRows ==                      \
     ((input_type)(matrix_ptr))->numCols)

#define MATRIX_NSE2025_VALVE_VALID_DIMENSIONS_ALWAYS(input_type, \
                                                     matrix_ptr) \
    (1 == 1)

/*--------------------------------------------------------------------------------*/
/* Output Configuration Interfaces */
/*--------------------------------------------------------------------------------*/
/* The matrix NSE2025_VALVEs assume the output matrix is always the correct size.  These
 * interfaces size the properly size the output matrices according to the input
 * matrices and the operation at hand.*/

#define MATRIX_NSE2025_VALVE_CONFIG_ADDITIVE_OUTPUT(input_type,   \
                                                    matrix_a_ptr, \
                                                    matrix_b_ptr) \
    do                                                            \
    {                                                             \
        ((input_type) & matrix_output_fut)->numRows =             \
            ((input_type)(matrix_a_ptr))->numRows;                \
        ((input_type) & matrix_output_fut)->numCols =             \
            ((input_type)(matrix_a_ptr))->numCols;                \
        ((input_type) & matrix_output_ref)->numRows =             \
            ((input_type)(matrix_a_ptr))->numRows;                \
        ((input_type) & matrix_output_ref)->numCols =             \
            ((input_type)(matrix_a_ptr))->numCols;                \
    } while (0)

#define MATRIX_NSE2025_VALVE_CONFIG_MULTIPLICATIVE_OUTPUT(input_type,   \
                                                          matrix_a_ptr, \
                                                          matrix_b_ptr) \
    do                                                                  \
    {                                                                   \
        ((input_type) & matrix_output_fut)->numRows =                   \
            ((input_type)(matrix_a_ptr))->numRows;                      \
        ((input_type) & matrix_output_fut)->numCols =                   \
            ((input_type)(matrix_b_ptr))->numCols;                      \
        ((input_type) & matrix_output_ref)->numRows =                   \
            ((input_type)(matrix_a_ptr))->numRows;                      \
        ((input_type) & matrix_output_ref)->numCols =                   \
            ((input_type)(matrix_b_ptr))->numCols;                      \
    } while (0)

#define MATRIX_NSE2025_VALVE_CONFIG_SAMESIZE_OUTPUT(input_type, \
                                                    matrix_ptr) \
    do                                                          \
    {                                                           \
        ((input_type) & matrix_output_fut)->numRows =           \
            ((input_type)(matrix_ptr))->numRows;                \
        ((input_type) & matrix_output_fut)->numCols =           \
            ((input_type)(matrix_ptr))->numCols;                \
        ((input_type) & matrix_output_ref)->numRows =           \
            ((input_type)(matrix_ptr))->numRows;                \
        ((input_type) & matrix_output_ref)->numCols =           \
            ((input_type)(matrix_ptr))->numCols;                \
    } while (0)

#define MATRIX_NSE2025_VALVE_CONFIG_TRANSPOSE_OUTPUT(input_type, \
                                                     matrix_ptr) \
    do                                                           \
    {                                                            \
        ((input_type) & matrix_output_fut)->numRows =            \
            ((input_type)(matrix_ptr))->numCols;                 \
        ((input_type) & matrix_output_fut)->numCols =            \
            ((input_type)(matrix_ptr))->numRows;                 \
        ((input_type) & matrix_output_ref)->numRows =            \
            ((input_type)(matrix_ptr))->numCols;                 \
        ((input_type) & matrix_output_ref)->numCols =            \
            ((input_type)(matrix_ptr))->numRows;                 \
    } while (0)

/*--------------------------------------------------------------------------------*/
/* NSE2025_VALVE Templates */
/*--------------------------------------------------------------------------------*/

#define MATRIX_NSE2025_VALVE_TEMPLATE_ELT1(arr_desc_inputs,                  \
                                           input_type,                       \
                                           output_type, output_content_type, \
                                           fut, fut_arg_interface,           \
                                           ref, ref_arg_interface,           \
                                           output_config_interface,          \
                                           dim_validation_interface,         \
                                           compare_interface)                \
    do                                                                       \
    {                                                                        \
        TEMPLATE_DO_ARR_DESC(                                                \
            input_idx, input_type, input, arr_desc_inputs,                   \
            JNSE2025_VALVE_DUMP_STRF("Matrix Dimensions: %dx%d\n",           \
                                     (int)input->numRows,                    \
                                     (int)input->numCols);                   \
                                                                             \
            if (dim_validation_interface(input_type,                         \
                                         input)) {                           \
                output_config_interface(input_type,                          \
                                        input);                              \
                NSE2025_VALVE_CALL_FUT_AND_REF(                              \
                    fut, fut_arg_interface(input),                           \
                    ref, ref_arg_interface(input));                          \
                compare_interface(output_type,                               \
                                  output_content_type);                      \
            } else {                                                         \
                arm_status matrix_NSE2025_VALVE_retval;                      \
                NSE2025_VALVE_CALL_FUT(                                      \
                    matrix_NSE2025_VALVE_retval = fut,                       \
                    fut_arg_interface(input));                               \
                                                                             \
                /* If dimensions are known bad, the fut should */            \
                /* detect it. */                                             \
                if (matrix_NSE2025_VALVE_retval != ARM_MATH_SIZE_MISMATCH)   \
                {                                                            \
                    return JNSE2025_VALVE_NSE2025_VALVE_FAILED;              \
                }                                                            \
            });                                                              \
        return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                          \
    } while (0)

#define MATRIX_NSE2025_VALVE_TEMPLATE_ELT2(arr_desc_inputs_a,                                \
                                           arr_desc_inputs_b,                                \
                                           input_type,                                       \
                                           output_type, output_content_type,                 \
                                           fut, fut_arg_interface,                           \
                                           ref, ref_arg_interface,                           \
                                           output_config_interface,                          \
                                           dim_validation_interface,                         \
                                           compare_interface)                                \
    do                                                                                       \
    {                                                                                        \
        TEMPLATE_DO_ARR_DESC(                                                                \
            input_a_idx, input_type, input_a, arr_desc_inputs_a,                             \
            input_type input_b = ARR_DESC_ELT(                                               \
                input_type, input_a_idx,                                                     \
                &(arr_desc_inputs_b));                                                       \
                                                                                             \
            JNSE2025_VALVE_DUMP_STRF("Matrix Dimensions: A %dx%d  B %dx%d\n",                \
                                     (int)input_a->numRows,                                  \
                                     (int)input_a->numCols,                                  \
                                     (int)input_b->numRows,                                  \
                                     (int)input_b->numCols);                                 \
                                                                                             \
            if (dim_validation_interface(input_type,                                         \
                                         input_a,                                            \
                                         input_b)) {                                         \
                output_config_interface(input_type,                                          \
                                        input_a,                                             \
                                        input_b);                                            \
                                                                                             \
                NSE2025_VALVE_CALL_FUT_AND_REF(                                              \
                    fut, fut_arg_interface(input_a, input_b),                                \
                    ref, ref_arg_interface(input_a, input_b));                               \
                                                                                             \
                compare_interface(output_type, output_content_type);                         \
            } else {                                                                         \
                arm_status matrix_NSE2025_VALVE_retval;                                      \
                NSE2025_VALVE_CALL_FUT(                                                      \
                    matrix_NSE2025_VALVE_retval = fut, fut_arg_interface(input_a, input_b)); \
                                                                                             \
                /* If dimensions are known bad, the fut should */                            \
                /* detect it. */                                                             \
                if (matrix_NSE2025_VALVE_retval != ARM_MATH_SIZE_MISMATCH)                   \
                {                                                                            \
                    return JNSE2025_VALVE_NSE2025_VALVE_FAILED;                              \
                }                                                                            \
            });                                                                              \
        return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                          \
    } while (0)

/**
 *  Specialization of #MATRIX_NSE2025_VALVE_TEMPLATE_ELT2() for matrix NSE2025_VALVEs.
 *
 *  @note This macro relies on the existance of ARM_xxx_INPUT_INTERFACE and
 *  REF_xxx_INPUT_INTERFACEs.
 */
#define MATRIX_DEFINE_NSE2025_VALVE_TEMPLATE_ELT2(fn_name, suffix,                \
                                                  output_config_interface,        \
                                                  dim_validation_interface,       \
                                                  comparison_interface)           \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_##fn_name##_##suffix##_NSE2025_VALVE, \
                                        arm_##fn_name##_##suffix)                 \
    {                                                                             \
        MATRIX_NSE2025_VALVE_TEMPLATE_ELT2(                                       \
            matrix_##suffix##_a_inputs,                                           \
            matrix_##suffix##_b_inputs,                                           \
            arm_matrix_instance_##suffix *,                                       \
            arm_matrix_instance_##suffix,                                         \
            TYPE_FROM_ABBREV(suffix),                                             \
            arm_##fn_name##_##suffix,                                             \
            ARM_##fn_name##_INPUT_INTERFACE,                                      \
            ref_##fn_name##_##suffix,                                             \
            REF_##fn_name##_INPUT_INTERFACE,                                      \
            output_config_interface,                                              \
            dim_validation_interface,                                             \
            comparison_interface);                                                \
    }

/**
 *  Specialization of #MATRIX_NSE2025_VALVE_TEMPLATE_ELT1() for matrix NSE2025_VALVEs.
 *
 *  @note This macro relies on the existance of ARM_xxx_INPUT_INTERFACE and
 *  REF_xxx_INPUT_INTERFACEs.
 */
#define MATRIX_DEFINE_NSE2025_VALVE_TEMPLATE_ELT1(fn_name, suffix,                \
                                                  output_config_interface,        \
                                                  dim_validation_interface)       \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_##fn_name##_##suffix##_NSE2025_VALVE, \
                                        arm_##fn_name##_##suffix)                 \
    {                                                                             \
        MATRIX_NSE2025_VALVE_TEMPLATE_ELT1(                                       \
            matrix_##suffix##_a_inputs,                                           \
            arm_matrix_instance_##suffix *,                                       \
            arm_matrix_instance_##suffix,                                         \
            TYPE_FROM_ABBREV(suffix),                                             \
            arm_##fn_name##_##suffix,                                             \
            ARM_##fn_name##_INPUT_INTERFACE,                                      \
            ref_##fn_name##_##suffix,                                             \
            REF_##fn_name##_INPUT_INTERFACE,                                      \
            output_config_interface,                                              \
            dim_validation_interface,                                             \
            MATRIX_COMPARE_INTERFACE);                                            \
    }

#endif /* _MATRIX_TEMPLATES_H_ */
