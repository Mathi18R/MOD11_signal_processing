#pragma once
#include "cross_correlation.h"
#include <utility>

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
    float tau_LF_RF = 0;
    float tau_LF_LB = 0;
    float tau_LF_RB = 0;
	float tau_RF_LB = 0;
    float tau_RF_RB = 0;
    float tau_LB_RB = 0;

    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferRF, cross_correlation_buffer);
    tau_LF_RF = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;
    cross_correlation<float, SIZE, sample_swing>(bufferLB, bufferRB, cross_correlation_buffer);
    tau_LB_RB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;

    //Serial.print("tau_LF_RF:\t");
    //Serial.print((tau_LF_RF*1000000)+750);
    //Serial.print("\ttau_LB_RB:\t");
    //Serial.println((tau_LB_RB*1000000)+750);



    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferLB, cross_correlation_buffer);
    tau_LF_LB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;
    cross_correlation<float, SIZE, sample_swing>(bufferRF, bufferRB, cross_correlation_buffer);
    tau_RF_RB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;

    


    //Serial.print("tau_LF_LB: ");
    //Serial.println(tau_LF_LB*1000000);

    //Serial.print("tau_RF_RB: ");
    //Serial.println(tau_RF_RB*1000000);

    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferRB, cross_correlation_buffer);
    tau_LF_RB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;
    cross_correlation<float, SIZE, sample_swing>(bufferRF, bufferLB, cross_correlation_buffer);
    tau_RF_LB = float(get_maximum(cross_correlation_buffer) - sample_swing) / Fs_mic;

    Serial.print("tau_LF_RB:\t");
    Serial.print((tau_LF_RB*1000000)+750);
    Serial.print("\ttau_RF_LB:\t");
    Serial.println((tau_RF_LB*1000000)-750);

     //region = my_least_mean_square.find_region(tau_LF_RF, tau_LF_LB, tau_LF_RB, tau_RF_LB, tau_RF_RB, tau_LB_RB);
     //Serial.print("region: ");
     //Serial.println(region);
}
