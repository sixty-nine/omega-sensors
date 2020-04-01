#include <Arduino.h>
#include "HardwareInfo.h"

HardwareInfo::HardwareInfo()
{
}

// Helper function for free ram.
//   With use of https://playground.arduino.cc/Code/AvailableMemory
//
int HardwareInfo::freeRam(void)
{
  extern unsigned int __heap_start;
  extern void *__brkval;

  int free_memory;
  int stack_here;

  if (__brkval == 0)
    free_memory = (int) &stack_here - (int) &__heap_start;
  else
    free_memory = (int) &stack_here - (int) __brkval;

  return (free_memory);
}

HardwareInfoRes HardwareInfo::getInfo()
{
    HardwareInfoRes res;
    res.ramFree = freeRam();
    res.ramUsed = (RAMEND + 1) - res.ramFree;
    res.ramUsedPercent = (float) res.ramUsed / ((float) RAMEND + 1.0) * 100.0;
    res.version =(float) ARDUINO / 100.0, 2;
    return res;
}
