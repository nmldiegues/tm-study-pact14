#ifndef TM_H
#define TM_H 1

#include "stm.h"

#include <stdio.h>

#define MAIN(argc, argv)              int main (int argc, char** argv)
#define MAIN_RETURN(val)              return val
#define GOTO_SIM()                    /* nothing */
#define GOTO_REAL()                   /* nothing */
#define IS_IN_SIM()                   (0)
#define SIM_GET_NUM_CPU(var)          /* nothing */
#define TM_PRINTF                     printf
#define TM_PRINT0                     printf
#define TM_PRINT1                     printf
#define TM_PRINT2                     printf
#define TM_PRINT3                     printf

#define P_MEMORY_STARTUP(numThread)   /* nothing */
#define P_MEMORY_SHUTDOWN()           /* nothing */

#include <string.h>

#define TM_ARGDECL_ALONE                tx_desc* tx
#define TM_ARGDECL                      tx_desc* tx,
#define TM_ARG                          tx,
#define TM_ARG_ALONE                    tx

#define TM_STARTUP(numThread)     wlpdstm_global_init()
#define TM_SHUTDOWN()             wlpdstm_global_shutdown()

#define TM_THREAD_ENTER()         wlpdstm_thread_init(); \
                                  tx_desc *tx = wlpdstm_get_tx_desc(); \
                                  wlpdstm_start_thread_profiling_desc(tx)
#define TM_THREAD_EXIT()          wlpdstm_end_thread_profiling_desc(tx); \
                                  wlpdstm_thread_shutdown()

#define P_MALLOC(size)            wlpdstm_s_malloc(size)
#define P_FREE(ptr)               wlpdstm_s_free(ptr)
#define TM_MALLOC(size)           wlpdstm_tx_malloc_desc(tx, size)
#define TM_FREE(ptr)              wlpdstm_tx_free_desc(tx, ptr, sizeof(*ptr))

#define TM_BEGIN()                BEGIN_TRANSACTION_DESC
#define TM_BEGIN_ID(ID)           BEGIN_TRANSACTION_DESC_ID(ID)
#define TM_BEGIN_RO()             STM_BEGIN()
#define TM_END()                  END_TRANSACTION
#define TM_RESTART()              wlpdstm_restart_tx_desc(tx)

#define TM_EARLY_RELEASE(var)       /* nothing */

#define TM_SHARED_READ(var)           wlpdstm_read_word_desc(tx, (Word *)(&var))
#define TM_SHARED_READ_P(var)         (void *)wlpdstm_read_word_desc(tx, (Word *)(&var))
#define TM_SHARED_READ_D(var)         wlpdstm_read_double_desc(tx, (&var))

#define TM_SHARED_WRITE(var, val)     wlpdstm_write_word_desc(tx, (Word *)(&var), (Word)(val))
#define TM_SHARED_WRITE_P(var, val)   wlpdstm_write_word_desc(tx, (Word *)(&var), (Word)(val))
#define TM_SHARED_WRITE_D(var, val)   wlpdstm_write_double_desc(tx, (&var), (val))

#define TM_LOCAL_WRITE(var, val)      ({var = val; var;})
#define TM_LOCAL_WRITE_P(var, val)    ({var = val; var;})
#define TM_LOCAL_WRITE_D(var, val)    ({var = val; var;})

#define TM_CALLABLE                     /* nothing */
#define TM_PURE                         /* nothing */


#endif /* TM_H */

