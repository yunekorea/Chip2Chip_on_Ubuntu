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
  Request *fin_req;
  tags[result->tag]->result_time = result->time_spent;
  tags[result->tag]->result_chip_time = result->chip_time_spent;

  tags[result->tag]->complete = 1;
  tags[result->tag]->error = result->error;
  tags[result->tag]->global_time = result->global_time;
  fin_req = tags[result->tag];

  tags[result->tag] = NULL;
  free(result);

  return fin_req;
}

int tag_list_empty(void)
{
  for(int i = 0; i < 128; i++) {
    if(tags[i] != NULL)
      return 0;
  }
  return 1;
}
