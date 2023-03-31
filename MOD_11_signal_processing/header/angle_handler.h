#pragma once
#include "buffer.h"
#include "cross_correlation.h"
#include "least_mean_square.h"
#include <mutex>


template<typename T, int SIZE>
void angle_handler(){
    
    buffer<float, (2*SIZE)-1> cross_correlation_buffer;
    float tau_LF_RF;
    float tau_LF_LB;
    float tau_LF_RB;
	float tau_RF_LB;
    float tau_RF_RB;
    float tau_LB_RB;
    least_mean_square my_least_mean_square({ -0.1, 0.1 }, { 0.1, 0.1 }, { 0.1, -0.1 }, { -0.1, -0.1 });
    
    unsigned long end_time;
    unsigned long Ts_us = int(1000000.0f/Fs_output);

    while(1){
        end_time = micros() + Ts_us;
        //calculate cross_correlation & find tau

        buffer_mutex.lock();
        cross_correlation(bufferLF, bufferRF, cross_correlation_buffer);
        tau_LF_RF = float(cross_correlation_buffer.get_maximum() + SIZE - 1) / Fs_mic;
        cross_correlation(bufferLF, bufferLB, cross_correlation_buffer);
        tau_LF_LB = float(cross_correlation_buffer.get_maximum() + SIZE - 1) / Fs_mic;
        cross_correlation(bufferLF, bufferRB, cross_correlation_buffer);
        tau_LF_RB = float(cross_correlation_buffer.get_maximum() + SIZE - 1) / Fs_mic;
        cross_correlation(bufferRF, bufferLB, cross_correlation_buffer);
        tau_RF_LB = float(cross_correlation_buffer.get_maximum() + SIZE - 1) / Fs_mic;
        cross_correlation(bufferRF, bufferRB, cross_correlation_buffer);
        tau_RF_RB = float(cross_correlation_buffer.get_maximum() + SIZE - 1) / Fs_mic;
        cross_correlation(bufferLB, bufferRB, cross_correlation_buffer);
        tau_LB_RB = float(cross_correlation_buffer.get_maximum() + SIZE - 1) / Fs_mic;

        //Do least_mean_square

        region = my_least_mean_square.find_region(tau_LF_RF, tau_LF_LB, tau_LF_RB, tau_RF_LB, tau_RF_RB, tau_LB_RB);

        
        unsigned long delay;
        while(micros < end_time){
            delay++;
        }
    }
}
