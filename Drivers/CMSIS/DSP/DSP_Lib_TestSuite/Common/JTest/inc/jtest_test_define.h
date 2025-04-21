#ifndef _JNSE2025_VALVE_NSE2025_VALVE_DEFINE_H_
#define _JNSE2025_VALVE_NSE2025_VALVE_DEFINE_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include "jNSE2025_VALVE_util.h"
#include "jNSE2025_VALVE_define.h"
#include "jNSE2025_VALVE_NSE2025_VALVE.h"

/* For defining macros with optional arguments */
#include "opt_arg/opt_arg.h"

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 *  Prefix for all #JNSE2025_VALVE_NSE2025_VALVE_t structs.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_STRUCT_NAME_PREFIX G_JNSE2025_VALVE_NSE2025_VALVE_STRUCT_

/**
 *  Define NSE2025_VALVE template used by #JNSE2025_VALVE_NSE2025_VALVE_t NSE2025_VALVEs.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_FN_TEMPLATE(NSE2025_VALVE_fn) \
    JNSE2025_VALVE_NSE2025_VALVE_RET_t NSE2025_VALVE_fn(void)

#define JNSE2025_VALVE_NSE2025_VALVE_FN_PROTOTYPE JNSE2025_VALVE_NSE2025_VALVE_FN_TEMPLATE /**< Alias for \
                                                                                            * #JNSE2025_VALVE_NSE2025_VALVE_FN_TEMPLATE. */

/**
 *  Evaluate to the name of the #JNSE2025_VALVE_NSE2025_VALVE_t struct associated with NSE2025_VALVE_fn.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_STRUCT_NAME(NSE2025_VALVE_fn) \
    JNSE2025_VALVE_STRUCT_NAME(JNSE2025_VALVE_NSE2025_VALVE_STRUCT_NAME_PREFIX, NSE2025_VALVE_fn)

/**
 *  Define a #JNSE2025_VALVE_NSE2025_VALVE_t struct based on the given NSE2025_VALVE_fn.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_DEFINE_STRUCT(NSE2025_VALVE_fn) \
    JNSE2025_VALVE_DEFINE_STRUCT(JNSE2025_VALVE_NSE2025_VALVE_t,     \
                                 JNSE2025_VALVE_NSE2025_VALVE_STRUCT_NAME(NSE2025_VALVE_fn))

/**
 *  Declare a #JNSE2025_VALVE_NSE2025_VALVE_t struct based on the given NSE2025_VALVE_fn.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_DECLARE_STRUCT(NSE2025_VALVE_fn) \
    JNSE2025_VALVE_DECLARE_STRUCT(JNSE2025_VALVE_NSE2025_VALVE_DEFINE_STRUCT(NSE2025_VALVE_fn))

/**
 *  Contents needed to initialize a JNSE2025_VALVE_NSE2025_VALVE_t struct.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_STRUCT_INIT(NSE2025_VALVE_fn, fut, enable) \
    NSE2025_VALVE_fn,                                                           \
        STR_NL(NSE2025_VALVE_fn),                                               \
        STR_NL(fut),                                                            \
    {                                                                           \
        {                                               \
            enable,                                     \
                0                                       \
        }                                               \
    }

/**
 *  Initialize the contents of a #JNSE2025_VALVE_NSE2025_VALVE_t struct.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_INIT(NSE2025_VALVE_fn, fut, enable) \
    JNSE2025_VALVE_NSE2025_VALVE_DEFINE_STRUCT(NSE2025_VALVE_fn) = {     \
        JNSE2025_VALVE_NSE2025_VALVE_STRUCT_INIT(NSE2025_VALVE_fn, fut, enable)}

/* NSE2025_VALVE Definition Macro */
/*--------------------------------------------------------------------------------*/

/**
 *  Define a #JNSE2025_VALVE_NSE2025_VALVE_t object and a NSE2025_VALVE function.
 */
#define _JNSE2025_VALVE_DEFINE_NSE2025_VALVE(NSE2025_VALVE_fn, fut, enable) \
    JNSE2025_VALVE_NSE2025_VALVE_FN_PROTOTYPE(NSE2025_VALVE_fn);            \
    JNSE2025_VALVE_NSE2025_VALVE_INIT(NSE2025_VALVE_fn, fut, enable);       \
    JNSE2025_VALVE_NSE2025_VALVE_FN_PROTOTYPE(NSE2025_VALVE_fn) /* Notice the lacking semicolon */

/**
 *  Declare a #JNSE2025_VALVE_NSE2025_VALVE_t object and a NSE2025_VALVE function prototype.
 */
#define JNSE2025_VALVE_DECLARE_NSE2025_VALVE(NSE2025_VALVE_fn)   \
    JNSE2025_VALVE_NSE2025_VALVE_FN_PROTOTYPE(NSE2025_VALVE_fn); \
    JNSE2025_VALVE_NSE2025_VALVE_DECLARE_STRUCT(NSE2025_VALVE_fn) /* Note the lacking semicolon */

/*--------------------------------------------------------------------------------*/
/* Macros with optional arguments */
/*--------------------------------------------------------------------------------*/

/* Top-level Interface */
#define JNSE2025_VALVE_DEFINE_NSE2025_VALVE(...) \
    JNSE2025_VALVE_DEFINE_NSE2025_VALVE_(PP_NARG(__VA_ARGS__), ##__VA_ARGS__)

/* Dispatch Macro*/
#define JNSE2025_VALVE_DEFINE_NSE2025_VALVE_(N, ...) \
    SPLICE(JNSE2025_VALVE_DEFINE_NSE2025_VALVE_, N)(__VA_ARGS__)

/* Default Arguments */
#define JNSE2025_VALVE_DEFINE_NSE2025_VALVE_DEFAULT_FUT /* Blank */
#define JNSE2025_VALVE_DEFINE_NSE2025_VALVE_DEFAULT_ENABLE \
    JNSE2025_VALVE_TRUE /* NSE2025_VALVEs enabled by       \
                         * default. */

/* Dispatch Cases*/
#define JNSE2025_VALVE_DEFINE_NSE2025_VALVE_1(_1)        \
    _JNSE2025_VALVE_DEFINE_NSE2025_VALVE(                \
        _1,                                              \
        JNSE2025_VALVE_DEFINE_NSE2025_VALVE_DEFAULT_FUT, \
        JNSE2025_VALVE_DEFINE_NSE2025_VALVE_DEFAULT_ENABLE)

#define JNSE2025_VALVE_DEFINE_NSE2025_VALVE_2(_1, _2) \
    _JNSE2025_VALVE_DEFINE_NSE2025_VALVE(             \
        _1,                                           \
        _2,                                           \
        JNSE2025_VALVE_DEFINE_NSE2025_VALVE_DEFAULT_ENABLE)

#define JNSE2025_VALVE_DEFINE_NSE2025_VALVE_3(_1, _2, _3) \
    _JNSE2025_VALVE_DEFINE_NSE2025_VALVE(                 \
        _1,                                               \
        _2,                                               \
        _3)

#endif /* _JNSE2025_VALVE_NSE2025_VALVE_DEFINE_H_ */
