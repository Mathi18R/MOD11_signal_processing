#pragma once
#include <utility>
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

void cross_LF_RF(void* pvParameters){
    *((*(std::pair<float*, bool*>*)pvParameters).second) = true;
    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferRF, cross_correlation_buffer1);
    *((*(std::pair<float*, bool*>*)pvParameters).first) = float(get_maximum(cross_correlation_buffer1) - sample_swing) / Fs_mic;
    *((*(std::pair<float*, bool*>*)pvParameters).second) = false;
}

void cross_LB_RB(float* tau_location){
    cross_correlation<float, SIZE, sample_swing>(bufferLB, bufferRB, cross_correlation_buffer2);
    *tau_location = float(get_maximum(cross_correlation_buffer2) - sample_swing) / Fs_mic;
}

void cross_LF_LB(void* pvParameters){
    *((*(std::pair<float*, bool*>*)pvParameters).second) = true;
    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferLB, cross_correlation_buffer1);
    *((*(std::pair<float*, bool*>*)pvParameters).first) = float(get_maximum(cross_correlation_buffer1) - sample_swing) / Fs_mic;
    *((*(std::pair<float*, bool*>*)pvParameters).second) = false;
}

void cross_RF_RB(float* tau_location){
    cross_correlation<float, SIZE, sample_swing>(bufferRF, bufferRB, cross_correlation_buffer2);
    *tau_location = float(get_maximum(cross_correlation_buffer2) - sample_swing) / Fs_mic;
}

void cross_LF_RB(void* pvParameters){
    *((*(std::pair<float*, bool*>*)pvParameters).second) = true;
    cross_correlation<float, SIZE, sample_swing>(bufferLF, bufferRB, cross_correlation_buffer1);
    *((*(std::pair<float*, bool*>*)pvParameters).first) = float(get_maximum(cross_correlation_buffer1) - sample_swing) / Fs_mic;
    *((*(std::pair<float*, bool*>*)pvParameters).second) = false;
}

void cross_RF_LB(float* tau_location){
    cross_correlation<float, SIZE, sample_swing>(bufferRF, bufferLB, cross_correlation_buffer2);
    *tau_location = float(get_maximum(cross_correlation_buffer2) - sample_swing) / Fs_mic;
}

template<typename T, int SIZE>
void angle_handler(){
    float tau_LF_RF = 0;
    float tau_LF_LB = 0;
    float tau_LF_RB = 0;
	float tau_RF_LB = 0;
    float tau_RF_RB = 0;
    float tau_LB_RB = 0;
    int delay = 0;

    bool thread_running = false;

    std::pair<float*, bool*> input;
    input.second = &thread_running;
    input.first = &tau_LF_RF;
    void* input_arguments = &input;
    

    TaskHandle_t TaskHandle;
    xTaskCreatePinnedToCore(    
        cross_LF_RF,                    /*function*/
        "cross_LF_RF",                  /* name of task */
        1024,                           /* Stack size of task */
        input_arguments,                /* parameter of the task */
        1,                              /* priority of the task */
        &TaskHandle,                    /* Task handle to keep track of created task */
        1 - xPortGetCoreID()            /* pin task to core 0 */ 
        );
    cross_LB_RB(&tau_LB_RB);
    delay = 0;
    while (thread_running == true)
    {
        delay++;
    }

    input.first = &tau_LF_LB;
    input_arguments = &input;
    xTaskCreatePinnedToCore(    
        cross_LF_LB,                    /*function*/
        "cross_LF_LB",                  /* name of task */
        1024,                           /* Stack size of task */
        input_arguments,                /* parameter of the task */
        1,                              /* priority of the task */
        &TaskHandle,                    /* Task handle to keep track of created task */
        1 - xPortGetCoreID()            /* pin task to core 0 */ 
        );
    cross_RF_RB(&tau_RF_RB);
    delay = 0;
    while (thread_running == true)
    {
        delay++;
    }
    
    input.first = &tau_LF_RB;
    input_arguments = &input;
    xTaskCreatePinnedToCore(    
        cross_LF_RB,                    /*function*/
        "cross_LF_RB",                  /* name of task */
        1024,                           /* Stack size of task */
        input_arguments,                /* parameter of the task */
        1,                              /* priority of the task */
        &TaskHandle,                    /* Task handle to keep track of created task */
        1 - xPortGetCoreID()            /* pin task to core 0 */ 
        );
    cross_RF_LB(&tau_RF_LB);
    delay = 0;
    while (thread_running == true)
    {
        delay++;
    }

    region = my_least_mean_square.find_region(tau_LF_RF, tau_LF_LB, tau_LF_RB, tau_RF_LB, tau_RF_RB, tau_LB_RB);

}
