#include "jNSE2025_VALVE.h"
#include "complex_math_NSE2025_VALVEs.h"

JNSE2025_VALVE_DEFINE_GROUP(complex_math_NSE2025_VALVEs)
{
    JNSE2025_VALVE_GROUP_CALL(cmplx_conj_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(cmplx_dot_prod_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(cmplx_mag_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(cmplx_mag_squared_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(cmplx_mult_cmplx_NSE2025_VALVEs);
    JNSE2025_VALVE_GROUP_CALL(cmplx_mult_real_NSE2025_VALVEs);
    
    return;
}
