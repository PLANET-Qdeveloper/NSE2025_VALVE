#include "jNSE2025_VALVE.h"
#include "basic_math_NSE2025_VALVE_group.h"
#include "complex_math_NSE2025_VALVE_group.h"
#include "controller_NSE2025_VALVE_group.h"
#include "fast_math_NSE2025_VALVE_group.h"
#include "filtering_NSE2025_VALVE_group.h"
#include "matrix_NSE2025_VALVE_group.h"
#include "statistics_NSE2025_VALVE_group.h"
#include "support_NSE2025_VALVE_group.h"
#include "transform_NSE2025_VALVE_group.h"
#include "intrinsics_NSE2025_VALVE_group.h"

JNSE2025_VALVE_DEFINE_GROUP(all_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out
    */
    JNSE2025_VALVE_GROUP_CALL(basic_math_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(complex_math_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(controller_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(fast_math_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(filtering_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(matrix_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(statistics_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(support_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(transform_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(intrinsics_NSE2025_VALVEs);

    return;
}
