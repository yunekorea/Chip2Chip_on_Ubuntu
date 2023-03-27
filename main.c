#include "main.h"

int main(void){
	u64 readData_upper_arr[520] = {0,};
	u64 readData_lower_arr[520] = {0,};
	/*u64 writeBuf_upper_arr[520];
	u64 writeBuf_lower_arr[520];*/
	u64 wireData_upper = 0;
	u64 wireData_lower = 100;
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
	printf("first: read page start\r\n");
	if(read_page(bus, chip, block, page, readData_upper_arr, readData_lower_arr) != 0)
		printf("Error: read_page function return -1\r\n");
	for(int j = 0; j < 514; j++){
		printf("[line %d] upper data: 0x%08x, ", j, (int)readData_upper_arr[j]);
		printf("lower data: 0x%08x\r\n ", (int)readData_lower_arr[j]);
	}
	printf("\r\n\r\n");





	printf("second: erase block start\r\n");
	if(erase_block(bus, chip, block) != 0){
		printf("Error: erase_block function return -1\r\n");
	}
	printf("read page start\r\n");
	if(read_page(bus, chip, block, page, readData_upper_arr, readData_lower_arr) != 0)
		printf("Error: read_page function return -1\r\n");
	for(int j = 0; j < 514; j++){
		printf("[line %d] upper data: 0x%08x, ", j, (int)readData_upper_arr[j]);
		printf("lower data: 0x%08x\r\n ", (int)readData_lower_arr[j]);
	}
	printf("\r\n\r\n");




	printf("third: write page start\r\n");
	if(write_page(bus, chip, block, page, &wireData_upper, &wireData_lower) != 0)
		printf("Error: write_page function return -1\r\n");
	printf("read page start\r\n");
	if(read_page(bus, chip, block, page, readData_upper_arr, readData_lower_arr) != 0)
		printf("Error: read_page function return -1\r\n");
	for(int j = 0; j < 514; j++){
		printf("[line %d] upper data: 0x%08x, ", j, (int)readData_upper_arr[j]);
		printf("lower data: 0x%08x\r\n ", (int)readData_lower_arr[j]);
	}

	printf("fourth: erase block start\r\n");
	if(erase_block(bus, chip, block) != 0){
		printf("Error: erase_block function return -1\r\n");
	}
	printf("read page start\r\n");
	if(read_page(bus, chip, block, page, readData_upper_arr, readData_lower_arr) != 0)
		printf("Error: read_page function return -1\r\n");
	for(int j = 0; j < 514; j++){
		printf("[line %d] upper data: 0x%08x, ", j, (int)readData_upper_arr[j]);
		printf("lower data: 0x%08x\r\n ", (int)readData_lower_arr[j]);
	}
	printf("\r\n\r\n");
	
	printf("termination process : ");
	if(c2c_terminate() == 0)
		return printf("done\n");
	else
		return printf("failed\n");
		
}


