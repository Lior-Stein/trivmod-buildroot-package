
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
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

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    printf("Close succeeded\n");

    return 0;
}
