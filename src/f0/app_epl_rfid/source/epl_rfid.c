#include "epl_rfid.h"

// Example blinker thread
THD_WORKING_AREA(waThread_epl_rfid,THREAD_SIZE);
THD_FUNCTION(Thread_epl_rfid, arg) {
  (void)arg;
  chRegSetThreadName("Thread_epl_rfid");

  while (!chThdShouldTerminateX()) {
    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}
