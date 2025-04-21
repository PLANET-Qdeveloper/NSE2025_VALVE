#include "jNSE2025_VALVE.h"
#include "filtering_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "filtering_templates.h"
#include "type_abbrev.h"

#define IIR_DEFINE_NSE2025_VALVE(suffix, output_type)                                              \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_iir_lattice_##suffix##_NSE2025_VALVE,                                     \
         arm_iir_lattice_##suffix)                                                        \
   {                                                                                      \
      arm_iir_lattice_instance_##suffix iir_inst_fut = { 0 };                             \
      arm_iir_lattice_instance_##suffix iir_inst_ref = { 0 };                             \
                                                                                          \
      TEMPLATE_DO_ARR_DESC(                                                               \
            blocksize_idx, uint32_t, blockSize, filtering_blocksizes                      \
            ,                                                                             \
         TEMPLATE_DO_ARR_DESC(                                                            \
               numstages_idx, uint16_t, numStages, filtering_numstages                    \
               ,                                                                          \
              /* Display NSE2025_VALVE parameter values */                                         \
              JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                                          \
                              "Number of Stages: %d\n",                                   \
                              (int)blockSize,                                             \
                              (int)numStages);                                            \
                                                                                          \
               /* Initialize the IIR Instances */                                         \
               arm_iir_lattice_init_##suffix(                                             \
                     &iir_inst_fut, numStages, (output_type*)filtering_coeffs_b_##suffix, \
                     (output_type*)filtering_coeffs_c_##suffix,                           \
                     (void *) filtering_pState, blockSize);                               \
                                                                                          \
               JNSE2025_VALVE_COUNT_CYCLES(                                                        \
                     arm_iir_lattice_##suffix(                                            \
                           &iir_inst_fut,                                                 \
                           (void *) filtering_##suffix##_inputs,                          \
                           (void *) filtering_output_fut,                                 \
                           blockSize));                                                   \
                                                                                          \
               arm_iir_lattice_init_##suffix(                                             \
                     &iir_inst_ref, numStages, (output_type*)filtering_coeffs_b_##suffix, \
                     (output_type*)filtering_coeffs_c_##suffix,                           \
                     (void *) filtering_pState, blockSize);                               \
                                                                                          \
               ref_iir_lattice_##suffix(                                                  \
                     &iir_inst_ref,                                                       \
                     (void *) filtering_##suffix##_inputs,                                \
                     (void *) filtering_output_ref,                                       \
                     blockSize);                                                          \
                                                                                          \
               FILTERING_SNR_COMPARE_INTERFACE(                                           \
                     blockSize,                                                           \
                     output_type)));                                                      \
                                                                                          \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                                     \
   }

IIR_DEFINE_NSE2025_VALVE(f32, float32_t);
IIR_DEFINE_NSE2025_VALVE(q31, q31_t);
IIR_DEFINE_NSE2025_VALVE(q15, q15_t);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(iir_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_iir_lattice_f32_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_iir_lattice_q31_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_iir_lattice_q15_NSE2025_VALVE);
}
