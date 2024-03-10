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
  
  printf("Chip2Chip HILS test\n");
  if(initialization() == -1)
    return(printf("Initialzation phase failed.\nAborting.\n"));
  else
    printf("Initialization phase success.\n");
 
  if(tracefile_open(trace) == -1)
    return(printf("Failed to open the tracefile.\nAborting\n"));

  

  if(termination() == -1)
    return(printf("Termination phase failed.\n"));
  else
    return(printf("Termination phase success.\n"));
}
