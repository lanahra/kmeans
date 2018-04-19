typedef struct Kmeans_context {
    // number of clusters
    unsigned long k;

    // number of observations
    unsigned long n;

    // number of features in observation
    unsigned long f;

    // array of k centroids
    double **centroids;

    // array of n observations
    double **observations;

    // map for each observation i to a cluster j
    unsigned long *cluster_map;
} Kmeans_context;

Kmeans_context* alloc_kmeans_context(
        unsigned long k,
        unsigned long n,
        unsigned long f
);

void free_kmeans_context(Kmeans_context *kc);

void kmeans(Kmeans_context *kc);
void kmeans_p(Kmeans_context *kc);

double distance(unsigned long f, double *a, double *b);

void update_centroid(
        unsigned long f,
        unsigned long n,
        const double **observation,
        unsigned long k,
        unsigned long *cluster_map,
        double *centroid
);
