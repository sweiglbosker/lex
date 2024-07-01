#ifndef _NFA_H
#define _NFA_H

#include <stdbool.h>

#include "fsm.h"

typedef struct StateList {
	State s;
	struct StateList *next;
} StateList;

typedef struct NFA {
	int n_states;
	State initial_state;
	State accepting_state; 
	StateList ***table; /* table[state][input] gives you a list of states */	
} NFA;

NFA *nfa_create(int n_states, State init, State final);
void nfa_delete(NFA *nfa);

void nfa_add_transition(NFA *nfa, State from, State to, unsigned char input);
State nfa_add_state(NFA *nfa);

void nfa_print(NFA *nfa);

bool nfa_state_exists(NFA *nfa, State state);

StateList *statelist_create(State s);
void statelist_free(StateList *sl);
StateList *statelist_append(StateList *sl, State s);

#endif /* _NFA_H */
