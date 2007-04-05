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

#define ACTIVE_SOURCE 7
#define QUEUED_SOURCE 5
#define NEW_SOURCE -1

#define ACTIVE_UPLOAD 1
#define QUEUEING_UPLOAD 2
#define NEW_UPLOAD -1

// download col managment
#define NUM_DOWN_COL 12
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
#define ID_DOWN_INDEX NUM_DOWN_COL - 1
// user col managmet
#define NUM_USER_COL 3
#define STATUS_USER_INDEX 0
#define POWER_USER_INDEX 1
#define ID_USER_INDEX NUM_USER_COL - 1
// upload col managmet
#define NUM_UP_COL 9
#define FILENAME_UP_INDEX 0
#define NICK_UP_INDEX 1
#define SPEED_UP_INDEX 2
#define STATUS_UP_INDEX 3
#define PRIORITY_UP_INDEX 4
#define OS_UP_INDEX 5
#define DIRECTSTATE_UP_INDEX 6
#define SHAREID_UP_INDEX NUM_UP_COL - 2
#define ID_UP_INDEX NUM_UP_COL - 1
// search col managmet
#define NUM_SEARCH_COL 5
#define TEXT_SEARCH_INDEX 0
#define SIZE_SEARCH_INDEX 1
#define COUNT_SEARCH_INDEX 2
#define CHECKSUM_SEARCH_INDEX 3
#define ID_SEARCH_INDEX NUM_SEARCH_COL - 1
// sever col managment
#define NUM_SERVER_COL 6
#define NAME_SERVER_INDEX 0
#define HOST_SERVER_INDEX 1
#define PORT_SERVER_INDEX 2
#define LASTSEEN_SERVER_INDEX 3
#define TESTS_SERVER_INDEX 4
#define ID_SERVER_INDEX NUM_SERVER_COL - 1
// share col managment
#define NUM_SHARE_COL 3
#define PATH_SHARE_INDEX 0
#define MODE_SHARE_INDEX 1
#define ID_SHARE_INDEX NUM_SHARE_COL - 1
// shared files col managment
#define NUM_SHARED_FILES_COL 2
#define FILENAME_SHARED_FILES_INDEX 0
#define ID_SHARED_FILES_INDEX NUM_SHARE_COL - 1
// ftp col management
#define NUM_FTP_COL 3
#define FILENAME_FTP_INDEX 0
#define SIZE_FTP_INDEX 1
#define ID_FTP_INDEX NUM_FTP_COL - 1



#define ONE_DAY 86400
#define ONE_HOUR 3600
#define ONE_MINUTE 60

#define ONE_GIG 1073741824
#define ONE_MEG 1048576
#define ONE_KILO 1024


#define WINDOWS "1"
#define LINUX "2"

#define WINDOWS_INT 1
#define LINUX_INT 2

#define DOWN_PAUSED 18
#define DOWN_FINISHED 14
#define DOWN_CANCELD 17
#define DOWN_SEARCHING 0
#define DOWN_LOADING 4

#include <QString>

typedef struct{
	QString nick, password, coreAddress, tcpPort, xmlPort, incomingDir, tempDir, autoconnect, savePassword;
	QString maxDown, maxUp, maxCon, maxSlot, maxSources, maxNewCon, allowBrowse;
	QString serverURL;
	int refresh;

	QString ftpServer, ftpPort, ftpUser, ftpPassword, ftpDir;
} AjSettings;

#endif
