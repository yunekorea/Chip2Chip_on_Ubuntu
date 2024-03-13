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


int get_trace(Trace *trace, Request *request)
{
  static char buffer[200];
  u64 timestamp;
  char *operation_name;
  u8 bus;
  u8 chip;
  u16 block;
  u8 page;

  u8 ope;
  if(feof(trace->trfile))
    return -1;
  
  fgets(buffer, 200, trace->trfile);
  sscanf(buffer, "%lld %s %d %d %d %d %d", &timestamp, &operation_name, &bus, &chip, &block, &page);
  request = malloc(sizeof(Request));
  request->timestamp = timestamp;
  if(strcmp(operation_name, "read") == 0)
    request->operation = 3;
  else if(strcmp(operation_name, "write") == 0)
    request->operation = 4;
  else if(strcmp(operation_name, "erase") == 0)
    request->operation = 5;
  else {
    printf("Trace's operation isn't valid\n\toperation : %s\n", operation_name);
    return -1;
  }
  request->bus = bus;
  request->chip = chip;
  request->block = block;
  request->page = page;
  request->next_request = NULL;
  request->tag = 0b10000000;

  return 0;
}

