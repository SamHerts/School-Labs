#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#define video_BYTES 8 // number of characters to read from /dev/video

int screen_x, screen_y;

int main(int argc, char *argv[]){
    int video_FD,
        index = 0,
        x, y;

    char buffer[video_BYTES], 
        *string, *found[3],     // buffer for data read from /dev/video
        command[64];            // buffer for commands written to /dev/video
    
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
    for(;;){
        for(y = 0; y < screen_y-1; y++){
            sprintf (command, "line %d,%d %d,%d %X \n", 0, y, screen_x - 1, y, 0xFFFF); // cyan
            write (video_FD, command, strlen(command));
            sprintf (command, "sync \n");
            usleep(16666.667);
            //write (video_FD, command, strlen(command));
            sprintf (command, "line %d,%d %d,%d %X \n", 0, y, screen_x - 1, y, 0x0000); // black
            write (video_FD, command, strlen(command));
            
        }
        for(y = screen_y-1; y >= 0; y--){
            sprintf (command, "line %d %d %d %d %X \n", 0, y, screen_x - 1, y, 0xFFFF); // cyan
            write (video_FD, command, strlen(command));
            sprintf (command, "sync \n");
            usleep(16666.667);
            //write (video_FD, command, strlen(command));
            sprintf (command, "line %d %d %d %d %X \n", 0, y, screen_x - 1, y, 0x0000); // black
            write (video_FD, command, strlen(command));
        }

    }
    close (video_FD);
    return 0;
}
