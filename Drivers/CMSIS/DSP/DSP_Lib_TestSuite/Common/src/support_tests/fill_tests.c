#include "jNSE2025_VALVE.h"
#include "support_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "support_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_FILL_NSE2025_VALVE(suffix)             \
    SUPPORT_DEFINE_NSE2025_VALVE_TEMPLATE_ELT1_BLK(      \
        fill,                                   \
        suffix,                                 \
        TYPE_FROM_ABBREV(suffix),               \
        TYPE_FROM_ABBREV(suffix),               \
        SUPPORT_COMPARE_INTERFACE)

JNSE2025_VALVE_ARM_FILL_NSE2025_VALVE(f32);
JNSE2025_VALVE_ARM_FILL_NSE2025_VALVE(q31);
JNSE2025_VALVE_ARM_FILL_NSE2025_VALVE(q15);
JNSE2025_VALVE_ARM_FILL_NSE2025_VALVE(q7);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(fill_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fill_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fill_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fill_q15_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fill_q7_NSE2025_VALVE);
}
