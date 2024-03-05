#include <pthread.h>
#include "io_operations.h"
#include "types.h"

typedef struct _Op_result {
  u64 command;
  u64 timestamp;
  u64 time_spent;
} Op_result;




