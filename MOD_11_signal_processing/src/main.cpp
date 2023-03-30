#include <Arduino.h> 
#include <mutex>

std::mutex buffer_mutex;
int SIZE = 100;
buffer<float, SIZE> bufferLF;
buffer<float, SIZE> bufferRF;
buffer<float, SIZE> bufferLB;
buffer<float, SIZE> bufferRB;

//Needs to be included after mutex and buffer declarations
#include "input_handler.h"

float Fs = 40000;

void setup() {
  // put your setup code here, to run once:


  TaskHandle_t input_handler;
  xTaskCreatePinnedToCore(
    update_buffer, /* Function to implement the task */
    "input_handler", /* Name of the task */
    10000,  /* Stack size in words */
    Fs ,  /* Task input parameter */
    0,  /* Priority of the task */
    &input_handler,  /* Task handle. */
    0); /* Core where the task should run */

}

void loop() {
  // put your main code here, to run repeatedly:
}