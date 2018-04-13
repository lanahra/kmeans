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
#endif // UNIT_TESTING

Kmeans_context* alloc_kmeans_context(unsigned int k, unsigned int n) {
    Kmeans_context *kc = malloc(sizeof *kc);

    kc->k = k;
    kc->n = n;

    kc->cluster_map = malloc(kc->n * sizeof *kc->cluster_map);

    return kc;
}

void free_kmeans_context(Kmeans_context *kc) {
    if (kc != 0) {
        free(kc->cluster_map);
        free(kc);
    }
}

int kmeans(Kmeans_context *kc) {

    return 0;
}

double point_distance(const void *a, const void *b) {
    Point pa = *(Point*)a;
    Point pb = *(Point*)b;

    double dx = pa.x - pb.x;
    double dy = pa.y - pb.y;

    return (dx * dx) + (dy * dy);
}

void point_update_centroid(
        unsigned int n,
        const void *observations,
        unsigned int k,
        const void *cluster_map,
        void *centroid) {
    Point mean = {.x = 0, .y = 0};
    unsigned int cluster_size = 0;

    int i;
    for (i = 0; i < n; i++) {
        if (k == ((unsigned int*)cluster_map)[i]) {
            mean.x += ((Point*)observations)[i].x;
            mean.y += ((Point*)observations)[i].y;
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
