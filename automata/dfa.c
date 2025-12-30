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

void *da_ptr(const DA *a, size_t idx) {
    if (!a || idx >= a->length) return NULL;
    return (char*)a->data + idx * a->element_size;
}

/* ------------------------------------------------------------------------------------------------------*/



typedef struct {

  int num_states;
  int start_state;
  DA* accept_state;
  DA * variable;
  DA * transitions;
}DFA;

DFA * dfa_alloc(int num_states, int variable_size){
  DFA * d = malloc(sizeof(DFA));
  if(!d) return INVLAID;

  d->num_states = num_states;
  d->start_state = 0;
  d->accept_state = da_create(sizeof(int), 4); // start with 4 states/
  d->variable = da_create(sizeof(char), (variable_size));
  d->transitions = da_create(sizeof(DA*), (num_states));

  for(int i  = 0; i < num_states; i++){
  
    DA * row = da_create(sizeof(int), (variable_size));

    for(int j = 0; j < variable_size; j++){
      int m = INVLAID;
      da_push(row, &m);
    }

    da_push(d->transitions, &row);
  }

  return d;



}

void dfa_free(DFA * d) {
  if(!d) return INVLAID;

  for(int i = 0; i < d->transitions->length; i++){
    DA *row = da_ptr(d->transitions, i );
    dfa_free(row);

  }

  dfa_free(d->transitions);
  dfa_free(d->variable);
  dfa_free(d->accept_state);

  free(d);
}





void dfa_pass_0(DFA *dfa) {
    if (!dfa) return;
    dfa->num_states = 0;
    dfa->start_state = 0;
    if (dfa->accept_states) { da_free(dfa->accept_states); dfa->accept_states = NULL; }
    if (dfa->alphabet) { da_free(dfa->alphabet); dfa->alphabet = NULL; }
    if (dfa->transitions) {
        for (size_t i = 0; i < dfa->transitions->length; ++i) {
            DA *row = DA_GET_PTR(dfa->transitions, i, DA);
            da_free(row);
        }
        da_free(dfa->transitions);
  
    dfa->transitions = NULL;
    
  }



    dfa->accept_states = da_create(sizeof(int), 4);
    dfa->alphabet = da_create(sizeof(char), 4);
    dfa->transitions = da_create(sizeof(DA*), 4);
}



int get_token(DFA * dfa, char c){
if(!dfa) return INVLAID;

  for(int i  = 0; i < dfa->variable->length; i++){

    if(da_ptr(dfa->variable,i) == c) return (int)i;

  }

  return INVLAID;


}




int accept_state(DFA * dfa, int state){

if(!dfa) return INVLAID;
  for(int i =0 ; i < dfa->accept_state->length; i++){
    if(da_ptr(da->accept_state, i)== state)return 1;
  }
  return INVLAID;

}


int dfa_move(DFA *dfa, int state, char element){
  if(!dfa) return INVLAID;
  int pos = get_token(dfa, element);
  if(pos < 0 )return INVLAID;
  if((int)state >= dfa->transitions->length) return INVLAID;

  DA * row = da_ptr(dfa->transitions, state, DA);
  if(!row) return INVLAID;

  return da_ptr(row, pos);
}


int dfa_final(DFA *dfa, int state){


  return accept_state(dfa, state);
}



int dfa_gen(DFA *dfa, char *elements) {

  if(!dfa) return INVLAID;

  int state = dfa->start_state;

  for(int i = 0; i <=10000 || elements[i] != '\0'; i++) {
    int next = dfa_move(dfa, state, elements[i]);
    if(next == INVLAID) return INVLAID;
    state = next;

  }

  return accept_state(dfa, state);
}



int e_find(int arr[], int x){
if(arr[x] !=x) arr[x] = e_find(arr, arr[x]);
  return arr[x];

}


void find_union(int arr[], int r[], int a, int b){
 

  a = e_find(arr, a);
  b = e_find(arr, b);

  if(a == b){
    return INVLAID;
  }
  if(r[a] < r[b]) arr[a] = b;
  else if( r[b] < r[a]) arr[b] = a;
  else {
    arr[b] =a;
   // arr[b] a;
    arr[a]++;
  }


}







DFA * dfa_minimize(DFA * d){

  if(!d) return INVLAID;

  int n = d->transitions->length;
  if(n <= 1) {
    DFA * cope = dfa_alloc(n, d->variable->length);

    if(!cope) return INVLAID;

    cope->start_state = d->start_state;

    for(int i  = 0; i < s->variable->length; i++){
    char ch = da_ptr(d->variable, i);
    
        da_insert(cope->variable, i, &ch);
    }

    for(int i = 0; i < n; i++){
      DA *start = da_ptr(d->transitions,i,DA);
      DA *pos   = da_ptr(cope->transitions,i,DA);

      for(int j = 0; j < start->length; j++){
        int dest = da_ptr(start, j);
        da_insert(dest, j, &dest);
      }
    }
    return cope;
  }

  int mark = -1;
  int **table  = malloc(n * sizeof(int*));

  for(int i  = 0; i < n; i++){
    table[i] = malloc(n * sizeof(int));
    for(int j = 0; j < n; j++) table[i][j] = mark;

  }

  int change = 1;
  int loop = 1;
  int variable_len = d->variable->length;

   for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            int fi = dfa_final(d, i);
            int fj = dfa_final(d, j);
      
        if (fi != fj) table[i][j] = 0;
        }
    }


  while(change) {
    change = 0;
    for(int i = 0; i < n; i++){
      for(int  j = i+ 1; j <  n; j++){
        if(table[i][j]) != mark) continue;

        for(int m=0; m < variable_len; m++){
          DA * row1 =da_ptr(d->transitions, i, DA);
          DA * row2 = da_ptr(d->transitions,i,DA);
     int p = (m < (int)row1->length) ? DA_GET_INT(row1, s) : INVLAID;
       int q = (m < (int)row2->length) ? DA_GET_INT(row2, s) : INVLAID;
        
        if(p == INVLAID || q == INVLAID) {
            table[i][j]  = loop;
            change = 1;
            break;
          }

          if(p > q) {
            int tmp = p; p = q; q = tmp;
          }
          if(p == q) continue;

          if(table[i][j] != mark) {
            table[i][j] = loop;
            change =    1;
            break;
          }

        }
      }
    }
  
    loop++;

  }

  int *arr = malloc(n * sizeof(int));
  int *r = calloc(n, sizeof(int));

  for(int i  = 0; i < n; i++) arr[i] = i;
  for(int i = 0; i <    n; i++){
    for(int j = i+ 1; j < n ; j++ ){
      if(table[i][j] == mark) find_union(arr, i, j);
    }
  }

  int *idx = malloc(n * sizeof(int));
  for(int i  = 0; i < n; i++) idx[i] = -1;

  int new = 0;

  for(int i = 0; i < n; i++)
  {
    if(e_find(arr, i) == i) 
      {
      idx[i] = new++;
    }
  }

  for(int i = 0; i < n; i++) {
    if(id[i] == -1)idx[i] = idx[e_find(arr,i)];

  }

  DFA *min = dfa_alloc(new, variable_len );

  if(!min) {
    for(int i = 0; i < n; i++ ) 
    {free(table[i])
    }
    free(arr);
    free(r);
    free(idx);
    return INVLAID;
  }



  for(int j = 0; j < variable_len; j++){
    char sch = da_ptr(d->variable,s);
    da_insert(min->variable, j, &sch);

  }


  min->start_state = idx[e_find(arr, d->start_state)];

  for(int i = 0; i < n ; i++) {

    if(e_find(arr, i) != i) continue;

    int new_idx = idx[i];
    DA *row = da_ptr(d->transitions, i, DA);
    DA *new_row = da_ptr(min->transitions, new_idx, DA);

      for(int j = 0; j < variable_len; j++){
        int old_dest = (j < row->length);
        if(old_dest == INVLAID) {
          int vari = INVLAID;
          da_insert(new_row, j, &vari);

        }
        else {
          int desti = e_find(arr, old_dest);
          int mapp = idx[desti];
          da_insert(new_row, j, &mapp);
        }
      }
  }

      for (int i = 0; i < n; ++i) 
{
  free(table[i]);
    }
    free(table);
    free(arr);
    free(r);
    free(idx);

    return min;




}

























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
