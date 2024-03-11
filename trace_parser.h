#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "types.h"

#ifndef _TRACE_PARSER
struct _IO_FILE;
typedef struct _IO_FILE FILE;

typedef struct _Request {
  u64 timestamp;
  u32 ppn;
  u32 size;
  u8 operation;
  u8 tag;
  u64 command;
  struct _Request *next_request;
}Request;

typedef struct _Trace {
  char filename[50];
  FILE *trfile;
} Trace;
#endif

int tracefile_open(Trace *trace);
int get_trace(Trace *trace, Request *request);
int free_request(Request *request);
