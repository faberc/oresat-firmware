#ifndef _EPL_RFID_H_
#define _EPL_RFID_H

#include "ch.h"
#include "hal.h"
#include <hal_i2c_lld.h>

#define THREAD_SIZE (1<<7)
#define THREAD_SIZE_12500HZ (1<<7)

#define PWM_TIMER_FREQ	1e6 /// Hz
#define PWM_FREQ				125e3 /// periods per sec
#define PWM_PERIOD			PWM_TIMER_FREQ/PWM_FREQ /// 100 ticks/period @ 8us/tick
#define PWM_CH1					0

#define ICU_FREQ	PWM_TIMER_FREQ //10e3 // Hz

#define DEBUG_SERIAL SD2
#define DEBUG_CHP ((BaseSequentialStream *) &DEBUG_SERIAL)

extern THD_WORKING_AREA(waThread_epl_rfid,THREAD_SIZE);
extern THD_FUNCTION(Thread_epl_rfid, arg);

/*
extern THD_WORKING_AREA(waThread_125,THREAD_SIZE);
extern THD_FUNCTION(Thread_125, arg);
//*/

#endif
