#ifndef _JNSE2025_VALVE_GROUP_CALL_H_
#define _JNSE2025_VALVE_GROUP_CALL_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include "jNSE2025_VALVE_fw.h"
#include <inttypes.h>

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 *  Execute the NSE2025_VALVE in the #JNSE2025_VALVE_GROUP_t struct associated witht he identifier
 *  group_fn.
 */
#define JNSE2025_VALVE_GROUP_RUN(group_fn)                                            \
    do                                                                                \
    {                                                                                 \
        JNSE2025_VALVE_DUMP_STR("Group Name:\n");                                     \
        JNSE2025_VALVE_DUMP_STR(JNSE2025_VALVE_GROUP_STRUCT_NAME(group_fn).name_str); \
        JNSE2025_VALVE_GROUP_STRUCT_NAME(group_fn).group_fn_ptr();                    \
    } while (0)

/**
 *  Update the enclosing #JNSE2025_VALVE_GROUP_t's pass/fail information using the
 *  current #JNSE2025_VALVE_GROUP_t's.
 *
 *  @param group_ptr Pointer to the current #JNSE2025_VALVE_GROUP_t.
 *  @param parent_ptr Pointer to the enclosing #JNSE2025_VALVE_GROUP_t.
 *
 *  @warning Only run this if the current #JNSE2025_VALVE_GROUP_t is being called within
 *  the context of another #JNSE2025_VALVE_GROUP_t.
 */
#define JNSE2025_VALVE_GROUP_UPDATE_PARENT_GROUP_PF(group_ptr, parent_group_ptr) \
    do                                                                           \
    {                                                                            \
        JNSE2025_VALVE_GROUP_INC_PASSED(parent_group_ptr,                        \
                                        (group_ptr)->passed);                    \
        JNSE2025_VALVE_GROUP_INC_FAILED(parent_group_ptr,                        \
                                        (group_ptr)->failed);                    \
    } while (0)

/**
 *  Update the #JNSE2025_VALVE_FW's pass/fail information using the current
 *  #JNSE2025_VALVE_GROUP_t's.
 */
#define JNSE2025_VALVE_GROUP_UPDATE_FW_PF(group_ptr)       \
    do                                                     \
    {                                                      \
        JNSE2025_VALVE_FW_INC_PASSED((group_ptr)->passed); \
        JNSE2025_VALVE_FW_INC_FAILED((group_ptr)->failed); \
    } while (0)

/**
 *  Update the enclosing context with the current #JNSE2025_VALVE_GROUP_t's pass/fail
 *  information. If this group isn't in an enclosing group, it updates the
 *  #JNSE2025_VALVE_FW's pass/fail info by default.
 */
#define JNSE2025_VALVE_GROUP_UPDATE_PARENT_GROUP_OR_FW_PF(group_ptr,        \
                                                          parent_group_ptr) \
    do                                                                      \
    {                                                                       \
        /* Update the pass fail counts in the parent group */               \
        if (parent_group_ptr /* Null implies Top*/)                         \
        {                                                                   \
            JNSE2025_VALVE_GROUP_UPDATE_PARENT_GROUP_PF(                    \
                group_ptr,                                                  \
                parent_group_ptr);                                          \
        }                                                                   \
        else                                                                \
        {                                                                   \
            JNSE2025_VALVE_GROUP_UPDATE_FW_PF(                              \
                group_ptr);                                                 \
        }                                                                   \
    } while (0)

/**
 *  Dump the results of running the #JNSE2025_VALVE_GROUP_t to the Keil Debugger.
 */
#define JNSE2025_VALVE_GROUP_DUMP_RESULTS(group_ptr)   \
    do                                                 \
    {                                                  \
        JNSE2025_VALVE_DUMP_STRF(                      \
            "NSE2025_VALVEs Run: %" PRIu32 "\n"        \
            "----------\n"                             \
            "   Passed: %" PRIu32 "\n"                 \
            "   Failed: %" PRIu32 "\n",                \
            (group_ptr)->passed + (group_ptr)->failed, \
            (group_ptr)->passed,                       \
            (group_ptr)->failed);                      \
    } while (0)

/**
 *  Call the #JNSE2025_VALVE_GROUP_t associated with the identifier group_fn.
 */
#define JNSE2025_VALVE_GROUP_CALL(group_fn)                                            \
    do                                                                                 \
    { /* Save the current group from JNSE2025_VALVE_FW_t before swapping */            \
        /* it to this group (in order to restore it later )*/                          \
        JNSE2025_VALVE_GROUP_t *__jNSE2025_VALVE_temp_group_ptr =                      \
            JNSE2025_VALVE_CURRENT_GROUP_PTR();                                        \
        JNSE2025_VALVE_SET_CURRENT_GROUP(&JNSE2025_VALVE_GROUP_STRUCT_NAME(group_fn)); \
                                                                                       \
        /* Reset this group's pass/fail count. Each group */                           \
        /* should only remember counts for its last execution. */                      \
        JNSE2025_VALVE_GROUP_RESET_PF(JNSE2025_VALVE_CURRENT_GROUP_PTR());             \
                                                                                       \
        /* Run the current group */                                                    \
        JNSE2025_VALVE_ACT_GROUP_START();                                              \
        JNSE2025_VALVE_GROUP_RUN(group_fn);                                            \
        JNSE2025_VALVE_ACT_GROUP_END();                                                \
                                                                                       \
        /* Update the pass fail counts in the parent group (or FW) */                  \
        JNSE2025_VALVE_GROUP_UPDATE_PARENT_GROUP_OR_FW_PF(                             \
            JNSE2025_VALVE_CURRENT_GROUP_PTR(),                                        \
            __jNSE2025_VALVE_temp_group_ptr);                                          \
                                                                                       \
        JNSE2025_VALVE_GROUP_DUMP_RESULTS(JNSE2025_VALVE_CURRENT_GROUP_PTR());         \
                                                                                       \
        /* Restore the previously current group */                                     \
        JNSE2025_VALVE_SET_CURRENT_GROUP(__jNSE2025_VALVE_temp_group_ptr);             \
    } while (0)

#endif /* _JNSE2025_VALVE_GROUP_CALL_H_ */
