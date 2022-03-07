#include <Arduino.h>
#include <ArduinoJson.h>

#ifndef microRTOS_interface_h
  #define microRTOS_interface_h

  struct moduleMessage
  {
    String state;
    String data;
  };

  class microRTOS_interface
  {
    public:
      static microRTOS_interface& getInstance() 
      {
        // Allocate with `new` in case Singleton is not trivially destructible.
        static microRTOS_interface* singleton = new microRTOS_interface();
        return *singleton;
      }
      moduleMessage parse(const char* message);

    private:
      microRTOS_interface() = default;
      microRTOS_interface(const microRTOS_interface&) = delete;
      microRTOS_interface& operator=(const microRTOS_interface&) = delete;
      microRTOS_interface(microRTOS_interface&&) = delete;
      microRTOS_interface& operator=(microRTOS_interface&&) = delete;
  };

#endif