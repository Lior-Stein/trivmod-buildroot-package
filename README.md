
# TrivMod – Buildroot Kernel Module

A loadable Linux kernel module developed for the Embedded Linux course and integrated into Buildroot as a package.

## Buildroot Integration

1. Copy `package/trivmod/` into Buildroot's `package/` directory.
2. Add the following line to `package/Config.in` under Miscellaneous:

   `source "package/trivmod/Config.in"`

3. Run `make menuconfig` and enable `Target packages -> Miscellaneous -> trivmod`.
4. Run `make` to build the package.

## Exercise – Dynamic Device Number Allocation

Extended the existing module to:

- Allocate a device number using `alloc_chrdev_region()` during initialization.
- Register one device with a dynamically assigned Major Number.
- Release the device number using `unregister_chrdev_region()` during module removal.

## Raspberry Pi Verification

- Loaded the module using `insmod`.
- Verified `240 trivMod` in `/proc/devices`.
- Removed the module using `rmmod`.
- Confirmed the registration disappeared from `/proc/devices`.

**Result:** Dynamic device number allocation and release completed successfully.

The Major Number is assigned dynamically and may differ between runs.


## Exercise – Character Device Registration

Extended the existing module to register a character device.

**Implementation:**
- Statically defined a `cdev` structure.
- Added `file_operations` with `THIS_MODULE`.
- Initialized and registered the device using `cdev_init()` and `cdev_add()`.
- Added error handling and resource cleanup.
- Added `cdev_del()` during module removal.

**Raspberry Pi Verification:**
- Confirmed `trivMod` was absent from `/proc/devices` before loading.
- Loaded the module using `insmod`.
- Verified the device appeared in `/proc/devices`.
- Removed the module using `rmmod`.
- Confirmed the registration disappeared.

**Result:** Character device registration and cleanup completed successfully.


## Exercise – Device File Creation

Extended the character device driver to dynamically create
and remove its device file.

**Implementation:**
- Added `class_create()` and `device_create()` during initialization.
- Added error handling and resource cleanup.
- Added `device_destroy()` and `class_destroy()` during module removal.

**Raspberry Pi Verification:**
- Loaded the module using `insmod`.
- Verified the creation of `/sys/class/trivMod` and `/dev/trivMod`.
- Removed the module using `rmmod`.
- Confirmed both entries disappeared.

**Result:** Dynamic device file creation and removal
completed successfully.

## Exercise – Open /Release

## Open and Release

Implemented the `open` and `release` operations for the character device.

- Used `container_of` to access the device's private structure.
- Stored the private data pointer in `filp->private_data`.
- Added counters for open and release operations.
- Added `printk` messages to display the counters.
- Created `test_trivMod.c`, a user-space program that opens and closes `/dev/trivMod`.

### Testing

Cross-compiled the test program for ARM64 and tested it on the Raspberry Pi.

- Open and close operations completed successfully.
- Verified the kernel messages using `dmesg`.
- Confirmed that both counters increased from 1 to 2 after running the test twice.
- Successfully unloaded the module using `rmmod`.

## Exercise – read/ write

## Read and Write

- `/dev/trivMod` stores data in a 4 KiB circular buffer in the device's private data.
- `write` copies data from user space at `head`, and `read` copies unread data back to user space from `tail`. When a transfer crosses the end of the buffer, it continues from the beginning in a second copy.

### Testing

On the Raspberry Pi, `test_trivMod.c` wrote `hello` (6 bytes including `'\0'`), read it back into a separate buffer, and verified with `memcmp()` that all 6 bytes matched.

Test output: `Read back 6 bytes: hello`


## Exercise – Driver Statistics with ioctl

The driver tracks opens, closes, bytes read, and bytes written in its private
data. The counters accumulate across runs while the module is loaded.

- `TRIVMOD_STATSGET` returns the four counters to user space.
- `TRIVMOD_STATSCLR` resets all four counters to zero.

### Testing 

Run `/root/test_trivMod` three times with the module freshly loaded. Each run
writes and reads back 6 bytes. The test prints the counters before closing
the device, so `closes` counts the completed closes from earlier runs.

```text
opens=1 closes=0 bytes_read=6 bytes_written=6
opens=2 closes=1 bytes_read=12 bytes_written=12
opens=3 closes=2 bytes_read=18 bytes_written=18
```

Run `/root/test_trivMod clear` while the same module is loaded. The program
resets the counters after the read/write test and then prints the result.

```text
opens=0 closes=0 bytes_read=0 bytes_written=0
```

After testing, unload the module with `rmmod trivMod`.


## Notes

The module is built through the Buildroot Package infrastructure, without simultaneous in-tree integration. Previous implementations are preserved in Git commit history.
