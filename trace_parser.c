#include "trace_parser.h"

Trace get_filename(void)
{
  Trace trace;
  printf("I/O trace file name : ");
  scanf("%s", trace.filename);
  return trace;
}

int get_trace(Trace *trace, Request *request)
{
  static char buffer[200];
  u64 timestamp;
  u32 lpn, offset, size;
  u8 ope;
  if(feof(trace->trfile))
    return -1;
  
  fgets(buffer, 200, trace->trfile);
  sscanf(buffer, "%lld %d %d %d %d", &timestamp, &lpn, &offset, &size, &ope);
  request = malloc(sizeof(Request));
  request->timestamp = timestamp;
  request->lpn = lpn;
  request->offset = offset;
  request->size = size;
  request->operation = ope;

  return 1;
}
