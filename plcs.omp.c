#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// #include "thread.h"
// #include "thread-sync.h"
#include <string.h>
#include <omp.h>

// #define DEBUG
#ifdef DEBUG
#include <sys/time.h>
#include <time.h>
#endif

#define MAXN 10000
int T, N, M;
char A[MAXN + 1], B[MAXN + 1];
int dp[MAXN][MAXN];
int result;

#define DP(x, y) (((x) >= 0 && (y) >= 0) ? dp[x][y] : 0)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#define MAX3(x, y, z) MAX(MAX(x, y), z)

int main(int argc, char *argv[])
{
    // No need to change
    assert(scanf("%s%s", A, B) == 2);
    N = strlen(A);
    M = strlen(B);
    const int iterations = N + M - 1;
    T = !argv[1] ? 1 : atoi(argv[1]);

    // Add preprocessing code here
    omp_set_num_threads(T);

#ifdef DEBUG
    struct timespec tsp_start, tsp_end;
    clock_gettime(CLOCK_REALTIME, &tsp_start);
#endif

    for (int d = 0; d < M + N - 1; d++)
    {
        // printf("d=%d\n", d);
#pragma omp parallel for
        for (int j = MAX(0, d - N + 1); j < MIN(M, d + 1); j++)
        {
            int i = d - j;
            // printf("i = %d, j = %d\n", i, j);
            // Always try to make DP code more readable
            int skip_a = DP(i - 1, j);
            int skip_b = DP(i, j - 1);
            int take_both = DP(i - 1, j - 1) + (A[i] == B[j]);
            dp[i][j] = MAX3(skip_a, skip_b, take_both);
        }
    }
    result = dp[N - 1][M - 1];
#ifdef DEBUG
    clock_gettime(CLOCK_REALTIME, &tsp_end);
    float elapsed_time = tsp_end.tv_sec - tsp_start.tv_sec + (tsp_end.tv_nsec - tsp_start.tv_nsec) * 1e-9;
    tsp_start = tsp_end;
    printf("elapsed time=%f\n", elapsed_time);
#endif

    printf("%d\n", result);
}
