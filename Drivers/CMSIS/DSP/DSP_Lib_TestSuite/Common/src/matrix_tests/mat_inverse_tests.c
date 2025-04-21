#include "jNSE2025_VALVE.h"
#include "matrix_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "matrix_templates.h"
#include "type_abbrev.h"

JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_mat_inverse_f32_NSE2025_VALVE, arm_mat_inverse_f32)
{
    TEMPLATE_DO_ARR_DESC(
        mat_idx, arm_matrix_instance_f32 *, mat_ptr, matrix_f32_invertible_inputs
        ,
        JNSE2025_VALVE_DUMP_STRF("Matrix Dimensions: %dx%d\n",                
                 (int)mat_ptr->numRows,                         
                 (int)mat_ptr->numCols); 
      
        if (MATRIX_NSE2025_VALVE_VALID_SQUARE_DIMENSIONS(arm_matrix_instance_f32 *, mat_ptr))
        {
            MATRIX_NSE2025_VALVE_CONFIG_SAMESIZE_OUTPUT(arm_matrix_instance_f32 *, mat_ptr);

            /* arm_mat_inverse_f32() modifies its source input. Use the scratch
             * buffer to store a copy of the intended input. */
            {
                float32_t * original_pdata_ptr = mat_ptr->pData;

                memcpy(matrix_output_scratch,
                       mat_ptr->pData,
                       mat_ptr->numRows * mat_ptr->numCols * sizeof(float32_t));
                mat_ptr->pData = (void*) &matrix_output_scratch;

                JNSE2025_VALVE_COUNT_CYCLES(arm_mat_inverse_f32(mat_ptr, &matrix_output_fut));
                mat_ptr->pData = original_pdata_ptr;
            }

            ref_mat_inverse_f32(mat_ptr, &matrix_output_ref);

            MATRIX_SNR_COMPARE_INTERFACE(arm_matrix_instance_f32,
                                         float32_t);
        });

    return JNSE2025_VALVE_NSE2025_VALVE_PASSED;
}

JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_mat_inverse_f64_NSE2025_VALVE, arm_mat_inverse_f64)
{
    TEMPLATE_DO_ARR_DESC(
        mat_idx, arm_matrix_instance_f64 *, mat_ptr, matrix_f64_invertible_inputs
        ,
        JNSE2025_VALVE_DUMP_STRF("Matrix Dimensions: %dx%d\n",                
                         (int)mat_ptr->numRows,                         
                         (int)mat_ptr->numCols);    
                         
        if (MATRIX_NSE2025_VALVE_VALID_SQUARE_DIMENSIONS(arm_matrix_instance_f64 *, mat_ptr))
        {
            MATRIX_NSE2025_VALVE_CONFIG_SAMESIZE_OUTPUT(arm_matrix_instance_f64 *, mat_ptr);

            /* arm_mat_inverse_f64() modifies its source input. Use the scratch
             * buffer to store a copy of the intended input. */
            {
                float64_t * original_pdata_ptr = mat_ptr->pData;

                memcpy(matrix_output_scratch,
                       mat_ptr->pData,
                       mat_ptr->numRows * mat_ptr->numCols * sizeof(float64_t));
                mat_ptr->pData = (void*) &matrix_output_scratch;

                JNSE2025_VALVE_COUNT_CYCLES(arm_mat_inverse_f64(mat_ptr, &matrix_output_fut64));
                mat_ptr->pData = original_pdata_ptr;
            }

            ref_mat_inverse_f64(mat_ptr, &matrix_output_ref64);

            MATRIX_DBL_SNR_COMPARE_INTERFACE(arm_matrix_instance_f64);
        });

    return JNSE2025_VALVE_NSE2025_VALVE_PASSED;
}

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(mat_inverse_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_inverse_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_inverse_f64_NSE2025_VALVE);
}
