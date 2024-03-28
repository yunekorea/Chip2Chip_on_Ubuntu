#include <stdio.h>
//#include "xil_printf.h"
//#include "xil_io.h"
//#include "xparameters.h"
#include <stdlib.h>
//#include "xuartps.h"
//#include <xil_types.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "types.h"
#include "trace_parser.h"
#include "result_process.h"
/* register address */

#define HILS_BASE_ADDR  0xa0000000
#define TS_OFFSET       0x00
#define CMD_OFFSET      0x08
#define RESULT_OFFSET   0x10
#define CHIP_RES_OFFSET 0x18

#define HILS_TS_ADDR        HILS_BASE_ADDR + TS_OFFSET
#define HILS_CMD_ADDR       HILS_BASE_ADDR + CMD_OFFSET
#define HILS_RESULT_ADDR    HILS_BASE_ADDR + RESULT_OFFSET
#define HILS_CHIP_RES_ADDR  HILS_BASE_ADDR + CHIP_RES_OFFSET

/* command */
#define CMD_ACCESS_KEY 			(u64)0x1E5ABC
#define CMD_OP_READ 				0xc0000000 //((u64)0x03 << CMD_OP_BIT)
#define CMD_OP_WRITE 				0x14000000 //((u64)0x04 << CMD_OP_BIT)
#define CMD_OP_ERASE 				0x18000000 //((u64)0x05 << CMD_OP_BIT)
#define CMD_KEY_BIT 				43
#define CMD_ACK_BIT         42
#define CMD_READY_BIT 			41
#define CMD_START_BIT       40
#define CMD_TAG_BIT 				33
#define CMD_OP_BIT 					30
#define CMD_BUS_BIT 				27
#define CMD_CHIP_BIT 			  24
#define CMD_BLOCK_BIT 		  8
#define CMD_PAGE_BIT 				0
//#define CMD_READY_MASK 			0x0000010000000000
#define CMD_READY_MASK      ((u64)0x01 << CMD_READY_BIT) //0x20000000000
#define CMD_ACK_MASK        ((u64)0x01 << CMD_ACK_BIT)
#define CMD_START_MASK      ((u64)0x01 << CMD_START_BIT)

/* result time */
#define RES_ACK_BIT         48
#define RES_READY_BIT       47
#define RES_TAG_BIT         40
#define RES_ACK_MASK        ((u64)0x01 << RES_ACK_BIT)
#define RES_READY_MASK      ((u64)0x01 << RES_READY_BIT)
#define RES_TAG_MASK        ((u64)0x7F << RES_TAG_BIT))
#define RES_TIME_MASK       (((u64)0x01 << RES_TAG_BIT) - 1)

/* chip result time */
#define CRES_TIME_BIT       19
#define CRES_TIME_MASK      (((u64)0x01 << (CRES_TIME_BIT + 1)) - 1)

/* acknowledge*/

#define VALID 					1
#define INVALID 				0

/* max waiting time*/
#define MAX_CMD_RDY_WAIT_CNT 		10000
#define MAX_RESULT_RDY_WAIT_CNT   1000000

#define REG_SIZE			0xA8

int vptr_mmap(u64** vptr, off_t addr); //Mapping registers to the host's memory
int rgstr_offset_map(u64** vptr, u64 offset);
int c2c_init(void);	//opening memory device as a file descriptor to use them with mmap/msync
int c2c_terminate(void);
void CTC_Out(u64* vptr, u64 data);
u64 CTC_In(u64* vptr);
u64 generate_command(Request *request);

int read_page(u64 bus, u64 chip, u64 block, u64 page, u64* pReadBuf_upper, u64* pReadBuf_lower);
int write_page(u64 bus, u64 chip, u64 block, u64 page, u64* pWriteBuf_upper, u64* pWriteBuf_lower);
int erase_block(u64 bus, u64 chip, u64 block);

int wait_cmd_ready(int value);
int wait_wrData_ready(int value);

int wait_result_ready(int value);
int wait_writeData_req(u64* requested_tag);
int wait_flash_operation(u64 op, u64 tag, int* Qnumber, u64* ack,u64* ack_tag);


int flush_command(void);
int send_command(Request *request);
Op_result* receive_result(void);
