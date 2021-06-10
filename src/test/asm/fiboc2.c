#include <stdio.h>

#include <sys/time.h>
long long ms_clock() {
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec*1000LL + tv.tv_usec/1000;
    return ms;
}

void fib(int n, int *out) {
	int s[10];
	s[0] = n;
	s[1] = 2;
	s[0] = s[0]<s[1] ? 1:0;
	if (s[0]) {
		s[0] = 1;
		out[0] = s[0];
		return;
	}
	s[0] = n;
	s[1] = 2;
	s[0] = s[0] - s[1];
	fib(s[0],out);
	s[0] = out[0];
	s[1] = n;
	s[2] = 1;
	s[1] = s[1] - s[2];
	fib(s[1],out);
	s[1] = out[0];
	s[0] = s[0] + s[1];
	out[0] = s[0];
}

void main() {
	int out[10];
	int t0 = ms_clock();
	fib(38,out);
	printf("time: %d ms\n",ms_clock()-t0);
	printf("out[0]: %d\n",out[0]);
}
