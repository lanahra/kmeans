#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "kmeans.h"

int main(int argc, char **argv) {
    int opt;
    bool parallel = false;

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

        FILE *input = fopen(input_path, "r");
        if (input == 0) {
            exit(EXIT_FAILURE);
        }

        unsigned long i;
        unsigned long j;
        for (i = 0; i < kc->n; i++) {
            for (j = 0; j < kc->f; j++) {
                fscanf(input, "%lf ", &kc->observations[i][j]);
            }
            fscanf(input, "\n");
        }

        rewind(input);

        for (i = 0; i < kc->k; i++) {
            for (j = 0; j < kc->f; j++) {
                fscanf(input, "%lf ", &kc->centroids[i][j]);
            }
            fscanf(input, "\n");
        }

        fclose(input);

        clock_t begin = clock();

        if (parallel) {
            kmeans_p(kc);
        } else {
            kmeans(kc);
        }

        clock_t end = clock();
        double time = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("time: %lf\n", time);

        FILE *output = fopen(output_path, "w");
        if (output == 0) {
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < kc->n; i++) {
            fprintf(output, "%lu\n", kc->cluster_map[i]);
        }

        fclose(output);
    } else {
        exit(EXIT_FAILURE);
    }

    return 0;
}
