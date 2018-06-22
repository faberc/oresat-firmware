#include "acs_statemachine.h"

char *state_name[] = {
	"ST_RDY",
	"ST_RW",
	"ST_MTQR",
	"ST_MAX_POWER"
};

char *function_name[] = {
	"FN_RW_SETDC",
	"FN_MTQR_SETDC",
	"END"
};

static void print_name(char *name){
	printf("%s",name);	
}

static void print_state(acs_state state){
	print_name(state_name[state]);
}

static void print_function(acs_function function){
	print_name(function_name[function]);
}

static int printTransition(acs_state state,char *s){
	printf("%s ",s);	
	print_state(state);
	printf("\n");
	return state;
}

static int printFunctionCall(acs_function function,char *s){
	printf("%s ",s);	
	print_function(function);
	printf("\n");
	return function;
}

// ST_RDY state transistion functions
static int entry_rdy(ACS *acs){
	(void)acs;
	return printTransition(ST_RDY,ENTRY_STRING);
}

static int exit_rdy(ACS *acs){
	(void)acs;
	return printTransition(ST_RDY,EXIT_STRING);;
}

// ST_RW state transistion functions
static int entry_rw(ACS *acs){
	(void)acs;
	return printTransition(ST_RW,ENTRY_STRING);
}

static int exit_rw(ACS *acs){
	(void)acs;
	return printTransition(ST_RW,EXIT_STRING);
}

// ST_MTQR state transistion functions
static int entry_mtqr(ACS *acs){
	(void)acs;
	return printTransition(ST_MTQR,ENTRY_STRING);
}

static int exit_mtqr(ACS *acs){
	(void)acs;
	return printTransition(ST_MTQR,EXIT_STRING);
}

// ST_MAX_POWER state transistion functions
static int entry_max_pwr(ACS *acs){
	(void)acs;
	return printTransition(ST_MAX_PWR,ENTRY_STRING);
}

static int exit_max_pwr(ACS *acs){
	(void)acs;
	return printTransition(ST_MAX_PWR,EXIT_STRING);
}

// functions
static int fn_rw_setdc(ACS *acs){
	(void)acs;
	return printFunctionCall(FN_RW_SETDC,FUNC_STRING);
}

static int fn_mtqr_setdc(ACS *acs){
	(void)acs;
	return printFunctionCall(FN_MTQR_SETDC,FUNC_STRING);
	return EXIT_SUCCESS;
}

//*
acs_function_rule func[] = {
	{ST_RW, 	FN_RW_SETDC,		&fn_rw_setdc},
	{ST_MTQR, FN_MTQR_SETDC,	&fn_mtqr_setdc}
};

#define EVENT_COUNT (int)(sizeof(func)/sizeof(acs_function))
//*/

/*
static int callFunction(ACS *acs){
	printf("%sfsm_trap, keeping state!\n",TRAP_STRING);	
	int i,trap_status;

	for(i = 0;i < EVENT_COUNT;++i){
		if(acs->cur_state == trap[i].state){
			if((acs->event == trap[i].event)){
				trap_status = (trap[i].fn)(acs);
				if(trap_status){
					printf("trap error!\n");
				}
				break;
			}
		}
	}

	return acs->cur_state;
}
//*/

acs_transition_rule trans[] = {
	{ST_RDY,			ST_RW,				&entry_rw,				&exit_rdy},
	{ST_RDY,			ST_MTQR,			&entry_mtqr,			&exit_rdy},
	{ST_RDY,			ST_MAX_PWR,		&entry_max_pwr,		&exit_rdy},
	{ST_RW,				ST_MAX_PWR,		&entry_max_pwr,		&exit_rw},
	{ST_MTQR,			ST_MAX_PWR,		&entry_max_pwr,		&exit_mtqr},
	{ST_RW,				ST_RDY,				&entry_rdy,				&exit_rw},
	{ST_MTQR,			ST_RDY,				&entry_rdy,				&exit_mtqr},
	{ST_MAX_PWR,	ST_RDY,				&entry_rdy,				&exit_max_pwr},
	{ST_MAX_PWR,	ST_RW,				&entry_rw,				&exit_max_pwr},
	{ST_MAX_PWR,	ST_MTQR,			&entry_mtqr,			&exit_max_pwr},
};

#define TRANS_COUNT (int)(sizeof(trans)/sizeof(acs_transition_rule))

void print_welcome(){
	printf("state enum: "); 
	printf("\t%d, \t%d, \t %d, \t %d\n",
			ST_RDY,ST_RW,ST_MTQR,ST_MAX_PWR);
	
	printf("state names: \t"); 
	print_state(ST_RDY); printf(", ");
	print_state(ST_RW); printf(", ");
	print_state(ST_MTQR); printf(", ");
	print_state(ST_MAX_PWR); printf("\n");
/*	
	printf("event enum: "); 
	printf("\t%d, \t%d, \t%d, \t %d, \t %d, \t %d, \t %d\n",
			EV_ANY,EV_OFF,EV_INIT,EV_RDY,EV_RW,EV_MTQR,EV_REP);

	printf("event names: \t"); 
	print_event(EV_ANY); printf(", ");
	print_event(EV_OFF); printf(", ");
	print_event(EV_INIT); printf(", ");
	print_event(EV_RDY); printf(", ");
	print_event(EV_RW); printf(", ");
	print_event(EV_MTQR); printf(", ");
	print_event(EV_REP); printf("\n");
	printf("TRANS_CNT: %d\n\n",TRANS_COUNT);
//*/
}

acs_state requestTransition(ACS *acs){
	acs_state state;
	char input[3]="-1\n";

	printf("current state: %d ",acs->cur_state);
	print_state(acs->cur_state);
	printf("\nrequest state? ");
	scanf(" %s", input);
	state = atoi(input);
	if(state < 0 || state >= MAX_STATES){
		printf("error, invalid state: %d\n",state);
		return acs->cur_state; 
	}
	printf("transition request %s received\n", state_name[state]);
	return state;
}

extern int acs_statemachine(ACS *acs){
	int i;

	print_welcome();

	printf("Starting state machine!\n\n");
	acs->cur_state = entry_rdy(acs);
	printf("\nIntitializing ACS to state: %s\n\n",state_name[acs->cur_state]);
	
	while(1){
		acs->req_state = requestTransition(acs);
		for (i = 0;i < TRANS_COUNT;++i) {
			if((acs->cur_state==trans[i].cur_state)&&(acs->req_state==trans[i].req_state)){
				acs->cur_state = (trans[i].fn_entry)(acs);
				acs->last_state = (trans[i].fn_exit)(acs);
				break;
			}
		}
		printf("\n");
	}
	
	return EXIT_SUCCESS;
}

