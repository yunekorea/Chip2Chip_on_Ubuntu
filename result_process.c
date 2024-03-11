#include <stdio.h>
#include <pthread.h>
#include "result_process.h"

int resultfile_open(FILE *res_file)
{
  if((res_file = fopen("result.dat", "w"))!=NULL)
    return 0;
  else
    return -1;
}


int save_result(FILE *res_file, Request *request) 
{
  fprintf(res_file, "%x, %d, %d\n", request->command, request->timestamp, request->result_time);
  return 0;
}
