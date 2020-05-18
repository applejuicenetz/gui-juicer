
QT += ftp
QT += widgets

TEMPLATE = subdirs 
SUBDIRS = src
CONFIG += warn_on \
          qt \
          thread \
          release
