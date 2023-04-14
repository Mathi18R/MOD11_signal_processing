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
int angle_handler(){
    float tau_LF_RF = 0;
    float tau_LF_LB = 0;
    float tau_LF_RB = 0;
	float tau_RF_LB = 0;
    float tau_RF_RB = 0;
    float tau_LB_RB = 0;

    cross_correlation(bufferLF, bufferRF, cross_correlation_buffer1);
    tau_LF_RF = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;
    cross_correlation(bufferLB, bufferRB, cross_correlation_buffer2);
    tau_LB_RB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;

    cross_correlation(bufferLF, bufferLB, cross_correlation_buffer1);
    tau_LF_LB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;
    cross_correlation(bufferRF, bufferRB, cross_correlation_buffer2);
    tau_RF_RB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;

    cross_correlation(bufferLF, bufferRB, cross_correlation_buffer1);
    tau_LF_RB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;
    cross_correlation(bufferRF, bufferLB, cross_correlation_buffer2);
    tau_RF_LB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;


    region = my_least_mean_square.find_region(tau_LF_RF, tau_LF_LB, tau_LF_RB, tau_RF_LB, tau_RF_RB, tau_LB_RB);

}
