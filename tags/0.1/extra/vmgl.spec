%define binary_version 0.1

Name:		 vmgl
Summary:	 VMGL, OpenGL Hardware Acceleration for Virtual Machines
Version:	 0.1
Release:	 1
License:	 GNU, BSD
Vendor:		 H. Andres Lagar-Cavilla
Group:		 User Interface/X Hardware Support
Source:		 %{name}-%{version}.tar.bz2
BuildRoot:	 %{_tmppath}/%{name}.%{version}-buildroot
URL:		 http://www.cs.toronto.edu/~andreslc/vmgl

%description

%package host
Summary:	 VMGL, OpenGL Hardware Acceleration for Virtual Machines, host package
Group:		 User Interface/X Hardware Support
%description host

%package guest
Summary:	 VMGL, OpenGL Hardware Acceleration for Virtual Machines, guest package
Group:		 User Interface/X Hardware Support
%description guest

%prep
%setup -q -n vmgl.hg

%build
make

%install
rm -rf $RPM_BUILD_ROOT

install -d $RPM_BUILD_ROOT/usr/local/bin \
	   $RPM_BUILD_ROOT/usr/local/lib/vmgl \
	   $RPM_BUILD_ROOT/usr/lib/xorg/modules/extensions \
	   $RPM_BUILD_ROOT/usr/share/doc/vmgl \
	   $RPM_BUILD_ROOT/usr/share/doc/vmgl/cr \
	   $RPM_BUILD_ROOT/etc/ld.so.conf.d

install dist/bin/* \
	$RPM_BUILD_ROOT/usr/local/bin/ 

install dist/lib/*.so \
	$RPM_BUILD_ROOT/usr/local/lib/vmgl/

install dist/libvmglext*so \
	$RPM_BUILD_ROOT/usr/lib/xorg/modules/extensions/

install -m 0644 cr/doc/* \
	$RPM_BUILD_ROOT/usr/share/doc/vmgl/cr/

install -m 0644 cr/LICENCE.txt \
	$RPM_BUILD_ROOT/usr/share/doc/vmgl/cr/

install -m 0644 README \
	$RPM_BUILD_ROOT/usr/share/doc/vmgl/
	
install -m 0644 LICENCE.txt \
	$RPM_BUILD_ROOT/usr/share/doc/vmgl/
	
install -m 0644 tightvnc/LICENCE.TXT -T \
	$RPM_BUILD_ROOT/usr/share/doc/vmgl/LICENCE.tightvnc

install -m 0644 dist/vmgl.conf \
	$RPM_BUILD_ROOT/etc/ld.so.conf.d/
	
%clean
rm -rf $RPM_BUILD_ROOT

%post host
/sbin/ldconfig

%postun host
rm -rf /usr/local/lib/vmgl
/sbin/ldconfig

%post guest
ln -s libvmgl.so /usr/local/lib/vmgl/libGL.so.1.2
ln -s libGL.so.1.2 /usr/local/lib/vmgl/libGL.so.1
ln -s libGL.so.1 /usr/local/lib/vmgl/libGL.so
/sbin/ldconfig

%postun guest
rm -rf /usr/local/lib/vmgl
/sbin/ldconfig

%files host
%defattr(-, root, root)

%docdir /usr/share/doc/vmgl
/usr/share/doc/vmgl

/usr/local/bin/vncviewer
/usr/local/bin/glstub
/usr/local/bin/stub-daemon
/usr/local/lib/vmgl/libcrutil.so
/usr/local/lib/vmgl/libspuload.so
/usr/local/lib/vmgl/librenderspu.so
/usr/local/lib/vmgl/liberrorspu.so
/etc/ld.so.conf.d/vmgl.conf

%files guest
%defattr(-, root, root)

%docdir /usr/share/doc/vmgl
/usr/share/doc/vmgl

/usr/local/bin/Xvnc
/usr/local/lib/vmgl/libcrutil.so
/usr/local/lib/vmgl/libspuload.so
/usr/local/lib/vmgl/libvmgl.so
/usr/local/lib/vmgl/libfeedbackspu.so
/usr/local/lib/vmgl/libarrayspu.so
/usr/local/lib/vmgl/libpackspu.so
/usr/local/lib/vmgl/liberrorspu.so
/usr/local/lib/vmgl/libpassthroughspu.so
/usr/lib/xorg/modules/extensions/libvmglext.so
/usr/lib/xorg/modules/extensions/libvmglext.7.1.so
/etc/ld.so.conf.d/vmgl.conf

%changelog
* Tue Feb 13 2007 H. Andres Lagar-Cavilla <andreslc@cs.toronto.edu>
- Still battling, 0.2

* Tue Sep 14 2006 H. Andres Lagar-Cavilla <andreslc@cs.toronto.edu>
- Fixes, still 0.1

* Mon Aug 29 2006 H. Andres Lagar-Cavilla <andreslc@cs.toronto.edu>
- Initial version
