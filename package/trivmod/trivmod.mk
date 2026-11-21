
TRIVMOD_VERSION = 3bd43f106b8e5b385407e3b4a63a3be524eb69d2
TRIVMOD_SITE = $(call github,Lior-Stein,trivmod-buildroot-package,$(TRIVMOD_VERSION))

$(eval $(kernel-module))
$(eval $(generic-package))
