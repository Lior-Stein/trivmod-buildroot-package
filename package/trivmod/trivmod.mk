
TRIVMOD_VERSION = 2023623c3e20ec31d7cc7380533f03021e93fa81
TRIVMOD_SITE = $(call github,Lior-Stein,trivmod-buildroot-package,$(TRIVMOD_VERSION))

$(eval $(kernel-module))
$(eval $(generic-package))
