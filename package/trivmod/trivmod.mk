
TRIVMOD_VERSION = 0192036900700058fb97e5fa4169ba7e28e5cd81
TRIVMOD_SITE = $(call github,Lior-Stein,trivmod-buildroot-package,$(TRIVMOD_VERSION))

$(eval $(kernel-module))
$(eval $(generic-package))
