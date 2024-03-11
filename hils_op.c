#include "hils_op.h"

int  fd_memory;   //File descriptor of the memory device

struct _rgstr_vptr {
	u64 *timestamp;		//Command register
	u64 *cmd;		//Read status register
	u64 *result_time;	//Read data upper register
	u64 *chip_res_time;	//Read data lower register
} rgstr_vptr;

int vptr_mmap(u64** vptr, off_t addr) { //Mapping registers to the host's memory
	if(*vptr = (u64 *)mmap(NULL, REG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_memory, addr))
    return 0;
  else
    return -1;
}

int rgstr_offset_map(u64** vptr, u64 offset) {
	*vptr = rgstr_vptr.cmd + offset/sizeof(u64*);
}

int c2c_init(void) {	//opening memory device as a file descriptor to use them with mmap/msync
	
	if((fd_memory = open("/dev/mem", O_RDWR | O_SYNC)) != -1) { //"open" the memory device
		//assign vptr with mmap function
		vptr_mmap(&rgstr_vptr.timestamp, HILS_TS_ADDR);

		rgstr_offset_map(&rgstr_vptr.cmd, HILS_CMD_ADDR);
		rgstr_offset_map(&rgstr_vptr.result_time, HILS_RESULT_ADDR);
		rgstr_offset_map(&rgstr_vptr.chip_res_time, HILS_CHIP_RES_ADDR);
		return 0;
		return -1;
	}
}

int c2c_terminate(void) {
  return close(fd_memory);
}

void CTC_Out(u64* vptr, u64 data) {
	*vptr = data;
	msync(vptr, REG_SIZE, MS_SYNC);
}

u64 CTC_In(u64* vptr) {
	return *vptr;
}

u64 generate_command(Request *request) {

  u64 key = CMD_ACCESS_KEY;
  u64 tag = request->tag;
  u8 operation = request->operation;
  u8 bus = request->bus;
  u8 chip = request->chip;
  u16 block = request->block;
  u8 page = request->page;
  u64 command;
  command = (key << CMD_KEY_BIT)      \
          | (tag << CMD_TAG_BIT)      \
          | (operation << CMD_OP_BIT) \
          | (bus << CMD_BUS_BIT)      \
          | (chip << CMD_CHIP_BIT)    \
          | (block << CMD_BLOCK_BIT)  \
          | (page << CMD_PAGE_BIT)    \
          | CMD_READY_MASK            \
          | CMD_ACK_MASK;

  return command;
}





int wait_cmd_ready(int value){
	int time_cnt = MAX_CMD_RDY_WAIT_CNT;
  if(value == 1) {
	  while((CTC_In(rgstr_vptr.cmd) & CMD_READY_MASK) == 0){
	  //while((Xil_In64(C2C_CMD_ADDR) & CMD_READY_MASK) == 0){
		  if(time_cnt == 0)
			  return -1;
		  time_cnt--;
	  }
  }
  else if(value == 0) {
    while((CTC_In(rgstr_vptr.cmd) & CMD_READY_MASK) == 1) {
      if(time_cnt == 0)
        return -1;
      time_cnt--;
    }
  }
	return 0;
}

int wait_result_ready(int value)
{
  int time_cnt = MAX_RESULT_RDY_WAIT_CNT;
  if(value == 1) {
    while((CTC_In(rgstr_vptr.result_time) & RES_READY_MASK) == 0){
      if(time_cnt == 0)
        return -1;
      time_cnt--;
    }
  }
  else if(value == 0) {
    while((CTC_In(rgstr_vptr.result_time) & RES_READY_MASK) == 1){
      if(time_cnt == 0)
        return -1;
      time_cnt--;
    }
  }
  return 0;
}

int send_command(Request *request)
{
  if(wait_cmd_ready(1) < 0) {
    printf("wait_cmd_ready timeout.\n");
    return -1;
  }
  CTC_Out(rgstr_vptr.cmd, request->command);
  CTC_Out(rgstr_vptr.timestamp, request->timestamp);
  if(wait_cmd_ready(0) < 0) {
    printf("wait_cmd_ready timeout.\n");
    return -1;
  }
  CTC_Out(rgstr_vptr.cmd, request->command & ~CMD_ACK_MASK & ~CMD_READY_MASK);
  return 0;
}

int receive_result(Op_result *result)
{
  if(wait_result_ready(1) < 0) {
    printf("wait_result_ready timeout.\n");
    return -1;
  }
  u64 result_reg;
  u64 result_time;
  u16 result_tag;
  CTC_Out(rgstr_vptr.result_time, RES_ACK_MASK);
  if(wait_result_ready(0) < 0) {
    printf("wait_result_ready timeout.\n");
    return -1;
  }

  result_reg = CTC_In(rgstr_vptr.result_time);
  result_tag = (result_reg | RES_TAG_MASK) >> RES_TAG_BIT;
  result_time = (result_reg | RES_TIME_MASK);
  CTC_Out(rgstr_vptr.result_time, result_reg & ~RES_ACK_MASK);
  result->tag = result_tag;
  result->time_spent = result_time;
  return 0;
}
