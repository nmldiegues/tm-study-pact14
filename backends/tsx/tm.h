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

#include <immintrin.h>
#include <rtmintrin.h>

#  define TM_ARG                        /* nothing */
#  define TM_ARG_ALONE                  /* nothing */
#  define TM_ARGDECL                    /* nothing */
#  define TM_ARGDECL_ALONE              /* nothing */
#  define TM_CALLABLE                   /* nothing */

#  define TM_STARTUP(numThread)         the_lock = init_lock_global(numThread); local_th_data = (lock_local_data *)malloc(numThread * sizeof(lock_local_data)); aux_lock = init_lock_global(numThread); aux_local = (lock_local_data *)malloc(numThread * sizeof(lock_local_data));
#  define TM_SHUTDOWN()                 /* nothing */

#  define TM_THREAD_ENTER()             /* nothing */
#  define TM_THREAD_EXIT()              /* nothing */
#  define TM_BEGIN_WAIVER()
#  define TM_END_WAIVER()

#  define P_MALLOC(size)                malloc(size)  //memory_get(thread_getId(), size)
#  define P_FREE(ptr)                   free(ptr)    /* TODO: thread local free is non-trivial */
#  define TM_MALLOC(size)               malloc(size)  //memory_get(thread_getId(), size)
#  define TM_FREE(ptr)                  free(ptr)    /* TODO: thread local free is non-trivial */

#    define TM_BEGIN()	{ \
							int tries = 4;	\
	while (1) {	\
							int status = _xbegin();	\
							if (status == _XBEGIN_STARTED) { \
                                if (is_free(&(local_th_data[phys_id]), &the_lock) == 0) _xabort(30); \
	                                break;	\
							} else { \
								tries--;	\
								if (tries == 2) acquire_write(&(aux_local[phys_id]), &aux_lock); \
								else if (tries <= 0) {	\
									acquire_write(&(local_th_data[phys_id]), &the_lock); \
	                                break; 	\
								}	\
								if (tries > 2) { \
								    if (tries > 2 && random_generate(randomFallback) % 100 < FALLBACK_PROB) tries = 3; \
								    else tries = 4; \
								} \
							}	\
	}


#    define TM_END()		if (tries > 0) {	\
								_xend();	\
								if (tries <= 2) { release_write(cluster_id, &(aux_local[phys_id]), &aux_lock); } \
							} else {	\
								release_write(cluster_id, &(local_th_data[phys_id]), &the_lock); \
								release_write(cluster_id, &(aux_local[phys_id]), &aux_lock); \
                            }\
                     	};



#    define TM_BEGIN_RO()                 TM_BEGIN()
#    define TM_RESTART()                  _xabort(0xab);
#    define TM_EARLY_RELEASE(var)         

#  define TM_SHARED_READ(var)         (var)
#  define TM_SHARED_WRITE(var, val)   ({var = val; var;})
#  define TM_LOCAL_WRITE(var, val)    ({var = val; var;})

#  define TM_SHARED_READ_I(var)         (var)
#  define TM_SHARED_READ_L(var)         (var)
#  define TM_SHARED_READ_P(var)         (var)
#  define TM_SHARED_READ_D(var)         (var)

#  define TM_SHARED_WRITE_I(var, val)   ({var = val; var;})
#  define TM_SHARED_WRITE_L(var, val)   ({var = val; var;})
#  define TM_SHARED_WRITE_P(var, val)   ({var = val; var;})
#  define TM_SHARED_WRITE_D(var, val)   ({var = val; var;})

#  define TM_LOCAL_WRITE_I(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_L(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_P(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_D(var, val)    ({var = val; var;})

#endif /* TM_H */
