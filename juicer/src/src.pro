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
          stl \
          releae \
 debug
TEMPLATE = app
QT += core gui network xml

RESOURCES += images.qrc \
translations.qrc
RC_FILE = resources/juicer.rc


DISTFILES += resources/juicer.rc \
 resources/translations/juicer_de.qm \
resources/button_ok.png \
resources/configure.png \
resources/connect_creating.png \
resources/dummy.png \
resources/editpaste.png \
resources/edit.png \
resources/emptytrash.png \
resources/exec.png \
resources/filesave.png \
resources/find.png \
resources/folder_open.png \
resources/juicer.png \
resources/launch.png \
resources/network.png \
resources/partlist.png \
resources/pause_fade.png \
resources/player_pause.png \
resources/player_play.png \
resources/reload.png \
resources/show_table_column.png \
resources/splash.png \
resources/stop.png \
resources/text_block.png \
resources/toggle_log.png \
resources/vcs_add.png \
resources/vcs_commit.png \
resources/vcs_remove.png \
resources/vcs_update.png \
resources/wizard.png \
 resources/small/arrow_down.png \
 resources/small/arrow_up.png \
 resources/small/basket_put.png \
 resources/small/basket_remove.png \
 resources/small/clock.png \
 resources/small/coins.png \
 resources/small/configure.png \
 resources/small/connect_creating.png \
 resources/small/connect_established.png \
 resources/small/down.png \
 resources/small/editpaste.png \
 resources/small/edit.png \
 resources/small/exec.png \
 resources/small/exit.png \
 resources/small/fileclose.png \
 resources/small/filesave.png \
 resources/small/find.png \
 resources/small/folder_open.png \
 resources/small/freebsd.png \
 resources/small/help.png \
 resources/small/launch.png \
 resources/small/mac.png \
 resources/small/netware.png \
 resources/small/network_local.png \
 resources/small/network.png \
 resources/small/ok.png \
 resources/small/partlist.png \
 resources/small/player_pause.png \
 resources/small/player_play.png \
 resources/small/server.png \
 resources/small/solaris.png \
 resources/small/stop.png \
 resources/small/text_block.png \
 resources/small/toggle_log.png \
 resources/small/trashcan_empty.png \
 resources/small/tux.png \
 resources/small/up.png \
 resources/small/vcs_add.png \
 resources/small/vcs_commit.png \
 resources/small/vcs_remove.png \
 resources/small/vcs_update.png \
 resources/small/viewmag.png \
 resources/small/windows.png \
 resources/options/de.png \
 resources/options/fonts.png \
 resources/options/gb.png \
 resources/options/kcmsystem.png \
 resources/options/launch.png \
 resources/options/network_local.png \
 resources/options/player_playlist.png \
 resources/options/superkaramba.png
TARGET = juicer

INSTALLS += TARGET \
 target

TRANSLATIONS += juicer_de.ts

CONFIG -= release

target.path = /usr/local/bin/

