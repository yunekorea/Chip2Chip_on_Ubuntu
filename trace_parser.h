#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "types.h"


struct _IO_FILE;
typedef struct _IO_FILE FILE;

typedef struct _Request {
  u64 timestamp;
  u32 lpn;
  u32 offset;
  u32 size;
  u8 operation;
}Request;

typedef struct _Trace {
  char filename[50];
  FILE *trfile;
} Trace;


Trace get_filename(void);
int get_trace(Trace *trace, Request *request);

