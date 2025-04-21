#include "jNSE2025_VALVE.h"
#include "statistics_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "statistics_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_POWER_NSE2025_VALVE(suffix, output_type)   \
    STATISTICS_DEFINE_NSE2025_VALVE_TEMPLATE_BUF1_BLK(       \
        power,                                      \
        suffix,                                     \
        TYPE_FROM_ABBREV(suffix),                   \
        TYPE_FROM_ABBREV(suffix),                   \
        STATISTICS_SNR_COMPARE_INTERFACE)

JNSE2025_VALVE_ARM_POWER_NSE2025_VALVE(f32, float32_t);
JNSE2025_VALVE_ARM_POWER_NSE2025_VALVE(q31, q63_t);
JNSE2025_VALVE_ARM_POWER_NSE2025_VALVE(q15, q63_t);
JNSE2025_VALVE_ARM_POWER_NSE2025_VALVE(q7,  q31_t);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(power_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_power_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_power_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_power_q15_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_power_q7_NSE2025_VALVE);
}
