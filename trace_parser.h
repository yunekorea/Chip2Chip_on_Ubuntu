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
  u8 operation;
  u8 bus;
  u8 chip;
  u16 block;
  u8 page;
  u8 tag;
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
int get_trace(Trace *trace, Request *request);
int free_request(Request *request);
