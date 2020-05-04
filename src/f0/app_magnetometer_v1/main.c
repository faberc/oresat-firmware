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
        union {
            struct __attribute__((packed)) {
                uint16_t x;
                uint16_t y;
                uint16_t z;
            };
            uint8_t data[6];
        };
    };
    uint8_t buf[7];
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

    i2cbuf_t i2cbuf;

    sdStart(&SD2, NULL);

    i2cStart(&I2CD1, &i2cconfig);

        while (true)
        {
            i2cbuf.reg = MMC5883_CTRL0_REG;
            i2cbuf.data[0] = 0x01;
            
            // DEBUG: Print out whole buffer just to see what's going on.
            chprintf((BaseSequentialStream*)&SD2, "i2cbuf: %x\r\n", i2cbuf.buf);

            // Write 0x01 to control register
            i2cMasterTransmitTimeout(&I2CD1, MMC5883_SADDR, i2cbuf.buf, 2, NULL, 0, TIME_INFINITE);

            i2cbuf.reg = MMC5883_STAT_REG;
            
            // Continuously read status register until bit 0 is a 1.
            do {
                i2cMasterTransmitTimeout(&I2CD1, MMC5883_SADDR, &i2cbuf.reg, sizeof(i2cbuf.reg), i2cbuf.data, 1, TIME_INFINITE);
            } while (!(i2cbuf.data[0] & 0x01));

            chprintf((BaseSequentialStream*)&SD2, "Measurement Complete\r\n");

            // Read from XOUT LSB values -- device will increment through 6 registers and read all axes.
            i2cbuf.reg = MMC5883_XOUT_LSB;
            i2cMasterTransmitTimeout(&I2CD1, MMC5883_SADDR, &i2cbuf.reg, sizeof(i2cbuf.reg), i2cbuf.data, sizeof(i2cbuf.data), TIME_INFINITE);

            // DEBUG: Print out each byte just to see what's going on.
            for (int i; i < 6; i++){
                chprintf((BaseSequentialStream*)&SD2, "%u\r\n", i2cbuf.data[i]);
            }

            // Print out x, y, and z magnetic field axis measurements.
            chprintf((BaseSequentialStream*)&SD2, "X= %u, Y=%u, Z=%u\r\n", i2cbuf.x, i2cbuf.y, i2cbuf.z);
            chThdSleepMilliseconds(2000);
        }

        return 0;
}
