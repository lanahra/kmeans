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

    kc->centroids = malloc(kc->k * sizeof *kc->centroids);
    kc->observations = malloc(kc->n * sizeof *kc->observations);
    kc->cluster_map = malloc(kc->n * sizeof *kc->cluster_map);

    return kc;
}

int free_kmeans_context(Kmeans_context *kc) {
    return 0;
}

int kmeans(Kmeans_context *kc) {
    return 0;
}
