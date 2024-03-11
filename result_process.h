#include "types.h"
#include "main.h"

struct _IO_FILE;
typedef struct _IO_FILE FILE;

typedef struct _Op_result {
  u16 tag;
  u64 time_spent;
  struct _Op_result *next_result;
} Op_result;

int resultfile_open(FILE *res_file);
int save_result(Request *req, Op_result *res, u64 result_time);
int save_file(FILE *res_file, Op_result *res);
