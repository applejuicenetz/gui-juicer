/***************************************************************************
 *   Copyright (C) 2004 by Matthias Reif                                   *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef TYPES_H_12345678901
#define TYPES_H_12345678901

#define DOWN_PAUSED "18"
#define DOWN_FINISHED "14"
#define DOWN_CANCELD "17"
#define DOWN_FINISHING "12"
#define DOWN_SEARCHING "0"
#define DOWN_LOADING "-1"

#define ACTIVE_SOURCE "7"
#define QUEUED_SOURCE "5"
#define NEW_SOURCE "-1"

#define ACTIVE_UPLOAD "1"
#define QUEUEING_UPLOAD "2"
#define NEW_UPLOAD "-1"

// download col managment
#define NUM_DOWN_COL 11
#define FILENAME_DOWN_INDEX 0
#define SOURCES_DOWN_INDEX 1
#define SPEED_DOWN_INDEX 2
#define STATUS_DOWN_INDEX 3
#define FINISHED_DOWN_INDEX 4
#define POWER_DOWN_INDEX 5
#define SIZE_DOWN_INDEX 6
#define FINISHED_SIZE_DOWN_INDEX 7
#define REMAIN_SIZE_DOWN_INDEX 8
#define REMAIN_TIME_DOWN_INDEX 9
#define MISSING_DOWN_INDEX 10
// #define PRIORITY_DOWN_INDEX 5
// user col managmet
#define NUM_USER_COL 2
#define STATUS_USER_INDEX 0
#define POWER_USER_INDEX 1
// upload col managmet
#define NUM_UP_COL 7
#define FILENAME_UP_INDEX 0
#define NICK_UP_INDEX 1
#define SPEED_UP_INDEX 2
#define STATUS_UP_INDEX 3
#define PRIORITY_UP_INDEX 4
#define OS_UP_INDEX 5
#define DIRECTSTATE_UP_INDEX 6
// search col managmet
#define NUM_SEARCH_COL 3
#define TEXT_SEARCH_INDEX 0
#define SIZE_SEARCH_INDEX 1
#define COUNT_SEARCH_INDEX 2
// sever col managment
#define NUM_SERVER_COL 5
#define NAME_SERVER_INDEX 0
#define HOST_SERVER_INDEX 1
#define PORT_SERVER_INDEX 2
#define LASTSEEN_SERVER_INDEX 3
#define TESTS_SERVER_INDEX 4
// share col managment
#define NUM_SHARE_COL 2
#define PATH_SHARE_INDEX 0
#define MODE_SHARE_INDEX 1
// shared files col managment
#define NUM_SHARED_FILES_COL 1
#define FILENAME_SHARED_FILES_INDEX 0
#define NUM_SHARED_FILES_OVERVIEW_COL 3
#define FILENAME_SHARED_FILE_INDEX 0
#define SIZE_SHARED_FILE_INDEX 1
#define PRIORITY_SHARED_FILE_INDEX 2
// ftp col management
#define NUM_INCOMING_COL 3
#define FILENAME_INCOMING_INDEX 0
#define SIZE_INCOMING_INDEX 1
#define DATE_INCOMING_INDEX 2

// filesystem type
#define FILESYSTEM_TYPE_WORKSTATION 1
#define FILESYSTEM_TYPE_DRIVE 2
#define FILESYSTEM_TYPE_DISKDRIVE 3
#define FILESYSTEM_TYPE_FOLDER 4
#define FILESYSTEM_TYPE_DESKTOP 5

#define CONNECTED_SINCE "0"
#define CORE_VERSION "1"
#define DOWNSTREAM "2"
#define UPSTREAM "3"
#define DOWNLOADED "4"
#define UPLOADED "5"
#define CREDITS "6"

#define ONE_DAY 86400
#define ONE_HOUR 3600
#define ONE_MINUTE 60

#define ONE_GIG 1073741824
#define ONE_MEG 1048576
#define ONE_KILO 1024

#define WINDOWS "1"
#define LINUX "2"
#define MAC "3"
#define SOLARIS "4"
#define FREEBSD "6"
#define NETWARE "7"

#ifdef Q_WS_WIN
    #define DEFAULT_LAUNCHER WIN_LAUNCHER
#else
#ifdef Q_WS_MAC
    #define DEFAULT_LAUNCHER MAC_LAUNCHER
#else
    #define DEFAULT_LAUNCHER KDE_LAUNCHER
#endif
#endif


#define WIN_LAUNCHER "Windows default"
#define MAC_LAUNCHER "MacOS default"
#define KDE_LAUNCHER "kfmclient (KDE)"
#define GNOME_LAUNCHER "gnome-open (Gnome)"

#include <QString>
#include <QStringList>
#include <QFile>
#include <QVariant>

class AjSettings {
    public:
    enum LOCATION{SAME=0,SPECIFIC=1,FTP=2};
    QString nick, tcpPort, xmlPort, incomingDir, tempDir, autoconnect;
    QString maxDown, maxUp, maxCon, maxSlot, maxSources, maxNewCon;
};

class Part {
    public:
    qulonglong fromPosition;
    int type;
};

class QAjDownloadItem;
class QAjUserItem;
class DownloadUser {
    public:
    DownloadUser() {download = NULL; user = NULL; }
    QAjDownloadItem* download;
    QAjUserItem* user;
};

class StoreInfo {
    public:
    QString srcFile;
    QFile* dstFile;
};


#endif
