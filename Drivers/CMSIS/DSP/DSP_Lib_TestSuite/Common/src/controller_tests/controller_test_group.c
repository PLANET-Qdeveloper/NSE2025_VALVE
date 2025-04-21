#include "jNSE2025_VALVE.h"
#include "controller_NSE2025_VALVEs.h"

JNSE2025_VALVE_DEFINE_GROUP(controller_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
    JNSE2025_VALVE_GROUP_CALL(pid_reset_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(pid_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(sin_cos_NSE2025_VALVEs);
    return;
}
