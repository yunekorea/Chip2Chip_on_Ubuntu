#include <pthread.h>
#include "hils_op.h"
#include "types.h"
#include "trace_parser.h"
#include "result_process.h"
#include "tag_mgmt.h"

int initialization(void);
int termination(void);

int thread_command_generator(Req_list *req_list, u8 *trace_eof, Trace *trace);
int thread_result_receiver(Req_list *req_list, u8 *trace_eof);
int thread_file_saver(Req_list *req_list, u8 *trace_eof, FILE *res_file);


