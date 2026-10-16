TEMPLATE = app
TARGET = serverlist_test
QT += core network
QT -= gui
INCLUDEPATH += ../src
SOURCES += serverlist_test.cpp ../src/serverlist.cpp
HEADERS += ../src/serverlist.h
