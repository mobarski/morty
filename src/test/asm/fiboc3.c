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
	int sp=0;
	s[sp] = n; sp+=1;
	s[sp] = 2; sp+=1;
	s[sp-2] = s[sp-2]<s[sp-1] ? 1:0; sp-=1;
	if (s[sp-1]) {
		sp-=1;
		s[sp] = 1; sp+=1;
		out[0] = s[sp-1];
		return;
	} sp-=1;
	s[sp] = n; sp+=1;
	s[sp] = 2; sp+=1;
	s[sp-2] = s[sp-2] - s[sp-1]; sp-=1;
	fib(s[sp-1],out); sp-=1;
	s[sp] = out[0]; sp+=1;
	s[sp] = n; sp+=1;
	s[sp] = 1; sp+=1;
	s[sp-2] = s[sp-2] - s[sp-1]; sp-=1;
	fib(s[sp-1],out); sp-=1;
	s[sp] = out[0]; sp+=1;
	s[sp-2] = s[sp-2] + s[sp-1]; sp-=1;
	out[0] = s[sp-1];
}

void main() {
	int out[10];
	int t0 = ms_clock();
	fib(38,out);
	printf("time: %d ms\n",ms_clock()-t0);
	printf("out[0]: %d\n",out[0]);
}
