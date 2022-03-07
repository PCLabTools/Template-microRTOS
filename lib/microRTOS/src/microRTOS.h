#include <Arduino.h>
#include "microRTOS_manager.h"
#include "microRTOS_interface.h"

#ifndef mircoRTOS_h
  #define microRTOS_h

  class microRTOS
  {
    public:
      microRTOS();
      uint8_t run(TaskFunction_t child, int stack_size = 2048, int queue_size = 5, int priority = 1);
      uint8_t run_core(int core, TaskFunction_t child, int stack_size = 2048, int queue_size = 5, int priority = 1);
      uint8_t stop();
      const char* cmd(const char* command, TickType_t timeout = portMAX_DELAY);
      String cmd(String command, TickType_t timeout = portMAX_DELAY);

    protected:
      const char* id;
      xTaskHandle hTask = NULL;
      xQueueHandle hCommand = NULL;
      xQueueHandle hResponse = NULL;
      microRTOS_manager& manager = microRTOS_manager::getInstance();
      microRTOS_interface& interface = microRTOS_interface::getInstance();
  };

#endif