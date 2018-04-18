#include <stdio.h>
#include <time.h>
#include "kmeans.h"

#define CENTROIDS 4
#define OBSERVATIONS 200000
Point34 centroids[CENTROIDS];
Point34 observations[OBSERVATIONS];

int main(int argc, char **argv) {
    clock_t begin = clock();

    Kmeans_context *kc = alloc_kmeans_context(CENTROIDS, OBSERVATIONS);
    kc->distance = point34_distance;
    kc->update_centroid = point34_update_centroid;

    FILE *f = fopen("observations.txt", "r");
    unsigned long i;
    int j;
    for (i = 0; i < kc->k; i++) {
        for (j = 0; j < 33; j++) {
            fscanf(f, "%lf ", &centroids[i].f[j]);
        }
        fscanf(f, "%lf \n", &centroids[i].f[33]);
        kc->centroids[i] = &centroids[i];
    }
    fclose(f);

    f = fopen("observations.txt", "r");
    for (i = 0; i < kc->n; i++) {
        for (j = 0; j < 33; j++) {
            fscanf(f, "%lf ", &observations[i].f[j]);
        }
        fscanf(f, "%lf \n", &observations[i].f[33]);
        kc->observations[i] = &observations[i];
    }
    fclose(f);

    kmeans(kc);

    f = fopen("output.txt", "w");
    for (i = 0; i < kc->n; i++) {
        fprintf(f, "%lu\n", kc->cluster_map[i]);
    }
    fclose(f);

    free_kmeans_context(kc);

    clock_t end = clock();
    double time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("time: %lf\n", time);

    return 0;
}
