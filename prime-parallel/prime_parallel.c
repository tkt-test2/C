#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define MAX_THREADS 4

typedef struct {
    int start;
    int end;
    int *primes;
    int *count;
    pthread_mutex_t *lock;
} ThreadData;

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void* prime_worker(void* arg) {
    ThreadData *data = (ThreadData*)arg;
    for (int i = data->start; i <= data->end; i++) {
        if (is_prime(i)) {
            pthread_mutex_lock(data->lock);
            data->primes[*(data->count)] = i;
            (*(data->count))++;
            pthread_mutex_unlock(data->lock);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <max_number>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int *primes = malloc(N * sizeof(int));
    int count = 0;

    pthread_t threads[MAX_THREADS];
    ThreadData tdata[MAX_THREADS];
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    clock_t start = clock();

    int chunk = N / MAX_THREADS;
    for (int i = 0; i < MAX_THREADS; i++) {
        tdata[i].start = i * chunk + 1;
        tdata[i].end = (i == MAX_THREADS - 1) ? N : (i + 1) * chunk;
        tdata[i].primes = primes;
        tdata[i].count = &count;
        tdata[i].lock = &lock;
        pthread_create(&threads[i], NULL, prime_worker, &tdata[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Found %d primes up to %d in %.2f seconds\n", count, N, elapsed);

    // 確認用に最初の20個だけ表示
    for (int i = 0; i < count && i < 20; i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");

    free(primes);
    pthread_mutex_destroy(&lock);

    return 0;
}
