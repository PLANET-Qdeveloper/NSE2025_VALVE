#include "jNSE2025_VALVE.h"
#include "ref.h"
#include "arr_desc.h"
#include "fast_math_templates.h"
#include "fast_math_NSE2025_VALVE_data.h"
#include "type_abbrev.h"

SQRT_NSE2025_VALVE_TEMPLATE_ELT1(q31);
SQRT_NSE2025_VALVE_TEMPLATE_ELT1(q15);

SIN_COS_NSE2025_VALVE_TEMPLATE_ELT1(f32, float32_t, sin);
SIN_COS_NSE2025_VALVE_TEMPLATE_ELT1(q31, q31_t, sin);
SIN_COS_NSE2025_VALVE_TEMPLATE_ELT1(q15, q15_t, sin);

SIN_COS_NSE2025_VALVE_TEMPLATE_ELT1(f32, float32_t, cos);
SIN_COS_NSE2025_VALVE_TEMPLATE_ELT1(q31, q31_t, cos);
SIN_COS_NSE2025_VALVE_TEMPLATE_ELT1(q15, q15_t, cos);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(fast_math_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_sqrt_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_sqrt_q15_NSE2025_VALVE);

    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_sin_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_sin_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_sin_q15_NSE2025_VALVE);

    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cos_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cos_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cos_q15_NSE2025_VALVE);
}
