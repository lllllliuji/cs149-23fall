#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <thread>

#include "CycleTimer.h"

using namespace std;

typedef struct {
    // Control work assignments
    int start, end;

    int startP, endP;  // start point, end point
    // Shared by all functions
    double *data;
    double *clusterCentroids;
    int *clusterAssignments;
    double *currCost;
    int M, N, K;
} WorkerArgs;

/**
 * Checks if the algorithm has converged.
 *
 * @param prevCost Pointer to the K dimensional array containing cluster costs
 *    from the previous iteration.
 * @param currCost Pointer to the K dimensional array containing cluster costs
 *    from the current iteration.
 * @param epsilon Predefined hyperparameter which is used to determine when
 *    the algorithm has converged.
 * @param K The number of clusters.
 *
 * NOTE: DO NOT MODIFY THIS FUNCTION!!!
 */
static bool stoppingConditionMet(double *prevCost, double *currCost, double epsilon, int K) {
    for (int k = 0; k < K; k++) {
        if (abs(prevCost[k] - currCost[k]) > epsilon) return false;
    }
    return true;
}

/**
 * Computes L2 distance between two points of dimension nDim.
 *
 * @param x Pointer to the beginning of the array representing the first
 *     data point.
 * @param y Poitner to the beginning of the array representing the second
 *     data point.
 * @param nDim The dimensionality (number of elements) in each data point
 *     (must be the same for x and y).
 */
double dist(double *x, double *y, int nDim) {
    double accum = 0.0;
    for (int i = 0; i < nDim; i++) {
        accum += pow((x[i] - y[i]), 2);
    }
    return sqrt(accum);
}

/**
 * Assigns each data point to its "closest" cluster centroid.
 */
void computeAssignments(WorkerArgs *const args) {
    double *minDist = new double[args->endP - args->startP];

    // Initialize arrays
    for (int m = args->startP; m < args->endP; m++) {
        minDist[m - args->startP] = 1e30;
        args->clusterAssignments[m] = -1;
    }

    // Assign datapoints to closest centroids
    for (int m = args->startP; m < args->endP; m++) {
        for (int k = args->start; k < args->end; k++) {
            double d = dist(&args->data[m * args->N], &args->clusterCentroids[k * args->N], args->N);
            if (d < minDist[m - args->startP]) {
                minDist[m - args->startP] = d;
                args->clusterAssignments[m] = k;
            }
        }
    }

    free(minDist);
}

/**
 * Given the cluster assignments, computes the new centroid locations for
 * each cluster.
 */
void computeCentroids(WorkerArgs *const args) {
    int counts[args->K];

    // Zero things out
    for (int k = 0; k < args->K; k++) {
        counts[k] = 0;
        for (int n = 0; n < args->N; n++) {
            args->clusterCentroids[k * args->N + n] = 0.0;
        }
    }

    // Sum up contributions from assigned examples
    for (int m = 0; m < args->M; m++) {
        int k = args->clusterAssignments[m];
        for (int n = 0; n < args->N; n++) {
            args->clusterCentroids[k * args->N + n] += args->data[m * args->N + n];
        }
        counts[k]++;
    }

    // Compute means
    for (int k = 0; k < args->K; k++) {
        counts[k] = max(counts[k], 1);  // prevent divide by 0
        for (int n = 0; n < args->N; n++) {
            args->clusterCentroids[k * args->N + n] /= counts[k];
        }
    }
}

/**
 * Computes the per-cluster cost. Used to check if the algorithm has converged.
 */
void computeCost(WorkerArgs *const args) {
    double accum[args->K];

    // Zero things out
    for (int k = 0; k < args->K; k++) {
        accum[k] = 0.0;
    }

    // Sum cost for all data points assigned to centroid
    for (int m = 0; m < args->M; m++) {
        int k = args->clusterAssignments[m];
        accum[k] += dist(&args->data[m * args->N], &args->clusterCentroids[k * args->N], args->N);
    }

    // Update costs
    for (int k = args->start; k < args->end; k++) {
        args->currCost[k] = accum[k];
    }
}

/**
 * Computes the K-Means algorithm, using std::thread to parallelize the work.
 *
 * @param data Pointer to an array of length M*N representing the M different N
 *     dimensional data points clustered. The data is layed out in a "data point
 *     major" format, so that data[i*N] is the start of the i'th data point in
 *     the array. The N values of the i'th datapoint are the N values in the
 *     range data[i*N] to data[(i+1) * N].
 * @param clusterCentroids Pointer to an array of length K*N representing the K
 *     different N dimensional cluster centroids. The data is laid out in
 *     the same way as explained above for data.
 * @param clusterAssignments Pointer to an array of length M representing the
 *     cluster assignments of each data point, where clusterAssignments[i] = j
 *     indicates that data point i is closest to cluster centroid j.
 * @param M The number of data points to cluster.
 * @param N The dimensionality of the data points.
 * @param K The number of cluster centroids.
 * @param epsilon The algorithm is said to have converged when
 *     |currCost[i] - prevCost[i]| < epsilon for all i where i = 0, 1, ..., K-1
 */
void kMeansThread(double *data, double *clusterCentroids, int *clusterAssignments, int M, int N, int K,
                  double epsilon) {
    // Used to track convergence
    double *prevCost = new double[K];
    double *currCost = new double[K];

    // The WorkerArgs array is used to pass inputs to and return output from
    // functions.
    WorkerArgs args;
    args.data = data;
    args.clusterCentroids = clusterCentroids;
    args.clusterAssignments = clusterAssignments;
    args.currCost = currCost;
    args.M = M;
    args.N = N;
    args.K = K;

    // Initialize arrays to track cost
    for (int k = 0; k < K; k++) {
        prevCost[k] = 1e30;
        currCost[k] = 0.0;
    }

    /* Main K-Means Algorithm Loop */
    int iter = 0;
    const int numThreads = 16;
    double computeAssignmentsTime = 0.0;
    double computeCentroidsTime = 0.0;
    double computeCostTime = 0.0;
    double startTime, endTime;
    WorkerArgs threadArgs[numThreads];
    int startP = 0;
    int extraP = M % numThreads;
    for (int i = 0; i < numThreads; i++) {
        threadArgs[i].start = 0;
        threadArgs[i].end = K;
        threadArgs[i].data = data;
        threadArgs[i].clusterCentroids = clusterCentroids;
        threadArgs[i].clusterAssignments = clusterAssignments;
        threadArgs[i].currCost = currCost;
        threadArgs[i].M = M;
        threadArgs[i].N = N;
        threadArgs[i].K = K;
        threadArgs[i].startP = startP;
        threadArgs[i].endP = startP + (M / numThreads) + (i < extraP);
        startP = threadArgs[i].endP;
    }
    while (!stoppingConditionMet(prevCost, currCost, epsilon, K)) {
        // Update cost arrays (for checking convergence criteria)
        for (int k = 0; k < K; k++) {
            prevCost[k] = currCost[k];
        }

        // Creates thread objects that do not yet represent a thread.
        std::thread workers[numThreads];

        startTime = CycleTimer::currentSeconds();
        // Spawn the worker threads.  Note that only numThreads-1 std::threads
        // are created and the main application thread is used as a worker
        // as well.
        for (int i = 1; i < numThreads; i++) {
            workers[i] = std::thread(computeAssignments, &threadArgs[i]);
        }

        computeAssignments(&threadArgs[0]);

        // join worker threads
        for (int i = 1; i < numThreads; i++) {
            workers[i].join();
        }
        // Setup args struct
        args.start = 0;
        args.end = K;

        // computeAssignments(&args);
        endTime = CycleTimer::currentSeconds();
        computeAssignmentsTime += endTime - startTime;

        startTime = CycleTimer::currentSeconds();
        computeCentroids(&args);
        endTime = CycleTimer::currentSeconds();
        computeCentroidsTime += endTime - startTime;

        startTime = CycleTimer::currentSeconds();
        computeCost(&args);
        endTime = CycleTimer::currentSeconds();
        computeCostTime += endTime - startTime;

        iter++;
    }
    printf("Iter %d assignment time %lfs centroids time %lfs cost time %lfs\n", iter, computeAssignmentsTime,
           computeCentroidsTime, computeCostTime);
    free(currCost);
    free(prevCost);
}
