#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "thread.h"
#include "thread-sync.h"

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

sem_t sem_pass[NTHREAD + 2];

#define DP(x, y) (((x) >= 0 && (y) >= 0) ? dp[x][y] : 0)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#define MAX3(x, y, z) MAX(MAX(x, y), z)

void Tworker(int id) {
    int my_length = M / T;
    // if (M % T == 0)
    if (M % T) // thread at the head handle one more
        my_length += 1;
    const int my_j_start = my_length * (id - 1);
    const int my_j_end = MIN(my_length * id, M);
#ifdef DEBUG
    // printf("my id = %d, my start = %d, my end = %d, my length = %d, M = %d, M%%T = %d\n", id, my_j_start, my_j_end, my_length, M, M % T);
#endif
    if (my_length == 0 && id > 0)
        return;
    for (int i = 0; i < N; i++) {
        P(&sem_pass[id - 1]);
        for (int j = my_j_start; j < my_j_end; j++) {
            // Always try to make DP code more readable
            int skip_a = DP(i - 1, j);
            int skip_b = DP(i, j - 1);
            int take_both = DP(i - 1, j - 1) + (A[i] == B[j]);
            dp[i][j] = MAX3(skip_a, skip_b, take_both);
        }
        V(&sem_pass[id]);
    }
    if (id == T) {
        V(&sem_pass[T + 1]);
        result = dp[N - 1][M - 1];
    }
    return;
    /*
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            // Always try to make DP code more readable
            int skip_a = DP(i - 1, j);
            int skip_b = DP(i, j - 1);
            int take_both = DP(i - 1, j - 1) + (A[i] == B[j]);
            dp[i][j] = MAX3(skip_a, skip_b, take_both);
        }
    }

    result = dp[N - 1][M - 1];
    return;
    */
}

int main(int argc, char *argv[]) {
    // No need to change
    assert(scanf("%s%s", A, B) == 2);
    N = strlen(A);
    M = strlen(B);
    const int iterations = N + M - 1;
    T = !argv[1] ? 1 : atoi(argv[1]);

    // Add preprocessing code here
    if (M / T == 0)
        T = M;
    SEM_INIT(&sem_pass[0], N);
    for (int i = 1; i <= T; i++) {
        SEM_INIT(&sem_pass[i], 0);
    }

#ifdef DEBUG
    struct timespec tsp_start, tsp_end;
    clock_gettime(CLOCK_REALTIME, &tsp_start);
#endif

    for (int i = 0; i < T; i++) {
        create(Tworker);
    }

    if (T > 1) {
        // Wait for last workers
        P(&sem_pass[T + 1]);
    }

    join(); // Wait for all workers
#ifdef DEBUG
    clock_gettime(CLOCK_REALTIME, &tsp_end);
    float elapsed_time = tsp_end.tv_sec - tsp_start.tv_sec + (tsp_end.tv_nsec - tsp_start.tv_nsec) * 1e-9;
    tsp_start = tsp_end;
    printf("elapsed time=%f\n", elapsed_time);
#endif

    printf("%d\n", result);
}
