#include "tag_mgmt.h"

Request *tags[128];

int allocate_tag(Request *request)
{
  static u8 tags_pointer = 0;
  while(tags[tags_pointer] != NULL) {
    tags_pointer++;
    if(tags_pointer > 127)
      tags_pointer = 0;
  }

  tags[tags_pointer] = request;
  request->tag = tags_pointer;
  tags_pointer++;
  if(tags_pointer > 127)
    tags_pointer = 0;

  return 0;
}

Request *save_result_to_request(Op_result *result)
{
  printf("save result to request\n");
  Request *fin_req;
  tags[(u8)result->tag]->result_time = result->time_spent;
  printf("SRTR : time save\n");
  fin_req = tags[(u8)result->tag];

  free(result);
  tags[(u8)result->tag] = NULL;

  return fin_req;
}
