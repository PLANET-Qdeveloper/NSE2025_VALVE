#include "jNSE2025_VALVE.h"
#include "basic_math_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "basic_math_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_NEGATE_NSE2025_VALVE(suffix)           \
    BASIC_MATH_DEFINE_NSE2025_VALVE_TEMPLATE_BUF1_BLK(   \
        negate,                                 \
        suffix,                                 \
        TYPE_FROM_ABBREV(suffix),               \
        TYPE_FROM_ABBREV(suffix))
    
JNSE2025_VALVE_ARM_NEGATE_NSE2025_VALVE(f32);
JNSE2025_VALVE_ARM_NEGATE_NSE2025_VALVE(q31);
JNSE2025_VALVE_ARM_NEGATE_NSE2025_VALVE(q15);
JNSE2025_VALVE_ARM_NEGATE_NSE2025_VALVE(q7);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(negate_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_negate_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_negate_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_negate_q15_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_negate_q7_NSE2025_VALVE);
}
