#include "magnetorquer.h"

static void pwmpcb(PWMDriver *pwmp) {
  (void)pwmp;
 
        //palSetPad(GPIOA,PH);  // Phase selection. 
        //palClearPad(GPIOA,PH);  // Phase selection. 
	pwmEnableChannel(
		&PWMD1,
		PWM_CH_MTQR,
		PWM_PERCENTAGE_TO_WIDTH(&PWMD1,9000)
	);

 /*       palSetPad(GPIOA,PH);  // Phase selection. 
        pwmEnableChannel(
		&PWMD1,
		PWM_CH_MTQR,
		PWM_PERCENTAGE_TO_WIDTH(&PWMD1,4000)
	);
        */

}

static const PWMConfig pwm_MTQRcfg = {
	MTQR_PWM_TIMER_FREQ,    // Frequency
	MTQR_PWM_PERIOD,        // period duration 
        pwmpcb,               	// Callback
	{ // Channels
		{PWM_OUTPUT_ACTIVE_HIGH,NULL},
		{PWM_OUTPUT_DISABLED,NULL},
		{PWM_OUTPUT_DISABLED,NULL},
		{PWM_OUTPUT_DISABLED,NULL}
	},
	0,
	0,
	0
};


extern void mtqrInit(MTQR *mtqr){
	(void)mtqr;
	mtqr->started = FALSE;
	//palSetPadMode(GPIOB,PH,PAL_MODE_OUTPUT_PUSHPULL); // Phase direction
//	palClearPad(GPIOA,PH); /// phase direction 
//	palClearPad(GPIOB,PH); /// phase direction // for dev
/*
  palSetPadMode(GPIOB,ENABLE,PAL_MODE_OUTPUT_PUSHPULL); 
	palClearPad(GPIOB,ENABLE); /// logic high	
//*/
//*
//  palSetPadMode(GPIOA,ENABLE,PAL_MODE_OUTPUT_PUSHPULL); 
	palClearPad(GPIOB,ENABLE); /// for rev3
        palClearPad(GPIOA,PH);  // Phase selection. 
	//palClearPad(GPIOA,ENABLE); /// for dev
//*/
}

extern void mtqrStart(MTQR *mtqr){
	if(mtqr->started){
		return;
	}
	pwmStart(&PWMD1,&pwm_MTQRcfg);
	mtqrSetDC(MTQR_STARTING_DC);

//	palSetPad(GPIOB,ENABLE);        // Set Enable high.
	palSetPad(GPIOB,ENABLE);        // Set Enable high.
	//palSetPad(GPIOA,ENABLE);        // Set Enable high.
	mtqr->started	= TRUE;
}

extern void mtqrStop(MTQR *mtqr){
	if(!mtqr->started){
		return;
	}
 	pwmDisableChannel(&PWMD1,PWM_CH_MTQR);
	palClearPad(GPIOB,ENABLE);
	//palClearPad(GPIOA,ENABLE);
//	palClearPad(GPIOB,ENABLE);
//	palClearPad(GPIOB,PH);
	palClearPad(GPIOA,PH);
	pwmStop(&PWMD1);
	mtqr->started	= FALSE;
}

extern void mtqrSetDC(uint16_t dc){
	pwmEnableChannel(
		&PWMD1,
		PWM_CH_MTQR,
		PWM_PERCENTAGE_TO_WIDTH(&PWMD1,dc)
	);
}

extern void mtqrSetDir(uint8_t dc){
	if(dc==FORWARD){	
		palClearPad(GPIOA,PH); /// phase direction 
		//palClearPad(GPIOB,PH); /// phase direction 
	}else if(dc==REVERSE){
		palSetPad(GPIOA,PH); /// phase direction 
		//palSetPad(GPIOB,PH); /// phase direction 
	}
}

extern void mtqrExit(MTQR *mtqr){
	if(!mtqr->started){	
		return;
	}
	mtqrStop(mtqr);
}


