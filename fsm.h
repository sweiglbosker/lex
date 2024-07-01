#ifndef _FSM_H
#define _FSM_H

#include <stdbool.h>

/* common definitions */

#define N_INPUT 127
#define EPSILON (-1)
#define STATE_NONE 0

typedef unsigned int State;

bool valid_input(char input);
int input_index(char input);

#endif /* _FSM_H */
