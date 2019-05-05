#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include "../../Includes/address_map_arm.h"
#include "../../Includes/interrupt_ID.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Altera University Program");
MODULE_DESCRIPTION("DE1SoC Timer Handler");

void * LW_virtual;         	// Lightweight bridge base address

volatile int *SEG1_ptr;		//Pointer for HEX3_HEX0
volatile int *SEG2_ptr;		//Pointer for HEX5_Hex4

volatile int *TIMER_Status;
volatile int *TIMER_Control;
volatile int *TIMER_Start_Low;
volatile int *TIMER_Start_High;

int dd = 0;//milliseconds
int ss = 0;//seconds
int mm = 0;//minutes

int DDHex;//coded Milliseconds
int SSHex;//coded Seconds
int MMHex;//coded Minutes

int setDigit(int);
 
irq_handler_t irq_handler(int irq, void *dev_id, struct pt_regs *regs){
	dd++; // every timer interrupt, increment the milliseconds
	
	DDHex = (setDigit((dd/10)%10) << 8) + setDigit(dd%10); // Set the first and second digit of each value
	SSHex = (setDigit((ss/10)%10) << 8) + setDigit(ss%10); // Splits the timing value into tens place and hundreds place, converts, and shifts accordingly
	MMHex = (setDigit((mm/10)%10) << 8) + setDigit(mm%10);
	
	*SEG1_ptr = (SSHex <<  16) + DDHex;
	*SEG2_ptr = MMHex;
	
	if(dd >= 99){
		dd = 0;
		ss++;
	}
	if(ss >= 59){
		ss = 0;
		mm++;
	}
	if(mm >= 59){
		mm = 0;
	}


   // Clear the edgecapture register (clears current interrupt)
   *TIMER_Status = 0x00;//reset the timer so it can receive another interrupt
   return (irq_handler_t) IRQ_HANDLED;
}

static int __init initialize_timer_handler(void){
   int value;
   // generate a virtual address for the FPGA lightweight bridge
   LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

   SEG1_ptr = LW_virtual + HEX3_HEX0_BASE; // init virtual address for HEX3 - HEX0
   SEG2_ptr = LW_virtual + HEX5_HEX4_BASE;

   *SEG1_ptr = 0x3F3F3F3F;
   *SEG2_ptr = 0x3F3F;
   
   TIMER_Status     = LW_virtual + TIMER_BASE;
   TIMER_Control    = LW_virtual + TIMER_BASE + 0x04; 
   TIMER_Start_Low  = LW_virtual + TIMER_BASE + 0x08;
   TIMER_Start_High = LW_virtual + TIMER_BASE + 0x0C;
  
   // *TIMER_Status = 0x00;
   // *TIMER_Control = 0x7;//set Start bit to HIGH, set Cont bit to HIGH, set ITO bit to HIGH 
   //set the timer start value to be 0x000F4240 or 1000000. Which converts to 1 millisecond when combined with the 100Mhz clock 
   *TIMER_Start_High = 0x000F;
   *TIMER_Start_Low = 0x4240;
   
   *TIMER_Control    = 0x07;//set control bits to on, continue, and interrupt
   *TIMER_Status     = 0x00;//clear TO and running bit
         

   // Register the interrupt handler.
   value = request_irq (INTERVAL_TIMER_IRQi, (irq_handler_t) irq_handler, IRQF_SHARED, "timer_irq_handler", (void *) (irq_handler));
   return value;
}

static void __exit cleanup_timer_handler(void){
   *SEG1_ptr 	     = 0x00;  // Turn off Segments
   *SEG2_ptr 	     = 0x00;

   free_irq (INTERVAL_TIMER_IRQi, (void*) irq_handler);
}

int setDigit(int input){
	int segment[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	return segment[input];
}

module_init(initialize_timer_handler);
module_exit(cleanup_timer_handler);

