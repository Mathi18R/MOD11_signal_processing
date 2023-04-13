#include "buffer.h"

#pragma once

template<typename T, int SIZE>
T cross_correlation_sub(const buffer<T, SIZE>& bufferA, const buffer<T, SIZE>& bufferB, int offset=0) {
    T result = T(0);

    for (int i = 0; i < SIZE; i++) {
        result += bufferA[i] * bufferB[i + offset];
    }

    return result;
}

template<typename T, int SIZE, int sample_swing>
void cross_correlation(const buffer<T, SIZE>& bufferA, const buffer<T, SIZE>& bufferB, T (&result)[(2 * sample_swing) - 1]) {
    for (int i = -sample_swing +1 ; i < sample_swing; i++) {
        result[sample_swing - i - 1] = cross_correlation_sub(bufferA, bufferB, i);
    }
}