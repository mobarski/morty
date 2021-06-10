/* -*- mode: c -*-
 * $Id: sieve.gcc,v 1.7 2001/05/06 04:37:45 doug Exp $
 * http://www.bagley.org/~doug/shootout/
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
long long ms_clock() {
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec*1000LL + tv.tv_usec/1000;
    return ms;
}

int
main(int argc, char *argv[]) {
	long long t = ms_clock();
    int NUM = ((argc == 2) ? atoi(argv[1]) : 1);
    static char flags[8192 + 1];
    long i, k;
    int count = 0;

    while (NUM--) {
    count = 0; 
    for (i=2; i <= 8192; i++) {
        flags[i] = 1;
    }
    for (i=2; i <= 8192; i++) {
        if (flags[i]) {
        // remove all multiples of prime: i
        for (k=i+i; k <= 8192; k+=i) {
            flags[k] = 0;
        }
        count++;
        }
    }
    }
    printf("Count: %d\n", count);
	printf("Time: %d\n",ms_clock()-t);
    return(0);
}
