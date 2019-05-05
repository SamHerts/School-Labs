#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define video_BYTES 8 // number of characters to read from /dev/video

int screen_x, screen_y;

int main(int argc, char *argv[])
{
    int video_FD, index = 0, x, y;
    char buffer[video_BYTES], *string, *found[3], command[64]; // buffer for commands written to /dev/video

        // Open the character device driver
    if ((video_FD = open("/dev/video", O_RDWR)) == -1)
    {
    	printf("Error opening /dev/video: %s\n", strerror(errno)); return -1;
    }
    else{
        // Set screen_x and screen_y by reading from the driver
        read(video_FD, buffer, video_BYTES); string = strdup(buffer);
        while ( (found[index] = strsep(&string, " ,")) != NULL)
            index++;
            screen_x = strtol(found[0], NULL, 10);
            screen_y = strtol(found[1], NULL, 10);
            printf("X:%d Y:%d\n", screen_x, screen_y);
    }

    // Use pixel commands to color some pixels on the screen
   for(y = 0; x < screen_x; x++){
        for (y = 0; y < screen_y; y++){
            sprintf(command, "pixel %d,%d 0xFFFF\n", x, y);
            write(video_FD, command, strlen(command));
        }
    }
    close (video_FD);
    return 0;
}
