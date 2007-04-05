# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/ajqt4gui

FORMS += qajoptionsdialogbase.ui 
HEADERS += ajqtgui.h \
           ftp.h \
           global.h \
           md5class.h \
           md5.h \
           qajapplication.h \
           qajcommonoptionswidget.h \
           qajdescription.h \
           qajdownloaditem.h \
           qajdownloadwidget.h \
           qajftpwidget.h \
           qajicons.h \
           qajitem.h \
           qajlistwidget.h \
           qajlogindialog.h \
           qajnetoptionswidget.h \
           qajnetworkwidget.h \
           qajoptionsdialog.h \
           qajoptionswidget.h \
           qajpart.h \
           qajpartlistwidget.h \
           qajpartswidget.h \
           qajsearchitem.h \
           qajsearchwidget.h \
           qajserversocket.h \
           qajserverwidget.h \
           qajshareitem.h \
           qajshareoptionswidget.h \
           qajsharewidget.h \
           qajsocket.h \
           qajtab.h \
           qajuploaditem.h \
           qajuploadwidget.h \
           qajuseritem.h \
           qconvert.h \
           qxmlhandler.h \
           qxmlmodule.h \
           types.h \
           ui_qajoptionsdialogbase.h 
SOURCES += ajqtgui.cpp \
           ftp.cpp \
           main.cpp \
           md5c.c \
           md5class.cpp \
           qajapplication.cpp \
           qajcommonoptionswidget.cpp \
           qajdescription.cpp \
           qajdownloaditem.cpp \
           qajdownloadwidget.cpp \
           qajftpwidget.cpp \
           qajicons.cpp \
           qajitem.cpp \
           qajlistwidget.cpp \
           qajlogindialog.cpp \
           qajnetoptionswidget.cpp \
           qajnetworkwidget.cpp \
           qajoptionsdialog.cpp \
           qajoptionswidget.cpp \
           qajpart.cpp \
           qajpartlistwidget.cpp \
           qajpartswidget.cpp \
           qajsearchitem.cpp \
           qajsearchwidget.cpp \
           qajserversocket.cpp \
           qajserverwidget.cpp \
           qajshareitem.cpp \
           qajshareoptionswidget.cpp \
           qajsharewidget.cpp \
           qajsocket.cpp \
           qajtab.cpp \
           qajuploaditem.cpp \
           qajuploadwidget.cpp \
           qajuseritem.cpp \
           qconvert.cpp \
           qxmlhandler.cpp \
           qxmlmodule.cpp 
TARGET = ../bin/ajqt4gui 
CONFIG += release \
          warn_on \
          qt \
          x11 \
          stl 
TEMPLATE = app 
QT += core gui network xml

