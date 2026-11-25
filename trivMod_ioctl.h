#ifndef TRIVMOD_IOCTL_H
#define TRIVMOD_IOCTL_H

#include <linux/types.h>
#include <linux/ioctl.h>

struct trivMod_stats {
    __u64 opens;
    __u64 closes;
    __u64 bytes_read;
    __u64 bytes_written;
};

#define TRIVMOD_TYPE     't'
#define TRIVMOD_STATSGET _IOR(TRIVMOD_TYPE, 1, struct trivMod_stats)
#define TRIVMOD_STATSCLR _IO(TRIVMOD_TYPE, 2)

#endif