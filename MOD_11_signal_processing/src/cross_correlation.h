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

template<typename T, int SIZE>
void cross_correlation(const buffer<T, SIZE>& bufferA, const buffer<T, SIZE>& bufferB, T (&result)[(2 * SIZE) - 1]) {
	Serial.print("hello5");
	for (int i = -SIZE +1 ; i < SIZE; i++) {
		Serial.print(i);
		Serial.print (" ");
		result[SIZE - i - 1] = cross_correlation_sub(bufferA, bufferB, i);
	}
	Serial.print("hello6");
}
