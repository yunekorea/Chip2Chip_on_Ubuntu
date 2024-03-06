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
  u8 tag;
  struct _Request *next_request;
}Request;

typedef struct _Trace {
  char filename[50];
  FILE *trfile;
} Trace;


int tracefile_open(Trace *trace);
int get_trace(Trace *trace, Request *request);

