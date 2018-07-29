/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

//=== ChibiOS header files
#include "ch.h"
#include "hal.h"
#include "chprintf.h"

//=== Project header files
#include "epl_rfid.h"

//=== Serial configuration
static SerialConfig ser_cfg ={
	115200,     //Baud rate
	0,          //
	0,          //
	0,          //
};

#if 0
static const I2CConfig i2cconfig = {
  STM32_TIMINGR_PRESC(8U)  |            /* 72MHz/9 = 8MHz I2CCLK.           */
  STM32_TIMINGR_SCLDEL(3U) | STM32_TIMINGR_SDADEL(3U) |
  STM32_TIMINGR_SCLH(3U)   | STM32_TIMINGR_SCLL(9U),
  0,
  0
};
#endif
#if 1
static const I2CConfig i2cconfig = {
  STM32_TIMINGR_PRESC(15U) |
  STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
  STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
  0,
  0
};
#endif

static void app_init(void) {
	//=== App initialization

	//i2cStart(&I2CD1, &i2cconfig);
	// Start up debug output
	sdStart(&SD2, &ser_cfg);
}

static void testI2C(void){
//    unsigned i;
    msg_t msg;
    static const uint8_t cmd[] = {0, 0};
    uint8_t data[16] = "123456789ABCD";

    msg = i2cMasterTransmitTimeout(
			&I2CD1, 0x52, 
			cmd, sizeof(cmd),
      data, sizeof(data), 
			TIME_INFINITE
		);
    if (msg != MSG_OK){
			palToggleLine(LINE_LED_GREEN);
		}
/*
		for (i = 0; i < 256; ++i) {
      msg = i2cMasterReceiveTimeout(
				&I2CD1, 0x52,
        data, sizeof(data), 
				TIME_INFINITE
			);
      if (msg != MSG_OK){
    		palToggleLine(LINE_LED_GREEN);
			}
    }
//*/
}

static void app_main(void) {
//*
	chThdCreateStatic(
		waThread_epl_rfid, 
		sizeof(waThread_epl_rfid), 
		NORMALPRIO, 
		Thread_epl_rfid, 
		NULL
	);
//*/

	/*
	 * Begin main loop
	 */
	while(true){
		//testI2C();

//    chThdSleepMilliseconds(500);
//    palToggleLine(LINE_LED_GREEN);

		chThdSleepMilliseconds(1000);
	}
}

int main(void) {
	halInit();
	chSysInit();

	// Initialize and start app
	app_init();
	app_main();

	return 0;
}
