# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/ajqt4gui

FORMS += qajoptionsdialogbase.ui  \
qajnetworkdialogbase.ui
HEADERS += ftp.h \
           global.h \
           md5class.h \
           md5.h \
           qajapplication.h \
           qajdownloaditem.h \
           qajdownloadwidget.h \
           qajftpwidget.h \
           qajitem.h \
           qajlistwidget.h \
           qajlogindialog.h \
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
           ui_qajoptionsdialogbase.h \
           juicer.h \
           qajnetworkdialog.h
SOURCES += ftp.cpp \
           main.cpp \
           md5c.c \
           md5class.cpp \
           qajapplication.cpp \
           qajdownloaditem.cpp \
           qajdownloadwidget.cpp \
           qajftpwidget.cpp \
           qajitem.cpp \
           qajlistwidget.cpp \
           qajlogindialog.cpp \
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
           qajnetworkdialog.cpp
TARGET = ../bin/ajqt4gui 
CONFIG += warn_on \
          qt \
          x11 \
          stl \
          debug
TEMPLATE = app 
QT += core gui network xml

RESOURCES += images.qrc

