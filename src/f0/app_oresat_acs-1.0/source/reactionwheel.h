#ifndef _REACTIONWHEEL_H_
#define _REACTIONWHEEL_H_

#define THREAD_SIZE	(1<<7)

#define DEBUG_SERIAL SD2
#define DEBUG_CHP ((BaseSequentialStream *) &DEBUG_SERIAL)

#define SCALE			9
#define STEPS			360 
#define STRETCH		1

#define PWM_TIMER_FREQ	48e6 // Hz

//**************************************************************
//
// TODO: we need to do math ASAP
//
#define PWM_FREQ				10e3 // periods per sec
#define PWM_PERIOD			PWM_TIMER_FREQ/PWM_FREQ 
//**************************************************************

#define PWM_U					0U
#define PWM_V					1U
#define PWM_W					2U

#define sinctrl_t uint16_t 

typedef struct{
	int count,		// period counter
			scale,		// scales the duty cycle
			steps,		// number of steps in lut 
			stretch;  
			sinctrl_t u,v,w,// signals
			phase_shift;		// 
	uint16_t position;				// motor position from encoder
					 sinctrl_t const *sinctrl; // pointer to the sin lut
	uint16_t spi_rxbuf[2] = {0}; // receive buffer
} bldc;

static const SPIConfig spicfg = {
	false,              // Enables circular buffer if == 1
	NULL,               // Operation complete call back.
	GPIOA,              // Chip select line
	GPIOA_SPI1_NSS,     // Chip select port
	SPI_CR1_BR_0|SPI_CR1_BR_1|SPI_CR1_BR_2|SPI_CR1_CPHA,//reg 1 mask
	SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3,//reg 2 mask
};

extern THD_WORKING_AREA(wa_spiThread,THREAD_SIZE);
extern THD_FUNCTION(spiThread,arg);

extern void acsInit(bldc *pbldc);
extern void bldcInit(bldc *pbldc);
extern void bldcStart(bldc *pbldc);
extern void bldcStop(bldc *pbldc);

#endif