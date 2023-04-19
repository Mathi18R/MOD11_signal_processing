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
int angle_handler(){
    float tau_LF_RF = 0;
    float tau_LF_LB = 0;
    float tau_RF_RB = 0;
    float tau_LB_RB = 0;

    float delayfix = 775.0f/1000000.0f;
    int offset = delayfix * Fs_mic;


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


  //Serial.print("tau_LF_RF:\t");
  //Serial.print((tau_LF_RF*1000000));
  //Serial.print("\ttau_LB_RB:\t");
  //Serial.print((tau_LB_RB*1000000));


  //Serial.print("\ttau_LF_LB:\t");
  //Serial.print(tau_LF_LB*1000000);
  //Serial.print("\ttau_RF_RB:\t");
  //Serial.print(tau_RF_RB*1000000);
  //

  //Serial.print("\ttau_LF_RB:\t");
  //Serial.print((tau_LF_RB*1000000));
  //Serial.print("\ttau_RF_LB:\t");
  //Serial.println((tau_RF_LB*1000000));

    if(my_moving_average.insert(maximum_LF_RF.first + maximum_LB_RB.first + 
                                maximum_LF_LB.first + maximum_LF_LB.first)){
        region = 0;
    }
    else{
        region = sign_angle_finder(tau_LF_RF, tau_LF_LB, tau_RF_RB, tau_LB_RB);
    }

    switch(region){
        case 0:
            break;
        case 1:
            Serial.println("front");
            break;
        case 2:
            Serial.println("front right");
            break;
        case 3:
            Serial.println("right");
            break;
        case 4:
            Serial.println("back right");
            break;
        case 5:
            Serial.println("back");
            break;
        case 6:
            Serial.println("back left");
            break;
        case 7:
            Serial.println("left");
            break;
        case 8:
            Serial.println("front left");
            break;
        default:
            break;
    }
        
    //Serial.println(region);

    return region;
     
}
