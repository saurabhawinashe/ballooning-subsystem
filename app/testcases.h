/* Use 4GB memory */
#include <unistd.h>
#define OFFSET 26 //this value will be provided during the evaluation
// F(x) simply computes a number based on x that will be 
// used to check for content matching later
#define F(x) ((x+OFFSET)%128)
#define TOTAL_MEMORY_SIZE	(1UL * 1024 * 1024 * 1024)


/*
 * Test-case 1
 */
long test_case_1(int *ptr, long len)
{
	long i, sum = 0;
	int iter, tmp;

	for (iter = 0; iter < 20; iter++) {
		for (i = 0; i < len/2; i++) {
			ptr[i] = ptr[i] + 1;
		}
		printf("iter : %d\n", iter);
	}
	return sum;
}

/*
     * test case to check whether demand paging works correctly
     * and the contents of the buffer are the same as expected
     */
    void test_case_content_check(char *ptr, unsigned long size)
    {
        unsigned long run_idx=1;
        printf("Starting content checking test...\n");
        sleep(2);
        while(1){
            for(unsigned long i=0; i<size; i++){
                if(ptr[i] != F(i)){
                    printf("run_idx %lu \t: ERROR: Content check failed at i= %lu\n", run_idx, i);
                    exit(-1);    
                }
            }
            printf("run_idx %lu \t: Completed\n", run_idx);
            run_idx++;
            sleep(1);
        }
    }

/*
 * main entry point for testing use-cases.
 */
void test_case_main(int *ptr, unsigned long size)
{
	long len;

	len = size / sizeof(int);
	test_case_1(ptr, len);
}
