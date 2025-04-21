#include "jNSE2025_VALVE.h"
#include "matrix_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "matrix_templates.h"
#include "type_abbrev.h"

/* This is for the two fixed point cases */
#define JNSE2025_VALVE_ARM_MAT_SCALE_NSE2025_VALVE(suffix,type)                           \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_mat_scale_##suffix##_NSE2025_VALVE, arm_mat_scale_##suffix) \
    {                                                                   \
        uint32_t i,j;                                                   \
                                                                        \
        TEMPLATE_DO_ARR_DESC(                                           \
            mat_idx, arm_matrix_instance_##suffix *,                    \
            mat_ptr, matrix_##suffix##_b_inputs                         \
            ,                                                           \
            MATRIX_NSE2025_VALVE_CONFIG_SAMESIZE_OUTPUT(                         \
                arm_matrix_instance_##suffix *, mat_ptr);               \
                                                                        \
            for(i=0;i<MATRIX_MAX_COEFFS_LEN;i++)                        \
            {                                                           \
                for(j=0;j<MATRIX_MAX_SHIFTS_LEN;j++)                    \
                {                                                       \
                    JNSE2025_VALVE_DUMP_STRF("Matrix Dimensions: %dx%d\n",       \
                         (int)mat_ptr->numRows,                         \
                         (int)mat_ptr->numCols);                        \
                                                                        \
                    JNSE2025_VALVE_COUNT_CYCLES(                                 \
                        arm_mat_scale_##suffix(mat_ptr,                 \
                                               matrix_##suffix##_scale_values[i], \
                                               matrix_shift_values[j],  \
                                               (arm_matrix_instance_##suffix*) &matrix_output_fut)); \
                                                                        \
                    ref_mat_scale_##suffix(mat_ptr,                     \
                                           matrix_##suffix##_scale_values[i], \
                                           matrix_shift_values[j],      \
                                           (arm_matrix_instance_##suffix*) &matrix_output_ref); \
                                                                        \
                        MATRIX_SNR_COMPARE_INTERFACE(arm_matrix_instance_##suffix, \
                                                     type);             \
                }                                                       \
            });                                                         \
                                                                        \
        return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                       \
    }

JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_mat_scale_f32_NSE2025_VALVE, arm_mat_scale_f32)
{
    uint32_t i;

    TEMPLATE_DO_ARR_DESC(
        mat_idx, arm_matrix_instance_f32 *, mat_ptr, matrix_f32_b_inputs
        ,
        MATRIX_NSE2025_VALVE_CONFIG_SAMESIZE_OUTPUT(arm_matrix_instance_f32 *, mat_ptr);

        for(i=0;i<MATRIX_MAX_COEFFS_LEN;i++)
        {
            JNSE2025_VALVE_DUMP_STRF("Matrix Dimensions: %dx%d\n",                
                         (int)mat_ptr->numRows,                         
                         (int)mat_ptr->numCols);                        
            JNSE2025_VALVE_COUNT_CYCLES(arm_mat_scale_f32(mat_ptr, matrix_f32_scale_values[i], &matrix_output_fut));

            ref_mat_scale_f32(mat_ptr, matrix_f32_scale_values[i], &matrix_output_ref);

            MATRIX_SNR_COMPARE_INTERFACE(arm_matrix_instance_f32,
                                         float32_t);
        });

    return JNSE2025_VALVE_NSE2025_VALVE_PASSED;
}

JNSE2025_VALVE_ARM_MAT_SCALE_NSE2025_VALVE(q31,q31_t);
JNSE2025_VALVE_ARM_MAT_SCALE_NSE2025_VALVE(q15,q15_t);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(mat_scale_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_scale_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_scale_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_scale_q15_NSE2025_VALVE);
}
