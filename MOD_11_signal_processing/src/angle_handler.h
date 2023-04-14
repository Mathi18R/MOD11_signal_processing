#pragma once
#include "cross_correlation.h"
#include "least_mean_square.h"

template<typename T, int SIZE>
int get_maximum(T (&array)[SIZE]){
    T maximum = 0;
    int index = 0;
    for(int i=1; i < SIZE; i++){
        if(array[i] > maximum){
            maximum = array[i];
            index = i;
        }
    }
    return index;
}

template<typename T, int SIZE>
void angle_handler(){
    float tau_LF_RF;

    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferRF, cross_correlation_buffer);
    tau_LF_RF = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;

    Serial.println(tau_LF_RF*1000000);
}
