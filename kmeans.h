typedef struct Kmeans_context {
    // number of clusters
    unsigned int k;

    // number of observations
    unsigned int n;

    // array of k centroids
    void **centroids;

    // array of n observations
    void **observations;

    // map for each observation i to a cluster j
    unsigned int *cluster_map;

    // distance between point a and b
    double (*distance)(const void *a, const void *b);

    // update centroid to center of its cluster
    void (*update_centroid)(
            unsigned int n,
            const void **observations,
            unsigned int k,
            const int *cluster_map,
            void *centroid
    );
} Kmeans_context;

Kmeans_context* alloc_kmeans_context(unsigned int k, unsigned int n);

int free_kmeans_context(Kmeans_context *kc);

int kmeans(Kmeans_context *kc);
