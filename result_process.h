#include "types.h"
#include "trace_parser.h"

#ifndef _RESULT_PROCESS
#define _RESULT_PROCESS

typedef struct _Op_result {
  u8 tag;
  u64 time_spent;
} Op_result;

typedef struct _Fin_req {
  Request *first;
  Request *last;
  u8 req_num;
} Fin_req;

#endif

FILE* resultfile_open(void);
int save_fined_to_file(FILE *res_file, Request *request, int freq);
int save_fin_req(Fin_req *fr, Request *request);
