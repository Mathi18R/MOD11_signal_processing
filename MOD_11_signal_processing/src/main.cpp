#include <Arduino.h>
#include <mutex>
#include <buffer.h>

//pins:
int pin_micLF = 32;
int pin_micRF = 33;

//other:
std::mutex buffer_mutex;
const int SIZE = 4000;

buffer<float, SIZE> bufferLF;
buffer<float, SIZE> bufferRF;

float Fs_mic = 40000;
float Fs_output = 20;

int region = 0;

//Needs to be included after mutex and buffer declarations
#include "input_handler.h"
#include "angle_handler.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  auto input_handler_function = input_handler<float, SIZE>;
 
  TaskHandle_t input_handler;
  xTaskCreatePinnedToCore(
    input_handler_function, // Function to implement the task
    "input_handler",            // Name of the task
    10000,                      // Stack size in words
    NULL,                       // Task input parameter
    0,                          // Priority of the task
    NULL,// Task handle. (pointer to function)
    0                           // Core where the task should run
);

auto angle_handler_function = angle_handler<float, SIZE>;

  TaskHandle_t angle_handler;
  xTaskCreatePinnedToCore(
    angle_handler_function, /* Function to implement the task */
    "angle_handler", /* Name of the task */
    10000,  /* Stack size in words */
    NULL ,  /* Task input parameter */
    0,  /* Priority of the task */
    NULL,  /* Task handle. */
    1); /* Core where the task should run */
}

void loop() {
  // put your main code here, to run repeatedly:
}