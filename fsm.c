#include <assert.h>
#include <stdbool.h>

#include "fsm.h"

bool valid_input(char input) {
	return ((input < N_INPUT && input > 0) || input == EPSILON);
}

int input_index(char input) {
	assert(valid_input(input));
	return (input == EPSILON) ? 0 : input;
}
