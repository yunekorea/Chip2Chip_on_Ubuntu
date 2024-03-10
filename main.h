#include <pthread.h>
#include "hils_op.h"
#include "types.h"
#include "trace_parser.h"

typedef struct _Op_result {
  u64 command;
  u64 timestamp;
  u64 time_spent;
  struct _Op_result *next_result;
} Op_result;

int initialization(void);
int termination(void);


