#include "jNSE2025_VALVE.h"
#include "basic_math_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "basic_math_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_ADD_NSE2025_VALVE(suffix)              \
    BASIC_MATH_DEFINE_NSE2025_VALVE_TEMPLATE_BUF2_BLK(   \
        add,                                    \
        suffix,                                 \
        TYPE_FROM_ABBREV(suffix),               \
        TYPE_FROM_ABBREV(suffix),               \
        BASIC_MATH_COMPARE_INTERFACE)

JNSE2025_VALVE_ARM_ADD_NSE2025_VALVE(f32);
JNSE2025_VALVE_ARM_ADD_NSE2025_VALVE(q31);
JNSE2025_VALVE_ARM_ADD_NSE2025_VALVE(q15);
JNSE2025_VALVE_ARM_ADD_NSE2025_VALVE(q7);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(add_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_add_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_add_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_add_q15_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_add_q7_NSE2025_VALVE);
}
