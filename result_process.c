#include <stdio.h>
#include <pthread.h>
#include "result_process.h"

FILE* resultfile_open(void)
{
  /*if((res_file = fopen("result.dat", "w"))!=NULL)
    return 0;
  else
    return -1;
  */
  return fopen("result.dat", "w");
}


int save_fined_to_file(FILE *res_file, Request *request, int freq) 
{
  Request *next;
  while(freq > 0) {
    next = request->next_request;
    fprintf(res_file, "%llx, %lld, %lld\n", request->command, request->timestamp, request->result_time);
    free(request);
    request = next;
    freq--;
  }
  return 0;
}

int save_fin_req(Fin_req *fr, Request *request)
{
  if(fr->req_num == 0) {
    fr->first = fr->last = request;
    fr->req_num++;
  }
  else {
    fr->last->next_request = request;
    fr->last = request;
    fr->req_num++;
  }

  return 0;
}
