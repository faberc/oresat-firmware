#ifndef _EPL_RFID_H_
#define _EPL_RFID_H

#include "ch.h"
#include "hal.h"

#define THREAD_SIZE (1<<7)

#include <hal_i2c_lld.h>

extern THD_WORKING_AREA(waThread_epl_rfid,THREAD_SIZE);
extern THD_FUNCTION(Thread_epl_rfid, arg);

#endif
