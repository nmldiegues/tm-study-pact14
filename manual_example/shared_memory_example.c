#include <stdio.h>
#include <stdlib.h>
#include <libpmemobj/base.h>
#include <unistd.h>
#include <stdbool.h>
#include <libpmemobj/pool_base.h>
#include <libpmemobj/types.h>
#include "list.h"
#include "thread.h"
#include <libpmemobj/tx_base.h>

#define shared_memory_size 10000

struct thread_context{
    char buff[shared_memory_size];
};

struct dram_thread_context{
    PMEMobjpool *pop;
    struct thread_context *th;
};

struct pmem_obj_root{
    struct thread_context th;
};

static void
log_stages(PMEMobjpool *pop_local, enum pobj_tx_stage stage, void *arg)
{
    /* Commenting this out because this is not required during normal execution. */
    /* dr_fprintf(STDERR, "cb stage: ", desc[stage], " "); */
}

bool file_exists(const char *path){
    return access(path, F_OK) != 0;
}

void *p_region(void *context){
    struct dram_thread_context *dramThreadContext = context;
    struct thread_context *th = dramThreadContext->th;

    int *attempt = malloc(sizeof(int));
    *attempt = 0;

    /* --------------------------- */
    //TM_THREAD_ENTER();
    Thread *Self = TxNewThread();
    TxInitThread(Self, thread_getId());
    /* --------------------------- */

    /* -------- */
    //TM_BEGIN();

    do { \
        STM_JMPBUF_T STM_JMPBUF; \
        sigsetjmp(STM_JMPBUF, 1); \
        TxStart(STM_SELF, &STM_JMPBUF); \
        } while (0);
     /* -----------*/

    //ABORTED TRANSACTION JUMPS TO THIS POINT

    char randomletter = 'A' + (random() % 26);
    for (int i = 0; i < shared_memory_size; i++) {
        TM_SHARED_WRITE(th->buff[i], randomletter);
    }

    /* --------------------------- */
    //TM_END();
    TxCommit(Self, dramThreadContext->pop);
    /* --------------------------- */
    //Changes are visible here
    printf("Post TM_END written to shared_write: %c Current local value: %c\n", randomletter, th->buff[0]);
    TM_THREAD_EXIT();
    return 0;
}


PMEMobjpool *mmap_pmem_object_pool(PMEMobjpool *pop){
    char *path_to_pmem = "/mnt/dax/test_outputs/norec_with_pmem";
    if (file_exists((path_to_pmem)) != 0) {
        if ((pop = pmemobj_create(path_to_pmem, POBJ_LAYOUT_NAME(list),10000000, 0666)) == NULL)
            perror("failed to create pool\n");
    } else {
        if ((pop = pmemobj_open(path_to_pmem, POBJ_LAYOUT_NAME(list))) == NULL) {
            perror("failed to open pool\n");
        }
    }
    return pop;
}

struct thread_context* initialise_thread_context_on_persistent_memory(PMEMobjpool *pop){
    PMEMoid pool_root = pmemobj_root(pop, sizeof(struct pmem_obj_root));
    struct pmem_obj_root *rootp = pmemobj_direct(pool_root);
    return &rootp->th;
}

MAIN(argc, argv)
{
    PMEMobjpool *pop = NULL;
    pop = mmap_pmem_object_pool(pop);
    struct thread_context *th = initialise_thread_context_on_persistent_memory(pop);
    struct dram_thread_context *dramThreadContext = malloc(sizeof (struct dram_thread_context));
    dramThreadContext->pop = pop;
    dramThreadContext->th = th;

    printf("Initial content:\n");
    for(int i=0; i< shared_memory_size; i++ ){
        printf("%c", th->buff[i]);
    }
    puts("\n");

    GOTO_REAL();
    int arr_len = 30;
    TM_STARTUP(arr_len);
    pthread_t thread_array[arr_len];

    for (int i = 0; i < arr_len; i++) {
        pthread_create(&thread_array[i], NULL, p_region, dramThreadContext);
    }

    for (int i = 0; i < arr_len; i++) {
        pthread_join(thread_array[i], NULL);
    }

    printf("Result:\n");
    for(int i=0; i< shared_memory_size; i++ ){
        printf("%c", th->buff[i]);
    }
    printf("\n");
    TM_SHUTDOWN();
    MAIN_RETURN(0);
}