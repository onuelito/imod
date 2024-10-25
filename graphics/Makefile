
# global variables
SOURCES=\
	src/window.c \

INCLUDE=\
	-I /usr/local/include \
	-I ../include \
	-I include \

LDFLAGS=-lm

LDPATHS=-L /usr/local/lib

# xlib exclusives
SOURCES_X11=\
	src/x11/window.c \
	src/x11/event.c \
	src/x11/data.c \

# wayland exclusives
SOURCES_WAYLAND=\
	src/wayland/listener.c \
	src/wayland/window.c \
	src/wayland/data.c \
	../wayland-protocols/src/xdg-shell-client-protocol.c \
	../wayland-protocols/src/xdg-decoration-unstable-v1-protocol.c \

SOURCES_OPENGL=\
	src/egl.c \

LDFLAGS_WAYLAND=-lwayland-client


# library location
LIBDIR=lib
LIBNAME=libimodgraphics

# Check for API support
OS_NAME !=uname -s


LIBEGL=
LIBGLESv3=
LIBVULKAN=

LIBX11 = -lX11
LIBWAYLAND = -lwayland-egl -lwayland-client

# FreeBSD variables
.if $(OS_NAME) == "FreeBSD"



# Graphics APIs
LIBVULKAN != [ -e "/usr/local/lib/libvulkan.so" ] && [ -e "/usr/local/include/vulkan" ] && echo -lvulkan
LIBGLESv3 != [ -e "/usr/local/lib/libGLESv2.so" ] && [ -e "/usr/local/include/GLES3" ] && echo -lGLESv2
LIBEGL != [ -e "/usr/local/lib/libEGL.so" ] && [ -e "/usr/local/include/EGL" ] && echo -lEGL

.if ${XDG_SESSION_TYPE} == "wayland"

SOURCES_X11=
LIBX11=

.elif $(XDG_SESSION_TYPE} == "x11"

SOURCES_WAYLAND=
LIBWAYLAND=

.endif

# OpenBSD variables (TBA)
# Note that OpenBSD does not support Wayland by the time 
# of writing this Makefile
.elif $(OS_NAME) == "OpenBSD"

INCLUDE+= -I /usr/X11R6/include
LDPATHS+= -L /usr/X11R6/lib

# OpenBSD does not support Wayland from what I know
# so removing these
LIBWAYLAND=
SOURCES_WAYLAND=

# Graphics APIs
LIBVULKAN != [ -e "/usr/X11R6/lib/libvulkan_radeon.so" ]  && [ -e "/usr/X11R6/include/vulkan" ] && echo -lvulkan_radeon
LIBVULKAN != [ -e "/usr/X11R6/lib/libvulkan_intel.so" ]  && [ -e "/usr/X11R6/include/vulkan" ] && echo $(LIBVULKAN) -lvulkan_intel
LIBGLESv3 != [ -e "/usr/X11R6/lib/libGLESv2.a" ] && [ -e "/usr/X11R6/include/GLES2" ] && echo -lGLESv2
LIBEGL != [ -e "/usr/X11R6/lib/libEGL.a" ] && [ -e "/usr/X11R6/include/EGL" ] && echo -lEGL

# NetBSD variables (TBA)
.elif $(OS_NAME) == "NetBSD"

.endif


all:
	@echo $(ME)
	mkdir -p $(LIBDIR)
	$(CC) -shared -fPIC -Wl,-soname,$(LIBNAME).so.1 -o $(LIBNAME).so.1.0.0 $(SOURCES) $(SOURCES_OPENGL) \
		$(SOURCES_WAYLAND) $(SOURCES_X11) $(INCLUDE) $(LDFLAGS) $(LIBVULKAN) $(LIBGLESv3) $(LIBEGL) \
		$(LIBWAYLAND) $(LIBX11) $(LDPATHS)
	ln -sf $(LIBNAME).so.1.0.0 $(LIBNAME).so.1
	ln -sf $(LIBNAME).so.1 $(LIBNAME).so
	mv $(LIBNAME)* $(LIBDIR)

clean:
	rm -rf $(LIBDIR)
