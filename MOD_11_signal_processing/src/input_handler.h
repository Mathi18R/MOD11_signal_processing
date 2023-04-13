#pragma once

template<typename T>
T get_value(int pin){
    //Do reading of microphone value here
    T value = analogRead(pin);
    return value;
}

template<typename T, int SIZE>
void input_handler(int sample_amount){
    //This sets up the variables for update_buffer
    unsigned long end_time;
    unsigned long Ts_us = int(1000000.0f/Fs_mic);
    Serial.print ("Hello input! UWU");
    Serial.println(xPortGetCoreID());
    int sample = 0;

    while(sample < sample_amount){
        //This is the loop to update the buffers
        end_time = micros() + Ts_us;
        T inputLF = get_value<T>(pin_micLF);
        T inputRF = get_value<T>(pin_micRF);
        

        buffer_mutex.lock();
        bufferLF.insert(inputLF);
        bufferRF.insert(inputRF);
        buffer_mutex.unlock();

        Serial.print ("inputLF: ");
        Serial.print (inputLF);
        Serial.print ("    inputRF: ");
        Serial.println (inputRF);

        unsigned long delay;
        while(micros() < end_time){
            delay++;
        }
        sample++;
    }
}
