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
  Trace *trace;
  FILE* res_file;
  printf("Chip2Chip HILS test\n");
  if(initialization() == -1)
    return(printf("Initialzation phase failed.\nAborting.\n"));
  else
    printf("Initialization phase success.\n");
 
  /*
  if(tracefile_open(trace) == -1)
    return(printf("Failed to open the tracefile.\nAborting\n"));
  */
  
  /*
  if(resultfile_open(res_file) == -1)
    return(printf("Failed to open the resultfile.\nAborting\n"));
  */

  Request *req = malloc(sizeof(Request));
	printf("malloc C\n");
  //get_trace(trace, req);
  req->bus = 0;
  req->chip = 0;
  req->block = 0;
  req->page = 0;
  req->operation = 1;
  allocate_tag(req);
  printf("allocate C\n");
  generate_command(req);
  printf("generate C\n");
  send_command(req);
  printf("send cmd C\n");

  Op_result *res = malloc(sizeof(Op_result));
  u64 result_time;
  receive_result(res);
  Request *fin_req = save_result_to_request(res);

  

  fclose(trace->trfile);
  fclose(res_file);

  if(termination() == -1)
    return(printf("Termination phase failed.\n"));
  else
    return(printf("Termination phase success.\n"));
}
