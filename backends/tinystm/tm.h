#ifndef TM_H
#define TM_H 1

#  include <stdio.h>

#  define MAIN(argc, argv)              int main (int argc, char** argv)
#  define MAIN_RETURN(val)              return val

#  define GOTO_SIM()                    /* nothing */
#  define GOTO_REAL()                   /* nothing */
#  define IS_IN_SIM()                   (0)

#  define SIM_GET_NUM_CPU(var)          /* nothing */

#  define TM_PRINTF                     printf
#  define TM_PRINT0                     printf
#  define TM_PRINT1                     printf
#  define TM_PRINT2                     printf
#  define TM_PRINT3                     printf

#  define P_MEMORY_STARTUP(numThread)   /* nothing */
#  define P_MEMORY_SHUTDOWN()           /* nothing */


#  include <string.h>
#  include <stm.h>
#  include "thread.h"

#    define TM_ARG                        /* nothing */
#    define TM_ARG_ALONE                  /* nothing */
#    define TM_ARGDECL                    /* nothing */
#    define TM_ARGDECL_ALONE              /* nothing */
#    define TM_CALLABLE                   /* nothing */

#      include <mod_mem.h>
#      include <mod_stats.h>

#      define TM_STARTUP(numThread)     if (sizeof(long) != sizeof(void *)) { \
                                          fprintf(stderr, "Error: unsupported long and pointer sizes\n"); \
                                          exit(1); \
                                        } \
                                        stm_init(); \
                                        mod_mem_init(0); \
                                        if (getenv("STM_STATS") != NULL) { \
                                          mod_stats_init(); \
                                        }
#      define TM_SHUTDOWN()             if (getenv("STM_STATS") != NULL) { \
                                          unsigned long u; \
                                          if (stm_get_global_stats("global_nb_commits", &u) != 0) \
                                            printf("#commits    : %lu\n", u); \
                                          if (stm_get_global_stats("global_nb_aborts", &u) != 0) \
                                            printf("#aborts     : %lu\n", u); \
                                          if (stm_get_global_stats("global_max_retries", &u) != 0) \
                                            printf("Max retries : %lu\n", u); \
                                        } \
                                        stm_exit()

#      define TM_THREAD_ENTER()         stm_init_thread()
#      define TM_THREAD_EXIT()          stm_exit_thread()

#      define P_MALLOC(size)            malloc(size)
#      define P_FREE(ptr)               free(ptr)
#      define TM_MALLOC(size)           stm_malloc(size)
#      define TM_FREE(ptr)              stm_free(ptr, sizeof(stm_word_t))


#    define TM_START(ro)                do { \
                                            stm_tx_attr_t _a = {{.read_only = ro}}; \
                                            sigjmp_buf *_e = stm_start(_a); \
                                            if (_e != NULL) sigsetjmp(*_e, 0); \
                                        } while (0)
#    define TM_BEGIN()                  TM_START(0)
#    define TM_BEGIN_RO()               TM_START(1)
#    define TM_END()                    stm_commit()
#    define TM_RESTART()                stm_abort(0)

#    define TM_EARLY_RELEASE(var)       /* nothing */

#  include <wrappers.h>


#  define TM_SHARED_READ(var)           stm_load((volatile stm_word_t *)(void *)&(var))
#  define TM_SHARED_READ_P(var)         stm_load_ptr((volatile void **)(void *)&(var))
#  define TM_SHARED_READ_D(var)         stm_load_double((volatile float *)(void *)&(var))

#  define TM_SHARED_WRITE(var, val)     stm_store((volatile stm_word_t *)(void *)&(var), (stm_word_t)val)
#  define TM_SHARED_WRITE_P(var, val)   stm_store_ptr((volatile void **)(void *)&(var), val)
#  define TM_SHARED_WRITE_D(var, val)   stm_store_double((volatile float *)(void *)&(var), val)

#  define TM_LOCAL_WRITE(var, val)      ({var = val; var;})
#  define TM_LOCAL_WRITE_P(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_D(var, val)    ({var = val; var;})

#endif /* TM_H */

