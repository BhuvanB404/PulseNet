#include "main.h"
#include"app.h"


typedef  enum {

  ERR_NONE,
  ERR_INVALID_INPUT,
  ERR_PARSE_FAIL,
}Err_type;

void err_handler(Err_type err){

  fprintf("ceer: The error is : " , err);

  return -1;
}


