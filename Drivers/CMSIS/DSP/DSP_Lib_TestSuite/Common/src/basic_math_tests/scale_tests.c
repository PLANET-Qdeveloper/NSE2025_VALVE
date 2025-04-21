#include "jNSE2025_VALVE.h"
#include "basic_math_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "basic_math_templates.h"
#include "type_abbrev.h"


#define JNSE2025_VALVE_ARM_SCALE_NSE2025_VALVE(suffix)                \
    BASIC_MATH_DEFINE_NSE2025_VALVE_TEMPLATE_BUF1_ELT2_BLK(  \
        scale,                                      \
        suffix,                                     \
        TYPE_FROM_ABBREV(suffix),                   \
        TYPE_FROM_ABBREV(suffix), /*elt1_type*/     \
        int8_t, /*elt2_type*/                       \
        TYPE_FROM_ABBREV(suffix))

/* float32_t defined separately because it has less arguments */
JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_scale_f32_NSE2025_VALVE,
                  arm_scale_f32)
{
    NSE2025_VALVE_TEMPLATE_BUF1_ELT1_BLK(
        basic_math_f_all,
        basic_math_eltsf,
        basic_math_block_sizes,
        float32_t,
        float32_t,
        float32_t,
        arm_scale_f32,
        ARM_scale_float_INPUT_INTERFACE,
        ref_scale_f32,
        REF_scale_float_INPUT_INTERFACE,
        BASIC_MATH_COMPARE_INTERFACE);
}

JNSE2025_VALVE_ARM_SCALE_NSE2025_VALVE(q31);
JNSE2025_VALVE_ARM_SCALE_NSE2025_VALVE(q15);
JNSE2025_VALVE_ARM_SCALE_NSE2025_VALVE(q7);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(scale_NSE2025_VALVEs)
{
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_scale_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_scale_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_scale_q15_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_scale_q7_NSE2025_VALVE);
}
