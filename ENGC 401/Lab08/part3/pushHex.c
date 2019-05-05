#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include "../../Includes/address_map_arm.h"
#include "../../Includes/interrupt_ID.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Altera University Program");
MODULE_DESCRIPTION("DE1SoC Pushbutton Interrupt Handler");

void * LW_virtual;         // Lightweight bridge base address
volatile int *LEDR_ptr;    // virtual address for the LEDR port
volatile int *KEY_ptr;     // virtual address for the KEY port
volatile int *SEG_ptr;

int count;

int setDigit(int);
int setSegment(int);

irq_handler_t irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
   *LEDR_ptr = *LEDR_ptr + 1;
   
    if(*LEDR_ptr > 521){//make sure left most LED is always on
  	*LEDR_ptr = 0x200;
   }
  count = count + 1;
  *SEG_ptr =  setSegment(count);
  if(count > 511){
  	count = 0;
  }

   // Clear the edgecapture register (clears current interrupt)
   *(KEY_ptr + 3) = 0xF; 
    
   return (irq_handler_t) IRQ_HANDLED;
}

static int __init initialize_pushbutton_handler(void)
{
   int value;
   count = 0;//set count to zero to start
   // generate a virtual address for the FPGA lightweight bridge
   LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

   LEDR_ptr = LW_virtual + LEDR_BASE;  // init virtual address for LEDR port
   *LEDR_ptr = 0x200;                  // turn on the leftmost light

   SEG_ptr = LW_virtual + HEX3_HEX0_BASE;
   setSegment(count);

   KEY_ptr = LW_virtual + KEY_BASE;    // init virtual address for KEY port
   // Clear the PIO edgecapture register (clear any pending interrupt)
   *(KEY_ptr + 3) = 0xF;
   // Enable IRQ generation for the 4 buttons
   *(KEY_ptr + 2) = 0xF;

   // Register the interrupt handler.
   value = request_irq (KEYS_IRQ, (irq_handler_t) irq_handler, IRQF_SHARED,
      "pushbutton_irq_handler", (void *) (irq_handler));
   return value;
}

static void __exit cleanup_pushbutton_handler(void)
{
  *LEDR_ptr = 0; // Turn off LEDs and de-register irq handler
  *SEG_ptr = 0; 
  free_irq (KEYS_IRQ, (void*) irq_handler);
}

int  setDigit(int value){
	int segment[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	return segment[value];
}

int setSegment(int value){
	int thousands = setDigit((value/1000)%10) * 0x1000000;
	int hundreds = setDigit((value/100)%10) * 0x10000;
	int tens = setDigit((value/10)%10) * 0x00100;
	int ones = setDigit(value%10);
	return ones;
}

module_init(initialize_pushbutton_handler);
module_exit(cleanup_pushbutton_handler);

