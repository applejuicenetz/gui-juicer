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

#include <QObject>
#include <QtXml>
#include <QHttp>
#include <QIODevice>
#include <QStringList>
#include <QLinkedList>
#include <QHash>
#include <QDomDocument>

#include "qajtab.h"
#include "types.h"
#include "md5class.h"
#include "qconvert.h"

#define GET_SESSION_XML 0
#define GET_SETTINGS_XML 1
#define MODIFIED_XML 2
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

#define SHARE_XML 23
#define EXTRA_INFORMATION_XML 24
#define DOWNLOAD_PARTLIST_XML 25
#define DOWNLOAD_PARTLIST_SIMPLE_XML 26
#define RENAME_DOWNLOAD_XML 27

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
 
	int setHost( const QString & hostname, quint16 portnumber = 80 );

	int get( int getCode, QString param = "" );
	int set( int setCode, QString param = "" );
	
   QString session;

	void setPassword( QString password );
	void setPasswordMD5( QString passwordMD5 ) { this->passwordMD5 = passwordMD5; }
	
protected:
	QString host;
	quint16 port;
	
	AjQtGUI *ajQtGUI;
	QAjTab *ajTab;
	QString timeStamp;
	QString passwordMD5;

    QDomDocument doc;

	QLinkedList<Part> partList;
	qulonglong partsSize;
	QHash<int, QString> partListRequests;
	QHash<int, QString> partListSimpleRequests;

public slots:
    void responseHeaderReceived ( const QHttpResponseHeader & resp );
    void requestFinished(int id, bool error);
    void httpDone(bool error);
signals:
    void settingsReady( AjSettings settings );
    void error( int code );
    void gotSession();
    void modifiedDone();
private:
    void handleSettings( QDomElement e );
    void handleShares( QDomNode node );
    void handleIds( QDomNode node );
    void handleNetworkInfo( QDomElement e );
    void handleUpload( QDomElement e );
    void handleDownload( QDomElement e );
    void handleUser( QDomElement e );
    void handleServer( QDomElement e );
    void handleSearch( QDomElement e );
    void handleSearchEntry( QDomElement e );
    void handleGeneralInformation( QDomNode node );
    void handleRemoved( QDomElement e );
    void handlePart( QDomElement e );
    void handlePartList( int id );
};

#endif
