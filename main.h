#include <pthread.h>
#include "hils_op.h"
#include "types.h"
#include "trace_parser.h"
#include "result_process.h"
#include "tag_mgmt.h"

int initialization(void);
int termination(void);

int thread_command_generator(void);
int thread_result_receiver(void);
int thread_file_saver(void);


