#ifndef _JNSE2025_VALVE_NSE2025_VALVE_H_
#define _JNSE2025_VALVE_NSE2025_VALVE_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include <stdint.h>
#include "jNSE2025_VALVE_util.h"
#include "jNSE2025_VALVE_NSE2025_VALVE_ret.h"

/*--------------------------------------------------------------------------------*/
/* Type Definitions */
/*--------------------------------------------------------------------------------*/

/**
 *  A struct which represents a NSE2025_VALVE in the JNSE2025_VALVE framework.  This struct is
 *  used to enable, run, and describe the NSE2025_VALVE it represents.
 */
typedef struct JNSE2025_VALVE_NSE2025_VALVE_struct
{
    JNSE2025_VALVE_NSE2025_VALVE_RET_t (*NSE2025_VALVE_fn_ptr)(void); /**< Pointer to the NSE2025_VALVE function. */
    char *NSE2025_VALVE_fn_str;                                       /**< Name of the NSE2025_VALVE function */
    char *fut_str;                                                    /**< Name of the function under NSE2025_VALVE. */

    /**
     *  Flags that govern how the #JNSE2025_VALVE_NSE2025_VALVE_t behaves.
     */
    union
    {
        struct
        {
            unsigned enabled : 1;
            unsigned unused : 7;
        } bits;
        uint8_t byte; /* Access all flags at once. */
    } flags;

} JNSE2025_VALVE_NSE2025_VALVE_t;

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 *  Assign a NSE2025_VALVE function to the #JNSE2025_VALVE_NSE2025_VALVE_t struct.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_SET_FN(jNSE2025_VALVE_NSE2025_VALVE_ptr, fn_ptr) \
    JNSE2025_VALVE_SET_STRUCT_ATTRIBUTE(jNSE2025_VALVE_NSE2025_VALVE_ptr, NSE2025_VALVE_fn_ptr, fn_ptr)

/**
 *  Specify a function under NSE2025_VALVE (FUT) for the #JNSE2025_VALVE_NSE2025_VALVE_t struct.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_SET_FUT(jNSE2025_VALVE_NSE2025_VALVE_ptr, str) \
    JNSE2025_VALVE_SET_STRUCT_ATTRIBUTE(jNSE2025_VALVE_NSE2025_VALVE_ptr, fut_str, str)

/* Macros concerning JNSE2025_VALVE_NSE2025_VALVE_t flags */
/*--------------------------------------------------------------------------------*/

#define JNSE2025_VALVE_NSE2025_VALVE_FLAG_SET 1 /**< Value of a set #JNSE2025_VALVE_NSE2025_VALVE_t flag. */
#define JNSE2025_VALVE_NSE2025_VALVE_FLAG_CLR 0 /**< Value of a cleared #JNSE2025_VALVE_NSE2025_VALVE_t flag. */

/**
 *  Evaluate to the flag in #JNSE2025_VALVE_NSE2025_VALVE_t having flag_name.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_FLAG(jNSE2025_VALVE_NSE2025_VALVE_ptr, flag_name) \
    ((jNSE2025_VALVE_NSE2025_VALVE_ptr)->flags.bits.flag_name)

/**
 *  Dispatch macro for setting and clearing #JNSE2025_VALVE_NSE2025_VALVE_t flags.
 *
 *  @param jNSE2025_VALVE_NSE2025_VALVE_ptr Pointer to a #JNSE2025_VALVE_NSE2025_VALVE_t struct.
 *  @param flag_name      Name of the flag to set in #JNSE2025_VALVE_NSE2025_VALVE_t.flags.bits
 *  @param xxx            Vaid values: "SET" or "CLR"
 *
 *  @note This function depends on JNSE2025_VALVE_NSE2025_VALVE_FLAG_SET and JNSE2025_VALVE_NSE2025_VALVE_FLAG_CLR.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_XXX_FLAG(jNSE2025_VALVE_NSE2025_VALVE_ptr, flag_name, xxx)                                   \
    do                                                                                                                            \
    {                                                                                                                             \
        JNSE2025_VALVE_NSE2025_VALVE_FLAG(jNSE2025_VALVE_NSE2025_VALVE_ptr, flag_name) = JNSE2025_VALVE_NSE2025_VALVE_FLAG_##xxx; \
    } while (0)

/**
 *  Specification of #JNSE2025_VALVE_NSE2025_VALVE_XXX_FLAG to set #JNSE2025_VALVE_NSE2025_VALVE_t flags.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_SET_FLAG(jNSE2025_VALVE_NSE2025_VALVE_ptr, flag_name) \
    JNSE2025_VALVE_NSE2025_VALVE_XXX_FLAG(jNSE2025_VALVE_NSE2025_VALVE_ptr, flag_name, SET)

/**
 *  Specification of #JNSE2025_VALVE_NSE2025_VALVE_XXX_FLAG to clear #JNSE2025_VALVE_NSE2025_VALVE_t flags.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_CLR_FLAG(jNSE2025_VALVE_NSE2025_VALVE_ptr, flag_name) \
    JNSE2025_VALVE_NSE2025_VALVE_XXX_FLAG(jNSE2025_VALVE_NSE2025_VALVE_ptr, flag_name, CLR)

/**
 *  Evaluate to true if the #JNSE2025_VALVE_NSE2025_VALVE_t is enabled.
 */
#define JNSE2025_VALVE_NSE2025_VALVE_IS_ENABLED(jNSE2025_VALVE_NSE2025_VALVE_ptr) \
    (JNSE2025_VALVE_NSE2025_VALVE_FLAG(jNSE2025_VALVE_NSE2025_VALVE_ptr, enabled) == JNSE2025_VALVE_NSE2025_VALVE_FLAG_SET)

#endif /* _JNSE2025_VALVE_NSE2025_VALVE_H_ */
