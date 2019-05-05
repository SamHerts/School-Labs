#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
// === FPGA side ===
#define HW_REGS_BASE ( 0xff200000 )
#define HW_REGS_SPAN ( 0x00200000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define PIO_LED_BASE 0x00
#define PIO_LED_DATA_WIDTH 10
#define ALT_LWFPGASLVS_OFST ( 0xff200000 )



int main(void)
{
	int fd;
	void* virtual_base;
	void* h2p_lw_led_addr;

	if((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1){
		printf("ERROR: Could not open \"/dev/mem\"...\n");
		return(1);
	}

	virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);

	if(virtual_base == MAP_FAILED)
	{
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return(1);
	}

	h2p_lw_led_addr = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + PIO_LED_BASE) & (unsigned long)(HW_REGS_MASK));

	int loop_count = 0, led_mask = 0x01, led_direction = 0;
	while(loop_count < 60)
	{
		*(volatile unsigned int *)h2p_lw_led_addr = ~led_mask;
		usleep(100*1000);
		if(led_direction == 0)
		{
			led_mask <<=1;
			if (led_mask == (0x01 << (PIO_LED_DATA_WIDTH -1)))
			{
				led_direction = 1;
			}
		}
		else
		{
			led_mask >>= 1;
			if(led_mask == 0x01)
			{
				led_direction = 0;
				loop_count++;
			}
		}
	}


	close(fd);
	return 0;
}
