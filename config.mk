# st version
VERSION = 0.9.3

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man
ICONPREFIX = $(PREFIX)/share/pixmaps
ICONNAME = st.png

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

PKG_CONFIG = pkg-config

# Uncomment this for the alpha patch / ALPHA_PATCH
#XRENDER = `$(PKG_CONFIG) --libs xrender`

# Uncomment this for the themed cursor patch / THEMED_CURSOR_PATCH
XCURSOR = `$(PKG_CONFIG) --libs xcursor`

# Uncomment the lines below for the ligatures patch / LIGATURES_PATCH
LIGATURES_C = hb.c
LIGATURES_H = hb.h

# Uncomment this for the SIXEL patch / SIXEL_PATCH
SIXEL_C = sixel.c sixel_hls.c

# Uncomment for the netwmicon patch / NETWMICON_PATCH
#NETWMICON_LIBS = `$(PKG_CONFIG) --libs gdlib`

# includes and libs, uncomment harfbuzz for the ligatures patch
IS_ANDROID = $(shell uname -o | grep -q Android && echo 1 || echo 0)
ifeq ($(IS_ANDROID),1)
LIGATURES_INC = -I/data/data/com.termux/files/usr/include/harfbuzz
LIGATURES_LIBS = -lharfbuzz
SIXEL_INC = `$(PKG_CONFIG) --cflags imlib2`
SIXEL_LIBS = `$(PKG_CONFIG) --libs imlib2`
else
LIGATURES_INC = `$(PKG_CONFIG) --cflags harfbuzz`
LIGATURES_LIBS = `$(PKG_CONFIG) --libs harfbuzz`
SIXEL_INC = `$(PKG_CONFIG) --cflags imlib2`
SIXEL_LIBS = `$(PKG_CONFIG) --libs imlib2`
endif

INCS = -I$(X11INC) \
       $(SIXEL_INC) \
       `$(PKG_CONFIG) --cflags fontconfig` \
       `$(PKG_CONFIG) --cflags freetype2` \
       $(LIGATURES_INC)
LIBS = -L$(X11LIB) -lm -lrt -lX11 -lutil -lXft -lXrender ${SIXEL_LIBS} ${XRENDER} ${XCURSOR}\
       `$(PKG_CONFIG) --libs zlib` \
       `$(PKG_CONFIG) --libs fontconfig` \
       `$(PKG_CONFIG) --libs freetype2` \
       $(LIGATURES_LIBS) \
       $(NETWMICON_LIBS)

# flags
STCPPFLAGS = -DVERSION=\"$(VERSION)\" -DICON=\"$(ICONPREFIX)/$(ICONNAME)\" -D_XOPEN_SOURCE=600
STCFLAGS = $(INCS) $(STCPPFLAGS) $(CPPFLAGS) $(CFLAGS)
STLDFLAGS = $(LIBS) $(LDFLAGS)

# OpenBSD:
#CPPFLAGS = $(STCPPFLAGS) -D_XOPEN_SOURCE=600
#MANPREFIX = ${PREFIX}/man

# compiler and linker
# CC = c99
