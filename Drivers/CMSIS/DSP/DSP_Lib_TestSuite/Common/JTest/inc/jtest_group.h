#ifndef _JNSE2025_VALVE_GROUP_H_
#define _JNSE2025_VALVE_GROUP_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include "jNSE2025_VALVE_pf.h"
#include "jNSE2025_VALVE_util.h"

/*--------------------------------------------------------------------------------*/
/* Type Definitions */
/*--------------------------------------------------------------------------------*/

/**
 *  A struct which represents a group of #JNSE2025_VALVE_NSE2025_VALVE_t structs. This struct is
 *  used to run the group of NSE2025_VALVEs, and report on their outcomes.
 */
typedef struct JNSE2025_VALVE_GROUP_struct
{
    void (*group_fn_ptr)(void); /**< Pointer to the NSE2025_VALVE group */
    char *name_str;             /**< Name of the group */

    /* Extend the #JNSE2025_VALVE_GROUP_t with Pass/Fail information.*/
    JNSE2025_VALVE_PF_MEMBERS;
} JNSE2025_VALVE_GROUP_t;

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 *  Set the name of JNSE2025_VALVE_GROUP_t.
 */
#define JNSE2025_VALVE_GROUP_SET_NAME(group_ptr, name) \
    JNSE2025_VALVE_SET_STRUCT_ATTRIBUTE(group_ptr, name_str, name)

#define JNSE2025_VALVE_GROUP_SET_FN(group_ptr, fn_ptr) \
    JNSE2025_VALVE_SET_STRUCT_ATTRIBUTE(group_ptr, group_fn_ptr, fn_ptr)

/**
 *  Increment the number of NSE2025_VALVEs passed in the JNSE2025_VALVE_GROUP_t pointed to by
 *  group_ptr.
 */
#define JNSE2025_VALVE_GROUP_INC_PASSED(group_ptr, amount) \
    JNSE2025_VALVE_PF_INC_PASSED(group_ptr, amount)

/**
 *  Increment the number of NSE2025_VALVEs failed in the JNSE2025_VALVE_GROUP_t pointed to by
 *  group_ptr.
 */
#define JNSE2025_VALVE_GROUP_INC_FAILED(group_ptr, amount) \
    JNSE2025_VALVE_PF_INC_FAILED(group_ptr, amount)

/**
 *  Reset the pass/fail information of the #JNSE2025_VALVE_GROUP_t pointed to by
 *  group_ptr.
 */
#define JNSE2025_VALVE_GROUP_RESET_PF(group_ptr)   \
    do                                             \
    {                                              \
        JNSE2025_VALVE_PF_RESET_PASSED(group_ptr); \
        JNSE2025_VALVE_PF_RESET_FAILED(group_ptr); \
    } while (0)

#endif /* _JNSE2025_VALVE_GROUP_H_ */
