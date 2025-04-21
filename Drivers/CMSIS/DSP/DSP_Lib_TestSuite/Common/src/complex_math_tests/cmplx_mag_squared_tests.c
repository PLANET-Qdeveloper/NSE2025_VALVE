#include "jNSE2025_VALVE.h"
#include "complex_math_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "complex_math_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_CMPLX_MAG_SQUARED_NSE2025_VALVE(suffix)    \
    COMPLEX_MATH_DEFINE_NSE2025_VALVE_TEMPLATE_BUF1_BLK(     \
        cmplx_mag_squared,                          \
        suffix,                                     \
        TYPE_FROM_ABBREV(suffix),                   \
        TYPE_FROM_ABBREV(suffix),                   \
        COMPLEX_MATH_COMPARE_RE_INTERFACE)

JNSE2025_VALVE_ARM_CMPLX_MAG_SQUARED_NSE2025_VALVE(f32);
JNSE2025_VALVE_ARM_CMPLX_MAG_SQUARED_NSE2025_VALVE(q31);
JNSE2025_VALVE_ARM_CMPLX_MAG_SQUARED_NSE2025_VALVE(q15);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(cmplx_mag_squared_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cmplx_mag_squared_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cmplx_mag_squared_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cmplx_mag_squared_q15_NSE2025_VALVE);
}
