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
#ifndef QXMLMODULE_H
#define QXMLMODULE_H

using namespace std;
#include <list>

#include <QObject>
#include <QtXml>
#include <QHttp>
#include <QIODevice>
#include <QBuffer>
#include <QStringList>

#include <QRegExp>

#include "qxmlhandler.h"

#include "qajtab.h"
#include "types.h"
#include "md5class.h"
#include "qconvert.h"

#include "qajpart.h"

#define MAX_ACTIVE 32
#define SHARE 0
#define REMOVED 1
#define IDS 3
#define APPLEJUICE 4
#define SHARES 5

#define GET_SESSION_XML 0
#define GET_SETTINGS_XML 1
#define MODIFIED_XML 2
#define INFORMATION_XML 3
#define DOWNLOAD_XML 4
#define UPLOAD_XML 5
#define IDS_XML 6
#define GET_OBJECT_XML 7
#define SET_POWER_XML 8
#define PROCESS_LINK_XML 9
#define SET_SETTINGS_XML 10
#define USER_XML 11
#define CANCEL_DOWNLOAD_XML 12
#define CLEAN_DOWNLOAD_XML 13
#define PAUSE_DOWNLOAD_XML 14
#define RESUME_DOWNLOAD_XML 15
#define REMOVE_SERVER_XML 16
#define CONNECT_SERVER_XML 17
#define SEARCH_XML 18
#define CANCEL_SEARCH_XML 19
#define EXIT_CORE_XML 20
#define EVERYTHING_XML 21
#define SERVER_XML 22
#define SHARE_XML 23
#define EXTRA_INFORMATION_XML 24
#define DOWNLOAD_PARTLIST_XML 25
#define DOWNLOAD_PARTLIST_SIMPLE_XML 26
#define RENAME_DOWNLOAD_XML 27

#define SELF_DESTRUCT true

#include <QDomDocument>

/**
@author Matthias Reif
*/

class AjQtGUI;
class QAjTab;
class QXMLHandler;

class QXMLModule : public QHttp
{
Q_OBJECT
public:
	QXMLModule(AjQtGUI *ajQtGUI, QAjTab *ajTab, QObject *parent = 0, const char *name = 0 );

	~QXMLModule();
 
	int setHost ( const QString & hostname, quint16 portnumber = 80 );

	bool startDocument();
	bool endDocument();
	bool startElement( const QString&, const QString&, const QString& , const QXmlAttributes& );
	bool endElement( const QString&, const QString&, const QString& );
	bool characters( const QString& s1 );
	int get( int getCode, QString param = "" );
	int set( int setCode, QString param = "" );
//protected:
	QXmlInputSource *inputSource;
	QXmlSimpleReader *simpleReader;
	QXmlAttributes *attributes;
	QXMLHandler *handler;
	QString session;

//	void saveSettings( AjSettings settings );
	void setPassword( QString password );
	void setPasswordMD5( QString passwordMD5_ ) { passwordMD5 = passwordMD5_; }
	
	QString getPasswordMD5( ) { return passwordMD5; }
	QString getHost( ) { return host; }
	quint16 getPort( ) { return port; }
	
	void setDoList( bool doIt = true ) { doList = doIt; }
	void setIds( QStringList ids ) { idList = ids; }
	
	bool isParsing() { return parsing; }
	
	int countElements( QString xmlString );

protected:
	int userCnt;

	QBuffer *buffer;
	QString name;
	QString request;
	
	QString host;
	quint16 port;
	
	bool doList;
	bool parsing;
	bool firstDoc;

	bool active[MAX_ACTIVE];
	int shareCnt;
	QString currSearchEntryId;
	AjQtGUI *ajQtGUI;
	QAjTab *ajTab;
	QString timeStamp, downTimeStamp, upTimeStamp, infoTimeStamp, searchTimeStamp, serverTimeStamp;
	QString *currTimeStamp;
	QString passwordMD5;
	
	AjSettings settings;
	
	int objectCnt;

	QStringList idList;
	
	list<QAjPart*> partList;
	qulonglong partsSize;
	map<int, QString> partListRequests;
	map<int, QString> partListSimpleRequests;

public slots:
	void responseHeaderReceived ( const QHttpResponseHeader & resp );
	void requestFinished(int id, bool error);
	void requestStarted(int id);
	void readyRead ( const QHttpResponseHeader & resp );
	void dataReadProgress ( int done, int total );
	
signals:
	void settingsReady( AjSettings settings );
	void error( int code );
	
	void gotIds( QStringList ids );
	
	void modifiedDone();
};

#endif
