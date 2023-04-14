#pragma once



template<typename T, int SIZE>
void input_handler (int sample_amount){
    int iterations = 8;
    size_t bytes_read;
    int ByteBufferSize = ceil(SIZE/iterations)*8;
    uint8_t buffer[ByteBufferSize]; //
    for(int j=0; j < iterations; j++){
        //unsigned long int timeddd = micros();
        i2s_read(I2S_NUM, buffer, sizeof(buffer), &bytes_read, portMAX_DELAY);
        //unsigned long int timediff = (micros() - timeddd) / 1000;
        //Serial.print("I2S read time:\t");
        //Serial.println(timediff);

        
        for (int i = 0; i < bytes_read; i += 8) {
            int32_t right_sample = (buffer[i + 3] << 24) | (buffer[i + 2] << 16) | (buffer[i + 1] << 8) | buffer[i];
            right_sample >>= 8;

            int32_t left_sample = (buffer[i + 7] << 24) | (buffer[i + 6] << 16) | (buffer[i + 5] << 8) | buffer[i + 4];
            left_sample >>= 8;

            bufferLF.insert(left_sample);
            bufferRF.insert(right_sample);

            //Serial.print(right_sample);
            //Serial.print(",");
            //Serial.println(left_sample);
        }
        
    }
   
}
