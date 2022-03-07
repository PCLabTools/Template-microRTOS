#include <Arduino.h>

#ifndef microRTOS_manager_h
  #define microRTOS_manager_h

  #ifndef microRTOS_MAX_MODULES
    #define microRTOS_MAX_MODULES 10
  #endif

  class microRTOS_manager
  {
    public:
      static microRTOS_manager& getInstance() 
      {
        // Allocate with `new` in case Singleton is not trivially destructible.
        static microRTOS_manager* singleton = new microRTOS_manager();
        return *singleton;
      }
      uint8_t reg(const char* name, xQueueHandle* command, xQueueHandle* response);
      uint8_t dereg(const char* name);
      const char* cmd(const char* name, const char* command, TickType_t timeout = portMAX_DELAY);
      String cmd(String name, String command, TickType_t timeout = portMAX_DELAY);

    private:
      microRTOS_manager();// = default;
      microRTOS_manager(const microRTOS_manager&) = delete;
      microRTOS_manager& operator=(const microRTOS_manager&) = delete;
      microRTOS_manager(microRTOS_manager&&) = delete;
      microRTOS_manager& operator=(microRTOS_manager&&) = delete;
      const char* names[microRTOS_MAX_MODULES];
      xQueueHandle* hCommands[microRTOS_MAX_MODULES];
      xQueueHandle* hResponses[microRTOS_MAX_MODULES];
  };

#endif