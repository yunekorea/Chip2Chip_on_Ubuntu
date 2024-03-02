#include "main.h"

int main(void){
	u64 readData_upper_arr[520] = {0,};
	u64 readData_lower_arr[520] = {0,};
	/*u64 writeBuf_upper_arr[520];
	u64 writeBuf_lower_arr[520];*/
	u64 writeData_upper = 0;
	u64 writeData_lower = 100;
	u64 bus = 0;
	u64 chip = 0;
	u64 block = 1;
	u64 page = 4;

	printf("initialization phase : ");
	if(c2c_init() == -1)
		return(printf("failed\n"));
	else
		printf("done\n");

	printf("program start \r\n\r\n");
///////////////////////////////////////////////////
	/*for(int j=0; j<514; j++){
		writeBuf_upper_arr[j] = j;
		writeBuf_lower_arr[j] = j+100;
	}*/
/*	printf("first: read page start\r\n");
	if(read_page(bus, chip, block, page, readData_upper_arr, readData_lower_arr) != 0)
		printf("Error: read_page function return -1\r\n");
	for(int j = 0; j < 514; j++){
		printf("[line %d] upper data: 0x%016llx, ", j, (u64)readData_upper_arr[j]);
		printf("lower data: 0x%016llx\r\n ", (u64)readData_lower_arr[j]);
	}
	printf("\r\n\r\n");
*/




	printf("second: erase block start\r\n");
	if(erase_block(bus, chip, block) != 0){
		printf("Error: erase_block function return -1\r\n");
	}
	//printf("wne_status_register : %llx\n", CTC_In(rgstr_vptr.wne_stat));
	printf("read page start\r\n");
	if(read_page(bus, chip, block, page, readData_upper_arr, readData_lower_arr) != 0)
		printf("Error: read_page function return -1\r\n");
	for(int j = 0; j < 514; j++){
		printf("[line %d] upper data: 0x%016llx, ", j, (u64)readData_upper_arr[j]);
		printf("lower data: 0x%016llx\r\n ", (u64)readData_lower_arr[j]);
	}
	printf("\r\n\r\n");



/*
	printf("third: write page start\r\n");
	if(write_page(bus, chip, block, page, &writeData_upper, &writeData_lower) != 0)
		printf("Error: write_page function return -1\r\n");
	printf("read page start\r\n");
	if(read_page(bus, chip, block, page, readData_upper_arr, readData_lower_arr) != 0)
		printf("Error: read_page function return -1\r\n");
	for(int j = 0; j < 514; j++){
		printf("[line %d] upper data: 0x%016llx, ", j, (u64)readData_upper_arr[j]);
		printf("lower data: 0x%016llx\r\n ", (u64)readData_lower_arr[j]);
	}

	printf("fourth: erase block start\r\n");
	if(erase_block(bus, chip, block) != 0){
		printf("Error: erase_block function return -1\r\n");
	}
	printf("read page start\r\n");
	if(read_page(bus, chip, block, page, readData_upper_arr, readData_lower_arr) != 0)
		printf("Error: read_page function return -1\r\n");
	for(int j = 0; j < 514; j++){
		printf("[line %d] upper data: 0x%016llx, ", j, (u64)readData_upper_arr[j]);
		printf("lower data: 0x%016llx\r\n ", (u64)readData_lower_arr[j]);
	}
	printf("\r\n\r\n");
*/	
	printf("termination process : ");
	if(c2c_terminate() == 0)
		return printf("done\n");
	else
		return printf("failed\n");
		
}

void vptr_mmap(u64** vptr, off_t addr) { //Mapping registers to the host's memory
	return (*vptr = (u64 *)mmap(NULL, REG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_memory, addr));
}

void rgstr_offset_map(u64** vptr, u64 offset) {
	*vptr = rgstr_vptr.cmd + offset/sizeof(u64*);
}

int c2c_init(void) {	//opening memory device as a file descriptor to use them with mmap/msync
	
	if((fd_memory = open("/dev/mem", O_RDWR | O_SYNC)) != -1) { //"open" the memory device
		//assign vptr with mmap function
		
		vptr_mmap(&rgstr_vptr.cmd, C2C_CMD_ADDR);
		
		rgstr_offset_map(&rgstr_vptr.read_stat, READ_STATUS_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_0u, READ_DATA0_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_0l, READ_DATA0_LOWER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_1u, READ_DATA1_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_1l, READ_DATA1_LOWER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_2u, READ_DATA2_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_2l, READ_DATA2_LOWER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_3u, READ_DATA3_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_3l, READ_DATA3_LOWER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_4u, READ_DATA4_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_4l, READ_DATA4_LOWER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_5u, READ_DATA5_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_5l, READ_DATA5_LOWER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_6u, READ_DATA6_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_6l, READ_DATA6_LOWER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_7u, READ_DATA7_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.read_data_7l, READ_DATA7_LOWER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.wne_stat, WRITE_ERASE_STATUS_OFFSET);
		rgstr_offset_map(&rgstr_vptr.write_data_u, WRITE_DATA_UPPER_OFFSET);
		rgstr_offset_map(&rgstr_vptr.write_data_l, WRITE_DATA_LOWER_OFFSET);

		return 0;
		
	} else {
		return -1;
	}
}

int c2c_terminate(void) {
  return close(fd_memory);
}

void CTC_Out(u64* vptr, u64 command) {
	*vptr = command;
	msync(vptr, REG_SIZE, MS_SYNC);
}

u64 CTC_In(u64* vptr) {
	return *vptr;
}

u64 CTC_Readq_In_Upper(u64 Qnumber) {
	switch(Qnumber) {
		case 0:
			return CTC_In(rgstr_vptr.read_data_0u);
		case 1:
			return CTC_In(rgstr_vptr.read_data_1u);
		case 2:
			return CTC_In(rgstr_vptr.read_data_2u);
		case 3:
			return CTC_In(rgstr_vptr.read_data_3u);
		case 4:
			return CTC_In(rgstr_vptr.read_data_4u);
		case 5:
			return CTC_In(rgstr_vptr.read_data_5u);
		case 6:
			return CTC_In(rgstr_vptr.read_data_6u);
		case 7:
			return CTC_In(rgstr_vptr.read_data_7u);
	}
}

u64 CTC_Readq_In_Lower(u64 Qnumber) {
	switch(Qnumber) {
		case 0:
			return CTC_In(rgstr_vptr.read_data_0l);
		case 1:
			return CTC_In(rgstr_vptr.read_data_1l);
		case 2:
			return CTC_In(rgstr_vptr.read_data_2l);
		case 3:
			return CTC_In(rgstr_vptr.read_data_3l);
		case 4:
			return CTC_In(rgstr_vptr.read_data_4l);
		case 5:
			return CTC_In(rgstr_vptr.read_data_5l);
		case 6:
			return CTC_In(rgstr_vptr.read_data_6l);
		case 7:
			return CTC_In(rgstr_vptr.read_data_7l);
	}
}

int read_page(u64 bus, u64 chip, u64 block, u64 page, u64* pReadBuf_upper, u64* pReadBuf_lower){
	u64 key = ACCESS_KEY;
	u64 tag = 0;
	u64 op = READ;
	u64 command;
	u64 readData_upper;
	u64 readData_lower;	
	int readyQ_number;
	int i = 0;
	command = (key << KEY_BIT)					\
			|(tag << TAG_BIT)					\
			|(op << OP_BIT)						\
			|(bus << BUS_BIT)					\
			|(chip << CHIP_BIT)					\
			|(block << BLOCK_BIT)				\
			|(page << PAGE_BIT);
	if(wait_cmd_ready() < 0){
		printf("Error: wait command ready signal time out\r\n");
		return -1;
	}
	CTC_Out(rgstr_vptr.cmd, command);
	//Xil_Out64(C2C_CMD_ADDR, command);

	if(wait_flash_operation(op, tag, &readyQ_number, 0, 0) !=0){
		printf("Error: wait read data Q ready time out\r\n");
		return -1;
	}
	for(int i = 0; i<514; i++){
		pReadBuf_upper[i] = CTC_Readq_In_Upper(readyQ_number);
		pReadBuf_lower[i] = CTC_Readq_In_Lower(readyQ_number);
	}

	printf("read page operation was finished\r\n");
	return 0;
}

int write_page(u64 bus, u64 chip, u64 block, u64 page, /*cosnt*/ u64* pWriteBuf_upper, /*cosnt*/ u64* pWriteBuf_lower){
	u64 key = ACCESS_KEY;
	u64 tag = 0;
	u64 op = WRITE;
	u64 command;
	u64 requested_tag;
	u64 writeData_tag =  (VALID<<WR_DATA_TAG_VALID_BIT)|(tag<<WR_DATA_TAG_BIT);
	u64 status_reg_value;
	int ack;
	int ack_tag;
	command = (key << KEY_BIT)					\
			|(tag << TAG_BIT)					\
			|(op << OP_BIT)						\
			|(bus << BUS_BIT)					\
			|(chip << CHIP_BIT)					\
			|(block << BLOCK_BIT)				\
			|(page << PAGE_BIT);
	if(wait_cmd_ready() < 0){
		printf("Error: wait command ready signal time out\r\n");
		return -1;
	}
	CTC_Out(rgstr_vptr.cmd, command);
	//Xil_Out64(C2C_CMD_ADDR, command);
	
	if(wait_writeData_req(&requested_tag)!=0){
		printf("Error: write data request time out\r\n");
		return -1;
	}
	if(requested_tag != tag){
		printf("Error: the tag of the current command and the requested tag are different.\r\n");
		return -1;
	}
	printf("write data requested\r\n");
	if(wait_wrData_ready() < 0){
		printf("Error: wait write data ready signal time out\r\n");
		return -1;
	}
	status_reg_value = CTC_In(rgstr_vptr.wne_stat);
	
	CTC_Out(rgstr_vptr.wne_stat, status_reg_value | writeData_tag);
	
	for(int j=0; j<514; j++){
		CTC_Out(rgstr_vptr.write_data_u, *pWriteBuf_upper);
		
		CTC_Out(rgstr_vptr.write_data_l, *pWriteBuf_lower);
		
		(*pWriteBuf_upper) += 1;//making write data for test
		(*pWriteBuf_lower) += 1;//making write data for test
	}
	CTC_Out(rgstr_vptr.wne_stat, status_reg_value & (~writeData_tag));
	
	if(wait_flash_operation(op, tag, 0, &ack, &ack_tag)!=0){
		printf("Error: wait ack time out\r\n");
		return -1;
	}
	if(ack != ACK_WRITE_DONE){
		printf("Error: ack is not ACK_WRITE_DONE\r\n");
		return -1;
	}
	printf("write page operation was finished\r\n");

	return 0;
}

int erase_block(u64 bus, u64 chip, u64 block){
	u64 key = ACCESS_KEY;
	u64 tag = 0;
	u64 op = ERASE;
	u64 command;
	u64 ack;	//changed the type of ack, ack_tag from int to u64
	u64 ack_tag;
	/* command »ý¼º*/
	command = (key << KEY_BIT)					\
			|(tag << TAG_BIT)					\
			|(op << OP_BIT)						\
			|(bus << BUS_BIT)					\
			|(chip << CHIP_BIT)					\
			|(block << BLOCK_BIT);
	if(wait_cmd_ready() < 0){
		printf("Error: wait command ready signal time out\r\n");
		return -1;
	}
	CTC_Out(rgstr_vptr.cmd, command);
	//Xil_Out64(C2C_CMD_ADDR, command);
	
	if(wait_flash_operation(op, tag, 0, &ack, &ack_tag)!=0){
		printf("Error: wait ack time out\r\n");
		return -1;
	}
	if(ack != ACK_ERASE_DONE){
		printf("Error: ack is not ACK_ERASE_DONE\r\n");
		return -1;
	}
	printf("erase block operation was finished\r\n");

	return 0;
}

int wait_cmd_ready(void){
	int time_cnt = MAX_CMD_RDY_WAIT_CNT;
	while((CTC_In(rgstr_vptr.cmd) & CMD_READY_MASK) == 0){
	//while((Xil_In64(C2C_CMD_ADDR) & CMD_READY_MASK) == 0){
		if(time_cnt == 0)
			return -1;
		time_cnt--;
	}
	return 0;
}

int wait_wrData_ready(void){
	int time_cnt = MAX_WR_DATA_READY_CNT;
	while((CTC_In(rgstr_vptr.cmd) & WR_DATA_READY_MASK) == 0){
	//while((Xil_In64(C2C_CMD_ADDR) & WR_DATA_READY_MASK) == 0){
		if(time_cnt == 0)
			return -1;
		time_cnt--;
	}
	return 0;
}

int wait_writeData_req(u64* requested_tag){
	u64 wr_er_status;
	u64 wr_dataReq_ready;
	u64 wr_dataReq_tag;
	int time_cnt = MAX_WR_DATA_REQ_WAIT_CNT;

	while(time_cnt-- != 0){
		wr_er_status = CTC_In(rgstr_vptr.wne_stat);
		//wr_er_status = Xil_In64(C2C_WRITE_ERASE_STATUS_ADDR);
		
		wr_dataReq_ready = wr_er_status & WR_DATA_REQ_READY_MASK;
		//wr_dataReq_ready = wr_er_status & WR_DATA_REQ_READY_MASK;
		
		wr_dataReq_tag = (wr_er_status & WR_DATA_REQ_TAG_MASK)>>WR_DATA_REQ_TAG_BIT;
		
		if(wr_dataReq_ready > 0){
			CTC_Out(rgstr_vptr.wne_stat, wr_er_status|WR_DATA_REQ_VALID);
			//Xil_Out64(C2C_WRITE_ERASE_STATUS_ADDR, wr_er_status|WR_DATA_REQ_VALID);
			
			CTC_Out(rgstr_vptr.wne_stat, wr_er_status&(~WR_DATA_REQ_VALID));
			//Xil_Out64(C2C_WRITE_ERASE_STATUS_ADDR, wr_er_status&(~WR_DATA_REQ_VALID));
			
			*requested_tag = wr_dataReq_tag;
			break;
		}
	}
	if(time_cnt == 0)
		return -1;
	else
		return 0;
}

int wait_flash_operation(u64 op, u64 tag, int* Qnumber, u64* ack,u64* ack_tag){
	u64 readQ_ready;
	u64 readQ_tag;
	u64 wr_er_status;
	u64 ack_ready;
	int time_cnt;		//limit wait time

	if(op == READ){
		time_cnt = MAX_READ_STATUS_WAIT_CNT;
		while(time_cnt-- != 0){
			readQ_ready = CTC_In(rgstr_vptr.read_stat) & READQ_READY_MASK;
			//readQ_ready = Xil_In64(C2C_READ_STATUS_ADDR) & READQ_READY_MASK;
			
			readQ_tag = CTC_In(rgstr_vptr.read_stat) & READQ_TAG_MASK;
			//readQ_tag = Xil_In64(C2C_READ_STATUS_ADDR) & READQ_TAG_MASK;

			if((readQ_ready&READ_DATA0_READY_VALUE)>0) // if read queue0ÀÇ ready ½ÅÈ£°¡ 1ÀÏ ¶§
				if((readQ_tag & READ_DATA0_TAG_MASK) == (tag<<READ_DATA0_TAG_BIT)) { // and if queue0°¡ °¡Áö°íÀÖ´Â µ¥ÀÌÅÍÀÇ tag°¡ º¸³Â´ø ¸í·ÉÀÇ tag¿Í ÀÏÄ¡ÇÏ¸é
					*Qnumber = 0; // ¸í·É¿¡ ´ëÇÑ read data°¡ ÀúÀåµÈ °÷Àº queue0
					break;
				}
			if((readQ_ready&READ_DATA1_READY_VALUE)>0)
				if((readQ_tag & READ_DATA1_TAG_MASK) == (tag<<READ_DATA1_TAG_BIT)) {// and if queue1°¡ °¡Áö°íÀÖ´Â µ¥ÀÌÅÍÀÇ tag°¡ º¸³Â´ø ¸í·ÉÀÇ tag¿Í ÀÏÄ¡ÇÏ¸é
					*Qnumber = 1;// ¸í·É¿¡ ´ëÇÑ read data°¡ ÀúÀåµÈ °÷Àº queue1
					break;
				}
			if((readQ_ready&READ_DATA2_READY_VALUE)>0)
				if((readQ_tag & READ_DATA2_TAG_MASK) == (tag<<READ_DATA2_TAG_BIT)) {// and if queue2°¡ °¡Áö°íÀÖ´Â µ¥ÀÌÅÍÀÇ tag°¡ º¸³Â´ø ¸í·ÉÀÇ tag¿Í ÀÏÄ¡ÇÏ¸é
					*Qnumber = 2;// ¸í·É¿¡ ´ëÇÑ read data°¡ ÀúÀåµÈ °÷Àº queue2
					break;
				}
			if((readQ_ready&READ_DATA3_READY_VALUE)>0)
				if((readQ_tag & READ_DATA3_TAG_MASK) == (tag<<READ_DATA3_TAG_BIT)) {// and if queue3°¡ °¡Áö°íÀÖ´Â µ¥ÀÌÅÍÀÇ tag°¡ º¸³Â´ø ¸í·ÉÀÇ tag¿Í ÀÏÄ¡ÇÏ¸é
					*Qnumber = 3;// ¸í·É¿¡ ´ëÇÑ read data°¡ ÀúÀåµÈ °÷Àº queue3
					break;
				}
			if((readQ_ready&READ_DATA4_READY_VALUE)>0)
				if((readQ_tag & READ_DATA4_TAG_MASK) == (tag<<READ_DATA4_TAG_BIT)) {// and if queue4°¡ °¡Áö°íÀÖ´Â µ¥ÀÌÅÍÀÇ tag°¡ º¸³Â´ø ¸í·ÉÀÇ tag¿Í ÀÏÄ¡ÇÏ¸é
					*Qnumber = 4;// ¸í·É¿¡ ´ëÇÑ read data°¡ ÀúÀåµÈ °÷Àº queue4
					break;
				}
			if((readQ_ready&READ_DATA5_READY_VALUE)>0)
				if((readQ_tag & READ_DATA5_TAG_MASK) == (tag<<READ_DATA5_TAG_BIT)) {// and if queue5°¡ °¡Áö°íÀÖ´Â µ¥ÀÌÅÍÀÇ tag°¡ º¸³Â´ø ¸í·ÉÀÇ tag¿Í ÀÏÄ¡ÇÏ¸é
					*Qnumber = 5;// ¸í·É¿¡ ´ëÇÑ read data°¡ ÀúÀåµÈ °÷Àº queue5
					break;
				}
			if((readQ_ready&READ_DATA6_READY_VALUE)>0)
				if((readQ_tag & READ_DATA6_TAG_MASK) == (tag<<READ_DATA6_TAG_BIT)) {// and if queue6°¡ °¡Áö°íÀÖ´Â µ¥ÀÌÅÍÀÇ tag°¡ º¸³Â´ø ¸í·ÉÀÇ tag¿Í ÀÏÄ¡ÇÏ¸é
					*Qnumber = 6;// ¸í·É¿¡ ´ëÇÑ read data°¡ ÀúÀåµÈ °÷Àº queue6
					break;
				}
			if((readQ_ready&READ_DATA7_READY_VALUE)>0)
				if((readQ_tag & READ_DATA7_TAG_MASK) == (tag<<READ_DATA7_TAG_BIT)) {// and if queue7°¡ °¡Áö°íÀÖ´Â µ¥ÀÌÅÍÀÇ tag°¡ º¸³Â´ø ¸í·ÉÀÇ tag¿Í ÀÏÄ¡ÇÏ¸é
					*Qnumber = 7;// ¸í·É¿¡ ´ëÇÑ read data°¡ ÀúÀåµÈ °÷Àº queue7
					break;
				}
		}// end while
	}// end if

	else if((op == WRITE)||(op == ERASE)){
		if(op == WRITE)
			/*Å¸ÀÓ Ä«¿îÆ® °ªÀ» ¹Ì¸® define ÇØ³õÀº write µ¿ÀÛÀÇ ÃÖ´ë ½Ã°£ °ªÀ¸·Î ¾÷µ¥ÀÌÆ®*/
			time_cnt = MAX_WRITE_STATUS_WAIT_CNT;
		else if(op == ERASE)
			/*Å¸ÀÓ Ä«¿îÆ® °ªÀ» ¹Ì¸® define ÇØ³õÀº erase µ¿ÀÛÀÇ ÃÖ´ë ½Ã°£ °ªÀ¸·Î ¾÷µ¥ÀÌÆ®*/
			time_cnt = MAX_ERASE_STATUS_WAIT_CNT;
		/*loop¸¦ ¹Ýº¹ÇÒ ¶§ ¸¶´Ù Å¸ÀÓ Ä«¿îÆ® °ªÀ» °¨¼Ò*/
		/*Å¸ÀÓ Ä«¿îÆ® °ª==0 ÀÌ¸é ¹Ýº¹¹® break*/
		while(time_cnt-- != 0){
			/* write & erase status ·¹Áö½ºÅÍÀÇ 64-bit ¸ðµÎ ÀÐ¾î¿È */
			wr_er_status = CTC_In(rgstr_vptr.wne_stat);
			//wr_er_status = Xil_In64(C2C_WRITE_ERASE_STATUS_ADDR);
			/* write & erase status ·¹Áö½ºÅÍ°ª¿¡¼­ ack ready ½ÅÈ£¸¸ ÃßÃâ */
			ack_ready = wr_er_status & WR_ER_ACK_READY_MASK;

			if(ack_ready > 0) {	//ack ready°¡ high ¶ó¸é if¹® ½ÇÇà -> while¹® break
				CTC_Out(rgstr_vptr.wne_stat, wr_er_status|WR_ER_ACK_VALID);
				//Xil_Out64(C2C_WRITE_ERASE_STATUS_ADDR, wr_er_status|WR_ER_ACK_VALID); //write & erase status ·¹Áö½ºÅÍÀÇ ack valid ÀÚ¸®¸¸ 1·Î ¸¸µê
				CTC_Out(rgstr_vptr.wne_stat, wr_er_status&(~WR_ER_ACK_VALID));
				//Xil_Out64(C2C_WRITE_ERASE_STATUS_ADDR, wr_er_status&(~WR_ER_ACK_VALID)); // write & erase status ·¹Áö½ºÅÍÀÇ ack valid ÀÚ¸®¸¸ 0À¸·Î ¸¸µê
				*ack = ((wr_er_status & ACK_CODE_MASK)>>ACK_BIT);  // µ¿ÀÛ Á¾·á °á°ú 2-bit °¡Á®¿Í¼­ Æ÷ÀÎÅÍ¿¡ ÀúÀå
				*ack_tag = ((wr_er_status & ACK_TAG_MASK)>>ACK_TAG_BIT); //ack¿¡ Æ÷ÇÔµÈ tag¸¦ °¡Á®¿Í¼­ Æ÷ÀÎÅÍ¿¡ ÀúÀå
				break;
			}
		}// end while
	}// end else if

	if(time_cnt == 0)
		return -1;
	else
		return 0;
}
