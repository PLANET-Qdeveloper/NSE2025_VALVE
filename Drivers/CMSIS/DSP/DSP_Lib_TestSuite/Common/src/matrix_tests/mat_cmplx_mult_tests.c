#include "jNSE2025_VALVE.h"
#include "matrix_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "matrix_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_MAT_CMPLX_MULT_NSE2025_VALVE(suffix, comparison_interface) \
    MATRIX_DEFINE_NSE2025_VALVE_TEMPLATE_ELT2(                               \
        mat_cmplx_mult,                                             \
        suffix,                                                     \
        MATRIX_NSE2025_VALVE_CONFIG_MULTIPLICATIVE_OUTPUT,                   \
        MATRIX_NSE2025_VALVE_VALID_MULTIPLICATIVE_DIMENSIONS,                \
        comparison_interface)

JNSE2025_VALVE_ARM_MAT_CMPLX_MULT_NSE2025_VALVE(f32, MATRIX_SNR_COMPARE_INTERFACE);
JNSE2025_VALVE_ARM_MAT_CMPLX_MULT_NSE2025_VALVE(q31, MATRIX_COMPARE_INTERFACE);

/*--------------------------------------------------------------------------------*/
/* Q15 Uses a Different interface than the others. */
/*--------------------------------------------------------------------------------*/

#define ARM_mat_cmplx_mult_q15_INPUT_INTERFACE(input_a_ptr, input_b_ptr) \
    PAREN(input_a_ptr, input_b_ptr,                                     \
          (void *) &matrix_output_fut,                                  \
          (q15_t *) matrix_output_scratch)

JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_mat_cmplx_mult_q15_NSE2025_VALVE, arm_mat_cmplx_mult_q15)
{
    MATRIX_NSE2025_VALVE_TEMPLATE_ELT2(
        matrix_q15_a_inputs,
        matrix_q15_b_inputs,
        arm_matrix_instance_q15 * ,
        arm_matrix_instance_q15,
        TYPE_FROM_ABBREV(q15),
        arm_mat_cmplx_mult_q15,
        ARM_mat_cmplx_mult_q15_INPUT_INTERFACE,
        ref_mat_cmplx_mult_q15,
        REF_mat_cmplx_mult_INPUT_INTERFACE,
        MATRIX_NSE2025_VALVE_CONFIG_MULTIPLICATIVE_OUTPUT,
        MATRIX_NSE2025_VALVE_VALID_MULTIPLICATIVE_DIMENSIONS,
        MATRIX_COMPARE_INTERFACE);
}

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(mat_cmplx_mult_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_cmplx_mult_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_cmplx_mult_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_cmplx_mult_q15_NSE2025_VALVE);
}
