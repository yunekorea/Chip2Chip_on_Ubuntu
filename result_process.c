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
    pthread_mutex_lock(&list->rl_mutex);
    next = current->next_request;
    if(list->req_num > 1) {
      next->prev_request = NULL;
      list->first = next;
    }
    else {
      list->first = NULL;
      list->last = NULL;
    }
    list->req_num--;
    pthread_mutex_unlock(&list->rl_mutex);
    save_num--;
    fprintf(res_file, "Command : %llx, Op : %d, Tag : %d, Timestamp : %lld \n",
            current->command, 
            current->operation,
            current->tag,
            current->timestamp);
    fprintf(res_file, "\tBus : %d Chip : %d Block : %d Page : %d\n",
            current->bus,
            current->chip,
            current->block,
            current->page);
    fprintf(res_file, "\tResult time : %lld, Chip Result time : %lld\n",
            current->result_time, 
            current->result_chip_time);
    free(current);
    current = next;
  }
  return 0;
}


