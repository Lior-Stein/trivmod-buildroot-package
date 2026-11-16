
# TrivMod - Buildroot Kernel Module Package

A simple Linux kernel module that prints messages
when loaded and unloaded.

## Buildroot Integration

1. Copy `package/trivmod/` into Buildroot's `package/` directory.

2. Add the following line to Buildroot's `package/Config.in`
   under Miscellaneous packages:

   source "package/trivmod/Config.in"

3. Run `make menuconfig` and enable:
   Target packages -> Miscellaneous -> trivmod

4. Run `make` to build the package.

## Notes

The module is built using Buildroot's kernel-module
infrastructure and installed as a loadable kernel module.

To avoid duplicate installations, do not also enable
the same module as an in-tree kernel module.
