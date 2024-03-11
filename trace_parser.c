#include "trace_parser.h"

int tracefile_open(Trace *trace)
{
  trace = malloc(sizeof(Trace));
  printf("I/O trace file name : ");
  scanf("%s", trace->filename);
  trace->trfile = fopen(trace->filename, "r");
  if(trace->trfile == NULL)
    return -1;
  return 0;
}

/*
int get_trace(Trace *trace, Request *request)
{
  static char buffer[200];
  u64 timestamp;
  u32 ppn, offset, size;
  u8 ope;
  if(feof(trace->trfile))
    return -1;
  
  fgets(buffer, 200, trace->trfile);
  sscanf(buffer, "%lld %d %d %d", &timestamp, &ppn, &size, &ope);
  request = malloc(sizeof(Request));
  request->timestamp = timestamp;
  request->ppn = ppn;
  request->size = size;
  request->operation = ope;
  request->next_request = NULL;
  request->tag = 0b10000000;

  return 0;
}
  */
