#ifndef _DFA_H
#define _DFA_H

#include <stdbool.h>
#include <stdint.h>

#include "fsm.h"

typedef struct DFA {
	int n_states;
	State initial_state;
	State accepting_state;
	State **table;	/* trans[State] */
} DFA;

DFA *dfa_create(int n_states, State init, State final);
void dfa_delete(DFA *dfa);

void dfa_add_transition(DFA *dfa, State from, State to, unsigned char input);
State dfa_add_state(DFA *dfa);
State dfa_trans(DFA *dfa, State current, char input);

void dfa_print(DFA *dfa);

bool dfa_state_exists(DFA *dfa, State state);
unsigned int dfa_table_size(DFA *dfa);


#endif /* _DFA_H */
