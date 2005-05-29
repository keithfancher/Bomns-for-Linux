## Process this file with automake to produce Makefile.in

bomnslauncher2_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h

# build GTK2 launcher?
if BUILD_LAUNCHER2

bin_PROGRAMS = bomnslauncher2

INCLUDES = @PACKAGE_CFLAGS@

bomnslauncher2_LDADD = @PACKAGE_LIBS@

endif

EXTRA_DIST = \
	bomnslauncher2.glade \
	bomnslauncher2.gladep

