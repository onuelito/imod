# Directories
PROTOCOLS=wayland-protocols
PROTOCOLS_INCLUDE=include/protocols
PROTOCOLS_SOURCES=$(PROTOCOLS)/src


all:
	mkdir -p $(PROTOCOLS_INCLUDE)
	mkdir -p $(PROTOCOLS_SOURCES)
	wayland-scanner client-header $(PROTOCOLS)/xdg-shell.xml $(PROTOCOLS_INCLUDE)/xdg-shell-client-protocol.h
	wayland-scanner public-code $(PROTOCOLS)/xdg-shell.xml $(PROTOCOLS_SOURCES)/xdg-shell-client-protocol.c
	wayland-scanner client-header $(PROTOCOLS)/xdg-decoration-unstable-v1.xml $(PROTOCOLS_INCLUDE)/xdg-decoration-unstable-v1-protocol.h
	wayland-scanner public-code $(PROTOCOLS)/xdg-decoration-unstable-v1.xml $(PROTOCOLS_SOURCES)/xdg-decoration-unstable-v1-protocol.c

