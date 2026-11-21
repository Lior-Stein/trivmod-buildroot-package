
TRIVMOD_VERSION = c34b0a5c99f79298e4eae2bbf5bf379c88d710cc
TRIVMOD_SITE = $(call github,Lior-Stein,trivmod-buildroot-package,$(TRIVMOD_VERSION))

$(eval $(kernel-module))
$(eval $(generic-package))
