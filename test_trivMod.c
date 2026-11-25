
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "trivMod_ioctl.h"

int main(int argc, char *argv[])
{
    char write_buf[] = "hello";
    char read_buf[sizeof(write_buf)] = {0};
    int fd = open("/dev/trivMod", O_RDWR);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("Open succeeded\n");

    ssize_t written = write(fd, write_buf, sizeof(write_buf));

    if (written != (ssize_t)sizeof(write_buf)) {
        fprintf(stderr, "Expected %zu bytes, wrote %zd\n",
                sizeof(write_buf), written);
        close(fd);
        return 1;
    }

    ssize_t bytes_read = read(fd, read_buf, sizeof(read_buf));


    if (bytes_read != written) {
        fprintf(stderr, "Expected to read %zd bytes, got %zd\n",
            written, bytes_read);
        close(fd);
        return 1;
    }

    if (memcmp(write_buf, read_buf, sizeof(write_buf)) != 0) {
        fprintf(stderr, "Data mismatch\n");
        close(fd);
        return 1;
    }

    printf("Read back %zd bytes: %s\n", bytes_read, read_buf);

    struct trivMod_stats stats;

    if (argc == 2 && strcmp(argv[1], "clear") == 0) {
        if (ioctl(fd, TRIVMOD_STATSCLR, 0) < 0) {
            perror("ioctl STATSCLR");
            close(fd);
            return 1;
        }
    }


    if (ioctl(fd, TRIVMOD_STATSGET, &stats) < 0) {
        perror("ioctl STATSGET");
        close(fd);
        return 1;
    }

    printf("opens=%llu closes=%llu bytes_read=%llu bytes_written=%llu\n",
           stats.opens, stats.closes, stats.bytes_read, stats.bytes_written);

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    printf("Close succeeded\n");

    return 0;
}
