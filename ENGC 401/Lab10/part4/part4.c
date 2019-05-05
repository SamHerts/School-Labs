#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BYTES 256 // max # of bytes to read from /dev/chardev

volatile sig_atomic_t stop;
void catchSIGINT(int signum){
	stop = 1;
}

char HEX_msg[BYTES]; // space for the new message
char KEY_msg[BYTES];
char SW_msg[BYTES];

int openDev(char*);
void readFile(int, char*);


/* This code uses the character device driver /dev/chardev. The code reads the
* default message from the driver and then prints it. After this the code
* changes the message in a loop by writing to the driver, and prints each new
* message. The program exits if it receives a kill signal (for example, ^C
* typed on stdin). */
int main(int argc, char *argv[]){

	int KEY_FD,SW_FD,LEDR_FD,HEX_FD; // file descriptor
	int SW_int, sum=0;
	char *ptr;

	// catch SIGINT from ^C, instead of having it abruptly close this program
	signal(SIGINT, catchSIGINT);
	KEY_FD = openDev("/dev/KEY"); // open Key file

	while (!stop) {

		readFile(KEY_FD,KEY_msg);//read key values into string

		if (atoi(KEY_msg)) {//if a button has been pressed

			SW_FD = openDev("/dev/SW");
			readFile(SW_FD,SW_msg);
			close(SW_FD);

			SW_int = strtol(SW_msg,&ptr, 2);
			sum += SW_int;

			LEDR_FD = openDev("/dev/LEDR");
			sprintf(SW_msg,"%d\n",SW_int);
			write(LEDR_FD, SW_msg, strlen(SW_msg));
			close(LEDR_FD);

			HEX_FD = openDev("/dev/HEX");
			sprintf(HEX_msg, "%d\n", sum);
			write(HEX_FD, HEX_msg, strlen(HEX_msg));
			close(HEX_FD);
		}
	}
	close(KEY_FD);//close file after reading

	return 0;
}

int openDev(char *filename) {
	int FD;

	if ((FD = open(filename, O_RDWR)) == -1) {
		printf("Error opening %s: %s\n",filename, strerror(errno));
		FD = -1;
	}
	return FD;
}

void readFile(int FD, char *buffer) {
	int ret_val, chars_read = 0; // number of characters read

	while ((ret_val = read(FD, buffer, BYTES)) != 0) {
		chars_read += ret_val; // read the driver until EOF
	}
	buffer[chars_read] = '\0'; // NULL terminate
}
