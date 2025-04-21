#include "jNSE2025_VALVE.h"
#include "statistics_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "statistics_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_VAR_NSE2025_VALVE(suffix)              \
    STATISTICS_DEFINE_NSE2025_VALVE_TEMPLATE_BUF1_BLK(   \
        var,                                    \
        suffix,                                 \
        TYPE_FROM_ABBREV(suffix),               \
        TYPE_FROM_ABBREV(suffix),               \
        STATISTICS_SNR_COMPARE_INTERFACE)

JNSE2025_VALVE_ARM_VAR_NSE2025_VALVE(f32);
JNSE2025_VALVE_ARM_VAR_NSE2025_VALVE(q31);
JNSE2025_VALVE_ARM_VAR_NSE2025_VALVE(q15);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(var_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_var_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_var_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_var_q15_NSE2025_VALVE);
}
