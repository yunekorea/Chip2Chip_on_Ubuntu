#include "types.h"

struct _IO_FILE;
typedef struct _IO_FILE FILE;

typedef struct _Op_result {
  u64 command;
  u64 timestamp;
  u16 tag;
  u64 time_spent;
  struct _Op_result *next_result;
} Op_result;

int resultfile_open(FILE *res_file);
int save_file(FILE *res_file, Op_result *res);
