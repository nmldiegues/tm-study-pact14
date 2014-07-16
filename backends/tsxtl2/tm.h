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
#include "stm.h"
#include "tl2.h"

#define STM_SELF                        Self

#    define TM_ARG                        STM_SELF,
#    define TM_ARG_ALONE                  STM_SELF
#    define TM_ARGDECL                    STM_THREAD_T* TM_ARG
#    define TM_ARGDECL_ALONE              STM_THREAD_T* TM_ARG_ALONE
#    define TM_CALLABLE                   /* nothing */

#      define TM_STARTUP(numThread)     STM_STARTUP()
#      define TM_SHUTDOWN()             STM_SHUTDOWN()

#      define TM_THREAD_ENTER()         TM_ARGDECL_ALONE = STM_NEW_THREAD(); \
                                        STM_INIT_THREAD(TM_ARG_ALONE, thread_getId()); \
                                        abortFunPtr = &abortHTM; \
                                        sharedReadFunPtr = &sharedReadHTM;  \
                                        sharedWriteFunPtr = &sharedWriteHTM;    \
                                        localWriteFunPtr = &localWriteHTM


#  define TM_THREAD_EXIT()              /* nothing */
#  define TM_BEGIN_WAIVER()
#  define TM_END_WAIVER()

#    define TM_BEGIN() { \
                    int tries = 4;	\
                    while (1) {	\
                        while (is_fallback != 0) {} \
                            if (tries > 0) { \
    							int status = _xbegin();	\
    							if (status == _XBEGIN_STARTED) { \
    							    if (is_fallback != 0) { _xabort(0xab); } \
    							    next_commit = NEXT_CLOCK(); \
    	                            break;	\
    							} else { \
                                    if (random_generate(randomFallback) % 100 < FALLBACK_PROB) tries = 0; \
    							}	\
    						} else {  \
    						    tries = 0; \
    						    STM_BEGIN_WR(); \
                                abortFunPtr = &abortSTM;    \
                                sharedReadFunPtr = &sharedReadSTM;  \
                                sharedWriteFunPtr = &sharedWriteSTM;    \
                                localWriteFunPtr = &localWriteSTM;  \
                                freeFunPtr = &freeSTM; \
				                mallocFunPtr = &mallocSTM; \
                                break;  \
    						} \
	}


#    define TM_END()		if (tries > 0) {	\
								_xend();	\
							} else {	\
							    int retriesCommit = 4; \
							    while (1) {  \
							         int status = _xbegin();  \
							         if (status == _XBEGIN_STARTED) { \
							             if (is_fallback != 0) { _xabort(0xab); } \
							             long res_htm = HYBRID_HTM_END(); \
							             if (res_htm == 0) { _xabort(0xab); }   \
                                         _xend();	\
                                         AFTER_COMMIT();    \
							             break;  \
							         } else if ((random_generate(randomFallback) % 100 < FALLBACK_PROB) || _XABORT_CODE(status) == 0xab) { \
							             __sync_add_and_fetch(&is_fallback,1);    \
                                         int ret = HYBRID_STM_END();  \
							             __sync_sub_and_fetch(&is_fallback,1);    \
							             if (ret == 0) { \
							             	STM_RESTART(); \
							             } \
								         AFTER_COMMIT(); \
							             break;  \
							         } \
							    }    \
                                abortFunPtr = &abortHTM;    \
                                sharedReadFunPtr = &sharedReadHTM;  \
                                sharedWriteFunPtr = &sharedWriteHTM;    \
                                localWriteFunPtr = &localWriteHTM;  \
                                freeFunPtr = &freeHTM; \
                                mallocFunPtr = &freeHTM; \
                            } \
                     	};




#    define TM_BEGIN_RO()                 TM_BEGIN()
#    define TM_RESTART()                  (*abortFunPtr)(Self);
#    define TM_EARLY_RELEASE(var)         

#      define P_MALLOC(size)            malloc(size)
#      define P_FREE(ptr)               free(ptr)
#      define SEQ_MALLOC(size)          malloc(size)
#      define SEQ_FREE(ptr)             free(ptr)

#      define TM_MALLOC(size)           malloc(size)
#      define TM_FREE(ptr)              /* */

#  define TM_SHARED_READ(var)         (*sharedReadFunPtr)(Self, (vintp*)(void*)&(var))
#  define TM_SHARED_READ_P(var)         IP2VP(TM_SHARED_READ(var))
#  define TM_SHARED_READ_D(var)         IP2D((*sharedReadFunPtr)(Self, (vintp*)DP2IPP(&(var))))

#  define TM_SHARED_WRITE(var, val)   (*sharedWriteFunPtr)(Self, (vintp*)(void*)&(var), (intptr_t)val)
#  define TM_SHARED_WRITE_P(var, val)   (*sharedWriteFunPtr)(Self, (vintp*)(void*)&(var), VP2IP(val))
#  define TM_SHARED_WRITE_D(var, val)   (*sharedWriteFunPtr)(Self, (vintp*)DP2IPP(&(var)), D2IP(val))

#  define TM_LOCAL_WRITE(var, val)      ({var = val; var;})
#  define TM_LOCAL_WRITE_P(var, val)    ({var = val; var;})
#  define TM_LOCAL_WRITE_F(var, val)    ({var = val; var;})

#endif /* TM_H */