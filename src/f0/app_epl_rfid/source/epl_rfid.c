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

THD_WORKING_AREA(waThread_15625Hz,THREAD_SIZE);
THD_FUNCTION(Thread_15625, arg) {
  (void)arg;
  chRegSetThreadName("Thread_15.625");

  while (!chThdShouldTerminateX()) {
	/*
    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_GREEN);
	//*/
    chThdSleepMilliseconds(500);
  }
}

THD_WORKING_AREA(waThread_12500Hz,THREAD_SIZE);
THD_FUNCTION(Thread_12500Hz, arg) {
  (void)arg;
  chRegSetThreadName("Thread_12.5KHz");

  while (!chThdShouldTerminateX()) {
	/*
    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_GREEN);
	//*/
    chThdSleepMilliseconds(500);
  }
}
