launcher_SOURCES = \
  callbacks.c \
  interface.c \
  main.c \
  support.c

# Build the launcher?
if BUILD_LAUNCHER

bin_PROGRAMS = launcher

INCLUDES = `gtk-config --cflags`

launcher_LDFLAGS = `gtk-config --libs`

endif

EXTRA_DIST = \
  callbacks.h \
  interface.h \
  main.h \
  support.h
