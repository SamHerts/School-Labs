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

volatile int *KEY_ptr;		//Pointer for the KEYS
volatile int *SW_ptr;

volatile int *TIMER_Status;
volatile int *TIMER_Control;
volatile int *TIMER_Start_Low;
volatile int *TIMER_Start_High;

int dd = 99;
int ss = 59;
int mm = 59;

int setDigit(int);
void refreshDisplay(void);

//timer IRQ handler
irq_handler_t irq_handler(int irq, void *dev_id, struct pt_regs *regs){

	if( dd+ss+mm <= 0){//if timer reaches zero
		dd = 0;
		ss = 0;
		mm = 0;
		*TIMER_Control ^= 0xC;//set timer to stop
		*TIMER_Status = 0x00;//clear status
	}else{

		if(dd <= 0){ //milliseconds counts all the way down, and if there is time left, reset milliseconds
			dd = 99;
			ss--;
		}
		if(ss <= 0){
			if(mm > 0){
				ss = 59;
				mm--;
			}else{
				ss = 0;
			}
		}
	dd--;
	}

	refreshDisplay();
	// Clear the edge capture register (clears current interrupt)
   	*TIMER_Status = 0x00;
   	return (irq_handler_t) IRQ_HANDLED;
}

irq_handler_t irq_handler_pushbutton(int irq, void *dev_id, struct pt_regs *regs){
	switch(*(KEY_ptr + 3)){
	case 0x01:
		if(dd+ss+mm > 0){//if there is time on the clock stop or start the clock.
			*TIMER_Status = 0x00;
			*TIMER_Control ^= 0xC; // switch from on to off using bitwise XOR'ing to flip on the 3rd and 4th bit.
		}
		break;
	case 0x02://increment milliseconds
		dd = *SW_ptr;
		if(dd > 99){
			dd = 99;
		}
		//dd++;
		//if(dd > 99){
		//	dd = 0;
		//}
		break;
	case 0x04://increment seconds
		//ss++;
		ss = *SW_ptr;
		if(ss > 59){
			ss = 59;
		}
		break;
	case 0x08://increment minutes
		//mm++;
		mm = *SW_ptr;
		if(mm > 59){
			mm = 59;
		}
		break;
	}
	refreshDisplay();


   // Clear the edgecapture register (clears current interrupt)
   *(KEY_ptr + 3) = 0xF;
   return (irq_handler_t) IRQ_HANDLED;
}


static int __init initialize_all_handler(void){
   int timer_interrupt_value;
   int key_interrupt_value;
   // generate a virtual address for the FPGA lightweight bridge
   LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

   SEG1_ptr = LW_virtual + HEX3_HEX0_BASE;//base address for 7Seg display
   SEG2_ptr = LW_virtual + HEX5_HEX4_BASE;
   SW_ptr = LW_virtual + SW_BASE;

   refreshDisplay();//set display values

   TIMER_Status     = LW_virtual + TIMER_BASE;
   TIMER_Control    = LW_virtual + TIMER_BASE + 0x04;
   TIMER_Start_Low  = LW_virtual + TIMER_BASE + 0x08;
   TIMER_Start_High = LW_virtual + TIMER_BASE + 0x0C;

   *TIMER_Start_High = 0x000F;//use 16 in the upper 16 bits to get 1 millisecond interrupts
   *TIMER_Control    = 0x7;//set on, set continue, and set interrupt
   *TIMER_Status     = 0x00;//clear status just in case


   KEY_ptr = LW_virtual + KEY_BASE;
   // Clear the PIO edgecapture register (clear any pending interrupt)
   *(KEY_ptr + 3) = 0xF;
   // Enable IRQ generation for the 4 buttons
   *(KEY_ptr + 2) = 0xF;

   // Register the interrupt handler.
   key_interrupt_value = request_irq (KEYS_IRQ, (irq_handler_t) irq_handler_pushbutton, IRQF_SHARED, 
      "pushbutton_irq_handler", (void *) (irq_handler_pushbutton));
   
   
   // Register the interrupt handler.
   timer_interrupt_value = request_irq (INTERVAL_TIMER_IRQi, (irq_handler_t) irq_handler, IRQF_SHARED, 
      "timer_irq_handler", (void *) (irq_handler));


   return timer_interrupt_value;
}

static void __exit cleanup_all_handler(void){
   *SEG1_ptr = 0x00;  // Turn off Segments
   *SEG2_ptr = 0x00;

   free_irq (INTERVAL_TIMER_IRQi, (void*) irq_handler);
   free_irq (KEYS_IRQ, (void*) irq_handler_pushbutton);
}

//Conversion from decimal to Hexcoded 7 seg
int setDigit(int digit){
	int array[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
	return array[digit];
}

void refreshDisplay(){
	int ddHex = (setDigit((dd/10)%10) << 8) + setDigit(dd%10);
	int ssHex = (setDigit((ss/10)%10) << 8) + setDigit(ss%10);
	int mmHex = (setDigit((mm/10)%10) << 8) + setDigit(mm%10);

	*SEG1_ptr = (ssHex << 16) + ddHex;
	*SEG2_ptr = mmHex;
}


module_init(initialize_all_handler);
module_exit(cleanup_all_handler);
