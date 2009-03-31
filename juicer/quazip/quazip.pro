TEMPLATE = lib
CONFIG += qt warn_on \
 staticlib
QT -= gui
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += crypt.h \
           ioapi.h \
           quazip.h \
           quazipfile.h \
           quazipfileinfo.h \
           quazipnewinfo.h \
           unzip.h \
           zip.h \
           zlib/crc32.h \
           zlib/deflate.h \
           zlib/inffast.h \
           zlib/inffixed.h \
           zlib/inflate.h \
           zlib/inftrees.h \
           zlib/trees.h \
           zlib/zconf.h \
           zlib/zconf.in.h \
           zlib/zlib.h \
           zlib/zutil.h

SOURCES += ioapi.c \
           quazip.cpp \
           quazipfile.cpp \
           quazipnewinfo.cpp \
           unzip.c \
           zip.c \
           zlib/compress.c \
           zlib/adler32.c \
           zlib/crc32.c \
           zlib/deflate.c \
           zlib/gzio.c \
           zlib/infback.c \
           zlib/inffast.c \
           zlib/inflate.c \
           zlib/inftrees.c \
           zlib/minigzip.c \
           zlib/trees.c \
           zlib/uncompr.c \
           zlib/zutil.c

