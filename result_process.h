#include "types.h"
#include "trace_parser.h"

#ifndef _RESULT_PROCESS
#define _RESULT_PROCESS

typedef struct _Op_result {
  u16 tag;
  u64 time_spent;
  u64 chip_time_spent;
  u64 global_time;
  u8 error;
} Op_result;

#endif

FILE* resultfile_open(void);
int save_fined_to_file(FILE *res_file, Req_list *req_list);
