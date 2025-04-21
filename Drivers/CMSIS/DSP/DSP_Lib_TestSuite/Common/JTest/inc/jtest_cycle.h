#ifndef _JNSE2025_VALVE_CYCLE_H_
#define _JNSE2025_VALVE_CYCLE_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include "jNSE2025_VALVE_fw.h" /* JNSE2025_VALVE_DUMP_STRF() */
#include "jNSE2025_VALVE_systick.h"
#include "jNSE2025_VALVE_util.h" /* STR() */

/*--------------------------------------------------------------------------------*/
/* Declare Module Variables */
/*--------------------------------------------------------------------------------*/
extern const char *JNSE2025_VALVE_CYCLE_STRF;

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 *  Wrap the function call, fn_call, to count execution cycles and display the
 *  results.
 */
/* skipp function name + param
#define JNSE2025_VALVE_COUNT_CYCLES(fn_call)                     \
    do                                                  \
    {                                                   \
        uint32_t __jNSE2025_VALVE_cycle_end_count;               \
                                                        \
        JNSE2025_VALVE_SYSTICK_RESET(SysTick);                   \
        JNSE2025_VALVE_SYSTICK_START(SysTick);                   \
                                                        \
        fn_call;                                        \
                                                        \
        __jNSE2025_VALVE_cycle_end_count =                       \
            JNSE2025_VALVE_SYSTICK_VALUE(SysTick);               \
                                                        \
        JNSE2025_VALVE_SYSTICK_RESET(SysTick);                   \
        JNSE2025_VALVE_DUMP_STRF(JNSE2025_VALVE_CYCLE_STRF,               \
                        STR(fn_call),                   \
                        (JNSE2025_VALVE_SYSTICK_INITIAL_VALUE -  \
                         __jNSE2025_VALVE_cycle_end_count));     \
    } while (0)
*/
#define JNSE2025_VALVE_COUNT_CYCLES(fn_call)                             \
    do                                                                   \
    {                                                                    \
        uint32_t __jNSE2025_VALVE_cycle_end_count;                       \
                                                                         \
        JNSE2025_VALVE_SYSTICK_RESET(SysTick);                           \
        JNSE2025_VALVE_SYSTICK_START(SysTick);                           \
                                                                         \
        fn_call;                                                         \
                                                                         \
        __jNSE2025_VALVE_cycle_end_count =                               \
            JNSE2025_VALVE_SYSTICK_VALUE(SysTick);                       \
                                                                         \
        JNSE2025_VALVE_SYSTICK_RESET(SysTick);                           \
        JNSE2025_VALVE_DUMP_STRF(JNSE2025_VALVE_CYCLE_STRF,              \
                                 (JNSE2025_VALVE_SYSTICK_INITIAL_VALUE - \
                                  __jNSE2025_VALVE_cycle_end_count));    \
    } while (0)

#endif /* _JNSE2025_VALVE_CYCLE_H_ */
