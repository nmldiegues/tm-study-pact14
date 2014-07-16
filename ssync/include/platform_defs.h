/*
 *  platform specific definitions
 */
#ifndef _PLATFORM_DEFS_H_INCLUDED_
#define _PLATFORM_DEFS_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

//for each machine that is used, one needs to define
//NUMBER_OF_SOCKETS: the number of sockets the machine has
//CORES_PER_SOCKET: the number of cores per socket
//CACHE_LINE_SIZE
//NOP_DURATION: the duration in cycles of a noop instruction (generally 1 cycle on most small machines)
//the_cores - a mapping from the core ids as configured in the OS to physical cores (the OS might not alwas be configured corrrectly)
//get_cluster - a function that given a core id returns the socket number ot belongs to

#define NUMBER_OF_SOCKETS 1
#define CORES_PER_SOCKET 8
#define CACHE_LINE_SIZE 64
#define NOP_DURATION 1

static uint8_t  the_cores[] = {
        0, 1, 2, 3, 4, 5, 6, 7
};
static uint8_t the_sockets[] =
{
        0, 0, 0, 0, 0, 0, 0, 0
};

#  define PREFETCHW(x)		

static inline int get_cluster(int thread_id) {
    if (thread_id>=8){
        perror("Thread id too high");
        return 0;
    }
    return the_sockets[thread_id];    
}

#ifdef __cplusplus
}

#endif


#endif
