#include "module.h"

module::module(const char* identity)
{
  #ifdef microRTOS_DEBUG
    printf("module(identity=\"%s\")\n", identity);
  #endif
  id = identity;
  printf("%s: initialise\n", identity);
}

uint8_t module::run()
{
  #ifdef microRTOS_DEBUG
    printf("module: run()\n");
  #endif
  microRTOS::run(&main_loop, 8192);
  return 0;
}

void module::main_loop(void* pModule)
{
  #ifdef microRTOS_DEBUG
    printf("module: main_loop(pModule=%p)\n", pModule);
  #endif
  module* mod = (module*)pModule;
  printf("%s: main_loop(start core: %d)\n", mod->id, xPortGetCoreID());
  for ( ;; )
  {
    const char* message = "";
    xQueueReceive(mod->hCommand, &message, 1000 / portTICK_PERIOD_MS);
    if (!strcmp(message, ""))
    {
      printf("%s: main_loop(timeout)\n", mod->id);
      continue;
    }
    else
    {
      printf("%s: main_loop(message: %s)\n", mod->id, message);
      // parse message
      String response = "";
      moduleMessage incoming = mod->interface.parse(message);
      if (incoming.state == "*idn?")
      {
        response = "PCLabTools microRTOS module";
      }
      else
      {
        response = "No state named: " + incoming.state;
      }
      xQueueSend(mod->hResponse, &response, portMAX_DELAY);
    }
  }
}