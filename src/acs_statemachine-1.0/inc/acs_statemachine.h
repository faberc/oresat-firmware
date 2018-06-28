#ifndef _ACS_STATEMACHINE_H_
#define _ACS_STATEMACHINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define EXIT_SUCCESS 0
//#define EXIT_FAILURE 1

#define ENTRY_STRING 	"*Entering state: " 
#define EXIT_STRING  	"*Exiting state: " 
#define FUNC_STRING 	"*Calling function: " 

extern char *state_name[];
extern char *event_name[];

typedef enum{
//	EXIT_SUCCESS,
//	EXIT_FAILURE,
	ERROR_THING
}return_code;

typedef enum{
	ST_RDY,		// low power
	ST_RW,
	ST_MTQR,
	ST_MAX_PWR
}acs_state;

#define MAX_STATES (int)(sizeof(acs_state))

typedef enum{
	FN_RW_SETDC,
	FN_MTQR_SETDC//,
	//END // this must be the last entry
}acs_function;

typedef struct ACS ACS;

struct ACS{
	acs_state cur_state;
	acs_function function;
	int (*fn_exit)(ACS *acs);
};

typedef struct{
	acs_state cur_state;
	acs_state req_state;
	int (*fn_entry)(ACS *acs);
	int (*fn_exit)(ACS *acs);
}acs_transition_rule;

typedef struct{
	acs_state state;
	acs_function function;
	int (*fn)(ACS *acs);
}acs_function_rule;

extern acs_transition_rule trans[];
extern acs_function_rule func[];

extern int acs_statemachine(ACS *acs);

#endif
