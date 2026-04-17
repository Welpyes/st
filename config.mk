# st version
VERSION = 0.9.2

# Customize below to fit your system

# detect Android/Termux
UNAME := $(shell uname -o 2>/dev/null || uname -s)

ifeq ($(UNAME),Android)
SHMEMU_INCS = `$(PKG_CONFIG) --cflags shmemu`
SHMEMU_LIBS = -lshmemu `$(PKG_CONFIG) --libs shmemu` # or however shmemu links
SHMEMU_FLAGS = -include shmemu.h
PREFIX = /data/data/com.termux/files/usr
else
PREFIX = /usr/local
SHMEMU_INCS =
SHMEMU_LIBS =
SHMEMU_FLAGS =
endif

# paths
APPPREFIX = $(PREFIX)/share/applications
MANPREFIX = $(PREFIX)/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

PKG_CONFIG = pkg-config

# includes and libs
INCS = -I$(X11INC) \
       `$(PKG_CONFIG) --cflags imlib2` \
       `$(PKG_CONFIG) --cflags fontconfig` \
       `$(PKG_CONFIG) --cflags freetype2` \
			 $(SHMEMU_INCS) \
       `$(PKG_CONFIG) --cflags harfbuzz`
LIBS = -L$(X11LIB) -lm -lX11 -lutil -lXft -lXrender\
       `$(PKG_CONFIG) --libs imlib2` \
       `$(PKG_CONFIG) --libs zlib` \
			 $(SHMEMU_LIBS) \
       `$(PKG_CONFIG) --libs fontconfig` \
       `$(PKG_CONFIG) --libs freetype2` \
       `$(PKG_CONFIG) --libs harfbuzz`

# flags
STCPPFLAGS = -DVERSION=\"$(VERSION)\" -D_XOPEN_SOURCE=600 $(SHMEMU_FLAGS)
STCFLAGS = $(INCS) $(STCPPFLAGS) $(CPPFLAGS) $(CFLAGS)
STLDFLAGS = $(LIBS) $(LDFLAGS)

# OpenBSD:
#CPPFLAGS = -DVERSION=\"$(VERSION)\" -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
#LIBS = -L$(X11LIB) -lm -lX11 -lutil -lXft \
#       `$(PKG_CONFIG) --libs fontconfig` \
#       `$(PKG_CONFIG) --libs freetype2`
#MANPREFIX = ${PREFIX}/man

# compiler and linker
CC = gcc
