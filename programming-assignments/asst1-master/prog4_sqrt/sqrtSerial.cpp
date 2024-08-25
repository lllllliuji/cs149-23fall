#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void sqrtSerial(int N, float initialGuess, float values[], float output[]) {
    // static const float kThreshold = 0.00001f;

    // for (int i=0; i<N; i++) {

    //     float x = values[i];
    //     float guess = initialGuess;

    //     float error = fabs(guess * guess * x - 1.f);

    //     while (error > kThreshold) {
    //         guess = (3.f * guess - x * guess * guess * guess) * 0.5f;
    //         error = fabs(guess * guess * x - 1.f);
    //     }

    //     output[i] = x * guess;
    // }

    static const float kThreshold = 0.00001f;
    __m256 vThreshold = _mm256_set1_ps(kThreshold);
    __m256 vInitialGuess = _mm256_set1_ps(initialGuess);
    __m256 vOne = _mm256_set1_ps(1.0f);
    __m256 vHalf = _mm256_set1_ps(0.5f);
    __m256 vThree = _mm256_set1_ps(3.0f);

    for (int i = 0; i < N; i += 8) {
        // Load values with masking for remaining elements
        __m256i mask = _mm256_cmpgt_epi32(_mm256_set1_epi32(N - i), _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0));
        __m256 vValues = _mm256_maskload_ps(&values[i], mask);
        __m256 vGuess = vInitialGuess;

        // Calculate initial error
        __m256 vError = _mm256_sub_ps(_mm256_mul_ps(_mm256_mul_ps(vGuess, vGuess), vValues), vOne);
        vError = _mm256_andnot_ps(_mm256_set1_ps(-0.0f), vError);  // fabs for SIMD

        // Iterate until all errors are below the threshold
        while (_mm256_movemask_ps(_mm256_cmp_ps(vError, vThreshold, _CMP_GT_OQ))) {
            vGuess = _mm256_mul_ps(
                _mm256_sub_ps(_mm256_mul_ps(vThree, vGuess),
                              _mm256_mul_ps(vValues, _mm256_mul_ps(vGuess, _mm256_mul_ps(vGuess, vGuess)))),
                vHalf);
            vError = _mm256_sub_ps(_mm256_mul_ps(_mm256_mul_ps(vGuess, vGuess), vValues), vOne);
            vError = _mm256_andnot_ps(_mm256_set1_ps(-0.0f), vError);  // fabs for SIMD
        }

        // Store the results with masking
        __m256 vOutput = _mm256_mul_ps(vValues, vGuess);
        _mm256_maskstore_ps(&output[i], mask, vOutput);
    }
}
