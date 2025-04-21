#ifndef _JNSE2025_VALVE_FW_H_
#define _JNSE2025_VALVE_FW_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include <stdint.h>            /* int32_t */
#include <string.h>            /* strcpy() */
#include <stdio.h>             /* sprintf() */
#include "jNSE2025_VALVE_pf.h" /* Extend JNSE2025_VALVE_FW_t with Pass/Fail data */
#include "jNSE2025_VALVE_group.h"

/*--------------------------------------------------------------------------------*/
/* Type Definitions */
/*--------------------------------------------------------------------------------*/

/**
 *  A struct used to interface with the Keil Debugger.
 */
typedef struct JNSE2025_VALVE_FW_struct
{
    /* Action Triggers: The Keil debugger monitors these values for changes.  In
     * response to a change, the debugger executes code on the host. */
    volatile int32_t NSE2025_VALVE_start;
    volatile int32_t NSE2025_VALVE_end;
    volatile int32_t group_start;
    volatile int32_t group_end;
    volatile int32_t dump_str;
    volatile int32_t dump_data;
    volatile int32_t exit_fw;

    JNSE2025_VALVE_GROUP_t *current_group_ptr;

    /* Buffers: The C-code cannot send strings and data directly to the
     * debugging framework. Instead, the debugger can be told to read 128 byte
     * (by default) chunks of memory.  Data received in this manner requires
     * post-processing to be legible.*/
    char *str_buffer;
    char *data_buffer;

    /* Pass/Fail Data */
    JNSE2025_VALVE_PF_MEMBERS;

} JNSE2025_VALVE_FW_t;

/*--------------------------------------------------------------------------------*/
/* Macros and Defines */
/*--------------------------------------------------------------------------------*/

/**
 *  Default name for the JNSE2025_VALVE_FW struct.
 *
 *  Define your own if you want the variable containing the #JNSE2025_VALVE_FW_t to have
 *  a different name.
 */
#ifndef JNSE2025_VALVE_FW
#define JNSE2025_VALVE_FW JNSE2025_VALVE_FW
#endif

/**
 *  Default name for the JNSE2025_VALVE_FW_STR_BUFFER.
 *
 *  Define your own if you want the variable containing the char buffer to have
 *  a different name.
 */
#ifndef JNSE2025_VALVE_FW_STR_BUFFER
#define JNSE2025_VALVE_FW_STR_BUFFER JNSE2025_VALVE_FW_STR_BUFFER
#endif

/**
 *  Size of the #JNSE2025_VALVE_FW_t, output string-buffer.
 *
 *  If you change this value, make sure the "dump_str_fn" and "dump_data_fn"
 *  functions in jNSE2025_VALVE_fns.ini uses the same size. If you aren't sure, read the
 *  documentation Keil Debugger Command 'DISPLAY'.
 */
#define JNSE2025_VALVE_BUF_SIZE 256

/**
 *  The maximum number of bytes output at once using #JNSE2025_VALVE_DUMP_STRF().
 */
#define JNSE2025_VALVE_STR_MAX_OUTPUT_SIZE 128

/**
 *  The maximum number of block transimissions needed to send a string from a
 *  buffer with JNSE2025_VALVE_BUF_SIZE.
 */
#define JNSE2025_VALVE_STR_MAX_OUTPUT_SEGMENTS \
    (JNSE2025_VALVE_BUF_SIZE / JNSE2025_VALVE_STR_MAX_OUTPUT_SIZE)

/**
 *  Initialize the JNSE2025_VALVE framework.
 */
#define JNSE2025_VALVE_INIT()                                        \
    do                                                               \
    {                                                                \
        JNSE2025_VALVE_FW.str_buffer = JNSE2025_VALVE_FW_STR_BUFFER; \
    } while (0)

/* Debugger Action-triggering Macros */
/*--------------------------------------------------------------------------------*/

/**
 *  Dispatch macro to trigger various actions in the Keil Debugger.
 */
#define JNSE2025_VALVE_TRIGGER_ACTION(action_name) \
    do                                             \
    {                                              \
        action_name();                             \
    } while (0)

/**
 *  Trigger the "NSE2025_VALVE Start" action in the Keil Debugger.
 */
#define JNSE2025_VALVE_ACT_NSE2025_VALVE_START() \
    JNSE2025_VALVE_TRIGGER_ACTION(NSE2025_VALVE_start)

/**
 *  Trigger the "NSE2025_VALVE End" action in the Keil Debugger.
 */
#define JNSE2025_VALVE_ACT_NSE2025_VALVE_END() \
    JNSE2025_VALVE_TRIGGER_ACTION(NSE2025_VALVE_end)

/**
 *  Trigger the "Group Start" action in the Keil Debugger.
 */
#define JNSE2025_VALVE_ACT_GROUP_START() \
    JNSE2025_VALVE_TRIGGER_ACTION(group_start)

/**
 *  Trigger the "Group End" action in the Keil Debugger.
 */
#define JNSE2025_VALVE_ACT_GROUP_END() \
    JNSE2025_VALVE_TRIGGER_ACTION(group_end)

/**
 *  Fill the buffer named buf_name with value and dump it to the Keil debugger
 *  using action.
 */
#define JNSE2025_VALVE_ACT_DUMP(action, buf_name, value) \
    do                                                   \
    {                                                    \
        JNSE2025_VALVE_CLEAR_BUFFER(buf_name);           \
        strcpy(JNSE2025_VALVE_FW.buf_name, (value));     \
        JNSE2025_VALVE_TRIGGER_ACTION(action);           \
    } while (0)

/**
 *  Trigger the "Exit Framework" action in the Keil Debugger.
 */
#define JNSE2025_VALVE_ACT_EXIT_FW()            \
    do                                          \
    {                                           \
        JNSE2025_VALVE_TRIGGER_ACTION(exit_fw); \
    } while (0)

/* Buffer Manipulation Macros */
/*--------------------------------------------------------------------------------*/

/**
 *  Clear the JNSE2025_VALVE_FW buffer with name buf_name.
 */
#define JNSE2025_VALVE_CLEAR_BUFFER(buf_name)                           \
    do                                                                  \
    {                                                                   \
        memset(JNSE2025_VALVE_FW.buf_name, 0, JNSE2025_VALVE_BUF_SIZE); \
    } while (0)

/**
 *  Clear the memory needed for the JNSE2025_VALVE_FW's string buffer.
 */
#define JNSE2025_VALVE_CLEAR_STR_BUFFER() \
    JNSE2025_VALVE_CLEAR_BUFFER(str_buffer)

/**
 *  Clear the memory needed for the JNSE2025_VALVE_FW's data buffer.
 */
#define JNSE2025_VALVE_CLEAR_DATA_BUFFER() \
    JNSE2025_VALVE_CLEAR_BUFFER(data_buffer)

/**
 *  Dump the given string to the Keil Debugger.
 */
#define JNSE2025_VALVE_DUMP_STR(string) \
    JNSE2025_VALVE_ACT_DUMP(dump_str, str_buffer, string)

/**
 *  Dump a formatted string to the Keil Debugger.
 */
#define JNSE2025_VALVE_DUMP_STRF(format_str, ...)                       \
    do                                                                  \
    {                                                                   \
        JNSE2025_VALVE_CLEAR_STR_BUFFER();                              \
        sprintf(JNSE2025_VALVE_FW.str_buffer, format_str, __VA_ARGS__); \
        jNSE2025_VALVE_dump_str_segments();                             \
    } while (0)

/* Pass/Fail Macros */
/*--------------------------------------------------------------------------------*/

/**
 *  Increment the number of passed NSE2025_VALVEs in #JNSE2025_VALVE_FW.
 */
#define JNSE2025_VALVE_FW_INC_PASSED(amount) \
    JNSE2025_VALVE_PF_INC_PASSED(&JNSE2025_VALVE_FW, amount)

/**
 *  Increment the number of passed NSE2025_VALVEs in #JNSE2025_VALVE_FW.
 */
#define JNSE2025_VALVE_FW_INC_FAILED(amount) \
    JNSE2025_VALVE_PF_INC_FAILED(&JNSE2025_VALVE_FW, amount)

/* Manipulating the Current Group */
/*--------------------------------------------------------------------------------*/

/**
 *  Evaluate to the current_group_ptr in #JNSE2025_VALVE_FW.
 */
#define JNSE2025_VALVE_CURRENT_GROUP_PTR() \
    (JNSE2025_VALVE_FW.current_group_ptr)

#define JNSE2025_VALVE_SET_CURRENT_GROUP(group_ptr)     \
    do                                                  \
    {                                                   \
        JNSE2025_VALVE_CURRENT_GROUP_PTR() = group_ptr; \
    } while (0)

/*--------------------------------------------------------------------------------*/
/* Declare Global Variables */
/*--------------------------------------------------------------------------------*/
extern char JNSE2025_VALVE_FW_STR_BUFFER[JNSE2025_VALVE_BUF_SIZE];
extern volatile JNSE2025_VALVE_FW_t JNSE2025_VALVE_FW;

/*--------------------------------------------------------------------------------*/
/* Function Prototypes */
/*--------------------------------------------------------------------------------*/
void jNSE2025_VALVE_dump_str_segments(void);

void NSE2025_VALVE_start(void);
void NSE2025_VALVE_end(void);
void group_start(void);
void group_end(void);
void dump_str(void);
void dump_data(void);
void exit_fw(void);

#endif /* _JNSE2025_VALVE_FW_H_ */
