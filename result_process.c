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


int save_fined_to_file(FILE *res_file, Req_list *list) 
{
  Request *next;
  Request *current = list->first;
  u16 save_num = 128;
  while(list->req_num > 0 && save_num > 0 && current != NULL && current->complete == 1) {
    next = current->next_request;
    if(next != NULL)
      next->prev_request = NULL;
    else
      list->last = NULL;
    list->first = next;
    list->req_num--;
    save_num--;
    fprintf(res_file, "Command : %llx, Tag : %d, Timestamp : %lld, Result time : %lld\n", current->command, current->tag, current->timestamp, current->result_time);
    free(current);
    current = next;
  }
  return 0;
}


