#include <string.h>
#include <stdlib.h>
#include "kmeans.h"

#if UNIT_TESTING
extern void* _test_malloc(const size_t size, const char* file, const int line);
extern void* _test_calloc(
        const size_t number_of_elements,
        const size_t size,
        const char* file,
        const int line
);
extern void _test_free(void* const ptr, const char* file, const int line);

#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
#define calloc(num, size) _test_calloc(num, size, __FILE__, __LINE__)
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)

#define static

#endif // UNIT_TESTING

Kmeans_context* alloc_kmeans_context(
        unsigned long k,
        unsigned long n,
        unsigned long f) {
    int i;
    Kmeans_context *kc = malloc(sizeof *kc);

    kc->k = k;
    kc->n = n;
    kc->f = f;

    kc->observations = malloc(n * sizeof *kc->observations);
    for (i = 0; i < n; i++) {
        kc->observations[i] = malloc(f * sizeof *kc->observations[i]);
    }

    kc->centroids = malloc(k * sizeof *kc->centroids);
    for (i = 0; i < k; i++) {
        kc->centroids[i] = malloc(f * sizeof *kc->centroids[i]);
    }

    kc->cluster_map = calloc(n, sizeof *kc->cluster_map);

    return kc;
}

void free_kmeans_context(Kmeans_context *kc) {
    int i;

    if (kc != 0) {
        for (i = 0; i < kc->n; i++) {
            free(kc->observations[i]);
        }
        free(kc->observations);

        for (i = 0; i < kc->k; i++) {
            free(kc->centroids[i]);
        }
        free(kc->centroids);
        free(kc->cluster_map);
        free(kc);
    }
}

static void assign_clusters(Kmeans_context *kc) {
    double shortest;
    double d;
    unsigned long cluster;

    long i;
    long j;
    for (i = 0; i < kc->n; i++) {
        shortest = distance(kc->f, kc->observations[i], kc->centroids[0]);
        cluster = 0;

        for (j = 1; j < kc->k; j++) {
            d = distance(kc->f, kc->observations[i], kc->centroids[j]);

            if (d < shortest) {
                shortest = d;
                cluster = j;
            }
        }

        kc->cluster_map[i] = cluster;
    }
}

static void update_centroids(Kmeans_context *kc) {
    long i;
    for (i = 0; i < kc->k; i++) {
        update_centroid(
                kc->f,
                kc->n,
                (const double**)kc->observations,
                i,
                kc->cluster_map,
                kc->centroids[i]
        );
    }
}

void kmeans(Kmeans_context *kc) {
    unsigned long *last = calloc(kc->n, sizeof *last);
    size_t size = kc->n * sizeof *kc->cluster_map;

    do {
        memcpy(last, kc->cluster_map, size);

        assign_clusters(kc);
        update_centroids(kc);
    } while(memcmp(last, kc->cluster_map, size) != 0);

    free(last);
}


double distance(unsigned long f, double *a, double *b) {
    double *diff = malloc(f * sizeof *diff);
    double distance = 0;

    int i;
    for (i = 0; i < f; i++) {
        diff[i] = a[i] - b[i];
        distance += diff[i] * diff[i];
    }

    free(diff);
    return distance;
}

void update_centroid(
        unsigned long f,
        unsigned long n,
        const double **observation,
        unsigned long k,
        unsigned long *cluster_map,
        double *centroid) {
    double *mean = calloc(f, sizeof *mean);
    unsigned long cluster_size = 0;

    unsigned long i;
    unsigned long j;

    for (i = 0; i < n; i++) {
        if (k == cluster_map[i]) {
            for (j = 0; j < f; j++) {
                mean[j] += observation[i][j];
            }
            cluster_size++;
        }
    }

    if (cluster_size != 0) {
        for (j = 0; j < f; j++) {
            centroid[j] = mean[j] / cluster_size;
        }
    }

    free(mean);
}
