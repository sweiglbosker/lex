#include <stdio.h>

#include "dfa.h"
#include "nfa.h"

int main(void) {
/*	puts("hello\n");
	DFA *d = dfa_create(11, 0, 10);	

	dfa_add_transition(d, 0, 7, EPSILON); 
	dfa_add_transition(d, 1, 4, EPSILON);
	dfa_add_transition(d, 4, 5, 'b');
	dfa_add_transition(d, 6, 7, EPSILON); 
	dfa_add_transition(d, 7, 8, 'a'); 
	dfa_add_transition(d, 8, 9, 'b'); 
	dfa_add_transition(d, 9, 10, 'b');

	dfa_print(d);

	dfa_delete(d); */

	NFA *n = nfa_create(3, 0, 2);

	nfa_add_transition(n, 0, 1, 'c');
	nfa_add_transition(n, 0, 1, 'a');
	nfa_add_transition(n, 0, 2, EPSILON);
	nfa_add_transition(n, 1, 0, 'c');
	
	nfa_print(n);
}
