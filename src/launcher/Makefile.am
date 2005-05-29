bomnslauncher_SOURCES = \
  callbacks.c \
  interface.c \
  main.c \
  support.c

# Build the launcher?
if BUILD_LAUNCHER1

bin_PROGRAMS = bomnslauncher

INCLUDES = `gtk-config --cflags`

bomnslauncher_LDFLAGS = `gtk-config --libs`

endif

EXTRA_DIST = \
  callbacks.h \
  interface.h \
  support.h
