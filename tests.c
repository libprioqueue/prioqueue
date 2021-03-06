#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prioqueue.h"

typedef struct keyval_struct {
    long key; /* identification */
    long prio; /* priority */
} KV;

/*
  cmp performs the x-y operation for 
  the field defined to be used as priority
  value.
*/
static int cmp(void *x, void *y) {
    KV *xx = NULL, *yy = NULL;

    assert(x && y);
    
    xx = (KV*)x;
    yy = (KV*)y;

    return xx->prio - yy->prio;
}
/*
  assert_equals checks if the pointers x and y 
  are equal.
*/
static void assert_equal(void *x, void *y) {
    KV *xx = NULL, *yy = NULL;

    xx = (KV*)x;
    yy = (KV*)y;    
    
    if (x != y) {
        fprintf(stderr, "test failed %ld != %ld\n", 
                xx->prio, yy->prio);
        exit(EXIT_FAILURE);
    }
}

/* 
  test function has the following reasoning:
  
  PQMin: assign priorities in descending order to 
  an array, adding them to priority queue. The test
  is performed by traversing the array backwards and 
  comparing the element of the array with the element
  delete from the priority queue.

  PQMax: assign priorities in ascending order to 
  an array, adding them to priority queue. The test
  is performed by traversing the array backwards and 
  comparing the element of the array with the element
  delete from the priority queue.
  
  order - is the priority queue order {PQMin, PQMax}
  factor - the number to multiply the initial capacity
           if the heap array to test memory allocations
           and relases.
*/
static void test(enum prioqueue_order order, long factor) {
    PrioQueue *pq;
    long i, delta, n = PRIOQUEUE_INITIAL_CAPACITY * factor;
    KV *kvs; /* array of key/prioues */
    KV *kv; /* pointer to kv struct */

    if (order == PQMin) {
        pq = prioqueue_new(cmp, PQMin);
        delta = n;
    } else {
        pq = prioqueue_new(cmp, PQMax);
        delta = 0;
    }

    kvs = (KV*)calloc(n, sizeof(KV));
    assert(kvs);
    for (i = 0; i < n; i++ ) {
        kvs[i].key = i;
        kvs[i].prio = labs(delta-i) * 10;
        prioqueue_insert(pq, &kvs[i]);
    }

    for (i = n-1; i >= 0; i--) {
        if (!prioqueue_is_empty(pq)) {
            kv = (KV*)prioqueue_delete(pq);
            assert_equal(kv, &kvs[i]);
        }
    }

    if (kvs)
        free(kvs);
    prioqueue_free(pq);
}
/*
  tests function sets the factor value and run 
  test function for PQmin and PQMax orders.
*/
static int tests() {
    int i;
    long factor = 4;

    for (i = 0; i < 2; i++)
        test(i, factor);

    printf("ok: all tests passed\n");

    return EXIT_SUCCESS;
}

/*
  bench performs a benchmark using test function.
  It's being studied the best method to measure
  the eficiency of each part of the micro-library.
*/
static int bench() {
    fprintf(stderr, "%s(): NOT IMPLEMENTED YET", __FUNCTION__);

    return EXIT_SUCCESS;
}

/*
  usage function prints how to use the program.
*/
static int usage(char **argv) {
    fprintf(stderr, "usage: %s [--bench]\n", argv[0]);
    fprintf(stderr, "--bench: perform a benchmark;\n");
    fprintf(stderr, 
            "with no arguments, the program performs a simple test.");

    return EXIT_FAILURE;
}

int main(int argc, char**argv) {
    if (argc == 2
        && strncmp(argv[1], "--bench", sizeof("--bench"))==0)
        return bench();
    else if (argc==1)
        return tests();
    else
        return usage(argv);
}
