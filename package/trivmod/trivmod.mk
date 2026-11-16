
TRIVMOD_VERSION = 21f56dd6d06c05b5e3020f6ee5e9f55bdd413768
TRIVMOD_SITE = $(call github,Lior-Stein,trivmod-buildroot-package,$(TRIVMOD_VERSION))

$(eval $(kernel-module))
$(eval $(generic-package))
