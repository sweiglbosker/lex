#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "fsm.h"
#include "dfa.h"

#define DFA_ROW_SIZE (N_INPUT * sizeof(State))

DFA *dfa_create(int n_states, State init, State final) {
	DFA *dfa = calloc(1, sizeof(struct DFA));

	dfa->initial_state = init;
	dfa->accepting_state = final;
	dfa->n_states = n_states;

	dfa->table = calloc(dfa->n_states, sizeof(State *));
	for (State s = 0; s < dfa->n_states; s++) {
		dfa->table[s] = calloc(N_INPUT, sizeof(State));
	}

	return dfa;
}

void dfa_delete(DFA *dfa) {
	for (int i = 0; i < dfa->n_states; i++) {
		free(dfa->table[i]);
	}
	free(dfa->table);
	free(dfa);
}

void dfa_add_transition(DFA *dfa, State from, State to, unsigned char input) {
	assert(dfa && dfa_state_exists(dfa, from) && dfa_state_exists(dfa, to));
	assert(valid_input(input));
	
	int i = input_index(input);

	dfa->table[from][i] = to;	
}

State dfa_add_state(DFA *dfa) {
	assert(dfa);
	State s = dfa->n_states++;
	dfa->table = realloc(dfa->table, dfa->n_states * sizeof(State *));
	dfa->table[s] = calloc(N_INPUT, sizeof(State));

	return s;
}

State dfa_trans(DFA *dfa, State current, char input) {
	int index = input_index(input);

	return dfa->table[current][index];
}

// this is slow because transition table is optimized for indexing not iteration
void dfa_print(DFA *dfa) {
	for (State state = 0; state < dfa->n_states; state++) {
		for (int i = 0; i < N_INPUT; i++) {
			if (dfa->table[state][i] != STATE_NONE) {
				printf("transition from state %d to state %d on input %d\n", state, dfa->table[state][i], i);
			}
		}
	}
}

bool dfa_state_exists(DFA *dfa, State state) {
	if (dfa == NULL) 
     		return false;

	return (dfa->n_states > state && state >= 0);
}


unsigned int dfa_table_size(DFA *dfa) {
	return dfa->n_states * DFA_ROW_SIZE;
}

