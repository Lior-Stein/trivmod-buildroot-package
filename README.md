
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

## Notes

The module is built through the Buildroot Package infrastructure, without simultaneous in-tree integration. Previous implementations are preserved in Git commit history.
