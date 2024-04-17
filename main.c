#include "main.h"

int initialization(void)
{
  printf("\e[?25l");
  printf("Initialization phase : ");
  if(c2c_init() == -1) {
    printf("failed.\n");
    return -1;
  }
  else {
    printf("success.\n");
    return 0;
  }
}

int termination(void)
{
  printf("\e[?25h");
  printf("\n\n\n");
  printf("Termination phase : ");
  if(c2c_terminate() == -1) {
    printf("failed.\n");
    return -1;
  }
  else {
    printf("success.\n");
    return 0;
  }
}

void* thread_command_generator(void *data)
{
  Thread_args *args = data;
  Request *req;

  while((req = get_trace(args->trace_file)) != NULL) {
#ifdef DEBUG
    printf("command generate\n");
#endif
    save_req_list(args->req_list, req);
    allocate_tag(req);
    generate_command(req);
    send_command(req);
    args->currentLine += 1;
    pthread_mutex_lock(&args->to_mutex);
    args->tags_occupied += 1;
    pthread_mutex_unlock(&args->to_mutex);
  }

  args->trace_eof = 1;
#ifdef DEBUG
  printf("thread_command_generator is closed.\n");
#endif
return 0;
}

void* thread_result_receiver(void *data)
{
  Thread_args *args = data;
  Op_result *res = NULL;
  Request *fin_req;
  while(args->trace_eof == 0 || tag_list_empty() == 0) {
    res = NULL;
    res = receive_result();
    if(res != NULL) {
#ifdef DEBUG
      printf("save result to request\n");
#endif
      fin_req = save_result_to_request(res);
      pthread_mutex_lock(&args->to_mutex);
      args->tags_occupied -= 1;
      pthread_mutex_unlock(&args->to_mutex);
    }
  }
#ifdef DEBUG
  printf("thread_result_receiver is closed.\n");
#endif
return 0;
}

void* thread_file_saver(void *data)
{
  Thread_args *args = data;
  while(args->trace_eof == 0 || args->req_list->last != NULL) {
#ifndef DEBUG
    printf("Current Line : %lld / %lld (%.2f%%)\t\n", 
           args->currentLine, 
           args->numberOfLine,
           ((double)args->currentLine/(double)args->numberOfLine)*100);
    printf("Tags occupied : %d / 128        \t\n",  args->tags_occupied);
    printf("Req list size : %d \t\n", args->req_list->req_num);
    printf("EOF Status : %d\t\n", args->trace_eof);
    printf("\x1b[%dA\r", 4);
#endif
    if(args->trace_eof == 0 && args->req_list->req_num > 256) {
#ifdef DEBUG
      printf("save file.\n");
#endif
      save_fined_to_file(args->res_file, args->req_list);
    }
    else if(args->trace_eof == 1 && args->req_list->last != NULL)
      //printf("thread_file_saver : eof is 1.\n");
      save_fined_to_file(args->res_file, args->req_list);
  }
#ifdef DEBUG
  printf("thread_file_saver is closed\n");
#endif
  return 0;
}

int main(void)
{
  Trace *trace = malloc(sizeof(Trace));
  FILE* res_file;
  pthread_t pthread[3];
  int thread_id;
  int status;
  //u8 trace_eof = 0;
  Thread_args *thread_args;
  Req_list *req_list;

  printf("Chip2Chip HILS test\n");
  if(initialization() == -1) {
    printf("Aborting.\n");
    return -1; 
  }

  if(tracefile_open(trace) == -1)
    return(printf("Failed to open the tracefile.\nAborting\n"));
  
  if((res_file = resultfile_open()) == NULL)
    return(printf("Failed to open the resultfile.\nAborting\n"));
  
  printf("file open complete.\n");

  flush_command();
  req_list = malloc(sizeof(Req_list));
  req_list->req_num = 0;
  pthread_mutex_init(&req_list->rl_mutex, NULL);

  thread_args = malloc(sizeof(Thread_args));
  thread_args->req_list = req_list;
  thread_args->trace_eof = 0;
  thread_args->trace_file = trace->trfile;
  thread_args->res_file = res_file;
  thread_args->numberOfLine = trace->numberOfLine;
  thread_args->currentLine = 0;
  thread_args->tags_occupied = 0;
  pthread_mutex_init(&thread_args->to_mutex, NULL);

  thread_id = pthread_create(&pthread[0], NULL, thread_command_generator, (void*)thread_args);
  thread_id = pthread_create(&pthread[1], NULL, thread_result_receiver, (void*)thread_args);
  thread_id = pthread_create(&pthread[2], NULL, thread_file_saver, (void*)thread_args);

  pthread_join(pthread[0], (void**)&status);
  pthread_join(pthread[1], (void**)&status);
  pthread_join(pthread[2], (void**)&status);

  fclose(trace->trfile);
  fclose(res_file);
  

  if(termination() == -1) {
    printf("Aborting.\n");
    return -1;
  }
  else
    return 0;

}
