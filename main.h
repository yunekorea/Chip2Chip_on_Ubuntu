#include <pthread.h>
#include "hils_op.h"
#include "types.h"
#include "trace_parser.h"
#include "result_process.h"
#include "tag_mgmt.h"

typedef struct _Thread_args {
  Req_list *req_list;
  u8 trace_eof;
  FILE *trace_file;
  FILE *res_file;
  u64 numberOfLine;
  u64 currentLine;
  u16 tags_occupied;
  pthread_mutex_t to_mutex;
} Thread_args;

int initialization(void);
int termination(void);

void* thread_command_generator(void *data);
void* thread_result_receiver(void *data);
void* thread_file_saver(void *data);


