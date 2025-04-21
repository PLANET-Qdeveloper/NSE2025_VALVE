#include "jNSE2025_VALVE.h"
#include "matrix_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "matrix_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_MAT_ADD_NSE2025_VALVE(suffix)          \
    MATRIX_DEFINE_NSE2025_VALVE_TEMPLATE_ELT2(           \
        mat_add,                                \
        suffix,                                 \
        MATRIX_NSE2025_VALVE_CONFIG_ADDITIVE_OUTPUT,     \
        MATRIX_NSE2025_VALVE_VALID_ADDITIVE_DIMENSIONS,  \
        MATRIX_COMPARE_INTERFACE)

JNSE2025_VALVE_ARM_MAT_ADD_NSE2025_VALVE(f32);
JNSE2025_VALVE_ARM_MAT_ADD_NSE2025_VALVE(q31);
JNSE2025_VALVE_ARM_MAT_ADD_NSE2025_VALVE(q15);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(mat_add_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_add_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_add_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mat_add_q15_NSE2025_VALVE);
}
