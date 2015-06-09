# 
# XwinTox
#

include Makefile.cfg
include Makefile.mk

build:
	@env ANALYSE="@#" $(MAKE) real

real: buildxwintox

buildxwintox: buildtoxaemia buildlibsunshine
	cd xwintox_src && env ROOTDIR=$(shell pwd) $(MAKE) all

buildtoxaemia: buildlibsunshine
	cd toxaemia_src && env ROOTDIR=$(shell pwd) $(MAKE) all

buildlibsunshine:
	cd libsunshine_src && env ROOTDIR=$(shell pwd) $(MAKE) all

clean:
	cd libsunshine_src && env ROOTDIR=$(shell pwd) $(MAKE) clean_subdir
	cd xwintox_src && env ROOTDIR=$(shell pwd) $(MAKE) clean_subdir
	cd toxaemia_src && env ROOTDIR=$(shell pwd) $(MAKE) clean_subdir

