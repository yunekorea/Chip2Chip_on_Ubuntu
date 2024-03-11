#include "types.h"
#include "trace_parser.h"

typedef struct _Op_result {
  u16 tag;
  u64 time_spent;
} Op_result;

int resultfile_open(FILE *res_file);
int save_fined_to_file(FILE *res_file, Request *request);
