#include <stdio.h>

#include <sys/time.h>
long long ms_clock() {
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec*1000LL + tv.tv_usec/1000;
    return ms;
}

void fib(int n, int *out) {
	int s0,s1,s2;
	s0 = n;
	s1 = 2;
	s0 = s0<s1 ? 1:0;
	if (s0) {
		s0 = 1;
		out[0] = s0;
		return;
	}
	s0 = n;
	s1 = 2;
	s0 = s0 - s1;
	fib(s0,out);
	s0 = out[0];
	s1 = n;
	s2 = 1;
	s1 = s1 - s2;
	fib(s1,out);
	s1 = out[0];
	s0 = s0 + s1;
	out[0] = s0;
}

void main() {
	int out[10];
	int t0 = ms_clock();
	fib(38,out);
	printf("time: %d ms\n",ms_clock()-t0);
	printf("out[0]: %d\n",out[0]);
}
