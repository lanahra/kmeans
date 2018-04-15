#include <stdio.h>
#include <kmeans.h>

static const unsigned int centroids_n = 10;
static const unsigned int observations_n = 33920;

int main(int argc, char **argv) {
    Kmeans_context *kc = alloc_kmeans_context(centroids_n, observations_n);
    kc->distance = point_distance;
    kc->update_centroid = point_update_centroid;

    Point centroids[centroids_n];
    Point observations[observations_n];

    FILE *f = fopen("centroids.txt", "r");
    int i;
    for (i = 0; i < kc->k; i++) {
        fscanf(f, "%lf %lf\n", &centroids[i].x, &centroids[i].y);
        kc->centroids[i] = &centroids[i];
    }
    fclose(f);

    f = fopen("observations.txt", "r");
    for (i = 0; i < kc->n; i++) {
        fscanf(f, "%lf %lf\n", &observations[i].x, &observations[i].y);
        kc->observations[i] = &observations[i];
    }
    fclose(f);

    kmeans(kc);

    f = fopen("gold.txt", "w");
    for (i = 0; i < kc->n; i++) {
        fprintf(f, "%u\n", kc->cluster_map[i]);
    }
    fclose(f);

    free_kmeans_context(kc);
    return 0;
}
