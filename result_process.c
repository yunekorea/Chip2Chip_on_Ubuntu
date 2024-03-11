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

int save_result(Request *req, Op_result *res, u64 result_time)
{
  res->command = req->command;
  res->timestamp = req->timestamp;
  res->time_spent = result_time;

  return 0;
}

int save_file(FILE *res_file, Op_result *res)
{
  fprintf(res_file, "%x, %d, %d\n", res->command, res->timestamp, res->time_spent);
  return 0;
}
