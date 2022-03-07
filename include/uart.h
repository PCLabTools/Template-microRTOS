#include <Arduino.h>

#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 64

/***************************************************************
 * High Level Functions
 * ************************************************************/

// Setup UART communication and create buffers
void uart_setup(int BAUD_RATE);

// Start transmit/receive tasks
void uart_run();

// Pause transmit/receive tasks
void uart_pause();

/***************************************************************
 * Core Tasks
 * ************************************************************/

// Send incoming bytes to processor
void uart_process_task(void*);
TaskHandle_t uart_process_handle = NULL;

// Transmits outgoing bytes from buffer
void uart_tx_task(void*);
TaskHandle_t uart_tx_handle = NULL;
QueueHandle_t uart_tx_queue = NULL;

/***************************************************************
 * Single Run Tasks
 * ************************************************************/

void IDN_REQ (void* TX_BUFFER);


/***************************************************************
 * Function Implementations
 * ************************************************************/

void uart_setup(int BAUD_RATE = 115200)
{
  Serial.begin(BAUD_RATE);
  uart_tx_queue = xQueueCreate(TX_BUFFER_SIZE, sizeof(char));
  if(uart_tx_queue == NULL){
    Serial.println("Error creating the queue");
  }
}

void uart_run()
{
  if (uart_process_handle == NULL && uart_tx_handle == NULL)
  {
    xTaskCreate(uart_process_task,"uart_rx",1024,NULL,1,&uart_process_handle);
    xTaskCreate(uart_tx_task,"uart_tx",1024,NULL,1,&uart_tx_handle);
  }
}

void uart_pause()
{
  vTaskDelete(&uart_process_handle);
  vTaskDelete(&uart_tx_handle);
  uart_process_handle = NULL;
  uart_tx_handle = NULL;
}

void uart_process_task(void*)
{
  for ( ;; )
  {
    while (Serial.available())
    {
      char inChar = Serial.read();
      xQueueSend(uart_tx_queue, &inChar, portMAX_DELAY);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void uart_tx_task(void*)
{
  for ( ;; )
  {
    char outChar;
    xQueueReceive(uart_tx_queue, &outChar, portMAX_DELAY);
    Serial.write(outChar);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void IDN_REQ (void* TX_BUFFER)
{
  const char* identity = "hello";
  for (int i = 0 ; i <= sizeof(identity) ; i++)
  {
    xQueueSend((QueueHandle_t)TX_BUFFER, &identity[i], portMAX_DELAY);
  }
  vTaskDelete(NULL);
}