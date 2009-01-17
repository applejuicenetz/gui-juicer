# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/ajqt4gui

FORMS += qajoptionsdialogbase.ui  \
qajnetworkdialogbase.ui \
qajlogindialogbase.ui \
 qajhandlerdialogbase.ui \
 qajpartlistdialogbase.ui \
 qajmainwindowbase.ui \
 qajshareselectiondialogbase.ui
HEADERS += ftp.h \
           global.h \
           md5class.h \
           md5.h \
           qajapplication.h \
           qajdownloaditem.h \
           qajitem.h \
           qajoptionsdialog.h \
           qajpartswidget.h \
           qajsearchitem.h \
           qajserversocket.h \
           qajshareitem.h \
           qajsocket.h \
           qajuploaditem.h \
           qajuseritem.h \
           qconvert.h \
           qxmlmodule.h \
           juicer.h \
           qajnetworkdialog.h \
           qajsearchentryitem.h \
           qajserveritem.h \
           qajincomingitem.h \
           qajsharefileitem.h \
           qajiconwidget.h \
           qajlogindialog.h \
           qajpowerspin.h \
 qajhandlerdialog.h \
 qajpartlistdialog.h \
 qajdownloadmodule.h \
 qajuploadmodule.h \
 qajsearchmodule.h \
 qajmodulebase.h \
 qajservermodule.h \
 qajsharemodule.h \
 qajincomingmodule.h \
 qajshareselectiondialog.h
SOURCES += ftp.cpp \
           main.cpp \
           md5c.c \
           md5class.cpp \
           qajapplication.cpp \
           qajdownloaditem.cpp \
           qajitem.cpp \
           qajoptionsdialog.cpp \
           qajpartswidget.cpp \
           qajsearchitem.cpp \
           qajserversocket.cpp \
           qajshareitem.cpp \
           qajsocket.cpp \
           qajuploaditem.cpp \
           qajuseritem.cpp \
           qconvert.cpp \
           qxmlmodule.cpp \
           juicer.cpp \
           qajnetworkdialog.cpp \
           qajsearchentryitem.cpp \
           qajserveritem.cpp \
           qajincomingitem.cpp \
           qajsharefileitem.cpp \
           qajiconwidget.cpp \
           qajlogindialog.cpp \
           qajpowerspin.cpp \
 qajhandlerdialog.cpp \
 qajpartlistdialog.cpp \
 qajdownloadmodule.cpp \
 qajuploadmodule.cpp \
 qajsearchmodule.cpp \
 qajmodulebase.cpp \
 qajservermodule.cpp \
 qajsharemodule.cpp \
 qajincomingmodule.cpp \
 qajshareselectiondialog.cpp
CONFIG += warn_on \
          qt \
          stl \
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
 resources/small/undo.png \
 resources/small/view_tree.png \
 resources/small/view_detailed.png \
 resources/options/de.png \
 resources/options/fonts.png \
 resources/options/gb.png \
 resources/options/kcmsystem.png \
 resources/options/launch.png \
 resources/options/network_local.png \
 resources/options/player_playlist.png \
 resources/options/superkaramba.png \
 resources/small/desktop.png \
 resources/small/folder.png \
 resources/small/floppy.png \
 resources/small/hdd.png \
 resources/small/system.png

TRANSLATIONS += juicer_de.ts

TARGET = juicer

CONFIG -= release

INSTALLS += target

target.path = /usr/local/bin

