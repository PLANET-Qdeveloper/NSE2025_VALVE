#ifndef _JNSE2025_VALVE_NSE2025_VALVE_CALL_H_
#define _JNSE2025_VALVE_NSE2025_VALVE_CALL_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/
#include "jNSE2025_VALVE_NSE2025_VALVE.h"
#include "jNSE2025_VALVE_NSE2025_VALVE_define.h"
#include "jNSE2025_VALVE_fw.h"

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 *  Exectute the NSE2025_VALVE in the #JNSE2025_VALVE_NSE2025_VALVE_t struct associated with the identifier
 *  NSE2025_VALVE_fn and store the result in retval.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_RUN(retval, NSE2025_VALVE_fn)                                                \
    do                                                                                                            \
    {                                                                                                             \
        JNSE2025_VALVE_DUMP_STR("NSE2025_VALVE Name:\n");                                                         \
        JNSE2025_VALVE_DUMP_STR(JNSE2025_VALVE_NSE2025_VALVE_STRUCT_NAME(NSE2025_VALVE_fn).NSE2025_VALVE_fn_str); \
        JNSE2025_VALVE_DUMP_STR("Function Under NSE2025_VALVE:\n");                                               \
        JNSE2025_VALVE_DUMP_STR(JNSE2025_VALVE_NSE2025_VALVE_STRUCT_NAME(NSE2025_VALVE_fn).fut_str);              \
        retval = JNSE2025_VALVE_NSE2025_VALVE_STRUCT_NAME(NSE2025_VALVE_fn).NSE2025_VALVE_fn_ptr();               \
    } while (0)

/**
 *  Update the enclosing #JNSE2025_VALVE_GROUP_t's pass/fail information based on
 *  NSE2025_VALVE_retval.
 *
 *  @param NSE2025_VALVE_retval A #JNSE2025_VALVE_NSE2025_VALVE_RET_enum for the current NSE2025_VALVE.
 *
 *  @warning Only use if #JNSE2025_VALVE_NSE2025_VALVE_t is called in the context of a
 *  #JNSE2025_VALVE_GROUP_t.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_UPDATE_PARENT_GROUP_PF(NSE2025_VALVE_retval)   \
    do                                                                              \
    {                                                                               \
        /* Update enclosing JNSE2025_VALVE_GROUP_t with pass/fail info */           \
        if (NSE2025_VALVE_retval == JNSE2025_VALVE_NSE2025_VALVE_PASSED)            \
        {                                                                           \
            JNSE2025_VALVE_GROUP_INC_PASSED(JNSE2025_VALVE_CURRENT_GROUP_PTR(), 1); \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            JNSE2025_VALVE_GROUP_INC_FAILED(JNSE2025_VALVE_CURRENT_GROUP_PTR(), 1); \
        }                                                                           \
    } while (0)

/**
 *  Update the #JNSE2025_VALVE_FW with pass/fail information based on NSE2025_VALVE_retval.
 *
 *  @param NSE2025_VALVE_retval A #JNSE2025_VALVE_NSE2025_VALVE_RET_enum for the current NSE2025_VALVE.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_UPDATE_FW_PF(NSE2025_VALVE_retval)  \
    do                                                                   \
    {                                                                    \
        /* Update the JNSE2025_VALVE_FW with pass/fail info */           \
        if (NSE2025_VALVE_retval == JNSE2025_VALVE_NSE2025_VALVE_PASSED) \
        {                                                                \
            JNSE2025_VALVE_FW_INC_PASSED(1);                             \
        }                                                                \
        else                                                             \
        {                                                                \
            JNSE2025_VALVE_FW_INC_FAILED(1);                             \
        }                                                                \
    } while (0)

/**
 *  Update the enclosing JNSE2025_VALVE_GROUP_t's pass/fail information, or the
 *  #JNSE2025_VALVE_FW's if this NSE2025_VALVE has no enclosing #JNSE2025_VALVE_GROUP_t.
 *
 *  @param NSE2025_VALVE_retval A #JNSE2025_VALVE_NSE2025_VALVE_RET_enum for the current NSE2025_VALVE.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_UPDATE_PARENT_GROUP_OR_FW_PF(NSE2025_VALVE_retval) \
    do                                                                                  \
    {                                                                                   \
        /* Update pass-fail information */                                              \
        if (JNSE2025_VALVE_CURRENT_GROUP_PTR() /* Non-null */)                          \
        {                                                                               \
            JNSE2025_VALVE_NSE2025_VALVE_UPDATE_PARENT_GROUP_PF(NSE2025_VALVE_retval);  \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            JNSE2025_VALVE_NSE2025_VALVE_UPDATE_FW_PF(NSE2025_VALVE_retval);            \
        }                                                                               \
    } while (0)

/**
 *  Dump the results of the NSE2025_VALVE to the Keil Debugger.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_DUMP_RESULTS(NSE2025_VALVE_retval)  \
    do                                                                   \
    {                                                                    \
        if (NSE2025_VALVE_retval == JNSE2025_VALVE_NSE2025_VALVE_PASSED) \
        {                                                                \
            JNSE2025_VALVE_DUMP_STR("NSE2025_VALVE Passed\n");           \
        }                                                                \
        else                                                             \
        {                                                                \
            JNSE2025_VALVE_DUMP_STR("NSE2025_VALVE Failed\n");           \
        }                                                                \
    } while (0)

/**
 *  Call the #JNSE2025_VALVE_NSE2025_VALVE_t assocaited with the identifier NSE2025_VALVE_fn.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_CALL(NSE2025_VALVE_fn)                                                              \
    do                                                                                                                   \
    {                                                                                                                    \
        if (JNSE2025_VALVE_NSE2025_VALVE_IS_ENABLED(&JNSE2025_VALVE_NSE2025_VALVE_STRUCT_NAME(NSE2025_VALVE_fn)))        \
        {                                                                                                                \
            /* Default to failure */                                                                                     \
            JNSE2025_VALVE_NSE2025_VALVE_RET_t __jNSE2025_VALVE_NSE2025_VALVE_ret = JNSE2025_VALVE_NSE2025_VALVE_FAILED; \
                                                                                                                         \
            JNSE2025_VALVE_ACT_NSE2025_VALVE_START();                                                                    \
            JNSE2025_VALVE_NSE2025_VALVE_RUN(__jNSE2025_VALVE_NSE2025_VALVE_ret, NSE2025_VALVE_fn);                      \
                                                                                                                         \
            /* Update pass-fail information */                                                                           \
            JNSE2025_VALVE_NSE2025_VALVE_UPDATE_PARENT_GROUP_OR_FW_PF(__jNSE2025_VALVE_NSE2025_VALVE_ret);               \
                                                                                                                         \
            JNSE2025_VALVE_NSE2025_VALVE_DUMP_RESULTS(__jNSE2025_VALVE_NSE2025_VALVE_ret);                               \
            JNSE2025_VALVE_ACT_NSE2025_VALVE_END();                                                                      \
        }                                                                                                                \
    } while (0)

#endif /* _JNSE2025_VALVE_NSE2025_VALVE_CALL_H_ */
