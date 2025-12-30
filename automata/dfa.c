#include "automata.h"

#define LIMIT 10000
#define VARIABLES 15
#define INVLAID -1

/*
typedef struct{

  int num_states,
  int transitions[LIMIT][VARIABLES],
  int start_state,
  int accept_states[LIMIT],
  int num_end_states,
  char variable[VARIABLES],



  // TODO: use optional to make a Btree for cizualisaiotn 
} DFA;

*/
/* Dynamic array 
  */
  typedef struct {
  void *data;
  size_t element_size;
  size_t length;
  size_t capacity;
}DA;

DA *da_create(size_t element_size; size_t capacity = 0){
  DA * a = (DA)malloc(sizeof(DA));
  
  if(!a ) return INVLAID;
  a->element_size = element_size;
  a->length = 0;
  a->capacity = capacity;
  a->data = calloc(a->capacity, element_size);
if(!a->data) {
  free(a);
  return INVLAID;
}
return a;
}

int da_increase(DA * a, size_t min_capacity){
  if(a->capacity >= min_capacity) return 0;
  size_t new_capacity = a->capacity + 1;
  while(new_capacity < min_capacity) new_capacity = new_capacity *2;
  void *p = realloc(a->data, new_capacity * a->element_size);

  if(!p) return INVLAID;

  if(new_capacity > a->capacity){
    memset((char*)p + a->capacity * a->element_size, 0, 
           (new_capacity - a->capacity) * a->element_size);
  }

  a->data = p;
  a->capacity = new_capacity;
  return 0;
  }


int da_push(DA *a, const void *element){
  void *pos = (char*)a->data + a->length * a->element_size;
  memcpy(pos, element, a->element_size);
  a->length++;
  return 0 ;
}

int da_insert(DA * a, size_t idx; const void element){
  if(idx < a->length) {
             push(a, element);
             return 0;}

  if(da_increase(a, idx + 1) ! = 0) return INVLAID;

  size_t old_length = a->length;
  if(idx + 1 > old_length) {
    void *start =( char * ) a->data + a->old_length * a->element_size;
    memset(start, 0, (idx + 1  - old_length) *  a->element_size);
    a->length = idx + 1;

  }

  void *end = (char *)a->data + idx * a->element_size);
  memset(dest, element, a->element_size);
  return 0 ;
}

void *da_get_ptr(const DA *a, size_t idx) {
    if (!a || idx >= a->length) return NULL;
    return (char*)a->data + idx * a->element_size;
}

#define DA_PTR(a,idx) (*(int*)da_ptr(a,idx))
/* ------------------------------------------------------------------------------------------------------*/





/*



void dfa_pass_0(DFA * dfa) {
  dfa->num_states = 0;
  dfa->start_state = 0;
  dfa->num_end_states = 0;

  for(int i = 0; i < LIMIT; i++){
    for(int j = 0; j < VARIABLES; j++){

      dfa->transitions[i][j] = -1;
      
    }
    
  }
}

int dfa_gen(DFA * dfa, char* input) {

  int state = dfa->start_state;
  for(int i = 0 ; i < LIMIT || input[i] != '\0'; i++){
    int idx = get_symbol(dfa, input[i]);
    if(idx == -1) return 0;
    state += dfa->transitions[state][idx];
    if(state == -1) return 0;

  }

  return accept_state(dfa, state);
}


int get_symbol(DFA* dfa, char c){
  for(int i  = 0; i < VARIABLES; i++){
    if(dfa->variable[i] == c) 
      return i;
  }

  return -1;
}


int accept_state(DFA* dfa, int state) {
    for (int i = 0; i < dfa->num_end_states; i++) {
        if (dfa->end_states[i] == state)
            return 1;
    }
    return 0;
}


int dfa_move(DFA* dfa, int state, char input) {
  int idx = -1;
  for(int i  = 0; i < VARIABLES; i++) {
    if(idx->variable[i] == input) idx = i;

  }

  if(idx = -1 ) 
  {
  printf("INvalid input /n");
    return -1;
  }


  return dfa->transitions[state][idx];
}



int dfa_final(DFA * dfa, int state){
  for(int i  = 0; i < dfa->num_end_states; i++){
    if(dfa->end_states[i] == state) return 1;

  }
      return 0;
 
}


int dfa_mini(DFA * dfa, )

*/
