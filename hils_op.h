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
#include "main.h"
/* register address */

#define HILS_BASE_ADDR  0xa0080000
#define TS_OFFSET       0x00
#define CMD_OFFSET      0x08
#define RESULT_OFFSET   0x10
#define CHIP_RES_OFFSET 0x18

#define HILS_TS_ADDR        HILS_BASE_ADDR + TS_OFFSET
#define HILS_CMD_ADDR       HILS_BASE_ADDR + CMD_OFFSET
#define HILS_RESULT_ADDR    HILS_BASE_ADDR + RESULT_OFFSET
#define HILS_CHIP_RES_ADDR  HILS_BASE_ADDR + CHIP_RES_OFFSET

/* command */
#define CMD_ACCESS_KEY 			0x1E5A
#define CMD_OP_READ 				0x3
#define CMD_OP_WRITE 				0x4
#define CMD_OP_ERASE 				0x5
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

/* result time */
#define RES_ACK_BIT         48
#define RES_READY_BIT       47
#define RES_TAG_BIT         40
#define RES_READY_MASK      ((u64)0x01 << RES_READY_BIT)
#define RES_TAG_MASK        ((u64)0x7F << RES_TAG_BIT))
#define RES_TIME_MASK       (((u64)0x01 << RES_TAG_BIT) - 1)


/* acknowledge*/
#define ACK_ERASE_ERR 			  0
#define ACK_ERASE_DONE 			  1
#define ACK_WRITE_DONE 			  2
#define ACK_BIT 				      9
#define ACK_TAG_BIT 			    11
#define ACK_CODE_MASK 			  0x0000000000000600
#define ACK_TAG_MASK 			    0x000000000003f800
#define WR_ER_ACK_READY_MASK 	0x0000000000040000
#define WR_ER_ACK_VALID	 		  0x0000000000080000

#define VALID 					1
#define INVALID 				0

/* write*/
#define WR_DATA_TAG_BIT 		20
#define WR_DATA_TAG_VALID_BIT 	27
#define WAIT_WRDATA_READY_BIT 	41
#define WR_DATA_READY_MASK 		0x0000020000000000
#define WR_DATA_REQ_READY_MASK 	0x0000000000000080
#define WR_DATA_REQ_TAG_MASK 	0x000000000000007f
#define WR_DATA_REQ_VALID	 	0x0000000000000100
#define WR_DATA_REQ_TAG_BIT 	0
/* read */
#define READQ_READY_MASK 		0x8080808080808080
#define READQ_TAG_MASK 			0x7f7f7f7f7f7f7f7f
#define READ_DATA0_READY_VALUE 	0x0000000000000080
#define READ_DATA1_READY_VALUE 	0x0000000000008000
#define READ_DATA2_READY_VALUE 	0x0000000000800000
#define READ_DATA3_READY_VALUE 	0x0000000080000000
#define READ_DATA4_READY_VALUE 	0x0000008000000000
#define READ_DATA5_READY_VALUE 	0x0000800000000000
#define READ_DATA6_READY_VALUE 	0x0080000000000000
#define READ_DATA7_READY_VALUE 	0x8000000000000000
#define READ_DATA0_TAG_MASK		0x000000000000007f
#define READ_DATA1_TAG_MASK		0x0000000000007f00
#define READ_DATA2_TAG_MASK		0x00000000007f0000
#define READ_DATA3_TAG_MASK		0x000000007f000000
#define READ_DATA4_TAG_MASK		0x0000007f00000000
#define READ_DATA5_TAG_MASK		0x00007f0000000000
#define READ_DATA6_TAG_MASK		0x007f000000000000
#define READ_DATA7_TAG_MASK		0x7f00000000000000
#define READ_DATA0_TAG_BIT 		0
#define READ_DATA1_TAG_BIT 		8
#define READ_DATA2_TAG_BIT 		16
#define READ_DATA3_TAG_BIT 		24
#define READ_DATA4_TAG_BIT 		32
#define READ_DATA5_TAG_BIT 		40
#define READ_DATA6_TAG_BIT 		48
#define READ_DATA7_TAG_BIT 		56

/* max waiting time*/
#define MAX_WR_DATA_REQ_WAIT_CNT 	100
#define MAX_READ_STATUS_WAIT_CNT 	100
#define MAX_WRITE_STATUS_WAIT_CNT 	10000
#define MAX_ERASE_STATUS_WAIT_CNT 	100000
#define MAX_CMD_RDY_WAIT_CNT 		10000
#define MAX_WR_DATA_READY_CNT 		10000
#define MAX_RESULT_RDY_WAIT_CNT   10000

#define REG_SIZE			0xA8

int vptr_mmap(u64** vptr, off_t addr); //Mapping registers to the host's memory
int rgstr_offset_map(u64** vptr, u64 offset);
int c2c_init(void);	//opening memory device as a file descriptor to use them with mmap/msync
int c2c_terminate(void);
void CTC_Out(u64* vptr, u64 data);
u64 CTC_In(u64* vptr);
u64 allocate_tag(Request *request);
u64 generate_command(Request *request);

int read_page(u64 bus, u64 chip, u64 block, u64 page, u64* pReadBuf_upper, u64* pReadBuf_lower);
int write_page(u64 bus, u64 chip, u64 block, u64 page, u64* pWriteBuf_upper, u64* pWriteBuf_lower);
int erase_block(u64 bus, u64 chip, u64 block);


int wait_cmd_ready(void);
int wait_wrData_ready(void);

int wait_result_ready(void);
int wait_writeData_req(u64* requested_tag);
int wait_flash_operation(u64 op, u64 tag, int* Qnumber, u64* ack,u64* ack_tag);

int send_command(Request *request);
int receive_result(Op_result *result);
