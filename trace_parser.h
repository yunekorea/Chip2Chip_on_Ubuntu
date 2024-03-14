#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#ifndef _TRACE_PARSER
#define _TRACE_PARSER
//struct _IO_FILE;
//typedef struct _IO_FILE FILE;

typedef struct _Request {
  u64 timestamp;
  u16 operation;
  u16 bus;
  u16 chip;
  u16 block;
  u16 page;
  u16 tag;
  u64 command;
  u64 result_time;
  struct _Request *next_request;
}Request;

typedef struct _Trace {
  char filename[50];
  FILE *trfile;
} Trace;
#endif

int tracefile_open(Trace *trace);
Request* get_trace(Trace *trace);
int free_request(Request *request);
