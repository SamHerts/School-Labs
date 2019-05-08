/* --------------------------------- */
/// 640x480 version!
/// This code will segfault the original
/// DE1 computer
/// compile with
/// gcc calculator.c -o life -O2
/// -- no optimization yields ??? execution time
/// -- opt -O1 yields ??? mS execution time
/// -- opt -O2 yields ??? mS execution time
/// -- opt -O3 yields ??? mS execution time
/* --------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "../Includes/address_map_arm_br14.h"

struct mouse {
	int x, y, click, previousColor;
};

/* function prototypes */
void VGA_text(int, int, char *);//draws text at x,y, string
void VGA_text_clear();//erases character buffer
void VGA_box(int, int, int, int, short);//draws solid box at x,y x,y , color
void VGA_line(int, int, int, int, short);
void VGA_PIXEL(int, int, short);
void drawCalc(void);//draws the 4x5 grid with colored squares and enter bar
int GET_PIXEL(int x, int y);
struct mouse getMouse(int);//returns mouse coordinates and click from charDev
void drawReg(void);//draws registers as strings to display values stored
int getClick(void);//returns generalized area where a click occured
char getOP(void);//returns which operation was clicked on
void drawMouse(struct mouse);//draws the mouse at current location and overwrites previous mouse's location
/* end function prototypes*/

/* variable definitions */
void *h2p_lw_virtual_base;// the light weight buss base

volatile unsigned int * vga_pixel_ptr = NULL;// pixel buffer
void *vga_pixel_virtual_base;

volatile unsigned int * vga_char_ptr = NULL;// character buffer
void *vga_char_virtual_base;


volatile unsigned int * ALU_ptr;

int fd_mem, fd_mouse;// /dev/mem file id
const char *mouseDevice = "/dev/input/mice";

#define WIDTH 640
#define HEIGHT 480

#define AlphaReg *(ALU_ptr + 0)
#define BetaReg *(ALU_ptr + 1)
#define ALUSelect *(ALU_ptr + 2)
#define ResultReg *(ALU_ptr + 3)
#define ALU_BASE 0x30000

key_t mem_key = 0xf0;// shared memory
int shared_mem_id;
int *shared_ptr;
int shared_time;
int shared_note;
char shared_str[64];


struct mouse Mouse = {.x = WIDTH/2, .y = HEIGHT/2, .click = 0,  .previousColor = 0x00};
struct mouse oldMouse = {WIDTH/2, HEIGHT/2, 0, 0x00};
//float AlphaReg = 0, BetaReg = 0, ResultReg = 0; //stores the values
char OP = '+';
int tempReg = 0;
char DATA[100];
/* end variable definitons*/

int main(void)
{
	// === shared memory =======================
	// with video process
	shared_mem_id = shmget(mem_key, 100, IPC_CREAT | 0666);
	shared_ptr = shmat(shared_mem_id, NULL, 0);

	// === get FPGA addresses ==================
	// Open /dev/mem
	if ((fd_mem = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
		printf("ERROR: could not open \"/dev/mem\"...\n");
		return(1);
	}

	// get virtual addr  that maps to physical
	h2p_lw_virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd_mem, HW_REGS_BASE);
	if (h2p_lw_virtual_base == MAP_FAILED) {
		printf("ERROR: mmap1() failed...\n");
		close(fd_mem);
		return(1);
	}

	// === get VGA char addr =====================
	// get virtual addr that maps to physical
	vga_char_virtual_base = mmap(NULL, FPGA_CHAR_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd_mem, FPGA_CHAR_BASE);
	if (vga_char_virtual_base == MAP_FAILED) {
		printf("ERROR: mmap2() failed...\n");
		close(fd_mem);
		return(1);
	}

	// Get the address that maps to the FPGA LED control
	vga_char_ptr = (unsigned int *)(vga_char_virtual_base);

	// === get VGA pixel addr ====================
	// get virtual addr that maps to physical
	vga_pixel_virtual_base = mmap(NULL, FPGA_ONCHIP_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd_mem, FPGA_ONCHIP_BASE);
	if (vga_pixel_virtual_base == MAP_FAILED) {
		printf("ERROR: mmap3() failed...\n");
		close(fd_mem);
		return(1);
	}

	// Get the address that maps to the FPGA pixel buffer
	vga_pixel_ptr = (unsigned int *)(vga_pixel_virtual_base);


	//------------- open mouse file
	fd_mouse = open(mouseDevice, O_RDWR);
	if (fd_mouse == -1)
	{
		printf("ERROR Opening %s\n", mouseDevice);
		return -1;
	}

	ALU_ptr = h2p_lw_virtual_base + ((unsigned int)(ALU_BASE));

	//-------------

	int count = 0;

	VGA_box(0, 0, WIDTH, HEIGHT, 0x00);//clear screen
	VGA_text_clear();//clear text
	drawReg();
	drawCalc();//initialize calculator graphics
	Mouse.previousColor = GET_PIXEL(Mouse.x,Mouse.y);
	oldMouse = Mouse;
	drawMouse(getMouse(fd_mouse));

	while (1) {
		//printf("mouse click? : %d\n", Mouse.click);
		if(Mouse.click == 1){
			int val;
			val = getClick();
			switch(val){
			case -1:
				Mouse.click = 0;
				break;
			case 0:
				//printf("NUMBER\n");
				if(tempReg == 0){
					tempReg = getDigit();
				}else {
					tempReg = (tempReg * 10) + getDigit();
				}
				break;
			case 1:
				//printf("ALPHA\n");
				AlphaReg = tempReg;
				//printf("AlphaReg: %d\n", AlphaReg);
				count = 0;
				tempReg = 0;
				break;
			case 2:
				//printf("BETA\n");
				//BetaReg = tempReg;
				//printf("BetaReg: %d\n", BetaReg);
				count = 0;
				tempReg = 0;
				break;
			case 3:
				//printf("OPERATION\n");
				OP = getOP();
				break;
			case 4:
				//printf("ENTER\n");
				//if(OP == '+'){ ResultReg = AlphaReg + BetaReg;}
				//if(OP == '-'){ ResultReg = AlphaReg - BetaReg;}
				//if(OP == 'X'){ ResultReg = AlphaReg * BetaReg;}
				//if(OP == '/' && BetaReg != 0){ResultReg = AlphaReg / BetaReg;}
				if(OP == '+'){
					ALUSelect = 0x0;
					}
				if(OP == '-'){
					ALUSelect = 0x1;
					}
				if(OP == 'X'){
					ALUSelect = 0x2;
					}
				if(OP == '/' && BetaReg != 0){
					ALUSelect = 0x3;
					}
				break;
			}//end switch
			drawReg();
			drawCalc();
		}//end if
		drawMouse(getMouse(fd_mouse));
	}//end while
} // end main
int getDigit(void){
	int myTemp;
	int i;
	i = (Mouse.x < 250 ? 0 : (Mouse.x < 300 ? 1 : 2));
	if(Mouse.y < 150){
		myTemp = 7+i;
	}else if(Mouse.y < 200){
		myTemp = 4+i;
	}else if(Mouse.y < 250){
		myTemp = 1+i;
	}else if(Mouse.y < 300){
		myTemp = -1+i;
	}
}
void drawMouse(struct mouse temp){
	Mouse.x += temp.x; //update relative X coordinate
	Mouse.y -= temp.y; //update relative Y coordinate
	Mouse.click = temp.click;//check if click occured
	Mouse.x = (Mouse.x >= WIDTH) ? WIDTH : ((Mouse.x <= 0) ? 0 : Mouse.x);
	Mouse.y = (Mouse.y >= HEIGHT) ? HEIGHT : ((Mouse.y <= 0) ? 0 : Mouse.y);
	VGA_text(0,3,"       ");
	VGA_text(0,4,"       ");
	sprintf(DATA, "%d , %d ", Mouse.x, Mouse.y);
	VGA_text(0, 0, DATA);
	Mouse.previousColor = GET_PIXEL(Mouse.x,Mouse.y);
	VGA_PIXEL(Mouse.x, Mouse.y, 0xFF);//draw new mouse
	VGA_PIXEL(oldMouse.x, oldMouse.y, oldMouse.previousColor);//erase old mouse
	oldMouse = Mouse;//save old coordinates of mouse
}

char getOP(void){
	char myOP;
	if(Mouse.y < 150){
		myOP = '+';
	}else if(Mouse.y < 200){
		myOP = '-';
	}else if(Mouse.y < 250){
		myOP = 'X';
	}else {
		myOP = '/';
	}
	return myOP;
}

int getClick(void){
	int val = -1;
	if((Mouse.x > 200 && Mouse.x < 350 && Mouse.y > 100 && Mouse.y < 250) || (Mouse.x > 250 && Mouse.x < 300 && Mouse.y > 250 && Mouse.y < 300))
	{	val = 0;//Number
	}
	if((Mouse.x > 200 && Mouse.x < 250 && Mouse.y > 250 && Mouse.y < 300))
	{	val = 1;//ALPHA
	}
	if((Mouse.x > 300 && Mouse.x < 350 && Mouse.y > 250 && Mouse.y < 300))
	{	val = 2;//BETA

	}
	if((Mouse.x > 350 && Mouse.x < 400 && Mouse.y > 100 && Mouse.y < 300))
	{	val = 3;//OPERATION
	}
	if((Mouse.x > 200 && Mouse.x < 400 && Mouse.y > 300 && Mouse.y < 350))
	{	val = 4;//ENTER
	}
	return val;
}

void drawReg(void){
	VGA_text_clear();
	sprintf(DATA, "Alpha: %d"   , AlphaReg);
	VGA_text(18, 11, DATA);
	sprintf(DATA, "Beta: %d"   , BetaReg);
	VGA_text(44, 11, DATA);
	sprintf(DATA, "Result: %d" , ResultReg);
	VGA_text(32, 45, DATA);
	sprintf(DATA, "%d", tempReg);
	VGA_text(37, 8, DATA);
	sprintf(DATA,"%c", OP);
	VGA_text(37,11, DATA);
}

void VGA_text(int x, int y, char * text_ptr)
{
	volatile char * character_buffer = (char *)vga_char_ptr;	// VGA character buffer
	int offset;
	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while (*(text_ptr))
	{
		// write to the character buffer
		*(character_buffer + offset) = *(text_ptr);
		++text_ptr;
		++offset;
	}
}

void VGA_text_clear()
{
	int x, y;
	for (x = 0; x < 80; x++) {
		for (y = 0; y < 60; y++) {
			// write to the character buffer
			VGA_text(x,y," ");
		}
	}
}

#define SWAP(X,Y) do{int temp=X; X=Y; Y=temp;}while(0) 

void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)
{
	char  *pixel_ptr;
	int row, col;

	/* check and fix box coordinates to be valid */
	if (x1 >= WIDTH) x1 = WIDTH-1;
	if (y1 >= HEIGHT) y1 = HEIGHT-1;
	if (x2 >= WIDTH) x2 = WIDTH-1;
	if (y2 >= HEIGHT) y2 = HEIGHT-1;
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 < 0) x2 = 0;
	if (y2 < 0) y2 = 0;
	if (x1 > x2) SWAP(x1, x2);
	if (y1 > y2) SWAP(y1, y2);
	for (row = y1; row <= y2; row++)
		for (col = x1; col <= x2; ++col)
		{
			//640x480
			//pixel_ptr = (char *)vga_pixel_ptr + (row << 10) + col;
			// set pixel color
			// *(char *)pixel_ptr = pixel_color;
			VGA_PIXEL(col, row, pixel_color);
		}
}

void VGA_line(int x1, int y1, int x2, int y2, short c) {
	int e;
	signed int dx, dy, j, temp;
	signed int s1, s2, xchange;
	signed int x, y;
	char *pixel_ptr;

	/* check and fix line coordinates to be valid */
	if (x1 >= WIDTH) x1 = WIDTH-1;
	if (y1 >= HEIGHT) y1 = HEIGHT-1;
	if (x2 >= WIDTH) x2 = WIDTH-1;
	if (y2 >= HEIGHT) y2 = HEIGHT;
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (x2 < 0) x2 = 0;
	if (y2 < 0) y2 = 0;

	x = x1;
	y = y1;

	//take absolute value
	if (x2 < x1) {
		dx = x1 - x2;
		s1 = -1;
	}

	else if (x2 == x1) {
		dx = 0;
		s1 = 0;
	}

	else {
		dx = x2 - x1;
		s1 = 1;
	}

	if (y2 < y1) {
		dy = y1 - y2;
		s2 = -1;
	}

	else if (y2 == y1) {
		dy = 0;
		s2 = 0;
	}

	else {
		dy = y2 - y1;
		s2 = 1;
	}

	xchange = 0;

	if (dy > dx) {
		temp = dx;
		dx = dy;
		dy = temp;
		xchange = 1;
	}

	e = ((int)dy << 1) - dx;

	for (j = 0; j <= dx; j++) {
		//video_pt(x,y,c); //640x480
		//pixel_ptr = (char *)vga_pixel_ptr + (y << 10) + x;
		// set pixel color
		//*(char *)pixel_ptr = c;
		VGA_PIXEL(x,y,c);
		if (e >= 0) {
			if (xchange == 1) x = x + s1;
			else y = y + s2;
			e = e - ((int)dx << 1);
		}

		if (xchange == 1) y = y + s2;
		else x = x + s1;

		e = e + ((int)dy << 1);
	}
}
void VGA_PIXEL(int x, int y, short color) {
	char  *pixel_ptr;
	pixel_ptr = (char *)vga_pixel_ptr + ((y) << 10) + (x);
	*(char *)pixel_ptr = (color);
}

struct mouse getMouse(int fd_mouse) {
	int bytes;
	unsigned char mouse_data[3];
	signed char x, y;
	struct mouse myMouse;

	// Read Mouse
	bytes = read(fd_mouse, mouse_data, sizeof(mouse_data));

	if (bytes > 0)
	{
		myMouse.click = mouse_data[0] & 0x1;
		//myMouse.right = mouse_data[0] & 0x2;
		//myMouse.middle = mouse_data[0] & 0x4;

		x = mouse_data[1];
		y = mouse_data[2];
	}
	myMouse.x = (int)x;
	myMouse.y = (int)y;
	//printf("Mouse relative coordinates: %d,%d\n",myMouse.x,myMouse.y);
	return myMouse;
}

void drawCalc(void) {
	//make a modular grid of colors
	int startX = 200;
	int startY = 100;
	int width = 50;
	int i, j;
	int sizeX = 4;
	int sizeY = 4;
	int startColor = 0x0;
	for (i = 1; i <= sizeX; i++) {
		for (j = 1; j <= sizeY; j++) {
			VGA_box(startX + width * (i - 1), startY + width * (j - 1), startX + width * i - 1, startY + width * j - 1, startColor + i + j);
		}
	}
	VGA_box(startX,startY+width*(sizeY),startX+width*sizeX - 1,startY+width*(sizeY+1), 0xF0);
	//put registers here VGA_text(10,10,Data);
	sprintf(DATA, "7     8      9     +");
	VGA_text(28,15,DATA);
	sprintf(DATA, "4     5      6     -");
	VGA_text(28,21,DATA);
	sprintf(DATA, "1     2      3     X");
	VGA_text(28,27,DATA);
	sprintf(DATA, "A     0      B     /");
	VGA_text(28,34,DATA);
	sprintf(DATA, "ENTER");
	VGA_text(35,40, DATA);

}

int GET_PIXEL(int x, int y) {
	char  *pixel_ptr;
	pixel_ptr = (char *)vga_pixel_ptr + ((y) << 10) + (x);
	return (*(char *)pixel_ptr);
}
