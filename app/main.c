#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>

#include "testcases.h"

void *buff;
unsigned long nr_signals = 0;
int flag = 0;
long *swap_out;

#define PAGE_SIZE		(4096)

/*
 * 			placeholder-3
 * implement your page replacement policy here
 */
void page_replace(int nr_signals)
{
	for(int i=0;i<16384;i++)
		swap_out[i] = (nr_signals-1)*16384 + i;
}


/*
 * 			placeholder-2
 * implement your signal handler here
 */
 uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}
 
void sig_handler(int signum)
{
	int a;
	nr_signals+=1;
	page_replace(nr_signals);
	a = syscall(444, buff, swap_out);
	//2^18 pages to be replaced
	
}


int main(int argc, char *argv[])
{
	int *ptr, nr_pages;
	long *to_replace;
	
    	ptr = mmap(NULL, TOTAL_MEMORY_SIZE, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);

	if (ptr == MAP_FAILED) {
		printf("mmap failed\n");
       		exit(1);
	}
	buff = ptr;
	//memset(buff, 0, TOTAL_MEMORY_SIZE);
	//ptr[0] = 5;
	
	for(unsigned long long i=0; i<TOTAL_MEMORY_SIZE; i++){
        ((char*)buff)[i] = F(i);
	}
	
	to_replace = mmap(NULL, 16384*sizeof(long), PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
			
	if (to_replace == MAP_FAILED) {
		printf("mmap failed\n");
       		exit(1);
	}
	
	swap_out = to_replace;
	for(int i=0;i<16384;i++)
		to_replace[i] = -1;
	/*
	 * 		placeholder-1
	 * register me with the kernel ballooning subsystem
	 */
	 
	 signal(44, sig_handler);
	 printf("Sig handler done\n");
	 
	 unsigned long long start = rdtsc();
	 int a = syscall(443);
	 unsigned long long end = rdtsc();
	 
	 printf("Number of clock ticks for system call : %llu\n", end-start);

	/* test-case */
	//test_case_main(buff, TOTAL_MEMORY_SIZE);
	test_case_content_check(buff, TOTAL_MEMORY_SIZE);
	
	munmap(ptr, TOTAL_MEMORY_SIZE);
	printf("I received SIGBALLOON %lu times\n", nr_signals);
	
	char pid_a[10];
	sprintf(pid_a, "%d", getpid());
	char filename[50];
	strcpy(filename, "/ballooning/swapfile_"); 
	remove(strcat(filename, pid_a));
}
