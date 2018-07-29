#include "epl_rfid.h"
//*
 static void pwmpcb(PWMDriver *pwmp) {
  (void)pwmp;
//  palSetLine(LINE_LED_GREEN);
}
//*/

//*
static void pwmc1cb(PWMDriver *pwmp) {
  (void)pwmp;
//	palClearLine(LINE_LED_GREEN);
}
//*/

//*
static PWMConfig pwmcfg = {
//  PWM_TIMER_FREQ,         // PWM clock frequency.
//  PWM_PERIOD,                  // Initial PWM period.
  125000,
	10000,
	pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
	0,
  0
};

//*/

icucnt_t last_width, last_period;

static void icuwidthcb(ICUDriver *icup) {
	// this is activated when the state of the driver
	// changes from ICU_ACTIVE to ICU_IDLE

  palSetLine(LINE_LED_GREEN);
  last_width = icuGetWidthX(icup);
}

static void icuperiodcb(ICUDriver *icup) {
	// this is activated when the state of the driver
	// changes from ICU_IDLE to ICU_ACTIVE  
  
//  palSetLine(LINE_LED_GREEN);
	palClearLine(LINE_LED_GREEN);
  last_period = icuGetPeriodX(icup);
	//chprintf(DEBUG_CHP,"enc pos: %u \n\r", last_period);
}

static void icuoverflowcb(ICUDriver *icup) {
  (void)icup;

}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  ICU_FREQ,                                    /* 10kHz ICU clock frequency.   */
  icuwidthcb,
  icuperiodcb,
  icuoverflowcb,
  ICU_CHANNEL_1,
  0
};

THD_WORKING_AREA(waThread_epl_rfid,THREAD_SIZE);
THD_FUNCTION(Thread_epl_rfid, arg) {
  (void)arg;
  chRegSetThreadName("Thread_epl_rfid");

  pwmStart(&PWMD1, &pwmcfg);
  pwmEnablePeriodicNotification(&PWMD1);
  
	icuStart(&ICUD3, &icucfg);
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(1));
  icuStartCapture(&ICUD3);
  icuEnableNotifications(&ICUD3);
  
	chThdSleepMilliseconds(2000);
  
	/*
   * Starts the PWM channel 0 using 50% duty cycle.
	 * scale 0 - 10000
   */
  pwmEnableChannel(&PWMD1,PWM_CH1,PWM_PERCENTAGE_TO_WIDTH(&PWMD1,900));
	pwmEnableChannelNotification(&PWMD1,PWM_CH1);
	
	/*
   * Changes PWM period to half second the duty cycle becomes 50%
   * implicitly.
   */
//  pwmChangePeriod(&PWMD1, 5000);
//  chThdSleepMilliseconds(5000);

	/*
  pwmDisableChannel(&PWMD1, 0);
  pwmStop(&PWMD1);
  icuStopCapture(&ICUD3);
  icuStop(&ICUD3);
  palClearPad(GPIOC, GPIOC_LED3);
  palClearPad(GPIOC, GPIOC_LED4);
//*/

  while (!chThdShouldTerminateX()) {
//    palClearLine(LINE_LED_GREEN);
//    chThdSleepMilliseconds(500);
//    palSetLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}

#if 0
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
#endif
