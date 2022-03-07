#include <Arduino.h>
#include <microRTOS.h>

#ifndef module_h
  #define module_h

  class module : public microRTOS
  {
    public:
      module(const char* identity);
      uint8_t run();
    
    protected:
      static void main_loop(void*);
  };

#endif