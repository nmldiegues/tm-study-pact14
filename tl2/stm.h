/* =============================================================================
 *
 * stm.h
 *
 * User program interface for STM. For an STM to interface with STAMP, it needs
 * to have its own stm.h for which it redefines the macros appropriately.
 *
 * =============================================================================
 *
 * Author: Chi Cao Minh
 *
 * =============================================================================
 */


#ifndef STM_H
#define STM_H 1


#include "tl2.h"
#include "util.h"

#define STM_THREAD_T                    Thread
#define STM_SELF                        Self
#define STM_RO_FLAG                     ROFlag

#define STM_MALLOC(size)                TxAlloc(STM_SELF, size)
#define STM_FREE(ptr)                   TxFree(STM_SELF, ptr)

#  define malloc(size)                  tmalloc_reserve_tl2(size)
#  define calloc(n, size)               tm_calloc(n, size)
#  define realloc(ptr, size)            tmalloc_reserve_tl2Again(ptr, size)
#  define free(ptr)                     tmalloc_release(ptr)

#  include <setjmp.h>
#  define STM_JMPBUF_T                  sigjmp_buf
#  define STM_JMPBUF                    buf


#define STM_VALID()                     (1)
#define STM_RESTART()                   TxAbort(STM_SELF)

#define STM_STARTUP()                   TxOnce()
#define STM_SHUTDOWN()                  TxShutdown()

#define STM_NEW_THREAD()                TxNewThread()
#define STM_INIT_THREAD(t, id)          TxInitThread(t, id)
#define STM_FREE_THREAD(t)              TxFreeThread(t)








#  define STM_BEGIN(isReadOnly)         do { \
                                            STM_JMPBUF_T STM_JMPBUF; \
                                            int STM_RO_FLAG = isReadOnly; \
                                            sigsetjmp(STM_JMPBUF, 1); \
                                            TxStart(STM_SELF, &STM_JMPBUF, &STM_RO_FLAG); \
                                        } while (0) /* enforce comma */

#define STM_BEGIN_RD()                  STM_BEGIN(1)
#define STM_BEGIN_WR()                  STM_BEGIN(0)
#define STM_END()                       TxCommit(STM_SELF)
#define HYBRID_HTM_END()				 TxCommitNoAbortHTM(STM_SELF)
#define HYBRID_STM_END()				 TxCommitNoAbortSTM(STM_SELF)
#define AFTER_COMMIT()					 AfterCommit(STM_SELF)
#define NEXT_CLOCK()					 GVGenerateWV(STM_SELF, 0)

typedef volatile intptr_t               vintp;

#define HYBRID_READ(var)		TxLoad(STM_SELF, var)
#define HYBRID_WRITE(var, val)		TxStore(STM_SELF, var, val)

#define STM_READ(var)                   TxLoad(STM_SELF, (vintp*)(void*)&(var))
#define STM_READ_D(var)                 IP2D(TxLoad(STM_SELF, \
                                                    (vintp*)DP2IPP(&(var))))
#define STM_READ_P(var)                 IP2VP(TxLoad(STM_SELF, \
                                                     (vintp*)(void*)&(var)))

#define STM_WRITE(var, val)             TxStore(STM_SELF, \
                                                (vintp*)(void*)&(var), \
                                                (intptr_t)(val))
#define STM_WRITE_D(var, val)           TxStore(STM_SELF, \
                                                (vintp*)DP2IPP(&(var)), \
                                                D2IP(val))
#define STM_WRITE_P(var, val)           TxStore(STM_SELF, \
                                                (vintp*)(void*)&(var), \
                                                VP2IP(val))

#define HTM_WRITE(var, val, clock)	     TxStoreHTM(STM_SELF, var, val, clock)

#define STM_LOCAL_WRITE(var, val)       ({var = val; var;})
#define STM_LOCAL_WRITE_D(var, val)     ({var = val; var;})
#define STM_LOCAL_WRITE_P(var, val)     ({var = val; var;})


#endif /* STM_H */


/* =============================================================================
 *
 * End of stm.h
 *
 * =============================================================================
 */
