#include "main.h"

int initialization(void)
{
  printf("Initialization phase : ");
  if(c2c_init() == -1)
    return -1;
  else
    return 0;
}

int termination(void)
{
  printf("Termination phase : ");
  if(c2c_terminate() == -1)
    return -1;
  else
    return 0;
}

void* thread_command_generator(void *data)
{
  Thread_args *args = data;
  Request *req;

  while((req = get_trace(args->trace_file)) != NULL) {
    save_req_list(args->req_list, req);
    allocate_tag(req);
    generate_command(req);
    send_command(req);
  }

  *args->trace_eof = 1;
  
  return 0;
}

void* thread_result_receiver(void *data)
{
  Thread_args *args = data;
  Op_result *res = NULL;
  Request *fin_req;
  while(*args->trace_eof == 0 || tag_list_empty() == 0) {
    res = receive_result();
    if(res != NULL)
      fin_req = save_result_to_request(res);
  }

  return 0;
}

void* thread_file_saver(void *data)
{
  Thread_args *args = data;
  while(*args->trace_eof == 0 || args->req_list->last != NULL) {
    if(*args->trace_eof == 0 && args->req_list->req_num > 256)
      save_fined_to_file(args->res_file, args->req_list);
    else if(*args->trace_eof == 1 && args->req_list->last != NULL)
      save_fined_to_file(args->res_file, args->req_list);
  }
  return 0;
}

int main(void)
{
  Trace *trace = malloc(sizeof(Trace));
  FILE* res_file;
  pthread_t pthread[3];
  int thread_id;
  int status;
  u8 trace_eof = 0;
  Thread_args *thread_args;
  Req_list *req_list;

  printf("Chip2Chip HILS test\n");
  if(initialization() == -1)
    return(printf("Initialzation phase failed.\nAborting.\n"));
  else
    printf("Initialization phase success.\n");
  
  if(tracefile_open(trace) == -1)
    return(printf("Failed to open the tracefile.\nAborting\n"));
  
  if((res_file = resultfile_open()) == NULL)
    return(printf("Failed to open the resultfile.\nAborting\n"));
  
  printf("file open complete.\n");

  flush_command();
  req_list = malloc(sizeof(Req_list));
  req_list->req_num = 0;

  thread_args = malloc(sizeof(Thread_args));
  thread_args->req_list = req_list;
  thread_args->trace_eof = &trace_eof;
  thread_args->trace_file = trace->trfile;
  thread_args->res_file = res_file;

  thread_id = pthread_create(&pthread[0], NULL, thread_command_generator, (void*)thread_args);
  thread_id = pthread_create(&pthread[1], NULL, thread_result_receiver, (void*)thread_args);
  //thread_id = pthread_create(&pthread[2], NULL, thread_file_saver, (void*)thread_args);

  pthread_join(pthread[0], (void**)&status);
  pthread_join(pthread[1], (void**)&status);
  //pthread_join(pthread[2], (void**)&status);

  fclose(trace->trfile);
  fclose(res_file);

  if(termination() == -1)
    return(printf("Termination phase failed.\n"));
  else
    return(printf("Termination phase success.\n"));
}
