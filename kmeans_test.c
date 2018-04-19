#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmockery.h>
#include "kmeans.h"

extern void assign_clusters(Kmeans_context *kc);
extern void update_centroids(Kmeans_context *kc);
extern void assign_clusters_p(Kmeans_context *kc);

void test_alloc_free_kmeans_context(void **state) {
    Kmeans_context* const kc = alloc_kmeans_context(3, 10, 2);

    assert_true(kc);
    assert_int_equal(kc->k, 3);
    assert_int_equal(kc->n, 10);
    assert_int_equal(kc->f, 2);
    assert_true(kc->observations);
    assert_true(kc->centroids);
    assert_true(kc->cluster_map);

    free_kmeans_context(kc);
}

void test_distance(void **state) {
    unsigned long f = 2;
    double *a = malloc(f * sizeof *a);
    double *b = malloc(f * sizeof *a);

    a[0] = 1;
    a[1] = 1;

    b[0] = 3;
    b[1] = 3;

    double result = distance(2, a, b);
    double expected = 8;

    assert_memory_equal(&result, &expected, sizeof expected);

    free(a);
    free(b);
}

void test_update_centroid(void **state) {
    double points[5][2] = {
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {5, 5},
    };

    double result[2];

    unsigned long cluster_map[5] = {0, 0, 0, 1, 1};

    unsigned long f = 2;
    unsigned long n = 5;

    double **observations = malloc(n * sizeof *observations);

    int i;
    int j;
    for (i = 0; i < n; i++) {
        observations[i] = malloc(f * sizeof *observations[i]);

        for (j = 0; j < f; j++ ) {
            observations[i][j] = points[i][j];
        }
    }


   update_centroid(f, n, (const double**)observations, 0, cluster_map, result);

   double expected[2] = {2, 2};

   assert_memory_equal(result, expected, sizeof expected);

   for (i = 0; i < n; i++) {
       free(observations[i]);
   }

   free(observations);
}

void test_assign_clusters(void **state) {
    double observations[5][2] = {
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {5, 5},
    };

    double centroids[2][2] = {
        {1, 1},
        {4, 4},
    };

    Kmeans_context *kc = alloc_kmeans_context(2, 5, 2);

    int i;
    int j;
    for (i = 0; i < kc->n; i++) {
        for (j = 0; j < kc->f; j++) {
            kc->observations[i][j] = observations[i][j];
        }
    }

    for (i = 0; i < kc->k; i++) {
        for (j = 0; j < kc->f; j++) {
            kc->centroids[i][j] = centroids[i][j];
        }
    }

    assign_clusters(kc);

    unsigned long expected[5] = {0, 0, 1, 1, 1};

    assert_memory_equal(kc->cluster_map, expected, sizeof expected);

    free_kmeans_context(kc);
}

void test_update_centroids(void **state) {
    double observations[5][2] = {
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {5, 5},
    };

    unsigned long cluster_map[5] = {0, 0, 1, 1, 1};

    Kmeans_context *kc = alloc_kmeans_context(2, 5, 2);

    int i;
    int j;
    for (i = 0; i < kc->n; i++) {
        kc->cluster_map[i] = cluster_map[i];

        for (j = 0; j < kc->f; j++) {
            kc->observations[i][j] = observations[i][j];
        }
    }

    update_centroids(kc);

    double expected[2][2] = {
        {1.5, 1.5},
        {4.0, 4.0},
    };

    for (i = 0; i < kc->k; i++) {
        assert_memory_equal(kc->centroids[i], expected[i], sizeof expected[i]);
    }

    free_kmeans_context(kc);
}

void test_kmeans(void **state) {
    double observations[5][2] = {
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4},
        {5, 5},
    };

    double centroids[2][2] = {
        {1, 1},
        {4, 4},
    };

    Kmeans_context *kc = alloc_kmeans_context(2, 5, 2);

    int i;
    int j;
    for (i = 0; i < kc->n; i++) {
        for (j = 0; j < kc->f; j++) {
            kc->observations[i][j] = observations[i][j];
        }
    }

    for (i = 0; i < kc->k; i++) {
        for (j = 0; j < kc->f; j++) {
            kc->centroids[i][j] = centroids[i][j];
        }
    }

    kmeans(kc);

    unsigned long expected[5] = {0, 0, 1, 1, 1};

    assert_memory_equal(kc->cluster_map, expected, sizeof expected);

    free_kmeans_context(kc);
}

int main(int argc, char **argv) {
    const UnitTest tests[] = {
        unit_test(test_alloc_free_kmeans_context),
        unit_test(test_distance),
        unit_test(test_update_centroid),
        unit_test(test_assign_clusters),
        unit_test(test_update_centroids),
        unit_test(test_kmeans),
    };

    return run_tests(tests);
}
