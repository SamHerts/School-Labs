#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include "../../Includes/address_map_arm.h"

/* Prototypes for functions used to access physical memory addresses */
int open_physical(int);
void * map_physical(int, unsigned int, unsigned int);
void close_physical(int);
int unmap_physical(void *, unsigned int);

void msSleep(int); //turns nanosleep into millisecond sleep

/* This program increments the contents of the red LED parallel port */
int main(void)
{
	volatile int * LEDR_ptr; // virtual address pointer to red LEDs
	volatile int * SEG1_ptr;
	volatile int * SEG2_ptr;
	
	// Intel SoC FPGA
	volatile int sentence[15] = { 0x04, 0x54, 0x78, 0x79, 0x38, 0x00, 0x6D, 0x5C, 0x39, 0x00, 0x71, 0x73, 0x7D, 0x77, 0x00};
   	
	int fd = -1; // used to open /dev/mem
	void *LW_virtual; // physical addresses for light-weight bridge

			// Create virtual memory access to the FPGA light-weight bridge
	if ((fd = open_physical(fd)) == -1)
		return (-1);
	if ((LW_virtual = map_physical(fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) ==
		NULL)
		return (-1);

	// Set virtual address pointer to I/O port
	LEDR_ptr = (unsigned int *)(LW_virtual + LEDR_BASE);
	SEG1_ptr = (unsigned int *)(LW_virtual + HEX3_HEX0_BASE);
	SEG2_ptr = (unsigned int *)(LW_virtual + HEX5_HEX4_BASE);
	// Bounce LED's back and forth
	int delay = 300;
	int count = 0;
	int save;
	*LEDR_ptr = 0x01;
	*SEG1_ptr = 0;
	*SEG2_ptr = 0;
	while (1) {
		save = *SEG1_ptr;//save the current value of the first 4 segments
		*SEG1_ptr = *SEG1_ptr << 8;//move the first 4 over by 1
		*SEG1_ptr = *SEG1_ptr + sentence[count]; // add the next letter to first 4
		
		*SEG2_ptr = *SEG2_ptr << 8;//move second 2 segments over by 1
		*SEG2_ptr = *SEG2_ptr + (save >> (8*3) ); // add the last letter of first
		
		msSleep(delay);//wait
		count++; // increment
		
		*LEDR_ptr = *LEDR_ptr << 1; // move led showing number of steps
		
		if(count > 14){
			count = 0;
			*LEDR_ptr = 1;
		}
	}

	unmap_physical(LW_virtual, LW_BRIDGE_SPAN);
	close_physical(fd);
	return 0;
}
/* Open /dev/mem to give access to physical addresses */
int open_physical(int fd)
{
	if (fd == -1) // check if already open
		if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
		{
			printf("ERROR: could not open \"/dev/mem\"...\n");
			return (-1);
		}
	return fd;
}

/* Close /dev/mem to give access to physical addresses */
void close_physical(int fd)
{
	close(fd);
}

/*
* Establish a virtual address mapping for the physical addresses starting
* at base, and extending by span bytes */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
	void *virtual_base;

	// Get a mapping from physical addresses to virtual addresses
	virtual_base = mmap(NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED,
		fd, base);
	if (virtual_base == MAP_FAILED)
	{
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return (NULL);
	}
	return virtual_base;
}

/* Close the previously-opened virtual address mapping */
int unmap_physical(void * virtual_base, unsigned int span)
{
	if (munmap(virtual_base, span) != 0)
	{
		printf("ERROR: munmap() failed...\n");
		return (-1);
	}
	return 0;
}
//Converts Given MilliSeconds into nanoseconds -- i must be less than 1000;
void msSleep(int i){
	struct timespec req, rem; //define two timespect structs
	req.tv_sec = 0; //set the Seconds value to zero
	req.tv_nsec = i * 1000000; // set the nanosecond value to milli*nano
	nanosleep(&req, &rem);
}
