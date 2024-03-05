#include "main.h"

int main()
{
  u64 readData_upper[520] = {0,};
  u64 readData_lower[520] = {0,};
  u64 writeData_upper[520] = {0,};
  u64 writeData_lower[520] = {0,};
  
  printf("Chip2Chip HILS test\n");
  printf("Initialization phase : ");
  if(c2c_init() == -1)
    return(printf("failed\n"));
  else
    printf("success\n");
  
  
  printf("Termination phase : ");
  if(c2c_terminate() == -1)
    return printf("failed\n");
  else
    printf("success\n");
}
