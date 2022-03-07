#include "microRTOS.h"

microRTOS::microRTOS()
{
  #ifdef microRTOS_DEBUG
    printf("microRTOS()\n");
  #endif
}

uint8_t microRTOS::run(TaskFunction_t child, int stack_size, int queue_size, int priority)
{
  #ifdef microRTOS_DEBUG
    printf("microRTOS: run(child=task[%p], stack_size=%d, queue_size=%d, priority=%d)\n", child, stack_size, queue_size, priority);
  #endif
  hCommand = xQueueCreate(queue_size, sizeof(const char*));
  hResponse = xQueueCreate(1, sizeof(const char*));
  xTaskCreate(child, id, stack_size, this, 1, &hTask);
  manager.reg(id, &hCommand, &hResponse);
  return 0;
}

uint8_t microRTOS::run_core(int core, TaskFunction_t child, int stack_size, int queue_size, int priority)
{
  #ifdef microRTOS_DEBUG
    printf("microRTOS: run(core=%d, child=task[%p], stack_size=%d, queue_size=%d, priority=%d)\n", core, child, stack_size, queue_size, priority);
  #endif
  hCommand = xQueueCreate(queue_size, sizeof(const char*));
  hResponse = xQueueCreate(1, sizeof(const char*));
  xTaskCreatePinnedToCore(child, id, stack_size, this, 1, &hTask, core);
  manager.reg(id, &hCommand, &hResponse);
  return 0;
}

uint8_t microRTOS::stop()
{
  #ifdef microRTOS_DEBUG
    printf("microRTOS: stop()\n");
  #endif
  vTaskDelete(hTask);
  hTask = NULL;
  manager.dereg(id);
  vQueueDelete(hCommand);
  hCommand = NULL;
  vQueueDelete(hResponse);
  hResponse = NULL;
  return 0;
}

const char* microRTOS::cmd(const char* command, TickType_t timeout)
{
  #ifdef microRTOS_DEBUG
    printf("microRTOS: cmd(command=\"%s\", timeout=%d)\n", command, timeout);
  #endif
  xQueueSend(hCommand, &command, portMAX_DELAY);
  const char* response;
  xQueueReceive(hResponse, &response, timeout);
  return response;
}

String microRTOS::cmd(String command, TickType_t timeout)
{
  #ifdef microRTOS_DEBUG
    printf("microRTOS: cmd(command=\"%s\", timeout=%d)\n", command, timeout);
  #endif
  xQueueSend(hCommand, &command, portMAX_DELAY);
  String response;
  xQueueReceive(hResponse, &response, timeout);
  return response;
}