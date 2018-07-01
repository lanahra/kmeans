#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "kmeans.h"

int main(int argc, char **argv) {
    clock_t begin = clock();
    int opt;
    bool parallel = false;

    remove("/tmp/kmeans/sdc");

    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt) {
            case 'p':
                parallel = true;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        unsigned long k;
        unsigned long n;
        unsigned long f;

        sscanf(argv[optind++], "%lu", &k);
        sscanf(argv[optind++], "%lu", &n);
        sscanf(argv[optind++], "%lu", &f);

        char *input_path = argv[optind++];
        char *output_path = argv[optind++];

        Kmeans_context *kc = alloc_kmeans_context(k, n, f);
        Kmeans_context *duplicate = alloc_kmeans_context(k, n, f);

        FILE *input = fopen(input_path, "r");
        if (input == 0) {
            exit(EXIT_FAILURE);
        }

        unsigned long i;
        unsigned long j;
        for (i = 0; i < kc->n; i++) {
            for (j = 0; j < kc->f; j++) {
                fscanf(input, "%lf ", &kc->observations[i][j]);
                duplicate->observations[i][j] = kc->observations[i][j];
            }
            fscanf(input, "\n");
        }

        rewind(input);

        for (i = 0; i < kc->k; i++) {
            for (j = 0; j < kc->f; j++) {
                fscanf(input, "%lf ", &kc->centroids[i][j]);
                duplicate->centroids[i][j] = kc->centroids[i][j];
            }
            fscanf(input, "\n");
        }

        fclose(input);

        clock_t end;
        double time;
        do {
            end = clock();
            time = (double)(end - begin) / CLOCKS_PER_SEC;
        } while (time < 2.0);

        printf("read time: %lf\n", time);

        kmeans(kc);
        kmeans(duplicate);

        end = clock();
        time = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("time: %lf\n", time);

        if (memcmp(kc->cluster_map, duplicate->cluster_map, n * sizeof *kc->cluster_map) != 0) {
            FILE *sdc = fopen("/tmp/kmeans/sdc", "w");
            fclose(sdc);
        }

        FILE *output = fopen(output_path, "w");
        if (output == 0) {
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < kc->n; i++) {
            fprintf(output, "%lu\n", kc->cluster_map[i]);
        }

        fclose(output);

        free_kmeans_context(kc);
        free_kmeans_context(duplicate);
    } else {
        exit(EXIT_FAILURE);
    }

    return 0;
}
