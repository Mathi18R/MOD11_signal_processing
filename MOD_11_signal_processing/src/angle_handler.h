#pragma once
#include "cross_correlation.h"
#include <utility>

template<typename T, int SIZE>
std::pair<T, int> get_maximum(T (&array)[SIZE]){
    std::pair<T, int> return_pair;
    return_pair.first = 0;
    return_pair.second = 0;
    for(int i=1; i < SIZE; i++){
        if(array[i] > return_pair.first){
            return_pair.first = array[i];
            return_pair.second = i;
        }
    }
    return return_pair;
}


template<typename T, int SIZE>
void angle_handler(){
    float tau_LF_RF = 0;
    float tau_LF_LB = 0;
    float tau_LF_RB = 0;
	float tau_RF_LB = 0;
    float tau_RF_RB = 0;
    float tau_LB_RB = 0;

    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferRF, cross_correlation_buffer);
    std::pair<T, int> maximum_LF_RF = get_maximum(cross_correlation_buffer);
    tau_LF_RF = float(maximum_LF_RF.second - sample_swing) / Fs_mic;
    cross_correlation<float, SIZE, sample_swing>(bufferLB, bufferRB, cross_correlation_buffer);
    std::pair<T, int> maximum_LB_RB = get_maximum(cross_correlation_buffer);
    tau_LB_RB = float(maximum_LB_RB.second - sample_swing) / Fs_mic;

    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferLB, cross_correlation_buffer);
    std::pair<T, int> maximum_LF_LB = get_maximum(cross_correlation_buffer);
    tau_LF_LB = float(maximum_LF_LB.second - sample_swing) / Fs_mic;
    cross_correlation<float, SIZE, sample_swing>(bufferRF, bufferRB, cross_correlation_buffer);
    std::pair<T, int> maximum_tau_LF_LB = get_maximum(cross_correlation_buffer);
    tau_RF_RB = float(maximum_RF_RB.second - sample_swing) / Fs_mic;

    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferRB, cross_correlation_buffer);
    std::pair<T, int> maximum_LF_RB = get_maximum(cross_correlation_buffer);
    tau_LF_RB = float(maximum_LF_RB.second - sample_swing) / Fs_mic;
    cross_correlation<float, SIZE, sample_swing>(bufferRF, bufferLB, cross_correlation_buffer);
    std::pair<T, int> maximum_RF_LB = get_maximum(cross_correlation_buffer);
    tau_RF_LB = float(maximum_RF_LB.second - sample_swing) / Fs_mic;

    if(my_moving_average.insert(maximum_LF_RF.first + maximum_LB_RB.first + 
                                maximum_LF_LB.first + maximum_LF_LB.first +
                                maximum_LF_RB.first + maximum_RF_LB.first)){
        region = 0;
    }
    else{
        region = my_least_mean_square.find_region(tau_LF_RF, tau_LF_LB, tau_LF_RB, tau_RF_LB, tau_RF_RB, tau_LB_RB);
    }
}
