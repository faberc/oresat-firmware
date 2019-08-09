/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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

/***************************************************
 *     Modification Log
 *     04/15/2018    Malay Das    Initial Code.
 ***************************************************/


/*
 * Include Files
 */
#include <stdbool.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "util_version.h"
#include "util_numbers.h"
#include "ax5043.h"

//#include "adf7030.h"

#define     DEBUG_SERIAL                    SD2
#define     DEBUG_CHP                       ((BaseSequentialStream *) &DEBUG_SERIAL)


const struct axradio_address remoteaddr_tx = {
	{ 0x32, 0x34, 0x00, 0x00}
};
const struct axradio_address_mask localaddr_tx = {
	{ 0x33, 0x34, 0x00, 0x00},
	{ 0xFF, 0x00, 0x00, 0x00}
};

uint8_t axradio_rxbuffer[256];  //buffer to receive radio data

/*
 * Serial Driver Configuration
 */
static SerialConfig ser_cfg =
{
    115200,                                 //Baud rate
    0,                                      //
    0,                                      //
    0,                                      //
};


/*
 * Receive and Transmit SPI Configurations
 */
static const SPIConfig spicfg_rx =
{
    false,
    NULL,                                   // Operation complete callback
    GPIOA,                                  // Slave select port
    GPIOA_SPI1_NSS,                         // Slave select pad
    // SPI cr1 data                            (see 446 ref man.)
    SPI_CR1_SPE     |                       // SPI enable
    SPI_CR1_MSTR    |                       // Master
    //SPI_CR1_BR_2    |
    SPI_CR1_BR_1    |
    SPI_CR1_BR_0   |                        // fpclk/16  approx 5Mhz? BR = 0x011
    SPI_CR1_SSM,
    0, // SPI_CR2_SSOE,
};

static const SPIConfig spicfg_tx =
{
    false,
    NULL,                                   // Operation complete callback
    GPIOB,                                  // Slave select port
    GPIOB_SPI2_NSS,                         // Slave select pad
    // SPI cr1 data                         (see 446 ref man.)
    SPI_CR1_SPE     |                       // SPI enable
    SPI_CR1_MSTR    |                       // Master
    //SPI_CR1_BR_2    |
    SPI_CR1_BR_1    |
    SPI_CR1_BR_0   |                        // fpclk/16  approx 5Mhz? BR = 0x011
    SPI_CR1_SSM,
    0
};


/*
 * Initialize the SPI drivers and configure the ax5043 chips
 */
static void app_init(void)
{
    // Start up debug output, chprintf(DEBUG_CHP,...)
    sdStart(&DEBUG_SERIAL, &ser_cfg);

    set_util_fwversion(&version_info);
    set_util_hwversion(&version_info);


    //Print FW/HW information
    chprintf(DEBUG_CHP, "\r\nFirmware Info\r\n");
    chprintf(DEBUG_CHP, "FW HASH: %s\r\n", version_info.firmware);
    chprintf(DEBUG_CHP, "STF0x UNIQUE HW ID (H,C,L):\r\n0x%x\t0x%x\t0x%x\r\n"
             , version_info.hardware.id_high
             , version_info.hardware.id_center
             , version_info.hardware.id_low
            );


    spiStart(&SPID1, &spicfg_rx);
    spiStart(&SPID2, &spicfg_tx);
	//spiSelect(&SPID2);





    //uint16_t reg=0;
    //uint8_t value=0;
    //uint8_t value1=0x55;
    //uint8_t ret_value[3]={0,0,0};
    //int i;



    chprintf(DEBUG_CHP, "Configuring AX5043\r\n");
    chThdSleepMilliseconds(50);
    ax5043_init(&SPID1);
    ax5043_set_addr(&SPID1, localaddr_tx);
    chprintf(DEBUG_CHP, "done reseting AX5043\r\n");



     ax5043_prepare_rx(&SPID1);
  



}


/*
 * main loop blinks the led
 */
static void main_loop(void)
{
  chThdSleepMilliseconds(500);
  uint8_t packet_len=0;
  uint8_t ret_value[3]={0,0,0};

  /* Enabling events on both edges of the button line.*/
  //palEnableLineEvent(LINE_BUTTON, PAL_EVENT_MODE_RISING_EDGE);
  palEnableLineEvent(LINE_ARD_A5, PAL_EVENT_MODE_RISING_EDGE);


  while(true)
  {
    //palWaitLineTimeout(LINE_BUTTON, TIME_INFINITE);
    //palWaitLineTimeout(LINE_ARD_A5, TIME_INFINITE);
    //palWaitLineTimeout(LINE_ARD_A5, TIME_MS2I(5000));

    palWaitLineTimeout(LINE_SX_INT0, TIME_MS2I(5000));

    if (palReadLine(LINE_SX_INT0))
      chprintf(DEBUG_CHP, "\r\r int line is HIGH ** \r\n");
    else
      chprintf(DEBUG_CHP, "\r\r int line is LOW ** \r\n");
    
    packet_len=receive_loop(&SPID1, axradio_rxbuffer);

    if(packet_len > 0)
      chprintf(DEBUG_CHP,"INFO: Received packet %d\r\n",axradio_rxbuffer[3]);
  }

}


/*
 * Entry to our code
 */
int main(void)
{
    halInit();
    chSysInit();
    app_init();

    // Enabling events on both edges of the button line.*/
    //palEnableLineEvent(GPIOC_SX_DIO3, PAL_EVENT_MODE_RISING_EDGES);

	//chThdCreateStatic(waThread_sx1236_rx,      sizeof(waThread_sx1236_rx),   NORMALPRIO, Thread_sx1236_rx, NULL);
    //chThdSleepMilliseconds(500);
    //chThdCreateStatic(waThread_sx1236_tx,      sizeof(waThread_sx1236_tx),   NORMALPRIO, Thread_sx1236_tx, NULL);
    chThdSleepMilliseconds(500);

    main_loop();
    return 0;
}
