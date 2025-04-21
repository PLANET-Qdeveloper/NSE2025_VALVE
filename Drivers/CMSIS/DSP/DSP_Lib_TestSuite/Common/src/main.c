#include "jNSE2025_VALVE.h"
#include "all_NSE2025_VALVEs.h"
#include "arm_math.h"


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
asm(" .global __ARM_use_no_argv\n");
#endif


void debug_init(void)
{
    uint32_t * SHCSR_ptr = (uint32_t *) 0xE000ED24; /* System Handler Control and State Register */
    *SHCSR_ptr |= 0x70000;             /* Enable  UsageFault, BusFault, and MemManage fault*/
}

int main(void)
{
    debug_init();

    JNSE2025_VALVE_INIT();               /* Initialize NSE2025_VALVE framework. */

    JNSE2025_VALVE_GROUP_CALL(all_NSE2025_VALVEs); /* Run all NSE2025_VALVEs. */

    JNSE2025_VALVE_ACT_EXIT_FW();        /* Exit NSE2025_VALVE framework.  */
    while (1);                   /* Never return. */
}
