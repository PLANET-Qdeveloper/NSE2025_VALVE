#include "jNSE2025_VALVE.h"
#include "support_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "support_templates.h"
#include "type_abbrev.h"

/* Aliases to play nicely with templates. */
#define arm_f32_to_q31 arm_float_to_q31
#define arm_f32_to_q15 arm_float_to_q15
#define arm_f32_to_q7  arm_float_to_q7
#define arm_q31_to_f32 arm_q31_to_float
#define arm_q15_to_f32 arm_q15_to_float
#define arm_q7_to_f32  arm_q7_to_float
#define ref_f32_to_q31 ref_float_to_q31
#define ref_f32_to_q15 ref_float_to_q15
#define ref_f32_to_q7  ref_float_to_q7
#define ref_q31_to_f32 ref_q31_to_float
#define ref_q15_to_f32 ref_q15_to_float
#define ref_q7_to_f32  ref_q7_to_float

#define JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(prefix, suffix)               \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_##prefix##_to_##suffix##_NSE2025_VALVE,    \
                      arm_##prefix##_to_##suffix)           \
    {                                                       \
        NSE2025_VALVE_TEMPLATE_BUF1_BLK(                             \
            support_f_all,                                  \
            support_block_sizes,                            \
            TYPE_FROM_ABBREV(prefix),                       \
            TYPE_FROM_ABBREV(suffix),                       \
            arm_##prefix##_to_##suffix,                     \
            ARM_x_to_y_INPUT_INTERFACE,                     \
            ref_##prefix##_to_##suffix,                     \
            REF_x_to_y_INPUT_INTERFACE,                     \
            SUPPORT_COMPARE_INTERFACE);                     \
    }

JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(f32, q31);
JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(f32, q15);
JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(f32, q7);

JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q31, f32);
JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q31, q15);
JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q31, q7);

JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q15, f32);
JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q15, q31);
JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q15, q7);

JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q7, f32);
JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q7, q31);
JNSE2025_VALVE_ARM_X_TO_Y_NSE2025_VALVE(q7, q15);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(x_to_y_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_f32_to_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_f32_to_q15_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_f32_to_q7_NSE2025_VALVE);

    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q31_to_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q31_to_q15_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q31_to_q7_NSE2025_VALVE);

    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q15_to_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q15_to_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q15_to_q7_NSE2025_VALVE);

    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q7_to_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q7_to_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_q7_to_q15_NSE2025_VALVE);
}
