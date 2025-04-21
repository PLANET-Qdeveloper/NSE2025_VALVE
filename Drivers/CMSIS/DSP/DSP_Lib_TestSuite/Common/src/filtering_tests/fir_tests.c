#include "jNSE2025_VALVE.h"
#include "filtering_NSE2025_VALVE_data.h"
#include "arr_desc.h"
#include "arm_math.h"           /* FUTs */
#include "ref.h"                /* Reference Functions */
#include "NSE2025_VALVE_templates.h"
#include "filtering_templates.h"
#include "type_abbrev.h"

#define FIR_DEFINE_NSE2025_VALVE(suffix, config_suffix, output_type)             \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_fir##config_suffix##_##suffix##_NSE2025_VALVE,          \
         arm_fir##config_suffix##_##suffix)                             \
   {                                                                    \
      arm_fir_instance_##suffix fir_inst_fut = { 0 };                   \
      arm_fir_instance_##suffix fir_inst_ref = { 0 };                   \
                                                                        \
      TEMPLATE_DO_ARR_DESC(                                             \
            blocksize_idx, uint32_t, blockSize, filtering_blocksizes    \
            ,                                                           \
         TEMPLATE_DO_ARR_DESC(                                          \
               numtaps_idx, uint16_t, numTaps, filtering_numtaps        \
               ,                                                        \
               /* Initialize the FIR Instances */                       \
               arm_fir_init_##suffix(                                   \
                     &fir_inst_fut, numTaps,                            \
                     (output_type*)filtering_coeffs_##suffix,           \
                     (void *) filtering_pState, blockSize);             \
                                                                        \
              /* Display NSE2025_VALVE parameter values */                       \
              JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                        \
                              "Number of Taps: %d\n",                   \
                         (int)blockSize,                                \
                         (int)numTaps);                                 \
                                                                        \
               JNSE2025_VALVE_COUNT_CYCLES(                                      \
                     arm_fir##config_suffix##_##suffix(                 \
                           &fir_inst_fut,                               \
                           (void *) filtering_##suffix##_inputs,        \
                           (void *) filtering_output_fut,               \
                           blockSize));                                 \
                                                                        \
               arm_fir_init_##suffix(                                   \
                     &fir_inst_ref, numTaps,                            \
                     (output_type*)filtering_coeffs_##suffix,           \
                     (void *) filtering_pState, blockSize);             \
                                                                        \
               ref_fir##config_suffix##_##suffix(                       \
                     &fir_inst_ref,                                     \
                     (void *) filtering_##suffix##_inputs,              \
                     (void *) filtering_output_ref,                     \
                     blockSize);                                        \
                                                                        \
               FILTERING_SNR_COMPARE_INTERFACE(                         \
                     blockSize,                                         \
                     output_type)));                                    \
                                                                        \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                   \
   }

#define FIR_INTERPOLATE_DEFINE_NSE2025_VALVE(suffix, output_type)                      \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_fir_interpolate_##suffix##_NSE2025_VALVE,                     \
         arm_fir_interpolate_##suffix)                                        \
   {                                                                          \
      arm_fir_interpolate_instance_##suffix fir_inst_fut = { 0 };             \
      arm_fir_interpolate_instance_##suffix fir_inst_ref = { 0 };             \
                                                                              \
      TEMPLATE_DO_ARR_DESC(                                                   \
            blocksize_idx, uint32_t, blockSize, filtering_blocksizes          \
            ,                                                                 \
         TEMPLATE_DO_ARR_DESC(                                                \
               numtaps_idx, uint16_t, numTaps, filtering_numtaps2             \
               ,                                                              \
            TEMPLATE_DO_ARR_DESC(                                             \
                  L_idx, uint8_t, L, filtering_Ls                             \
                  ,                                                           \
                  /* Display NSE2025_VALVE parameter values */                         \
                  JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                          \
                                 "Number of Taps: %d\n"                       \
                                 "Upsample factor: %d\n",                     \
                                (int)blockSize,                               \
                                (int)numTaps,                                 \
                                (int)L);                                      \
                                                                              \
                  /* Initialize the FIR Instances */                          \
                  arm_fir_interpolate_init_##suffix(                          \
                        &fir_inst_fut, L, numTaps,                            \
                        (output_type*)filtering_coeffs_##suffix,              \
                        (void *) filtering_pState, blockSize);                \
                                                                              \
                  JNSE2025_VALVE_COUNT_CYCLES(                                         \
                        arm_fir_interpolate_##suffix(                         \
                              &fir_inst_fut,                                  \
                              (void *) filtering_##suffix##_inputs,           \
                              (void *) filtering_output_fut,                  \
                              blockSize));                                    \
                                                                              \
                  arm_fir_interpolate_init_##suffix(                          \
                        &fir_inst_ref, L, numTaps,                            \
                        (output_type*)filtering_coeffs_##suffix,              \
                        (void *) filtering_pState, blockSize);                \
                                                                              \
                  ref_fir_interpolate_##suffix(                               \
                        &fir_inst_ref,                                        \
                        (void *) filtering_##suffix##_inputs,                 \
                        (void *) filtering_output_ref,                        \
                        blockSize);                                           \
                                                                              \
                  FILTERING_SNR_COMPARE_INTERFACE(                            \
                        blockSize * (uint32_t)L,                              \
                        output_type))));                                      \
                                                                              \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                         \
   }

#define FIR_DECIMATE_DEFINE_NSE2025_VALVE(suffix, config_suffix, output_type)       \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_fir_decimate##config_suffix##_##suffix##_NSE2025_VALVE,    \
         arm_fir_decimate##config_suffix##_##suffix)                       \
   {                                                                       \
      arm_fir_decimate_instance_##suffix fir_inst_fut = { 0 };             \
      arm_fir_decimate_instance_##suffix fir_inst_ref = { 0 };             \
                                                                           \
      TEMPLATE_DO_ARR_DESC(                                                \
            blocksize_idx, uint32_t, blockSize, filtering_blocksizes       \
            ,                                                              \
         TEMPLATE_DO_ARR_DESC(                                             \
               numtaps_idx, uint16_t, numTaps, filtering_numtaps           \
               ,                                                           \
            TEMPLATE_DO_ARR_DESC(                                          \
                  M_idx, uint8_t, M, filtering_Ms                          \
                  ,                                                        \
                  if (blockSize % M == 0)                                   \
                  {                                                        \
                     /* Display NSE2025_VALVE parameter values */                   \
                     JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                    \
                                     "Number of Taps: %d\n"                \
                                     "Decimation Factor: %d\n",            \
                                     (int)blockSize,                       \
                                     (int)numTaps,                         \
                                     (int)M);                              \
                                                                           \
                     /* Initialize the FIR Instances */                    \
                     arm_fir_decimate_init_##suffix(                       \
                           &fir_inst_fut, numTaps, M,                      \
                           (output_type*)filtering_coeffs_##suffix,        \
                           (void *) filtering_pState, blockSize);          \
                                                                           \
                     JNSE2025_VALVE_COUNT_CYCLES(                                   \
                           arm_fir_decimate##config_suffix##_##suffix(     \
                                 &fir_inst_fut,                            \
                                 (void *) filtering_##suffix##_inputs,     \
                                 (void *) filtering_output_fut,            \
                                 blockSize));                              \
                                                                           \
                     arm_fir_decimate_init_##suffix(                       \
                           &fir_inst_ref, numTaps, M,                      \
                           (output_type*)filtering_coeffs_##suffix,        \
                           (void *) filtering_pState, blockSize);          \
                                                                           \
                     ref_fir_decimate##config_suffix##_##suffix(           \
                           &fir_inst_ref,                                  \
                           (void *) filtering_##suffix##_inputs,           \
                           (void *) filtering_output_ref,                  \
                           blockSize);                                     \
                                                                           \
                     FILTERING_SNR_COMPARE_INTERFACE(                      \
                           blockSize / M,                                  \
                           output_type);                                   \
                  })));                                                    \
                                                                           \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                      \
   }

#define FIR_LATTICE_DEFINE_NSE2025_VALVE(suffix, output_type)                       \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_fir_lattice_##suffix##_NSE2025_VALVE,                      \
         arm_fir_lattice_##suffix)                                         \
   {                                                                       \
      arm_fir_lattice_instance_##suffix fir_inst_fut = { 0 };              \
      arm_fir_lattice_instance_##suffix fir_inst_ref = { 0 };              \
                                                                           \
      TEMPLATE_DO_ARR_DESC(                                                \
            blocksize_idx, uint32_t, blockSize, filtering_blocksizes       \
            ,                                                              \
         TEMPLATE_DO_ARR_DESC(                                             \
               numstages_idx, uint16_t, numStages, filtering_numstages     \
               ,                                                           \
               /* Display NSE2025_VALVE parameter values */                         \
               JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                          \
                               "Number of Stages: %d\n",                   \
                               (int)blockSize,                             \
                               (int)numStages);                            \
                                                                           \
               /* Initialize the FIR Instances */                          \
               arm_fir_lattice_init_##suffix(                              \
                     &fir_inst_fut, numStages,                             \
                     (output_type*)filtering_coeffs_##suffix,              \
                     (void *) filtering_pState);                           \
                                                                           \
               JNSE2025_VALVE_COUNT_CYCLES(                                         \
                     arm_fir_lattice_##suffix(                             \
                           &fir_inst_fut,                                  \
                           (void *) filtering_##suffix##_inputs,           \
                           (void *) filtering_output_fut,                  \
                           blockSize));                                    \
                                                                           \
               arm_fir_lattice_init_##suffix(                              \
                     &fir_inst_ref, numStages,                             \
                     (output_type*)filtering_coeffs_##suffix,              \
                     (void *) filtering_pState);                           \
                                                                           \
               ref_fir_lattice_##suffix(                                   \
                     &fir_inst_ref,                                        \
                     (void *) filtering_##suffix##_inputs,                 \
                     (void *) filtering_output_ref,                        \
                     blockSize);                                           \
                                                                           \
               FILTERING_SNR_COMPARE_INTERFACE(                            \
                     blockSize,                                            \
                     output_type)));                                       \
                                                                           \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                      \
   }


#define FIR_SPARSE_DEFINE_NSE2025_VALVE(suffix, output_type)                     \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_fir_sparse_##suffix##_NSE2025_VALVE,                    \
         arm_fir_sparse_##suffix)                                       \
   {                                                                    \
      arm_fir_sparse_instance_##suffix fir_inst_fut = { 0 };            \
      arm_fir_sparse_instance_##suffix fir_inst_ref = { 0 };            \
                                                                        \
      TEMPLATE_DO_ARR_DESC(                                             \
            blocksize_idx, uint32_t, blockSize, filtering_blocksizes    \
            ,                                                           \
         TEMPLATE_DO_ARR_DESC(                                          \
               numtaps_idx, uint16_t, numTaps, filtering_numtaps        \
               ,                                                        \
               /* Display NSE2025_VALVE parameter values */                      \
               JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                       \
                               "Number of Taps: %d\n"                   \
                               "Tap Delay: %d\n",                       \
                               (int)blockSize,                          \
                               (int)numTaps,                            \
                               (int)FILTERING_MAX_TAP_DELAY);           \
                                                                        \
               /* Initialize the FIR Instances */                       \
               arm_fir_sparse_init_##suffix(                            \
                     &fir_inst_fut, numTaps,                            \
                     (output_type*)filtering_coeffs_##suffix,           \
                     (void *) filtering_pState,                         \
                     (int32_t*)filtering_tap_delay,                     \
                     FILTERING_MAX_TAP_DELAY, blockSize);               \
                                                                        \
               JNSE2025_VALVE_COUNT_CYCLES(                                      \
                     arm_fir_sparse_##suffix(                           \
                           &fir_inst_fut,                               \
                           (void *) filtering_##suffix##_inputs,        \
                           (void *) filtering_output_fut,               \
                           (void *) filtering_scratch,                  \
                           blockSize));                                 \
                                                                        \
               arm_fir_sparse_init_##suffix(                            \
                     &fir_inst_ref, numTaps,                            \
                     (output_type*)filtering_coeffs_##suffix,           \
                     (void *) filtering_pState,                         \
                     (int32_t*)filtering_tap_delay,                     \
                     FILTERING_MAX_TAP_DELAY, blockSize);               \
                                                                        \
               ref_fir_sparse_##suffix(                                 \
                     &fir_inst_ref,                                     \
                     (void *) filtering_##suffix##_inputs,              \
                     (void *) filtering_output_ref,                     \
                     (void *) filtering_scratch,                        \
                     blockSize);                                        \
                                                                        \
               FILTERING_SNR_COMPARE_INTERFACE(                         \
                     blockSize,                                         \
                     output_type)));                                    \
                                                                        \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                   \
   }

#define FIR_SPARSE2_DEFINE_NSE2025_VALVE(suffix, output_type)                    \
   JNSE2025_VALVE_DEFINE_NSE2025_VALVE(arm_fir_sparse_##suffix##_NSE2025_VALVE,                    \
         arm_fir_sparse_##suffix)                                       \
   {                                                                    \
      arm_fir_sparse_instance_##suffix fir_inst_fut = { 0 };            \
      arm_fir_sparse_instance_##suffix fir_inst_ref = { 0 };            \
                                                                        \
      TEMPLATE_DO_ARR_DESC(                                             \
            blocksize_idx, uint32_t, blockSize, filtering_blocksizes    \
            ,                                                           \
         TEMPLATE_DO_ARR_DESC(                                          \
               numtaps_idx, uint16_t, numTaps, filtering_numtaps        \
               ,                                                        \
              /* Display NSE2025_VALVE parameter values */                       \
              JNSE2025_VALVE_DUMP_STRF("Block Size: %d\n"                        \
                              "Number of Taps: %d\n"                    \
                              "Tap Delay: %d\n",                        \
                              (int)blockSize,                           \
                              (int)numTaps,                             \
                              (int)FILTERING_MAX_TAP_DELAY);            \
                                                                        \
               /* Initialize the FIR Instances */                       \
               arm_fir_sparse_init_##suffix(                            \
                     &fir_inst_fut, numTaps,                            \
                     (output_type*)filtering_coeffs_##suffix,           \
                     (void *) filtering_pState,                         \
                     (int32_t*)filtering_tap_delay,                     \
                     FILTERING_MAX_TAP_DELAY, blockSize);               \
                                                                        \
               JNSE2025_VALVE_COUNT_CYCLES(                                      \
                     arm_fir_sparse_##suffix(                           \
                           &fir_inst_fut,                               \
                           (void *) filtering_##suffix##_inputs,        \
                           (void *) filtering_output_fut,               \
                           (void *) filtering_scratch,                  \
                           (void *) filtering_scratch2,                 \
                           blockSize));                                 \
                                                                        \
               arm_fir_sparse_init_##suffix(                            \
                     &fir_inst_ref, numTaps,                            \
                     (output_type*)filtering_coeffs_##suffix,           \
                     (void *) filtering_pState,                         \
                     (int32_t*)filtering_tap_delay,                     \
                     FILTERING_MAX_TAP_DELAY, blockSize);               \
                                                                        \
               ref_fir_sparse_##suffix(                                 \
                     &fir_inst_ref,                                     \
                     (void *) filtering_##suffix##_inputs,              \
                     (void *) filtering_output_ref,                     \
                     (void *) filtering_scratch,                        \
                     (void *) filtering_scratch2,                       \
                     blockSize);                                        \
                                                                        \
               FILTERING_SNR_COMPARE_INTERFACE(                         \
                     blockSize,                                         \
                     output_type)));                                    \
                                                                        \
            return JNSE2025_VALVE_NSE2025_VALVE_PASSED;                                   \
   }

FIR_DEFINE_NSE2025_VALVE(f32,,float32_t);
FIR_DEFINE_NSE2025_VALVE(q31,,q31_t);
FIR_DEFINE_NSE2025_VALVE(q15,,q15_t);
FIR_DEFINE_NSE2025_VALVE(q31,_fast,q31_t);
FIR_DEFINE_NSE2025_VALVE(q15,_fast,q15_t);
FIR_DEFINE_NSE2025_VALVE(q7,,q7_t);

FIR_LATTICE_DEFINE_NSE2025_VALVE(f32,float32_t);
FIR_LATTICE_DEFINE_NSE2025_VALVE(q31,q31_t);
FIR_LATTICE_DEFINE_NSE2025_VALVE(q15,q15_t);

FIR_INTERPOLATE_DEFINE_NSE2025_VALVE(f32,float32_t);
FIR_INTERPOLATE_DEFINE_NSE2025_VALVE(q31,q31_t);
FIR_INTERPOLATE_DEFINE_NSE2025_VALVE(q15,q15_t);

FIR_DECIMATE_DEFINE_NSE2025_VALVE(f32,,float32_t);
FIR_DECIMATE_DEFINE_NSE2025_VALVE(q31,,q31_t);
FIR_DECIMATE_DEFINE_NSE2025_VALVE(q15,,q15_t);
FIR_DECIMATE_DEFINE_NSE2025_VALVE(q31,_fast,q31_t);
FIR_DECIMATE_DEFINE_NSE2025_VALVE(q15,_fast,q15_t);

FIR_SPARSE_DEFINE_NSE2025_VALVE(f32,float32_t);
FIR_SPARSE_DEFINE_NSE2025_VALVE(q31,q31_t);
FIR_SPARSE2_DEFINE_NSE2025_VALVE(q15,q15_t);
FIR_SPARSE2_DEFINE_NSE2025_VALVE(q7,q7_t);

/*--------------------------------------------------------------------------------*/
/* Collect all NSE2025_VALVEs in a group. */
/*--------------------------------------------------------------------------------*/

JNSE2025_VALVE_DEFINE_GROUP(fir_NSE2025_VALVEs)
{
    /*
      To skip a NSE2025_VALVE, comment it out.
    */
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_f32_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_q31_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_q15_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_q7_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_fast_q31_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_fast_q15_NSE2025_VALVE);

   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_lattice_f32_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_lattice_q31_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_lattice_q15_NSE2025_VALVE);

   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_interpolate_f32_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_interpolate_q31_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_interpolate_q15_NSE2025_VALVE);

   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_decimate_f32_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_decimate_q31_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_decimate_q15_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_decimate_fast_q31_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_decimate_fast_q15_NSE2025_VALVE);

   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_sparse_f32_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_sparse_q31_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_sparse_q15_NSE2025_VALVE);
   JNSE2025_VALVE_NSE2025_VALVE_CALL(arm_fir_sparse_q7_NSE2025_VALVE);
}
