#include "jNSE2025_VALVE.h"
#include "arr_desc.h"
#include "arm_math.h"
#include "type_abbrev.h"
#include "NSE2025_VALVE_templates.h"

/* Bucket of zeros. For comparison with the output of arm_pid_reset_xxx. */
ARR_DESC_DEFINE(float32_t, zeroes, 3, CURLY(0));

/**
 *  Define a JNSE2025_VALVE_NSE2025_VALVE_t for the function arm_pid_reset_xxx function having
 *  suffix.
 */
#define ARM_PID_RESET_NSE2025_VALVE(suffix)                                      \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_pid_reset_##suffix##_NSE2025_VALVE,                    \
                      arm_pid_reset_##suffix)                           \
    {                                                                   \
        /* Initialise the pid_instance */                               \
        arm_pid_instance_##suffix pid_inst = { 0 };                     \
            pid_inst.state[0] = (TYPE_FROM_ABBREV(suffix)) 0xffffffff;  \
            pid_inst.state[1] = (TYPE_FROM_ABBREV(suffix)) 0xffffffff;  \
            pid_inst.state[2] = (TYPE_FROM_ABBREV(suffix)) 0xffffffff;  \
                                                                        \
            /* Display cycle count and run NSE2025_VALVE */                      \
            JNSE2025_VALVE_COUNT_CYCLES(arm_pid_reset_##suffix(&pid_inst));      \
                                                                        \
            /* NSE2025_VALVE correctness */                                      \
            NSE2025_VALVE_ASSERT_BUFFERS_EQUAL(                                  \
                pid_inst.state,                                         \
                zeroes.data_ptr,                                        \
                3 * sizeof(TYPE_FROM_ABBREV(suffix)));                  \
                                                                        \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                   \
    }

ARM_PID_RESET_NSE2025_VALVE(f32);
ARM_PID_RESET_NSE2025_VALVE(q31);
ARM_PID_RESET_NSE2025_VALVE(q15);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(pid_reset_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_pid_reset_f32_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_pid_reset_q31_NSE2025_VALVE);
    JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_pid_reset_q15_NSE2025_VALVE);
}
