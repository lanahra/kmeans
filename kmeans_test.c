#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmockery.h>
#include <kmeans.h>

void test_alloc_kmeans_context(void **state) {
    Kmeans_context* const kc = alloc_kmeans_context(3, 10);

    assert_true(kc);
    assert_int_equal(kc->k, 3);
    assert_int_equal(kc->n, 10);
    assert_true(kc->centroids);
    assert_true(kc->observations);
    assert_true(kc->cluster_map);

    free(kc->centroids);
    free(kc->observations);
    free(kc->cluster_map);
    free(kc);
}

void test_free_kmeans_context(void **state) {
    Kmeans_context* const kc = malloc(sizeof *kc);
    kc->k = 3;
    kc->n = 10;
    kc->centroids = malloc(kc->k * sizeof *kc->centroids);
    kc->observations = malloc(kc->n * sizeof *kc->observations);
    kc->cluster_map = malloc(kc->n * sizeof *kc->cluster_map);

    free_kmeans_context(kc);
}

int main(int argc, char **argv) {
    const UnitTest tests[] = {
        unit_test(test_alloc_kmeans_context),
        unit_test(test_free_kmeans_context)
    };

    return run_tests(tests);
}
