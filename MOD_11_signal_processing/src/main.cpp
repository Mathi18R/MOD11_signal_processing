#include <Arduino.h>
#include "esp_task_wdt.h"
#include "esp_task_wdt.h"
#include <driver/i2s.h>
#include "soc/dport_access.h"
#include <buffer.h>
//#include "least_mean_square.h"
#include "sign_angle_finder.h"
#include <DisplayHandler.h>
#include <SPI.h>
#include <Wire.h>
#include <utility>
#include "moving_average.h"

DisplayHandler dp;

//pins:
#define I2S_NUM_0         I2S_NUM_0
#define I2S_NUM_1         I2S_NUM_1
#define I2S_WS_PIN_0      25
#define I2S_SCK_PIN_0     26
#define I2S_SD_PIN_0      33
#define I2S_WS_PIN_1      14
#define I2S_SCK_PIN_1     12
#define I2S_SD_PIN_1      27
//comment
const int SIZE = 3000;

buffer<float, SIZE> bufferLF;
buffer<float, SIZE> bufferRF;
buffer<float, SIZE> bufferLB;
buffer<float, SIZE> bufferRB;

const float Fs_mic = 40000;
const float Fs_output = 1000;
const int sample_swing = 40000*0.0015;//Change this when changing Fs_mic
float cross_correlation_buffer[2*sample_swing-1];
int prev_time = 0;

moving_average_factor_threshold my_moving_average(1.2, 0.2);

//Needs to be included after buffer declarations
#include "input_handler.h"
#include "angle_handler.h"

static void updateWrapper(void* parameter);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Hello world!");
  Serial.println(xPortGetCoreID());

  //configurate the i2s protocol
  i2s_config_t i2s_config = {
    .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = Fs_mic,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  //config pins
  i2s_pin_config_t pin_config_0 = {
    .bck_io_num = I2S_SCK_PIN_0,
    .ws_io_num = I2S_WS_PIN_0,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD_PIN_0
  };

  i2s_pin_config_t pin_config_1 = {
    .bck_io_num = I2S_SCK_PIN_1,
    .ws_io_num = I2S_WS_PIN_1,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD_PIN_1
  };

  //install driver and set pins
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config_0);

  i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_1, &pin_config_1);

  dp.init();

}

void loop() {
  int region = 0;
  
  input_handler<float, SIZE>(SIZE);
  Serial.print("loop time: ");
  Serial.println((micros()-prev_time)/1000);
  prev_time = micros();
  
  region = angle_handler<float, SIZE>();
  //Serial.print("angle time: ");
  //Serial.println((micros()-prev_time)/1000);
  //prev_time = micros();

  TaskHandle_t TaskHandle;
  std::pair<DisplayHandler*, int> input_pair;
  input_pair.first = &dp;//
  input_pair.second = region;
  void *input_arguments = &input_pair;
  TaskFunction_t my_function = updateWrapper;
  xTaskCreatePinnedToCore(    
    my_function,                    /*function*/
    "screen_update",                  /* name of task */
    10000,                           /* Stack size of task */
    input_arguments,                /* parameter of the task */
    1,                              /* priority of the task */
    &TaskHandle,                    /* Task handle to keep track of created task */
    1 - xPortGetCoreID()            /* pin task to opposite core */ 
    );
}

static void updateWrapper(void* parameter) {
  int prev_time_screen = micros();
  std::pair<DisplayHandler*, int> input_pair = *((std::pair<DisplayHandler*, int>*)parameter);
  (input_pair.first)->update(input_pair.second);
  Serial.print("screen time: ");
  Serial.println((micros()-prev_time_screen)/1000);
  vTaskDelete( NULL );
}