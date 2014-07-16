#include "htlock.h"

__thread uint32_t my_node, my_id;

htlock_t* 
create_htlock()
{
    htlock_t* htl;
    if (posix_memalign((void**) &htl, CACHE_LINE_SIZE, sizeof(htlock_t)) < 0)
    {
        fprintf(stderr, "Error @ posix_memalign : create_htlock\n");
    }
    assert(htl != NULL);

    if (posix_memalign((void**) &htl->global, CACHE_LINE_SIZE, sizeof(htlock_global_t)) < 0)
    {
        fprintf(stderr, "Error @ posix_memalign : create_htlock\n");
    }
    assert(htl->global != NULL);


    uint32_t s;
    for (s = 0; s < NUMBER_OF_SOCKETS; s++)
    {

        numa_set_preferred(s);
        htl->local[s] = (htlock_local_t*) numa_alloc_onnode(sizeof(htlock_local_t), s);
        assert(htl->local != NULL);
    }

    numa_set_preferred(my_node);

    htl->global->cur = 0;
    htl->global->nxt = 0;
    uint32_t n;
    for (n = 0; n < NUMBER_OF_SOCKETS; n++)
    {
        htl->local[n]->cur = NB_TICKETS_LOCAL;
        htl->local[n]->nxt = 0;
    }

    return htl;
}


void
init_htlock(htlock_t* htl)
{
    assert(htl != NULL);
    htl->global->cur = 0;
    htl->global->nxt = 0;
    uint32_t n;
    for (n = 0; n < NUMBER_OF_SOCKETS; n++)
    {
        htl->local[n]->cur = NB_TICKETS_LOCAL;
        htl->local[n]->nxt = 0;
    }
}

void
init_thread_htlocks(uint32_t phys_core)
{
    set_cpu(phys_core);

    __sync_synchronize();
    uint32_t real_core_num;
    uint32_t i;
    for (i = 0; i < (NUMBER_OF_SOCKETS * CORES_PER_SOCKET); i++)
    {
        if (the_cores[i]==phys_core)
        {
            real_core_num = i;
            break;
        }
    }
    __sync_synchronize();
    MEM_BARRIER;
    my_id = real_core_num;
    my_node = get_cluster(phys_core);
    /* printf("core %02d / node %3d\n", phys_core, my_node); */
}

uint32_t
is_free_hticket(htlock_t* htl)
{
    htlock_global_t* glb = htl->global;
    if (glb->cur == glb->nxt)
    {
        return 1;
    }
    return 0;
}

static htlock_t* 
create_htlock_no_alloc(htlock_t* htl, htlock_local_t* locals[NUMBER_OF_SOCKETS], size_t offset)
{
    if (posix_memalign((void**) &htl->global, CACHE_LINE_SIZE, sizeof(htlock_global_t)) < 0)
    {
        fprintf(stderr, "Error @ posix_memalign : create_htlock\n");
    }
    assert(htl->global != NULL);


    uint32_t s;
    for (s = 0; s < NUMBER_OF_SOCKETS; s++)
    {
        htl->local[s] = locals[s] + offset;
    }


    htl->global->cur = 0;
    htl->global->nxt = 0;
    uint32_t n;
    for (n = 0; n < NUMBER_OF_SOCKETS; n++)
    {
        htl->local[n]->cur = NB_TICKETS_LOCAL;
        htl->local[n]->nxt = 0;
    }

    return htl;
}

htlock_t*
init_htlocks(uint32_t num_locks)
{
    htlock_t* htls;
    if (posix_memalign((void**) &htls, 64, num_locks * sizeof(htlock_t)) < 0)
    {
        fprintf(stderr, "Error @ posix_memalign : init_htlocks\n");
    }
    assert(htls != NULL);


    size_t alloc_locks = (num_locks < 64) ? 64 : num_locks;

    htlock_local_t* locals[NUMBER_OF_SOCKETS];
    uint32_t n;
    for (n = 0; n < NUMBER_OF_SOCKETS; n++)
    {
        numa_set_preferred(n);
        _mm_mfence();
        locals[n] = (htlock_local_t*) calloc(alloc_locks, sizeof(htlock_local_t));
        //numa_alloc_onnode(num_locks * sizeof(htlock_local_t), n);
        assert(locals[n] != NULL);
    }

    numa_set_preferred(my_node);

    uint32_t i;
    for (i = 0; i < num_locks; i++)
    {
        create_htlock_no_alloc(htls + i, locals, i);
    }

    return htls;
}


void 
free_htlocks(htlock_t* locks)
{
    free(locks);
}

static inline uint32_t
sub_abs(const uint32_t a, const uint32_t b)
{
    if (a > b)
    {
        return a - b;
    }
    else
    {
        return b - a;
    }
}


#define TICKET_BASE_WAIT 512
#define TICKET_MAX_WAIT  4095
#define TICKET_WAIT_NEXT 64


static inline void
htlock_wait_ticket(htlock_local_t* lock, const uint32_t ticket)
{

    while (lock->cur != ticket)
    {
        uint32_t distance = sub_abs(lock->cur, ticket);
        if (distance > 1)
        {
            nop_rep(distance * TICKET_BASE_WAIT);
        }
        else
        {
            PAUSE;
        }
    }
}

static inline void
htlock_wait_global(htlock_local_t* lock, const uint32_t ticket)
{
    while (lock->cur != ticket)
    {
        uint32_t distance = sub_abs(lock->cur, ticket);
        if (distance > 1)
        {
            wait_cycles(distance * 256);
        }
        else
        {
            PAUSE;
        }
    }
}

void
htlock_lock(htlock_t* l)
{
    htlock_local_t* localp = l->local[my_node];
    int32_t local_ticket;

    /* _mm_clflush(localp); */

    ticks s, e;

    again_local:
    /* if (my_id == 32) */
    /*   { */
    /*     s = getticks(); */
    /*   } */
    local_ticket = DAF_U32(&localp->nxt);
    /* if (my_id == 32) */
    /*   { */
    /*     e = getticks(); */
    /*     printf("%5llu - ", e - s - 74); */
    /*   } */
    /* only the guy which gets local_ticket == -1 is allowed to share tickets */
    if (local_ticket < -1)
    {
        PAUSE;
        wait_cycles(-local_ticket * 120);
        PAUSE;
        goto again_local;
    }

    if (local_ticket >= 0)	/* local grabing successful */
    {
        htlock_wait_ticket((htlock_local_t*) localp, local_ticket);
    }
    else				/* no local ticket available */
    {
        do
        {
        } while (localp->cur != NB_TICKETS_LOCAL);
        localp->nxt = NB_TICKETS_LOCAL; /* give tickets to the local neighbors */

        htlock_global_t* globalp = l->global;
        uint32_t global_ticket = FAI_U32(&globalp->nxt);

        /* htlock_wait_ticket((htlock_local_t*) globalp, global_ticket); */
        htlock_wait_global((htlock_local_t*) globalp, global_ticket);
    }
}

void
htlock_release(htlock_t* l)
{
    htlock_local_t* localp = l->local[my_node];
    int32_t local_cur = localp->cur;
    int32_t local_nxt = CAS_U32(&localp->nxt, local_cur, 0);
    if (local_cur == 0 || local_cur == local_nxt) /* global */
    {
        localp->cur = NB_TICKETS_LOCAL;
        l->global->cur++;
    }
    else				/* local */
    {
        localp->cur = local_cur - 1;
    }
}

uint32_t 
htlock_trylock(htlock_t* l)
{
    htlock_global_t* globalp = l->global;
    PREFETCHW(globalp);
    uint32_t global_nxt = globalp->nxt;

    htlock_global_t tmp =
    {
            .nxt = global_nxt,
            .cur = global_nxt
    };
    htlock_global_t tmp_new =
    {
            .nxt = global_nxt + 1,
            .cur = global_nxt
    };

    uint64_t tmp64 = *(uint64_t*) &tmp;
    uint64_t tmp_new64 = *(uint64_t*) &tmp_new;

    if (CAS_U64((uint64_t*) globalp, tmp64, tmp_new64) == tmp64)
    {
        return 1;
    }

    return 0;
}


inline void
htlock_release_try(htlock_t* l)	/* trylock rls */
{
    PREFETCHW((l->global));
    l->global->cur++;
}

