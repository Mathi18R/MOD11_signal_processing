#pragma once
#include "buffer.h"
#include "cross_correlation.h"
#include "least_mean_square.h"
#include <mutex>

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
void angle_handler(void *pvParameters){
    
    esp_task_wdt_deinit();
    float tau_LF_RF;
    unsigned long end_time;
    unsigned long Ts_us = int(1000000.0f/Fs_output);
    Serial.print ("Hello angle! WOW");
    Serial.println(xPortGetCoreID());
    while(1){
        Serial.print("hello1");
        end_time = micros() + Ts_us;
        //calculate cross_correlation & find tau
        Serial.print("hello2");
        buffer_mutex.lock();
        Serial.print("hello3");
        cross_correlation(bufferLF, bufferRF, cross_correlation_buffer);
        tau_LF_RF = float(get_maximum(cross_correlation_buffer) - SIZE) / Fs_mic;
        Serial.print("hello4");
        buffer_mutex.unlock();

        //Send tau

        Serial.print(tau_LF_RF);

        
        unsigned long delay;
        while(micros() < end_time){
            delay++;
            
        }
       

    }
}
