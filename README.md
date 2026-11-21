
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


## Notes

The module is built through the Buildroot Package infrastructure, without simultaneous in-tree integration. Previous implementations are preserved in Git commit history.
