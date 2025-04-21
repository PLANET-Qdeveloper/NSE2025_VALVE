#ifndef _JNSE2025_VALVE_UTIL_H_
#define _JNSE2025_VALVE_UTIL_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include "util/util.h"

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/* Define boolean values for the framework. */
#define JNSE2025_VALVE_TRUE 1  /**< Value used for TRUE in JNSE2025_VALVE. */
#define JNSE2025_VALVE_FALSE 0 /**< Value used for FALSE in JNSE2025_VALVE. */

/**
 *  Set the value of the attribute in the struct to by struct_ptr to value.
 */
#define JNSE2025_VALVE_SET_STRUCT_ATTRIBUTE(struct_ptr, attribute, value) \
    do                                                                    \
    {                                                                     \
        (struct_ptr)->attribute = (value);                                \
    } while (0)

#endif /* _JNSE2025_VALVE_UTIL_H_ */
