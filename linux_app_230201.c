#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Make the SDK console work in the debugger
#define printf(...) \
 fprintf(stdout, __VA_ARGS__); \
 fflush(stdout);
#define OP_DONE_MASK 0x0002000000000000

typedef long long int u64;

int main()
{

   unsigned int bram_size = 0x800;
   unsigned int cmdStat_reg_size = 0x8;
   off_t bram_pbase = 0xA0000000; // physical base address
   off_t cmdStat_reg_pbase = 0xA0080000; // physical base address
   u64 *bram64_vptr;
   u64 *cmdStat_reg_vptr;
   int fd_timeResult_BRAM;
   int fd_cmdStat;

   printf("program start\r\n");

   if((fd_cmdStat = open("/dev/mem", O_RDWR | O_SYNC)) != -1){
	  printf("fd_cmdStat: file open success\r\n");
	  cmdStat_reg_vptr = (u64 *)mmap(NULL, cmdStat_reg_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd_cmdStat, cmdStat_reg_pbase);
	  printf("cmdStat_reg mmap finished\r\n");
	  cmdStat_reg_vptr[0] = 0x79680000ffe00000;// reset
     msync(cmdStat_reg_vptr, cmdStat_reg_size,MS_SYNC);
	  cmdStat_reg_vptr[0] = 0x79680000ffe00001;// send read command
     msync(cmdStat_reg_vptr, cmdStat_reg_size,MS_SYNC);
     /*read pages from block 0 page 0 to block 15 page 127*/

     while((*cmdStat_reg_vptr & OP_DONE_MASK) == 0); // wait flash operation
	  close(fd_cmdStat);
   }
   else{
	   printf("fd_cmdStat: mem open fail\r\n");
   }

   printf("\r\nstart bram mmap\r\n");

   if ((fd_timeResult_BRAM = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {
	  printf("fd_timeResult_BRAM: file open success\r\n");

      bram64_vptr = (u64 *)mmap(NULL, bram_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd_timeResult_BRAM, bram_pbase);
      printf("bram mmap finished\r\n");
      
      /*Print the operation time which stored in BRAM*/
      for(int i=0; i <0x100; i++){
         printf("%uns\r\n", bram64_vptr[i]*10);
      }
      close(fd_timeResult_BRAM);
   }
   else{
	   printf("fd_timeResult_BRAM: mem open fail\r\n");
   }

   printf("normal termination\r\n");
 }
