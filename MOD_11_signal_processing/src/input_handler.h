#pragma once

template<typename T, int SIZE>
void input_handler (int sample_amount){
    int iterations = 50;
    
    int ByteBufferSize = ceil(SIZE/iterations)*8;
    size_t bytes_read_L, bytes_read_R;
    uint8_t buffer_L[ByteBufferSize], buffer_R[ByteBufferSize];
     //
    for(int j=0; j < iterations; j++){
        //unsigned long int timeddd = micros();
        i2s_read(I2S_NUM_0, buffer_L, sizeof(buffer_L), &bytes_read_L, portMAX_DELAY);
        i2s_read(I2S_NUM_1, buffer_R, sizeof(buffer_R), &bytes_read_R, portMAX_DELAY);
        //unsigned long int timediff = (micros() - timeddd) / 1000;
        //Serial.print("I2S read time:\t");
        //Serial.println(timediff);
        
        for (int i = 0; i < bytes_read_L && i < bytes_read_R; i += 8) {
            
            //int32_t input_gnd_L = ((buffer_L[i] & 0x3F) << 26) | (buffer_L[i + 1] << 10) | (buffer_L[i + 2] << 2) | (buffer_L[i + 3] >> 6);
            int32_t input_gnd_L = (buffer_L[i + 3] << 24) | (buffer_L[i + 2] << 16) | (buffer_L[i + 1] << 8) | buffer_L[i];
             input_gnd_L >>= 8;

            int32_t input_high_L = (buffer_L[i + 7] << 24) | (buffer_L[i + 6] << 16) | (buffer_L[i + 5] << 8) | buffer_L[i + 4];
            input_high_L >>= 8;

            // Extract samples for I2S_NUM_1
            int32_t input_gnd_R = (buffer_R[i + 3] << 24) | (buffer_R[i + 2] << 16) | (buffer_R[i + 1] << 8) | buffer_R[i];
            input_gnd_R >>= 8;

            int32_t input_high_R = (buffer_R[i + 7] << 24) | (buffer_R[i + 6] << 16) | (buffer_R[i + 5] << 8) | buffer_R[i + 4];
            input_high_R >>= 8;

            //Print samples for all microphones in CSV format
           //Serial.print(input_gnd_L); //LB
           //Serial.print(",");
           //Serial.print(input_high_L); //LF
           //Serial.print(",");
           //Serial.print(input_high_R);// RB
           //Serial.print(",");
           //Serial.println(input_gnd_R); //RF

            bufferLB.insert(input_gnd_L);
            bufferLF.insert(input_high_L);
            bufferRB.insert(input_high_R);
            bufferRF.insert(input_gnd_R);
        }   
    }   
}