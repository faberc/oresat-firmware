#ifndef _BLINKY_H_
#define _BLINKY_H_

#include "ch.h"
#include "hal.h"

#define CH_DBG_SYSTEM_STATE_CHECK TRUE
#define DEBUG_SERIAL SD2
#define DEBUG_CHP ((BaseSequentialStream *) &DEBUG_SERIAL)

// Green LED blinker thread prototypes, times are in milliseconds.
extern THD_WORKING_AREA(waThread1, 128);
extern THD_FUNCTION(Thread1, arg);

#endif
