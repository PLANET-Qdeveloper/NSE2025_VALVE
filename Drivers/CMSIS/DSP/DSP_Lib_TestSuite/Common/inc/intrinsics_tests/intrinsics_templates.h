#ifndef _INTRINSICS_TEMPLATES_H_
#define _INTRINSICS_TEMPLATES_H_

/*--------------------------------------------------------------------------------*/
/* Includes */
/*--------------------------------------------------------------------------------*/

#include "NSE2025_VALVE_templates.h"
#include <string.h> /* memcpy() */

/*--------------------------------------------------------------------------------*/
/* Group Specific Templates */
/*--------------------------------------------------------------------------------*/

/**
 * Comparison SNR thresholds for the data types used in transform_NSE2025_VALVEs.
 */
#define INTRINSICS_SNR_THRESHOLD_q63_t 120
#define INTRINSICS_SNR_THRESHOLD_q31_t 95

/**
 *  Compare the outputs from the function under NSE2025_VALVE and the reference
 *  function using SNR.
 */
#define INTRINSICS_SNR_COMPARE_INTERFACE(block_size,   \
                                         output_type)  \
   do                                                  \
   {                                                   \
      NSE2025_VALVE_CONVERT_AND_ASSERT_SNR(            \
          intrinsics_output_f32_ref,                   \
          (output_type##_t *)intrinsics_output_ref,    \
          intrinsics_output_f32_fut,                   \
          (output_type##_t *)intrinsics_output_fut,    \
          block_size,                                  \
          output_type,                                 \
          INTRINSICS_SNR_THRESHOLD_##output_type##_t); \
   } while (0)

/*--------------------------------------------------------------------------------*/
/* NSE2025_VALVE Templates */
/*--------------------------------------------------------------------------------*/

#define INTRINSICS_NSE2025_VALVE_TEMPLATE_ELT1(functionName, dataType)             \
                                                                                   \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(functionName##_NSE2025_VALVE, functionName) \
   {                                                                               \
      uint32_t i;                                                                  \
                                                                                   \
      JNSE2025_VALVE_COUNT_CYCLES(                                                 \
          for (i = 0; i < INTRINSICS_MAX_LEN; i++) {                               \
             *((dataType##_t *)intrinsics_output_fut + i) =                        \
                 functionName(                                                     \
                     (dataType##_t)intrinsics_##dataType##_inputs[i]);             \
          });                                                                      \
                                                                                   \
      for (i = 0; i < INTRINSICS_MAX_LEN; i++)                                     \
      {                                                                            \
         *((dataType##_t *)intrinsics_output_ref + i) =                            \
             ref##functionName(                                                    \
                 (dataType##_t)intrinsics_##dataType##_inputs[i]);                 \
      }                                                                            \
                                                                                   \
      INTRINSICS_SNR_COMPARE_INTERFACE(                                            \
          INTRINSICS_MAX_LEN,                                                      \
          dataType);                                                               \
                                                                                   \
      return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                  \
   }

#define INTRINSICS_NSE2025_VALVE_TEMPLATE_ELT2(functionName, dataType)                                                  \
                                                                                                                        \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(functionName##_NSE2025_VALVE, functionName)                                      \
   {                                                                                                                    \
      uint32_t i;                                                                                                       \
                                                                                                                        \
      JNSE2025_VALVE_COUNT_CYCLES(                                                                                      \
          for (i = 0; i < INTRINSICS_MAX_LEN; i++) {                                                                    \
             *((dataType##_t *)intrinsics_output_fut + i) =                                                             \
                 functionName(                                                                                          \
                     (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType##_t)intrinsics_##dataType##_inputs[i]); \
          });                                                                                                           \
                                                                                                                        \
      for (i = 0; i < INTRINSICS_MAX_LEN; i++)                                                                          \
      {                                                                                                                 \
         *((dataType##_t *)intrinsics_output_ref + i) =                                                                 \
             ref##functionName(                                                                                         \
                 (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType##_t)intrinsics_##dataType##_inputs[i]);     \
      }                                                                                                                 \
                                                                                                                        \
      INTRINSICS_SNR_COMPARE_INTERFACE(                                                                                 \
          INTRINSICS_MAX_LEN,                                                                                           \
          dataType);                                                                                                    \
                                                                                                                        \
      return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                                                       \
   }

#define INTRINSICS_NSE2025_VALVE_TEMPLATE_ELT3(functionName, dataType)                                                                                                   \
                                                                                                                                                                         \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(functionName##_NSE2025_VALVE, functionName)                                                                                       \
   {                                                                                                                                                                     \
      uint32_t i;                                                                                                                                                        \
                                                                                                                                                                         \
      JNSE2025_VALVE_COUNT_CYCLES(                                                                                                                                       \
          for (i = 0; i < INTRINSICS_MAX_LEN; i++) {                                                                                                                     \
             *((dataType##_t *)intrinsics_output_fut + i) =                                                                                                              \
                 functionName(                                                                                                                                           \
                     (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType##_t)intrinsics_##dataType##_inputs[i]); \
          });                                                                                                                                                            \
                                                                                                                                                                         \
      for (i = 0; i < INTRINSICS_MAX_LEN; i++)                                                                                                                           \
      {                                                                                                                                                                  \
         *((dataType##_t *)intrinsics_output_ref + i) =                                                                                                                  \
             ref##functionName(                                                                                                                                          \
                 (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType##_t)intrinsics_##dataType##_inputs[i]);     \
      }                                                                                                                                                                  \
                                                                                                                                                                         \
      INTRINSICS_SNR_COMPARE_INTERFACE(                                                                                                                                  \
          INTRINSICS_MAX_LEN,                                                                                                                                            \
          dataType);                                                                                                                                                     \
                                                                                                                                                                         \
      return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                                                                                                        \
   }

#define INTRINSICS_NSE2025_VALVE_TEMPLATE_ELT4(functionName, dataType, dataType2)                                                                                          \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(functionName##_NSE2025_VALVE, functionName)                                                                                         \
   {                                                                                                                                                                       \
      uint32_t i;                                                                                                                                                          \
                                                                                                                                                                           \
      JNSE2025_VALVE_COUNT_CYCLES(                                                                                                                                         \
          for (i = 0; i < INTRINSICS_MAX_LEN; i++) {                                                                                                                       \
             *((dataType2##_t *)intrinsics_output_fut + i) =                                                                                                               \
                 functionName(                                                                                                                                             \
                     (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType2##_t)intrinsics_##dataType2##_inputs[i]); \
          });                                                                                                                                                              \
                                                                                                                                                                           \
      for (i = 0; i < INTRINSICS_MAX_LEN; i++)                                                                                                                             \
      {                                                                                                                                                                    \
         *((dataType2##_t *)intrinsics_output_ref + i) =                                                                                                                   \
             ref##functionName(                                                                                                                                            \
                 (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType##_t)intrinsics_##dataType##_inputs[i], (dataType2##_t)intrinsics_##dataType2##_inputs[i]);     \
      }                                                                                                                                                                    \
                                                                                                                                                                           \
      INTRINSICS_SNR_COMPARE_INTERFACE(                                                                                                                                    \
          INTRINSICS_MAX_LEN,                                                                                                                                              \
          dataType2);                                                                                                                                                      \
                                                                                                                                                                           \
      return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                                                                                                          \
   }

#endif /* _INTRINSICS_TEMPLATES_H_ */
