#ifndef _JNSE2025_VALVE_PF_H_
#define _JNSE2025_VALVE_PF_H_

/*--------------------------------------------------------------------------------*/
/* Purpose */
/*--------------------------------------------------------------------------------*/
/* jNSE2025_VALVE_pf.h Contains macros useful for capturing pass/fail data. */

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 * Members that can be added to other structs to extend them pass/fail data and
 * corresponding functionality.
 */
#define JNSE2025_VALVE_PF_MEMBERS \
    uint32_t passed;              \
    uint32_t failed /* Note the lacking semicolon*/

/**
 *  Used for initializing JNSE2025_VALVE_PF_MEMBERS in a struct declaration.
 */
#define JNSE2025_VALVE_PF_MEMBER_INIT \
    0,                                \
        0

/* Member-Incrementing Macros */
/*--------------------------------------------------------------------------------*/

/**
 *  Dispatch macro for incrementing #JNSE2025_VALVE_PF_MEMBERS.
 *
 *  @param xxx Values: 'passed', 'failed'
 */
#define JNSE2025_VALVE_PF_INC_XXX(xxx, struct_pf_ptr, amount) \
    do                                                        \
    {                                                         \
        ((struct_pf_ptr)->xxx) += (amount);                   \
    } while (0)

/**
 *  Specialization of the #JNSE2025_VALVE_PF_INC_XXX macro to increment the passed
 *  member.
 */
#define JNSE2025_VALVE_PF_INC_PASSED(struct_pf_ptr, amount) \
    JNSE2025_VALVE_PF_INC_XXX(passed, struct_pf_ptr, amount)

/**
 *  Specialization of the #JNSE2025_VALVE_PF_INC_XXX macro to increment the failed
 *  member.
 */
#define JNSE2025_VALVE_PF_INC_FAILED(struct_pf_ptr, amount) \
    JNSE2025_VALVE_PF_INC_XXX(failed, struct_pf_ptr, amount)

/* Member-Resetting Macros */
/*--------------------------------------------------------------------------------*/

/**
 *  Dispatch macro for setting #JNSE2025_VALVE_PF_MEMBERS to zero.
 *
 *  @param xxx Values: 'passed', 'failed'
 */
#define JNSE2025_VALVE_PF_RESET_XXX(xxx, struct_pf_ptr) \
    do                                                  \
    {                                                   \
        ((struct_pf_ptr)->xxx) = UINT32_C(0);           \
    } while (0)

/**
 *  Specialization of #JNSE2025_VALVE_PF_RESET_XXX for the 'passed' member.
 */
#define JNSE2025_VALVE_PF_RESET_PASSED(struct_pf_ptr) \
    JNSE2025_VALVE_PF_RESET_XXX(passed, struct_pf_ptr)

/**
 *  Specialization of #JNSE2025_VALVE_PF_RESET_XXX for the 'failed' member.
 */
#define JNSE2025_VALVE_PF_RESET_FAILED(struct_pf_ptr) \
    JNSE2025_VALVE_PF_RESET_XXX(failed, struct_pf_ptr)

#endif /* _JNSE2025_VALVE_PF_H_ */
