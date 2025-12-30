#include "automata.h"

#define LIMIT 10000
typedef struct{

  int num_states;
  int transitions[LIMIT][LIMIT];
  int start_state;
  int accept_states[LIMIT];
} DFA;

int dfa_gen(DFA * dfa, char* input) {

  int state = dfa->start_state;
  for(int i = 0 ; i < LIMIT || input[i] != '\0'; i++){
    state += dfa->transitions[state][input[i]];

  }
  return accept_state(dfa, state);
}


