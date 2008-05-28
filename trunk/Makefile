# Top-level VMGL makefile
# Copyright (c) H. Andres Lagar-Cavilla, University of Toronto, 2006-2007

MAKE = make
XMKMF = xmkmf
INSTALL = install
INSTALL_FLAGS := -s -m 0755
CONFIGURE = ./configure
RM = rm

TOP = $(shell pwd)
SUBDIRS = cr tightvnc tightvnc/Xvnc

DOM0_BIN = vncviewer glstub stub-daemon
DOMU_BIN = Xvnc
DOM0_LIB = libcrutil.so libspuload.so liberrorspu.so librenderspu.so
DOMU_LIB = libcrutil.so libspuload.so liberrorspu.so libvmgl.so libarrayspu.so libfeedbackspu.so libpackspu.so libpassthroughspu.so

ifdef VMGLPATH
INSTALLPATH = $(VMGLPATH)
else
INSTALLPATH = /usr/local/
endif

all:
	$(INSTALL) -d $(TOP)/dist/lib/
	$(INSTALL) -d $(TOP)/dist/bin/
	(cd cr ; \
	    $(MAKE) ; \
	    $(INSTALL) $(INSTALL_FLAGS) dist/lib/*.so $(TOP)/dist/lib/ ; \
	    $(INSTALL) $(INSTALL_FLAGS) dist/bin/* $(TOP)/dist/bin/ ; \
	) || exit 1
	(cd tightvnc ; \
	    $(XMKMF) -a ; \
	    $(MAKE) ; \
	    $(INSTALL) $(INSTALL_FLAGS) vncviewer/vncviewer ../dist/bin/vncviewer ; \
	    cd Xvnc ; \
		$(CONFIGURE) ; \
		$(MAKE) ; \
		$(INSTALL) $(INSTALL_FLAGS) programs/Xserver/Xvnc ../../dist/bin/Xvnc ; \
	) || exit 1

clean:
	@for dir in $(SUBDIRS) ; do \
		(cd $$dir ; $(MAKE) clean) ; \
	done

install-host: all
	$(INSTALL) -d $(INSTALLPATH)/bin/
	for binary in $(DOM0_BIN); do \
	    $(INSTALL) $(TOP)/dist/bin/$$binary $(INSTALLPATH)/bin/$$binary ; \
	done
	$(INSTALL) -d $(INSTALLPATH)/lib/vmgl
	for library in $(DOM0_LIB); do \
	    $(INSTALL) $(TOP)/dist/lib/$$library $(INSTALLPATH)/lib/vmgl/$$library ; \
	done
	echo "$(INSTALLPATH)/lib/vmgl/" > /etc/ld.so.conf.d/vmgl.conf
	/sbin/ldconfig

install-guest: all
	$(INSTALL) -d $(INSTALLPATH)/bin/
	for binary in $(DOMU_BIN); do \
	    $(INSTALL) $(TOP)/dist/bin/$$binary $(INSTALLPATH)/bin/$$binary ; \
	done
	$(INSTALL) -d $(INSTALLPATH)/lib/vmgl
	for library in $(DOMU_LIB); do \
	    $(INSTALL) $(TOP)/dist/lib/$$library $(INSTALLPATH)/lib/vmgl/$$library ; \
	done
	ln -s libvmgl.so $(INSTALLPATH)/lib/vmgl/libGL.so.1.2
	ln -s libGL.so.1.2 $(INSTALLPATH)/lib/vmgl/libGL.so.1
	ln -s libGL.so.1 $(INSTALLPATH)/lib/vmgl/libGL.so
	$(INSTALL) $(TOP)/dist/libvmglext.so /usr/lib/xorg/modules/extensions
	echo "$(INSTALLPATH)/lib/vmgl/" > /etc/ld.so.conf.d/vmgl.conf
	/sbin/ldconfig

uninstall-host: 
	for binary in $(DOM0_BIN); do \
	    $(RM) -f $(INSTALLPATH)/bin/$$binary ; \
	done
	$(RM) -rf $(INSTALLPATH)/lib/vmgl
	$(RM) /etc/ld.so.conf.d/vmgl.conf
	/sbin/ldconfig

uninstall-guest: 
	for binary in $(DOMU_BIN); do \
	    $(RM) -f $(INSTALLPATH)/bin/$$binary ; \
	done
	$(RM) -rf $(INSTALLPATH)/lib/vmgl
	$(RM) -f /usr/lib/xorg/modules/extensions/libvmglext.so
	$(RM) /etc/ld.so.conf.d/vmgl.conf
	/sbin/ldconfig

