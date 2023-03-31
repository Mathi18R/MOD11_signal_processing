#pragma once
#include "buffer.h"
#include <mutex>

template<typename T>
T get_value(){
    //Do reading of microphone value here
    return 0;
}

template<typename T, int SIZE>
void input_handler(float Fs){
    //This sets up the variables for update_buffer
    unsigned long end_time;
    unsigned long Ts_us = int(1000000.0f/Fs_mic);

    while(1){
        //This is the loop to update the buffers
        end_time = micros() + Ts_us;
        T inputLF = get_value();
        T inputRF = get_value();
        T inputLB = get_value();
        T inputRB = get_value();

        buffer_mutex.lock();
        bufferLF.insert(inputLF);
        bufferRF.insert(inputRF);
        bufferLB.insert(inputLB);
        bufferRB.insert(inputRB);
        buffer_mutex.unlock();

        unsigned long delay;
        while(micros < end_time){
            delay++;
        }
    }
}