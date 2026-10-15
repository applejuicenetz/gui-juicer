TEMPLATE = app
TARGET = updatecheck_test
QT += core gui network
INCLUDEPATH += ../src
SOURCES += updatecheck_test.cpp ../src/updatecheck.cpp ../src/convert.cpp
HEADERS += ../src/updatecheck.h ../src/convert.h
