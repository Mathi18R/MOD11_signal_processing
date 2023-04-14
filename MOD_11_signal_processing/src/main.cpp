#include <Arduino.h>
#include <buffer.h>
#include "esp_task_wdt.h"
#include "esp_task_wdt.h"
#include "least_mean_square.h"

//pins:
int pin_micLF = 32;
int pin_micRF = 33;

const int SIZE = 4000;

buffer<float, SIZE> bufferLF;
buffer<float, SIZE> bufferRF;
buffer<float, SIZE> bufferLB;
buffer<float, SIZE> bufferRB;

const float Fs_mic = 20000;
const float Fs_output = 1000;

const int sample_swing = 20000*0.001;//Change this when changing Fs_mic
float cross_correlation_buffer1[2*sample_swing-1];
float cross_correlation_buffer2[2*sample_swing-1];

int region = 0;

least_mean_square my_least_mean_square({ -0.1, 0.1 }, { 0.1, 0.1 }, { 0.1, -0.1 }, { -0.1, -0.1 });
    

//Needs to be included after mutex and buffer declarations
#include "input_handler.h"
#include "angle_handler.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Hello world!");
  Serial.println(xPortGetCoreID());
}

void loop() {
  input_handler<float, SIZE>(SIZE);
  
  angle_handler<float, SIZE>();
}