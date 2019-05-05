#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GCD_A_REG      0
#define GCD_B_REG      1
#define GCD_START_REG  2
#define GCD_STATUS_REG 3
#define GCD_R_REG      4
#define GCD_CNT_FLD    0x0000FFFF
#define GCD_READY_BIT  0x80000000

#define RAND_MAX 100

#define HW_REGS_BASE ( 0xFF200000 )
#define HW_REGS_SPAN ( 0x00200000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define PIO_LED_BASE 0x00
#define GCD_BASE 0x30000
#define GCD_SPAN 0x1F

#define A *(GCD_ptr + GCD_A_REG)
#define B *(GCD_ptr + GCD_B_REG)
#define R *(GCD_ptr + GCD_R_REG)

int main(void)
{
	int fd;
	void* virtual_base;
	volatile unsigned int * GCD_ptr;

	if((fd = open("/dev/mem", ( O_RDWR | O_SYNC))) == -1)
	{
		printf("ERROR: COuld not open \"/dev/mem\"...\n");
		return(1);
	}

	virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);

	if(virtual_base == MAP_FAILED)
	{
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return(1);
	}
	GCD_ptr = virtual_base + ((unsigned int)(GCD_BASE));
	int alpha, beta, result;
	for(;;){
		alpha = (rand() % RAND_MAX) + 1;
		beta = (rand() % RAND_MAX) + 1;
		A = alpha;
		B = beta;

		*(GCD_ptr + GCD_START_REG) = 0x1;
		while((*(GCD_ptr + GCD_STATUS_REG) & GCD_READY_BIT) == 0);
		result = R;
		printf("A Register: %d, B Register: %d\nResult: %d\n", alpha, beta, result);
	}
}
