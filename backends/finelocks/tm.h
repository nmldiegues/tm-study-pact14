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

#  include <assert.h>
#  include "memory.h"
#  include "thread.h"
#  include "types.h"

#  define TM_ARG                        /* nothing */
#  define TM_ARG_ALONE                  /* nothing */
#  define TM_ARGDECL                    /* nothing */
#  define TM_ARGDECL_ALONE              /* nothing */
#  define TM_CALLABLE                   /* nothing */

#  define TM_STARTUP(numThread)         { int i = 0; for (; i < LOCKS; i++) { THREAD_MUTEX_INIT(global_rtm_mutex[i]); flags[i] = 0; } }
#  define TM_SHUTDOWN()                 /* nothing */

#  define LI_HASH(ptr, addr)			{ unsigned int tmp = (int)((size_t)ptr); unsigned int tmp2 = (int)LOCKS; unsigned int idx = (tmp *2654435761) % tmp2; *(addr) = idx; }
#  define TM_BEGIN_ARGS(arr, len)				{ quickSort(arr, 0, len); unsigned int i = 0; for (; i < len; i++) { unsigned int idx = arr[i]; if (flags[idx] == 0) { flags[idx] = 1; pthread_spin_lock(&(global_rtm_mutex[idx])); } } }
#  define TM_END_ARGS(arr, len)			{ unsigned int i = 0; for (; i < len; i++) { unsigned int idx = arr[i]; if (flags[idx] == 1) { flags[idx] = 0; pthread_spin_unlock(&(global_rtm_mutex[idx])); } } }
#  define SINGLE_LOCK(ptr)              { unsigned int tmp = (int)((size_t)ptr); unsigned int tmp2 = (int)LOCKS; unsigned int idx = (tmp *2654435761) % tmp2; pthread_spin_lock(&(global_rtm_mutex[idx])); }
#  define SINGLE_UNLOCK(ptr)              { unsigned int tmp = (int)((size_t)ptr); unsigned int tmp2 = (int)LOCKS; unsigned int idx = (tmp *2654435761) % tmp2; pthread_spin_unlock(&(global_rtm_mutex[idx])); }
#  define TM_BEGIN()                    /* nothing */
#  define TM_END()                      /* nothing */

#  define TM_THREAD_ENTER()             /* nothing */
#  define TM_THREAD_EXIT()              /* nothing */
#  define TM_BEGIN_WAIVER()
#  define TM_END_WAIVER()

#  define P_MALLOC(size)                malloc(size)  //memory_get(thread_getId(), size)
#  define P_FREE(ptr)                   free(ptr)    /* TODO: thread local free is non-trivial */
#  define TM_MALLOC(size)               malloc(size)  //memory_get(thread_getId(), size)
#  define TM_FREE(ptr)                  free(ptr)    /* TODO: thread local free is non-trivial */

#  define TM_SHARED_READ(var)         (var)
#  define TM_SHARED_WRITE(var, val)   ({var = val; var;})

#  define TM_SHARED_READ_I(var)         (var)
#  define TM_SHARED_READ_L(var)         (var)
#  define TM_SHARED_READ_P(var)         (var)
#  define TM_SHARED_READ_D(var)         (var)

#  define TM_SHARED_WRITE_I(var, val)   ({var = val; var;})
#  define TM_SHARED_WRITE_L(var, val)   ({var = val; var;})
#  define TM_SHARED_WRITE_P(var, val)   ({var = val; var;})
#  define TM_SHARED_WRITE_D(var, val)   ({var = val; var;})

#  define TM_LOCAL_WRITE(var, val)       ({var = val; var;})
#  define TM_LOCAL_WRITE_I(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_L(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_P(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_D(var, val)    ({var = val; var;})

#endif /* TM_H */
