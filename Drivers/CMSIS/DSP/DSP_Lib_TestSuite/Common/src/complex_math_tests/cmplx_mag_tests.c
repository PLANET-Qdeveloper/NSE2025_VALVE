#include "jNSE2025_VALVE.h"
#include "complex_math_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "complex_math_templates.h"
#include "type_abbrev.h"

#define JNSE2025_VALVE_ARM_CMPLX_MAG_NSE2025_VALVE(suffix, comparison_interface)  \
    COMPLEX_MATH_DEFINE_NSE2025_VALVE_TEMPLATE_BUF1_BLK(                 \
        cmplx_mag,                                              \
        suffix,                                                 \
        TYPE_FROM_ABBREV(suffix),                               \
        TYPE_FROM_ABBREV(suffix),                               \
        comparison_interface)

JNSE2025_VALVE_ARM_CMPLX_MAG_NSE2025_VALVE(f32, COMPLEX_MATH_COMPARE_RE_INTERFACE);
JNSE2025_VALVE_ARM_CMPLX_MAG_NSE2025_VALVE(q31, COMPLEX_MATH_SNR_COMPARE_RE_INTERFACE);
JNSE2025_VALVE_ARM_CMPLX_MAG_NSE2025_VALVE(q15, COMPLEX_MATH_SNR_COMPARE_RE_INTERFACE);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(cmplx_mag_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cmplx_mag_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cmplx_mag_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_cmplx_mag_q15_NSE2025_VALVE);
}
