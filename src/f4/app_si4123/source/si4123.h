/*! \file ax5043_common.h */


/*!
 * \addtogroup ax5043
 *  * @{
 *
 */
#ifndef _AX5043_COMMON_H
#define _AX5043_COMMON_H

#define DEBUG_SERIAL  SD2
#define DEBUG_CHP     ((BaseSequentialStream *) &DEBUG_SERIAL)

#include <stdbool.h>
#include <stdint.h>




/******************************************************************************
 ********************  AX5043 control SPI registers ***************************
 *****************************************************************************/

/* Status and test registers */
#define SI4123_REG_MAIN_CONFIG             0x0
#define SI4123_REG_PHASE_GAIN              0x1
#define SI4123_REG_PWRDOWN                 0x2
#define SI4123_REG_RF1_NDIV                0x3
#define SI4123_REG_RF2_NDIV                0x4
#define SI4123_REG_IF_NDIV                 0x5
#define SI4123_REG_RF1_RDIV                0x6
#define SI4123_REG_RF2_RDIV                0x7
#define SI4123_REG_IF_RDIV                 0x8




//function declaration starts here
void write_reg(uint8_t address, uint32_t data);

#endif
//! @}
