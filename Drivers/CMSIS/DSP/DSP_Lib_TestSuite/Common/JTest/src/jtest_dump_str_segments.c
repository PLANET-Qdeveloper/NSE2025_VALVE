#include "jNSE2025_VALVE_fw.h"

/**
 *  Dump the JNSE2025_VALVE_FW.str_buffer the Keil framework in pieces.
 *
 *  The JNSE2025_VALVE_FW.str_buffer contains more characters than the Keil framework can
 *  dump at once. This function dumps them in blocks.
 */
void jNSE2025_VALVE_dump_str_segments(void)
{
    uint32_t seg_idx      = 0;
    uint32_t memmove_idx = 0;
    uint32_t seg_cnt  =
        (strlen(JNSE2025_VALVE_FW.str_buffer) / JNSE2025_VALVE_STR_MAX_OUTPUT_SIZE) + 1;

    for( seg_idx = 0; seg_idx < seg_cnt; ++seg_idx)
    {
        JNSE2025_VALVE_TRIGGER_ACTION(dump_str);

        if (seg_idx < JNSE2025_VALVE_STR_MAX_OUTPUT_SEGMENTS)
        {
            memmove_idx = 0;
            while (memmove_idx < (seg_cnt - seg_idx -1) )
            {
                memmove(
                    JNSE2025_VALVE_FW.str_buffer+
                    (memmove_idx* JNSE2025_VALVE_STR_MAX_OUTPUT_SIZE),
                    JNSE2025_VALVE_FW.str_buffer+
                    ((memmove_idx+1)*JNSE2025_VALVE_STR_MAX_OUTPUT_SIZE),
                    JNSE2025_VALVE_BUF_SIZE);
                ++memmove_idx;
            }
        }
    }
    return;
}
