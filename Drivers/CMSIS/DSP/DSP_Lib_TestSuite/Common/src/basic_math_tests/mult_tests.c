#include "jNSE2025_VALVE.h"
#include "basic_math_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "basic_math_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_MULT_NSE2025_VALVE(suffix, compare_interface)  \
    BASIC_MATH_DEFINE_NSE2025_VALVE_TEMPLATE_BUF2_BLK(   \
        mult,                                   \
        suffix,                                 \
        TYPE_FROM_ABBREV(suffix),               \
        TYPE_FROM_ABBREV(suffix),               \
        compare_interface)

JNSE2025_VALVE_ARM_MULT_NSE2025_VALVE(f32, BASIC_MATH_COMPARE_INTERFACE);
JNSE2025_VALVE_ARM_MULT_NSE2025_VALVE(q31, BASIC_MATH_SNR_COMPARE_INTERFACE);
JNSE2025_VALVE_ARM_MULT_NSE2025_VALVE(q15, BASIC_MATH_COMPARE_INTERFACE);
JNSE2025_VALVE_ARM_MULT_NSE2025_VALVE(q7 , BASIC_MATH_COMPARE_INTERFACE);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(mult_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mult_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mult_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mult_q15_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_mult_q7_NSE2025_VALVE);
}
