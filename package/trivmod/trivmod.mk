
TRIVMOD_VERSION = 26513bc9d9bc5ef40ac4c37cfcedaa5a6f38cfa9
TRIVMOD_SITE = $(call github,Lior-Stein,trivmod-buildroot-package,$(TRIVMOD_VERSION))

$(eval $(kernel-module))
$(eval $(generic-package))
