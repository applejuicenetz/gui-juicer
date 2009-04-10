# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../bin/ajqt4gui

FORMS += optionsdialogbase.ui  \
networkdialogbase.ui \
logindialogbase.ui \
 handlerdialogbase.ui \
 partlistdialogbase.ui \
 mainwindowbase.ui \
 shareselectiondialogbase.ui \
 helpdialogbase.ui \
 updatedialogbase.ui \
 targetfolderdialogbase.ui
HEADERS += ftp.h \
           global.h \
           md5class.h \
           md5.h \
           application.h \
           downloaditem.h \
           item.h \
           optionsdialog.h \
           partswidget.h \
           searchitem.h \
           serversocket.h \
           shareitem.h \
           socket.h \
           uploaditem.h \
           useritem.h \
           qconvert.h \
           qxmlmodule.h \
           juicer.h \
           networkdialog.h \
           searchentryitem.h \
           serveritem.h \
           incomingitem.h \
           sharefileitem.h \
           iconwidget.h \
           logindialog.h \
           powerspin.h \
 handlerdialog.h \
 partlistdialog.h \
 downloadmodule.h \
 uploadmodule.h \
 searchmodule.h \
 modulebase.h \
 servermodule.h \
 sharemodule.h \
 incomingmodule.h \
 shareselectiondialog.h \
 helpdialog.h \
 autoupdate.h \
 updatedialog.h \
 targetfolderdialog.h
SOURCES += ftp.cpp \
           main.cpp \
           md5c.c \
           md5class.cpp \
           application.cpp \
           downloaditem.cpp \
           item.cpp \
           optionsdialog.cpp \
           partswidget.cpp \
           searchitem.cpp \
           serversocket.cpp \
           shareitem.cpp \
           socket.cpp \
           uploaditem.cpp \
           useritem.cpp \
           qconvert.cpp \
           qxmlmodule.cpp \
           juicer.cpp \
           networkdialog.cpp \
           searchentryitem.cpp \
           serveritem.cpp \
           incomingitem.cpp \
           sharefileitem.cpp \
           iconwidget.cpp \
           logindialog.cpp \
           powerspin.cpp \
 handlerdialog.cpp \
 partlistdialog.cpp \
 downloadmodule.cpp \
 uploadmodule.cpp \
 searchmodule.cpp \
 modulebase.cpp \
 servermodule.cpp \
 sharemodule.cpp \
 incomingmodule.cpp \
 shareselectiondialog.cpp \
 helpdialog.cpp \
 autoupdate.cpp \
 updatedialog.cpp \
 targetfolderdialog.cpp
CONFIG += warn_on \
          qt \
          stl \
 debug \
 assistant
TEMPLATE = app
QT += core gui network xml \
 webkit

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
resources/fileopen.png \
resources/juicer.png \
resources/launch.png \
resources/network.png \
resources/partlist.png \
resources/partlist2.png \
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
resources/help.png \
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
 resources/small/desktop.png \
 resources/small/folder.png \
 resources/small/floppy.png \
 resources/small/hdd.png \
 resources/small/system.png \
 resources/options/de.png \
 resources/options/fonts.png \
 resources/options/gb.png \
 resources/options/kcmsystem.png \
 resources/options/launch.png \
 resources/options/network_local.png \
 resources/options/player_playlist.png \
 resources/options/superkaramba.png \
 resources/options/undo.png \
 resources/options/viewmag.png

TRANSLATIONS += juicer_de.ts

TARGET = juicer

CONFIG -= release

INSTALLS += target

target.path = /usr/local/bin

TARGETDEPS += ../quazip/libquazip.a

INCLUDEPATH += ../quazip

LIBS += ../quazip/libquazip.a

