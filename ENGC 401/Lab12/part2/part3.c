#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#define video_BYTES 8 // number of characters to read from /dev/video

int screen_x, screen_y;
int TRUE = 1;

int main(int argc, char *argv[]){
    int video_FD,
        index = 0,
        y, count = 0;

    char buffer[video_BYTES], 
        *string, *found[3],     // buffer for data read from /dev/video
        command[64],buffer2[64];            // buffer for commands written to /dev/video
    
        // Open the character device driver
    if ((video_FD = open("/dev/video", O_RDWR)) == -1){
        printf("Error opening /dev/video: %s\n", strerror(errno));
        return -1;
    }
    else{
        // Set screen_x and screen_y by reading from the driver
        read(video_FD, buffer, video_BYTES);
        string = strdup(buffer); //Writes input to pointer.
        while ( (found[index] = strsep(&string, " ,")) != NULL) //splits the string up
            index++;
        screen_x = strtol(found[0], NULL, 10);
        screen_y = strtol(found[1], NULL, 10);
        printf("X:%d Y:%d\n", screen_x, screen_y); 
    }

    sprintf(command, "clear");
    write(video_FD, command, strlen(command));

    /* Bouncing the line*/
    while(TRUE){
        for(y = 0; y < 60; y++){
            sprintf (command, "text %d,%d %s\n", 0, y, "Hello_World!");
            write (video_FD, command, strlen(command));
            //sprintf (command, "sync \n");
            usleep(16666.667);
            //write (video_FD, command, strlen(command));
	sprintf (command, "text %d,%d %s\n", 0, y, "            ");
            write (video_FD, command, strlen(command));
        }
        for(y = 59; y >= 0; y--){
		sprintf (command, "text %d,%d %s\n", 0, y, "GoodBye_World!");
                write (video_FD, command, strlen(command));
                //sprintf (command, "sync \n");
                usleep(16666.667);
                //write (video_FD, command, strlen(command));
	 	sprintf (command, "text %d,%d %s\n", 0, y, "              ");
                write (video_FD, command, strlen(command));
        }

    }
    close (video_FD);
    return 0;
}
