#ifndef _EPL_RFID_H_
#define _EPL_RFID_H

#include "ch.h"
#include "hal.h"

#define THREAD_SIZE (1<<7)
#define	THREAD_SIZE_15625HZ (1<<7)
#define THREAD_SIZE_12500HZ (1<<7)

#include <hal_i2c_lld.h>

extern THD_WORKING_AREA(waThread_epl_rfid,THREAD_SIZE);
extern THD_FUNCTION(Thread_epl_rfid, arg);

extern THD_WORKING_AREA(waThread_15625,THREAD_SIZE);
extern THD_FUNCTION(Thread_15625, arg);

extern THD_WORKING_AREA(waThread_125,THREAD_SIZE);
extern THD_FUNCTION(Thread_125, arg);

#endif
