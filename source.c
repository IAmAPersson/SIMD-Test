#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#define LEN 1024

inline int op(int a, int b, int c)
{
	return a * b + a * c + b * c;
}

int main(void)
{
	int *a = (int*)memalign(64, sizeof(int) * LEN);
	int *b = (int*)memalign(64, sizeof(int) * LEN);
	int *c = (int*)memalign(64, sizeof(int) * LEN);
	int *avx2 = (int*)memalign(64, sizeof(int) * LEN);
	int *no_simd = (int*)memalign(64, sizeof(int) * LEN);
	
	srand(time(NULL));
	
	unsigned long nsec_avx2 = 0, nsec_no_simd = 0;
	
	for (int i = 0; i < 1000; i++)
	{
		for (int i = 0; i < LEN; i++)
		{
			a[i] = rand();
			b[i] = rand();
			c[i] = rand();
		}
		
		struct timespec startTime_avx2, endTime_avx2,
						startTime_no_simd, endTime_no_simd;
		
		clock_gettime(0, &startTime_avx2);
		
		#pragma omp simd simdlen(8)
		for (int i = 0; i < LEN; i++)
			avx2[i] = op(a[i], b[i], c[i]);
		
		clock_gettime(0, &endTime_avx2);
		
		nsec_avx2 += ((unsigned long)(endTime_avx2.tv_nsec - startTime_avx2.tv_nsec) + (unsigned long)(endTime_avx2.tv_sec - startTime_avx2.tv_sec) * 1000000000);
		
		clock_gettime(0, &startTime_no_simd);
		
		for (int i = 0; i < LEN; i++)
			no_simd[i] = op(a[i], b[i], c[i]);
		
		clock_gettime(0, &endTime_no_simd);
		
		nsec_no_simd += ((unsigned long)(endTime_no_simd.tv_nsec - startTime_no_simd.tv_nsec) + (unsigned long)(endTime_no_simd.tv_sec - startTime_no_simd.tv_sec) * 1000000000);
	}
	
	printf("Elapsed nanoseconds with AVX2: %ld\nElapsed nanoseconds with no SIMD: %ld\n", nsec_avx2 / 1000, nsec_no_simd / 1000);
	
	free(a);
	free(b);
	free(avx2);
	free(no_simd);
}