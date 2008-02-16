# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/ajqt4gui

FORMS += qajoptionsdialogbase.ui  \
qajnetworkdialogbase.ui \
qajlogindialogbase.ui
HEADERS += ftp.h \
           global.h \
           md5class.h \
           md5.h \
           qajapplication.h \
           qajdownloaditem.h \
           qajdownloadwidget.h \
           qajitem.h \
           qajlistwidget.h \
           qajoptionsdialog.h \
           qajpartlistwidget.h \
           qajpartswidget.h \
           qajsearchitem.h \
           qajsearchwidget.h \
           qajserversocket.h \
           qajserverwidget.h \
           qajshareitem.h \
           qajsharewidget.h \
           qajsocket.h \
           qajuploaditem.h \
           qajuploadwidget.h \
           qajuseritem.h \
           qconvert.h \
           qxmlmodule.h \
           types.h \
           juicer.h \
           qajnetworkdialog.h \
           qajservermetawidget.h \
           qajincomingwidget.h \
           qajsearchentryitem.h \
           qajserveritem.h \
           qajincomingitem.h \
           qajsharemetawidget.h \
           qajsharefileswidget.h \
           qajsharefileitem.h \
           qajfiledialog.h \
           qajfileitem.h\
           qajiconwidget.h \
           qajfilewidget.h \
           qajlogindialog.h \
           qajpowerspin.h
SOURCES += ftp.cpp \
           main.cpp \
           md5c.c \
           md5class.cpp \
           qajapplication.cpp \
           qajdownloaditem.cpp \
           qajdownloadwidget.cpp \
           qajitem.cpp \
           qajlistwidget.cpp \
           qajoptionsdialog.cpp \
           qajpartlistwidget.cpp \
           qajpartswidget.cpp \
           qajsearchitem.cpp \
           qajsearchwidget.cpp \
           qajserversocket.cpp \
           qajserverwidget.cpp \
           qajshareitem.cpp \
           qajsharewidget.cpp \
           qajsocket.cpp \
           qajuploaditem.cpp \
           qajuploadwidget.cpp \
           qajuseritem.cpp \
           qconvert.cpp \
           qxmlmodule.cpp \
           juicer.cpp \
           qajnetworkdialog.cpp \
           qajservermetawidget.cpp \
           qajincomingwidget.cpp \
           qajsearchentryitem.cpp \
           qajserveritem.cpp \
           qajincomingitem.cpp \
           qajsharemetawidget.cpp \
           qajsharefileswidget.cpp \
           qajsharefileitem.cpp \
           qajfiledialog.cpp \
           qajfileitem.cpp \
           qajiconwidget.cpp \
           qajlogindialog.cpp \
           qajfilewidget.cpp \
           qajpowerspin.cpp
CONFIG += warn_on \
          qt \
          x11 \
          stl \
          debug \
 console
TEMPLATE = app 
QT += core gui network xml

RESOURCES += images.qrc \
translations.qrc
RC_FILE = resources/juicer.rc
TARGET = ../bin/juicer

TRANSLATIONS += juicer_de.ts

DISTFILES += resources/juicer.rc

