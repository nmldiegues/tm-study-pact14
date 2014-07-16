/*
 * File:
 *   intset.c
 * Author(s):
 *   Pascal Felber <pascal.felber@unine.ch>
 *   Aleksandar Dragojevic <aleksandar.dragojevic@epfl.ch> (added support for wlpdstm)
 * Description:
 *   Integer set stress test.
 *
 * Copyright (c) 2007-2008.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "timer.h"
#include <time.h>

#include "tm.h"
#include "thread.h"

#define DEFAULT_DURATION                10000
#define DEFAULT_INITIAL                 256
#define DEFAULT_NB_THREADS              1
#define DEFAULT_RANGE                   0xFFFF
#define DEFAULT_SEED                    0
#define DEFAULT_UPDATE                  20

#define XSTR(s)                         STR(s)
#define STR(s)                          #s

/* ################################################################### *
 * GLOBALS
 * ################################################################### */

static volatile int stop;

#include "rbtree.h"


typedef rbtree_t intset_t;

intset_t *set_new()
{
  return rbtree_alloc();
}

void set_delete(intset_t *set)
{
  rbtree_free(set);
}

int set_add_seq(intset_t *set, intptr_t val) {
    return !rbtree_insert(set, val, val);
}

int set_add(TM_ARGDECL  intset_t *set, intptr_t val)
{
//  wlpdstm_choose_tm_desc(tx, TM_MIXED);
    int res = 0;

    TM_BEGIN();
    res = !TMrbtree_insert(TM_ARG set, val, val);

    TM_END();

    return res;
}

int set_remove(TM_ARGDECL intset_t *set, intptr_t val)
{
    int res = 0;

//  wlpdstm_choose_tm_desc(tx, TM_EAGER);
    TM_BEGIN();
    res = TMrbtree_delete(TM_ARG set, val);

    TM_END();

    return res;
}

int set_contains(TM_ARGDECL  intset_t *set, intptr_t val)
{
    int res = 0;

//  wlpdstm_choose_tm_desc(tx, TM_LAZY);
    TM_BEGIN();
    res = TMrbtree_contains(TM_ARG set, val);
    TM_END();

    return res;
}


/*typedef struct barrier {
  pthread_cond_t complete;
  pthread_mutex_t mutex;
  int count;
  int crossing;
} barrier_t;

void barrier_init(barrier_t *b, int n)
{
  pthread_cond_init(&b->complete, NULL);
  pthread_mutex_init(&b->mutex, NULL);
  b->count = n;
  b->crossing = 0;
}

void barrier_cross(barrier_t *b)
{
  pthread_mutex_lock(&b->mutex);
  b->crossing++;
  if (b->crossing < b->count) {
    pthread_cond_wait(&b->complete, &b->mutex);
  } else {
    pthread_cond_broadcast(&b->complete);
    b->crossing = 0;
  }
  pthread_mutex_unlock(&b->mutex);
}*/

/* ################################################################### *
 * STRESS TEST
 * ################################################################### */

  int range;
  int update;
  unsigned long nb_add;
  unsigned long nb_remove;
  unsigned long nb_contains;
  unsigned long nb_found;
  unsigned long nb_aborts;
  unsigned int nb_threads;
  int diff;
  unsigned int seed;
  unsigned int operations;
  intset_t *set;

void *test(void *data)
{
  int val;

  TM_THREAD_ENTER();

  unsigned int mySeed = seed;
  int myDiff = diff;

  unsigned int myOps = operations / nb_threads;

  while (myOps > 0) {
    val = rand_r(&mySeed) % 100;
    if (val < update) {
      if (val < update / 2) {
        /* Add random value */
        val = (rand_r(&mySeed) % range) + 1;
        if (set_add(TM_ARG set, val)) {
          myDiff++;
        }
      } else {
        /* Remove random value */
        val = (rand_r(&mySeed) % range) + 1;
        if (set_remove(TM_ARG set, val)) {
          myDiff--;
        }
      }
    } else {
      /* Look for random value */
      val = (rand_r(&mySeed) % range) + 1;
      set_contains(TM_ARG set, val);
    }

    myOps--;
  }

  TM_THREAD_EXIT();

  return NULL;
}

# define no_argument        0
# define required_argument  1
# define optional_argument  2

MAIN(argc, argv) {
    TIMER_T start;
    TIMER_T stop;


  struct option long_options[] = {
    // These options don't set a flag
    {"help",                      no_argument,       NULL, 'h'},
    {"duration",                  required_argument, NULL, 'd'},
    {"initial-size",              required_argument, NULL, 'i'},
    {"num-threads",               required_argument, NULL, 'n'},
    {"range",                     required_argument, NULL, 'r'},
    {"seed",                      required_argument, NULL, 's'},
    {"update-rate",               required_argument, NULL, 'u'},
    {NULL, 0, NULL, 0}
  };

  int i, c, val;
  operations = DEFAULT_DURATION;
  unsigned int initial = DEFAULT_INITIAL;
  nb_threads = DEFAULT_NB_THREADS;
  range = DEFAULT_RANGE;
  update = DEFAULT_UPDATE;

  while(1) {
    i = 0;
    c = getopt_long(argc, argv, "hd:i:n:r:s:u:", long_options, &i);

    if(c == -1)
      break;

    if(c == 0 && long_options[i].flag == 0)
      c = long_options[i].val;

    switch(c) {
     case 0:
       /* Flag is automatically set */
       break;
     case 'h':
       printf("intset -- STM stress test "
              "(red-black tree)\n"
              "\n"
              "Usage:\n"
              "  intset [options...]\n"
              "\n"
              "Options:\n"
              "  -h, --help\n"
              "        Print this message\n"
              "  -d, --duration <int>\n"
              "        Test duration in milliseconds (0=infinite, default=" XSTR(DEFAULT_DURATION) ")\n"
              "  -i, --initial-size <int>\n"
              "        Number of elements to insert before test (default=" XSTR(DEFAULT_INITIAL) ")\n"
              "  -n, --num-threads <int>\n"
              "        Number of threads (default=" XSTR(DEFAULT_NB_THREADS) ")\n"
              "  -r, --range <int>\n"
              "        Range of integer values inserted in set (default=" XSTR(DEFAULT_RANGE) ")\n"
              "  -s, --seed <int>\n"
              "        RNG seed (0=time-based, default=" XSTR(DEFAULT_SEED) ")\n"
              "  -u, --update-rate <int>\n"
              "        Percentage of update transactions (default=" XSTR(DEFAULT_UPDATE) ")\n"
         );
       exit(0);
     case 'd':
       operations = atoi(optarg);
       break;
     case 'i':
       initial = atoi(optarg);
       break;
     case 'n':
       nb_threads = atoi(optarg);
       break;
     case 'r':
       range = atoi(optarg);
       break;
     case 's':
       seed = atoi(optarg);
       break;
     case 'u':
       update = atoi(optarg);
       break;
     case '?':
       printf("Use -h or --help for help\n");
       exit(0);
     default:
       exit(1);
    }
  }

  if (seed == 0)
    srand((int)time(0));
  else
    srand(seed);

  set = set_new();

  SIM_GET_NUM_CPU(nb_threads);
  TM_STARTUP(nb_threads);
  P_MEMORY_STARTUP(nb_threads);
  thread_startup(nb_threads);

  /* Populate set */
  printf("Adding %d entries to set\n", initial);
  for (i = 0; i < initial; i++) {
    val = (rand() % range) + 1;
    set_add_seq(set, val);
  }

  diff = 0;
  seed = rand();

  TIMER_READ(start);
  GOTO_SIM();

  thread_start(test, NULL);

  GOTO_REAL();
  TIMER_READ(stop);

  puts("done.");
  printf("\nTime = %0.6lf\n", TIMER_DIFF_SECONDS(start, stop));
  fflush(stdout);

  TM_SHUTDOWN();
  P_MEMORY_SHUTDOWN();
  GOTO_SIM();
  thread_shutdown();
  MAIN_RETURN(0);
}
