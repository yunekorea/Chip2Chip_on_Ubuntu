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

int main()
{
  Trace *trace = malloc(sizeof(Trace));
  FILE* res_file;
  printf("Chip2Chip HILS test\n");
  if(initialization() == -1)
    return(printf("Initialzation phase failed.\nAborting.\n"));
  else
    printf("Initialization phase success.\n");
 
  
  if(tracefile_open(trace) == -1)
    return(printf("Failed to open the tracefile.\nAborting\n"));
  
  
  if(resultfile_open(res_file) == -1)
    return(printf("Failed to open the resultfile.\nAborting\n"));
  Request *req;
  printf("file open complete.\n");
  int trace_num = 0;
  while(get_trace(trace, req) == -1) {
    allocate_tag(req);
    generate_command(req);
    send_command(req);
    trace_num++;
  }
  
  Fin_req *freq = malloc(sizeof(Fin_req));
  freq->req_num = 0;

  while(trace_num > 0) {
    Op_result *res = malloc(sizeof(Op_result));
    receive_result(res);
    Request *fin_req = save_result_to_request(res);
    save_fin_req(freq, fin_req);
    trace_num--;
  }

  save_fined_to_file(res_file, freq->first, freq->req_num);

  fclose(trace->trfile);
  fclose(res_file);

  if(termination() == -1)
    return(printf("Termination phase failed.\n"));
  else
    return(printf("Termination phase success.\n"));
}
