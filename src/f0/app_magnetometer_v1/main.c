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

/* ChibiOS header files */
#include "ch.h"
#include "hal.h"

/* Remove */
#include "chprintf.h"

#define MMC5883_SADDR 0x30

// MMC5883 Registers
#define MMC5883_XOUT_LSB 0x00
#define MMC5883_STAT_REG 0x07
#define MMC5883_CTRL0_REG 0x08

/* I2C Configurations for STM32 -- Set prescaler and SCL high and low times --
    see peripheral device data sheet (page 3) for minimum period. */
static const I2CConfig i2cconfig = {
    STM32_TIMINGR_PRESC(0xBU) |
    STM32_TIMINGR_SCLDEL(0x4U) | STM32_TIMINGR_SDADEL(0x2U) |
    STM32_TIMINGR_SCLH(0xFU)  | STM32_TIMINGR_SCLL(0x13U),
    0,
    0
};

typedef union {
    struct __attribute__((packed)) {
        uint8_t reg;
        uint8_t byte;
    };
    uint8_t buf[2];
} i2cbuf_t;

int main(void)
{
    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chSysInit();

    uint8_t data[6];
    uint8_t rxbuf[1];
    i2cbuf_t txbuf;
    uint16_t x, y, z;

    sdStart(&SD2, NULL);

    i2cStart(&I2CD1, &i2cconfig);

        while (true)
        {
            txbuf.reg = MMC5883_CTRL0_REG;
            txbuf.byte = 0x01;

            chprintf((BaseSequentialStream*)&SD2, "txbuf: %x\r\n", txbuf.buf);

            // Write 0x01 to control register
            i2cMasterTransmitTimeout(&I2CD1, MMC5883_SADDR, txbuf.buf, sizeof(txbuf), NULL, 0, TIME_INFINITE);

            txbuf.reg = MMC5883_STAT_REG;
            
            // Continuously read status register until bit 0 is a 1.
            do {
                i2cMasterTransmitTimeout(&I2CD1, MMC5883_SADDR, &txbuf.reg, sizeof(txbuf.reg), rxbuf, sizeof(rxbuf), TIME_INFINITE);
            } while (!(rxbuf[0] & 0x01));

            chprintf((BaseSequentialStream*)&SD2, "Measurement Complete\r\n");

            // Read from XOUT LSB values -- device will increment through 6 registers and read all axes.
            txbuf.reg = MMC5883_XOUT_LSB;
            i2cMasterTransmitTimeout(&I2CD1, MMC5883_SADDR, &txbuf.reg, sizeof(txbuf.reg), data, 6, TIME_INFINITE);

            for (int i; i < 6; i++){
                chprintf((BaseSequentialStream*)&SD2, "%u\r\n", data[i]);
            }

            x = (data[1] << 8) + data[0];
            y = (data[3] << 8) + data[2];
            z = (data[5] << 8) + data[4];

            chprintf((BaseSequentialStream*)&SD2, "X= %u, Y=%u, Z=%u\r\n", x, y, z);
            chThdSleepMilliseconds(2000);
        }

        return 0;
}
