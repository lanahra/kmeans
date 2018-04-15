#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmockery.h>
#include <kmeans.h>

extern void assign_clusters(Kmeans_context *kc);

void test_alloc_kmeans_context(void **state) {
    Kmeans_context* const kc = alloc_kmeans_context(3, 10);

    assert_true(kc);
    assert_int_equal(kc->k, 3);
    assert_int_equal(kc->n, 10);
    assert_true(kc->observations);
    assert_true(kc->centroids);
    assert_true(kc->cluster_map);

    free(kc->observations);
    free(kc->centroids);
    free(kc->cluster_map);
    free(kc);
}

void test_free_kmeans_context(void **state) {
    Kmeans_context* const kc = malloc(sizeof *kc);
    kc->k = 3;
    kc->n = 10;
    kc->observations = malloc(kc->n * sizeof *kc->observations);
    kc->centroids = malloc(kc->k * sizeof *kc->centroids);
    kc->cluster_map = malloc(kc->n * sizeof *kc->cluster_map);

    free_kmeans_context(kc);
}

void test_point_distance(void **state) {
    Point a = {.x = 2, .y = 4};
    Point b = {.x = 1, .y = 2};

    double expected = 5.0;
    double result = point_distance(&a, &b);

    assert_memory_equal(&result, &expected, sizeof expected);
}

void test_point_update_centroid(void **state) {
    const Point observations[10] = {
        {.x = 1, .y = 1},
        {.x = 2, .y = 2},
        {.x = 3, .y = 3},
        {.x = 4, .y = 4},
        {.x = 5, .y = 5},
        {.x = 6, .y = 6},
        {.x = 7, .y = 7},
        {.x = 8, .y = 8},
        {.x = 9, .y = 9},
        {.x = 10, .y = 10},
    };
    unsigned int cluster_map[10] = {1, 1, 1, 2, 2, 2, 2, 3, 3, 3};

    Point result_a;
    Point result_b;
    Point result_c;
    point_update_centroid(10, observations, 1, cluster_map, &result_a);
    point_update_centroid(10, observations, 2, cluster_map, &result_b);
    point_update_centroid(10, observations, 3, cluster_map, &result_c);

    Point expected_a = {.x = 2, .y = 2};
    Point expected_b = {.x = 5.5, .y = 5.5};
    Point expected_c = {.x = 9, .y = 9};

    assert_memory_equal(&result_a, &expected_a, sizeof expected_a);
    assert_memory_equal(&result_b, &expected_b, sizeof expected_b);
    assert_memory_equal(&result_c, &expected_c, sizeof expected_c);
}

void test_assign_clusters(void **state) {
    Point centroids[3] = {
        {.x = 1, .y = 1},
        {.x = 5, .y = 5},
        {.x = 7, .y = 7},
    };
    Point observations[10] = {
        {.x = 1, .y = 1},
        {.x = 2, .y = 2},
        {.x = 3, .y = 3},
        {.x = 4, .y = 4},
        {.x = 5, .y = 5},
        {.x = 6, .y = 6},
        {.x = 7, .y = 7},
        {.x = 8, .y = 8},
        {.x = 9, .y = 9},
        {.x = 10, .y = 10},
    };

    Kmeans_context* const kc = alloc_kmeans_context(3, 10);
    kc->distance = point_distance;
    kc->update_centroid = point_update_centroid;

    int i;
    for (i = 0; i < kc->n; i++) {
        kc->observations[i] = &observations[i];
    }

    for (i = 0; i < kc->k; i++) {
        kc->centroids[i] = &centroids[i];
    }

    assign_clusters(kc);

    unsigned int expected[10] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 2};

    for (i = 0; i < kc->n; i++) {
        assert_int_equal(kc->cluster_map[i], expected[i]);
    }

    free_kmeans_context(kc);
}

void test_kmeans(void **state) {
}

int main(int argc, char **argv) {
    const UnitTest tests[] = {
        unit_test(test_alloc_kmeans_context),
        unit_test(test_free_kmeans_context),
        unit_test(test_point_distance),
        unit_test(test_point_update_centroid),
        unit_test(test_assign_clusters),
    };

    return run_tests(tests);
}
