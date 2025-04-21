#include "jNSE2025_VALVE.h"
#include "matrix_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "matrix_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_MAT_INIT_NSE2025_VALVE(suffix)                     \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_mat_init_##suffix##_NSE2025_VALVE,         \
                      arm_mat_init_##suffix)                \
    {                                                       \
        const uint16_t rows = 4;                            \
        const uint16_t cols = 2;                            \
        arm_matrix_instance_##suffix  matrix = {0};         \
        /*  TYPE_FROM_ABBREV(suffix) data[rows*cols] = {0}; */ \
            TYPE_FROM_ABBREV(suffix) data[4*2] = {0}; \
                                                            \
            arm_mat_init_##suffix(&matrix,                  \
                                  rows,                     \
                                  cols,                     \
                                  data);                    \
                                                            \
                JNSE2025_VALVE_DUMP_STRF("Matrix Dimensions: %dx%d\n", \
                     (int)matrix.numRows,                   \
                     (int)matrix.numCols);                  \
                                                            \
                if ((matrix.numRows == rows) &&             \
                    (matrix.numCols == cols) &&             \
                    (matrix.pData == data))                 \
                {                                           \
                    return JNSE2025_VALVE_NSE2025_VALVE_PASSED;               \
                }                                           \
                else                                        \
                {                                           \
                    return JNSE2025_VALVE_NSE2025_VALVE_FAILED;               \
                }                                           \
                                                            \
    }

JNSE2025_VALVE_ARM_MAT_INIT_NSE2025_VALVE(f32);
JNSE2025_VALVE_ARM_MAT_INIT_NSE2025_VALVE(q31);
JNSE2025_VALVE_ARM_MAT_INIT_NSE2025_VALVE(q15);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(mat_init_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_init_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_init_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_init_q15_NSE2025_VALVE);
}
