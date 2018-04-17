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

Kmeans_context* alloc_kmeans_context(unsigned long k, unsigned long n) {
    Kmeans_context *kc = malloc(sizeof *kc);

    kc->k = k;
    kc->n = n;

    kc->observations = malloc(kc->n * sizeof *kc->observations);
    kc->centroids = malloc(kc->k * sizeof *kc->centroids);
    kc->cluster_map = calloc(kc->n, sizeof *kc->cluster_map);

    return kc;
}

void free_kmeans_context(Kmeans_context *kc) {
    if (kc != 0) {
        free(kc->observations);
        free(kc->centroids);
        free(kc->cluster_map);
        free(kc);
    }
}

static void assign_clusters(Kmeans_context *kc) {
    double shortest;
    double distance;
    unsigned long cluster;

    long i;
    long j;
    for (i = 0; i < kc->n; i++) {
        shortest = kc->distance(kc->observations[i], kc->centroids[0]);
        cluster = 0;

        for (j = 1; j < kc->k; j++) {
            distance = kc->distance(kc->observations[i], kc->centroids[j]);

            if (distance < shortest) {
                shortest = distance;
                cluster = j;
            }
        }

        kc->cluster_map[i] = cluster;
    }
}

static void update_centroids(Kmeans_context *kc) {
    long i;
    for (i = 0; i < kc->k; i++) {
        kc->update_centroid(
                kc->n,
                *kc->observations,
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

double point_distance(const void *a, const void *b) {
    Point pa = *(Point*)a;
    Point pb = *(Point*)b;

    double dx = pa.x - pb.x;
    double dy = pa.y - pb.y;

    return (dx * dx) + (dy * dy);
}

void point_update_centroid(
        unsigned long n,
        const void *observations,
        unsigned long k,
        unsigned long *cluster_map,
        void *centroid) {
    Point mean = {.x = 0, .y = 0};
    unsigned long cluster_size = 0;
    Point *points = (Point*)observations;

    long i;
    for (i = 0; i < n; i++) {
        if (k == cluster_map[i]) {
            mean.x += points[i].x;
            mean.y += points[i].y;
            cluster_size++;
        }
    }

    if (cluster_size != 0) {
        mean.x /= cluster_size;
        mean.y /= cluster_size;

        Point *c = (Point*)centroid;
        *c = mean;
    }
}

double point34_distance(const void *a, const void *b) {
    Point34 pa = *(Point34*)a;
    Point34 pb = *(Point34*)b;
    double diff[34];
    double distance = 0;

    int i;
    for (i = 0; i < 34; i++) {
        diff[i] = pa.f[i] - pb.f[i];
        distance += diff[i] * diff[i];
    }

    return distance;
}


void point34_update_centroid(
        unsigned long n,
        const void *observations,
        unsigned long k,
        unsigned long *cluster_map,
        void *centroid) {
    Point34 mean = {.f = {0}};
    unsigned long cluster_size = 0;
    Point34 *points = (Point34*)observations;

    unsigned long i;
    int j;
    for (i = 0; i < n; i++) {
        if (k == cluster_map[i]) {
            for (j = 0; j < 34; j++) {
                mean.f[j] += points[i].f[j];
            }
            cluster_size++;
        }
    }

    if (cluster_size != 0) {
        for (j = 0; j < 34; j++) {
            mean.f[j] /= cluster_size;
        }

        Point34 *c = (Point34*)centroid;
        *c = mean;
    }
}
