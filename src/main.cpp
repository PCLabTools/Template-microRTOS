#include <Arduino.h>
#include <module.h>

module mod1("mod1");
module mod2("mod2");
microRTOS_manager& manager = microRTOS_manager::getInstance();

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  mod1.run();
  mod2.run();
  digitalWrite(BUILTIN_LED, HIGH);
  manager.cmd("mod1", "testing external comms to mod1...");
  manager.cmd("mod2", "testing external comms to mod2...");
}

void loop()
{
  if (millis() >= 5000 && millis() <= 10000)
  {
    printf("echo: %s\n", mod1.cmd("hello world!"));
  }
  if (millis() >= 10000)
  {
    mod1.stop();
    mod2.stop();
  }
  delay(500);
}