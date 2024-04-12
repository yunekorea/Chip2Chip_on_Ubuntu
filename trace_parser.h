#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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
  u64 result_chip_time;
  u8 complete;
  u8 error;
  struct _Request *prev_request;
  struct _Request *next_request;
}Request;

typedef struct _Trace {
  char filename[50];
  FILE *trfile;
  u64 numberOfLine;
} Trace;

typedef struct _Req_list {
  Request *first;
  Request *last;
  u16 req_num;
  pthread_mutex_t rl_mutex;
} Req_list;

#endif

int tracefile_open(Trace *trace);
u64 tracefile_total_line(char* filename);
Request* get_trace(FILE *trace);
int free_request(Request *request);
int save_req_list(Req_list *list, Request *request);
