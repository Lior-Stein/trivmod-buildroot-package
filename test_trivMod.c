
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = open("/dev/trivMod", O_RDONLY);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("Open succeeded\n");

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    printf("Close succeeded\n");

    return 0;
}
