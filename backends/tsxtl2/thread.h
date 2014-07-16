#ifndef THREAD_H
#define THREAD_H 1

#include <pthread.h>
#include <stdlib.h>
#include "types.h"

#include <immintrin.h>
#include <rtmintrin.h>

#include "stm.h"
#include "tl2.h"

#ifdef __cplusplus
extern "C" {
#endif


#define THREAD_T                            pthread_t
#define THREAD_ATTR_T                       pthread_attr_t

#define THREAD_ATTR_INIT(attr)              pthread_attr_init(&attr)
#define THREAD_JOIN(tid)                    pthread_join(tid, (void**)NULL)
#define THREAD_CREATE(tid, attr, fn, arg)   pthread_create(&(tid), \
                                                           &(attr), \
                                                           (void* (*)(void*))(fn), \
                                                           (void*)(arg))

#define THREAD_LOCAL_T                      pthread_key_t
#define THREAD_LOCAL_INIT(key)              pthread_key_create(&key, NULL)
#define THREAD_LOCAL_SET(key, val)          pthread_setspecific(key, (void*)(val))
#define THREAD_LOCAL_GET(key)               pthread_getspecific(key)

#define THREAD_MUTEX_T                      pthread_mutex_t
#define THREAD_MUTEX_INIT(lock)             pthread_spin_init(&(lock), NULL)
#define THREAD_MUTEX_LOCK(lock)             pthread_mutex_lock(&(lock))
#define THREAD_MUTEX_UNLOCK(lock)           pthread_mutex_unlock(&(lock))

#define THREAD_COND_T                       pthread_cond_t
#define THREAD_COND_INIT(cond)              pthread_cond_init(&(cond), NULL)
#define THREAD_COND_SIGNAL(cond)            pthread_cond_signal(&(cond))
#define THREAD_COND_BROADCAST(cond)         pthread_cond_broadcast(&(cond))
#define THREAD_COND_WAIT(cond, lock)        pthread_cond_wait(&(cond), &(lock))

#  define THREAD_BARRIER_T                  barrier_t
#  define THREAD_BARRIER_ALLOC(N)           barrier_alloc()
#  define THREAD_BARRIER_INIT(bar, N)       barrier_init(bar, N)
#  define THREAD_BARRIER(bar, tid)          barrier_cross(bar)
#  define THREAD_BARRIER_FREE(bar)          barrier_free(bar)

extern __thread vwLock next_commit;

extern void abortHTM(Thread* Self);
extern intptr_t sharedReadHTM(Thread* Self, intptr_t* addr);
extern void sharedWriteHTM(Thread* Self, intptr_t* addr, intptr_t val);
extern void localWriteHTM(Thread* Self, intptr_t* addr, intptr_t val);
extern void freeHTM(Thread* Self, void* ptr);
extern void* mallocHTM(Thread* Self, size_t size);

extern void abortSTM(Thread* Self);
extern intptr_t sharedReadSTM(Thread* Self, intptr_t* addr);
extern void sharedWriteSTM(Thread* Self, intptr_t* addr, intptr_t val);
extern void localWriteSTM(Thread* Self, intptr_t* addr, intptr_t val);
extern void freeSTM(Thread* Self, void* ptr);
extern void* mallocSTM(Thread* Self, size_t size);

extern __thread void (*abortFunPtr)(Thread* Self);
extern __thread intptr_t (*sharedReadFunPtr)(Thread* Self, intptr_t* addr);
extern __thread void (*sharedWriteFunPtr)(Thread* Self, intptr_t* addr, intptr_t val);
extern __thread void (*localWriteFunPtr)(Thread* Self, intptr_t* addr, intptr_t val);
extern __thread void (*freeFunPtr)(Thread* Self, void* ptr);
extern __thread void* (*mallocFunPtr)(Thread* Self, size_t size);

#include "random.h"

extern volatile unsigned long is_fallback;
extern __thread random_t* randomFallback;

typedef struct barrier {
    pthread_cond_t complete;
    pthread_mutex_t mutex;
    int count;
    int crossing;
} barrier_t;

barrier_t *barrier_alloc();

void barrier_free(barrier_t *b);

void barrier_init(barrier_t *b, int n);

void barrier_cross(barrier_t *b);

void thread_startup (long numThread);

void thread_start (void (*funcPtr)(void*), void* argPtr);

void thread_shutdown ();

void thread_barrier_wait();

long thread_getId();

long thread_getNumThread();



#ifdef __cplusplus
}
#endif


#endif /* THREAD_H */