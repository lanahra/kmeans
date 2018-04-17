typedef struct Kmeans_context {
    // number of clusters
    unsigned long k;

    // number of observations
    unsigned long n;

    // array of k centroids
    void **centroids;

    // array of n observations
    void **observations;

    // map for each observation i to a cluster j
    unsigned long *cluster_map;

    // distance between point a and b
    double (*distance)(const void *a, const void *b);

    // update centroid to center of its cluster
    void (*update_centroid)(
            unsigned long n,
            const void *observations,
            unsigned long k,
            unsigned long *cluster_map,
            void *centroid
    );
} Kmeans_context;

Kmeans_context* alloc_kmeans_context(unsigned long k, unsigned long n);

void free_kmeans_context(Kmeans_context *kc);

void kmeans(Kmeans_context *kc);

typedef struct Point {
    double x;
    double y;
} Point;

double point_distance(const void *a, const void *b);

void point_update_centroid(
        unsigned long n,
        const void *observations,
        unsigned long k,
        unsigned long *cluster_map,
        void *centroid
);

typedef struct Point34 {
    double f[34];
} Point34;

double point34_distance(const void *a, const void *b);

void point34_update_centroid(
        unsigned long n,
        const void *observations,
        unsigned long k,
        unsigned long *cluster_map,
        void *centroid
);
