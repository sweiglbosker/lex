#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "nfa.h"

NFA *nfa_create(int n_states, State init, State final) {
	NFA *nfa = calloc(1, sizeof(NFA));

	nfa->initial_state = init;
	nfa->accepting_state = final;
	nfa->n_states = n_states;

	nfa->table = calloc(nfa->n_states, sizeof(StateList **));

	for (int i = 0; i < n_states; i++) {
		nfa->table[i] = calloc(N_INPUT, sizeof(StateList *));
	}

	return nfa;
}

void nfa_delete(NFA *nfa) {
	for (int state = 0; state < nfa->n_states; state++) {
		for (int i = 0; i < N_INPUT; i++) {
			if (nfa->table[state][i] != NULL)
			statelist_free(nfa->table[state][i]);
		}
	}
}

void nfa_add_transition(NFA *nfa, State from, State to, unsigned char input) {
	assert(nfa && nfa_state_exists(nfa, from) && nfa_state_exists(nfa, to));
	assert(valid_input(input));
	
	int i = input_index(input);

	nfa->table[from][i] = statelist_append(nfa->table[from][i], to);
}

State nfa_add_state(NFA *nfa) {
	assert(nfa);
	State s = nfa->n_states++;
	nfa->table = realloc(nfa->table, nfa->n_states * sizeof(StateList **));
	nfa->table[s] = calloc(N_INPUT, sizeof(StateList *));

	return s;
}

void nfa_print(NFA *nfa) {
	for (State s = 0; s < nfa->n_states; s++) {
		for (int i = 0; i < N_INPUT; i++) {
			StateList *sl = nfa->table[s][i];
			if (sl == NULL)
				continue;

			while (sl) {
				printf("transition from state %d to state %d on input %d\n", s, sl->s, i);
				sl = sl->next;
			}
		}
	}
}

bool nfa_state_exists(NFA *nfa, State state) {
	if (nfa == NULL)
		return false;

	return (nfa->n_states > state && state >= 0);
}

StateList *statelist_create(State s) {
	StateList *sl = calloc(1, sizeof(StateList));
	
	sl->s = s;

	return sl;
}

void statelist_free(StateList *sl) {
	StateList *next = NULL;

	while (sl) {
		next = sl->next;
		free(sl);
		sl = next;
	}
}

StateList *statelist_append(StateList *sl, State s) {
	StateList *r = calloc(1, sizeof(StateList));

	r->next = sl;
	r->s = s;
	
	return r;
}
