#include "trace_parser.h"

int tracefile_open(Trace *trace)
{
  //trace = malloc(sizeof(Trace));
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
  char operation_name[5];
  u16 bus;
  u16 chip;
  u16 block;
  u16 page;

  u8 ope;
  if(fgets(buffer, 200, trace->trfile) == NULL) {
    printf("fgets NULL\n");
    if(feof(trace->trfile) != 0)
      return -1;
  }
  printf("fgets done\n");
  sscanf(buffer, "%lld %s %hu %hu %hu %hu", &timestamp, &operation_name, &bus, &chip, &block, &page);
  printf("%s\n", buffer);
  request = malloc(sizeof(Request));
  printf("%llu, %s, %hu, %hu, %hu, %hu\n", timestamp, operation_name, bus, chip, block, page);
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
  request->bus = (u8)bus;
  request->chip = (u8)chip;
  request->block = block;
  request->page = (u8)page;
  request->next_request = NULL;

  return 0;
}

