#ifndef _JNSE2025_VALVE_GROUP_DEFINE_H_
#define _JNSE2025_VALVE_GROUP_DEFINE_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include "jNSE2025_VALVE_util.h"
#include "jNSE2025_VALVE_define.h"
#include "jNSE2025_VALVE_group.h"

/* For defining macros with optional arguments */
#include "opt_arg/opt_arg.h"

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 *  Prefix for all #JNSE2025_VALVE_GROUP_t structs.
 */
#define JNSE2025_VALVE_GROUP_STRUCT_NAME_PREFIX G_JNSE2025_VALVE_GROUP_STRUCT_

/**
 *  Define NSE2025_VALVE template used by #JNSE2025_VALVE_GROUP_t NSE2025_VALVEs.
 */
#define JNSE2025_VALVE_GROUP_FN_TEMPLATE(group_fn) \
    void group_fn(void)

#define JNSE2025_VALVE_GROUP_FN_PROTOTYPE JNSE2025_VALVE_GROUP_FN_TEMPLATE /**< Alias for \
                                                            #JNSE2025_VALVE_GROUP_FN_TEMPLATE. */

/**
 *  Evaluate to the name of the #JNSE2025_VALVE_GROUP_t struct associated with group_fn.
 */
#define JNSE2025_VALVE_GROUP_STRUCT_NAME(group_fn) \
    JNSE2025_VALVE_STRUCT_NAME(JNSE2025_VALVE_GROUP_STRUCT_NAME_PREFIX, group_fn)

/**
 *  Define a #JNSE2025_VALVE_GROUP_t struct based on the given group_fn.
 */
#define JNSE2025_VALVE_GROUP_DEFINE_STRUCT(group_fn)     \
    JNSE2025_VALVE_DEFINE_STRUCT(JNSE2025_VALVE_GROUP_t, \
                                 JNSE2025_VALVE_GROUP_STRUCT_NAME(group_fn))

/**
 *  Declare a #JNSE2025_VALVE_GROUP_t struct based on the given group_fn.
 */
#define JNSE2025_VALVE_GROUP_DECLARE_STRUCT(group_fn) \
    JNSE2025_VALVE_DECLARE_STRUCT(JNSE2025_VALVE_GROUP_DEFINE_STRUCT(group_fn))

/**
 *  Contents needed to initialize a JNSE2025_VALVE_GROUP_t struct.
 */
#define JNSE2025_VALVE_GROUP_STRUCT_INIT(group_fn) \
    group_fn,                                      \
        STR_NL(group_fn),                          \
        JNSE2025_VALVE_PF_MEMBER_INIT

/**
 *  Initialize the contents of a #JNSE2025_VALVE_GROUP_t struct.
 */
#define JNSE2025_VALVE_GROUP_INIT(group_fn)          \
    JNSE2025_VALVE_GROUP_DEFINE_STRUCT(group_fn) = { \
        JNSE2025_VALVE_GROUP_STRUCT_INIT(group_fn)}

/* NSE2025_VALVE Definition Macro */
/*--------------------------------------------------------------------------------*/

/**
 *  Define a #JNSE2025_VALVE_GROUP_t object and a NSE2025_VALVE function.
 */
#define JNSE2025_VALVE_DEFINE_GROUP(group_fn)    \
    JNSE2025_VALVE_GROUP_FN_PROTOTYPE(group_fn); \
    JNSE2025_VALVE_GROUP_INIT(group_fn);         \
    JNSE2025_VALVE_GROUP_FN_PROTOTYPE(group_fn) /* Notice the lacking semicolon */

/**
 *  Declare a #JNSE2025_VALVE_GROUP_t object and a NSE2025_VALVE function prototype.
 */
#define JNSE2025_VALVE_DECLARE_GROUP(group_fn)   \
    JNSE2025_VALVE_GROUP_FN_PROTOTYPE(group_fn); \
    JNSE2025_VALVE_GROUP_DECLARE_STRUCT(group_fn) /* Note the lacking semicolon */

#endif /* _JNSE2025_VALVE_GROUP_DEFINE_H_ */
