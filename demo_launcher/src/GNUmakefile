# This is a GNU make file that uses GNU make extensions.

INSTALL_DIR = $(PREFIX)/bin

demo_launcher_SOURCES := demo_launcher.c

demo_launcher_CPPFLAGS := $(shell pkg-config --cflags gtk+-3.0)
demo_launcher_LDFLAGS := $(shell pkg-config --libs gtk+-3.0)

include quickbuild.make
