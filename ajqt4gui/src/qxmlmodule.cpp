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

#include "qxmlmodule.h"

#include "ajqtgui.h"

QXMLModule::QXMLModule(AjQtGUI *ajQtGUI, QAjTab *ajTab, QObject *parent, const char *name) : QHttp()
{
	userCnt=0;
	currTimeStamp = &timeStamp;
	timeStamp = "0";
	downTimeStamp = "0";
	upTimeStamp = "0";
	infoTimeStamp = "0";
	searchTimeStamp = "0";
	serverTimeStamp = "0";
	this->ajQtGUI = ajQtGUI;
	this->ajTab = ajTab;
	objectCnt = -1;
	bzero(active, sizeof(active));
	name = "dummy";
	currSearchEntryId = "";
	firstDoc = true;

	inputSource = new QXmlInputSource();
	simpleReader = new QXmlSimpleReader();
	attributes = new QXmlAttributes();
	handler = new QXMLHandler(this);
	simpleReader->setContentHandler(handler);
	
	buffer = new QBuffer();
	buffer->open( QIODevice::ReadWrite );
	
	QObject::connect(this, SIGNAL(readyRead(const QHttpResponseHeader&)), this, SLOT(readyRead( const QHttpResponseHeader&)));
	QObject::connect(this, SIGNAL(requestFinished ( int , bool )), this, SLOT(requestFinished(int, bool )));
	QObject::connect(this, SIGNAL(requestStarted ( int )), this, SLOT(requestStarted(int )));
	
	QObject::connect(this, SIGNAL(responseHeaderReceived ( const QHttpResponseHeader&)), this, SLOT(responseHeaderReceived( const QHttpResponseHeader&)));
	
	//QObject::connect(this, SIGNAL( dataReadProgress ( int, int ) ), this, SLOT(dataReadProgress(int, int )));
	
	session = "";
	parsing = false;
}

QXMLModule::~QXMLModule()
{
	buffer->close();
	delete inputSource;
	delete simpleReader;
	delete attributes;
	delete handler;
	delete buffer;
}

int QXMLModule::setHost ( const QString & hostname, quint16 portnumber )
{
	host = hostname;
	port = portnumber;
	return QHttp::setHost( hostname, portnumber );
}

bool QXMLModule::startDocument()
{
	parsing = true;
	return TRUE;
}

bool QXMLModule::endDocument()
{
	userCnt=0;
	if( ! idList.isEmpty() )
	{
		QStringList oldIdList = idList;
		gotIds( oldIdList );
		idList.clear();
	}
	parsing = false;
//	if( request == QString("modified") )
		modifiedDone();
	return TRUE;
}

bool QXMLModule::startElement( const QString& s1, const QString& s2, const QString& qName, const QXmlAttributes& attr)
{
	name = qName.trimmed();
	
//	if( active[IDS] )
//		printf("%s\n", name.latin1() );

	if(active[SHARE])
	{
		if(name == "directory")
		{
			QString path = attr.value("name");
			QString shareMode = attr.value("sharemode");
			ajTab->ajShareWidget->insertShare( path, shareMode );
			shareCnt++;
		}
	}

	if( name == "share" )
	{
		active[SHARE] = true;
		if( active[APPLEJUICE] )
		{
			QString id = attr.value("id");
			QString fileName = attr.value("filename");
			fileName = fileName.section( '/', -1, -1 );
			ajTab->ajUploadWidget->setFileName( id, fileName );
		}
		if( active[SHARES] )
		{
			QString id = attr.value("id");
			QString fileName = attr.value("filename");
			ajTab->ajShareWidget->insertSharedFile( id, fileName );
		}
		shareCnt = 0;
	}
	else if( name == "shares" )
	{
		active[SHARES] = true;
	}
	else if( name == "applejuice" )
	{
		active[APPLEJUICE] = true;
		partList.clear();
	}
	else if( name == "removed" )
	{
		active[REMOVED] = true;
	}

	else if( name == "ids" )
	{
		active[IDS] = true;
	}

	else if( (name == "downloadid") )
	{
		idList.prepend( attr.value("id") );
	}
	else if( (name == "uploadid")  || (name == "userid")  || (name == "serverid") )
	{
		idList.append( attr.value("id") );
	}

	else if(name == "session")
			session = attr.value("id");

	else if(name == "networkinfo")
	{
		ajQtGUI->networkWidget->setUsers( attr.value("users") );
		ajQtGUI->networkWidget->setFiles( attr.value("files") );
		ajQtGUI->networkWidget->setSize( QConvert::bytesLong(attr.value("filesize")) );
		ajQtGUI->networkWidget->setIp( attr.value("ip") );
		ajTab->ajServerWidget->connectedWith( attr.value("connectedwithserverid") );
		ajQtGUI->connectedSince( attr.value("connectedSince") );
	}
	else if( name == "download" )
	{
//		printf("download\n");
		QString id = attr.value("id");
		QString fileName = attr.value("filename");
		QString status = attr.value("status");
		QString size = attr.value("size");
		QString ready = attr.value("ready");
		QString power = attr.value("powerdownload");
		ajTab->ajDownloadWidget->mutex.lock();
// 		ajTab->ajDownloadWidget->setSortingEnabled( false );
		ajTab->ajDownloadWidget->insertDownload(id, fileName, status, size, ready, power);
// 		ajTab->ajDownloadWidget->setSortingEnabled( true );
		ajTab->ajDownloadWidget->mutex.unlock();
	}
	else if( name == "user" )
	{
		QString id = attr.value("id");
		QString downloadId = attr.value("downloadid");
		QString speed = attr.value("speed");
		QString fileName = attr.value("filename");
		QString status = attr.value("status");
		QString power = attr.value("powerdownload");
		QString queuePos = attr.value("queueposition");
		QString os = attr.value("operatingsystem");
		ajTab->ajDownloadWidget->mutex.lock();
// 		ajTab->ajDownloadWidget->setSortingEnabled( false );
		ajTab->ajDownloadWidget->insertUser(downloadId, id, fileName, speed, status, power, queuePos, os );
// 		ajTab->ajDownloadWidget->setSortingEnabled( true );
		ajTab->ajDownloadWidget->mutex.unlock();
		//printf("user fr %s mit id: %s\n", fileName.latin1(), id.latin1() );
	}
	else if( name == "upload" )
	{
//		printf("upload\n");
		QString id = attr.value("id");
		QString shareId = attr.value("shareid");
		QString version = attr.value("version");
		QString os = attr.value("operatingsystem");
		QString status = attr.value("status");
		QString directState = attr.value("directstate");
		QString priority = attr.value("priority");
		QString nick = attr.value("nick");
		QString speed = attr.value("speed");
		if( ! ajTab->ajUploadWidget->insertUpload( id, shareId, version, os, status, directState, priority, nick, speed ) )
			get( GET_OBJECT_XML, shareId );
	}
	else if( name == "server" )
	{
		QString id = attr.value("id");
		QString name = attr.value("name");
		QString host = attr.value("host");
		QString port = attr.value("port");
		QString lastseen = attr.value("lastseen");
		QString tests = attr.value("connectiontry");
		
		ajTab->ajServerWidget->insertServer( id, name, host, port, lastseen, tests );
	}
	else if( name == "search" )
	{
		QString id = attr.value("id");
		QString text = attr.value("searchtext");
		QString running = attr.value("running");
		QString foundFiles = attr.value("foundfiles");
		//printf("search: %s\n", text.latin1() );
		ajTab->ajSearchWidget->insertSearch( id, text, running, foundFiles );
	}
	else if( name == "searchentry" )
	{
		QString id = attr.value("id");
		QString searchId = attr.value("searchid");
		QString size = attr.value("size");
		QString checksum = attr.value("checksum");
		ajTab->ajSearchWidget->insertSearchEntry( id, searchId, size, checksum );
		currSearchEntryId = id;
	}
	else if( name == "filename" )
	{
		QString fileName = attr.value("name");
		//printf("filename: %s\n", fileName.latin1() );
		ajTab->ajSearchWidget->insertFileName( currSearchEntryId, fileName );
	}
	else if( name == "filesystem" )
	{
		ajQtGUI->setFilesystemSeparator( attr.value("seperator") );
	}
	else if( name == "information" )
	{
//		printf("info\n");
		ajQtGUI->setStatusBarText(
			attr.value("downloadspeed"),
			attr.value("uploadspeed"),
			attr.value("credits"),
			attr.value("sessiondownload"),
			attr.value("sessionupload")
		);
	}
	else if( name == "fileinformation" )
	{
		partsSize = attr.value( "filesize" ).toULongLong();
	}
	else if( name == "part" )
	{
		partList.push_back( new QAjPart( attr.value("fromposition").toULongLong(), attr.value("type").toInt() ) );
	}
	else if( active[REMOVED] && ( name == "object" ) )
	{
		QString id = attr.value("id").trimmed();
		ajTab->ajDownloadWidget->mutex.lock();
// 		ajTab->ajDownloadWidget->setSortingEnabled( false );
		//printf("remove %s\n", id.latin1() );
		if( ! ajTab->ajDownloadWidget->remove( id ) )
			if( ! ajTab->ajUploadWidget->remove( id ) )
				if( ! ajTab->ajServerWidget->remove( id ) )
					ajTab->ajSearchWidget->remove( id );
// 		ajTab->ajDownloadWidget->setSortingEnabled( true );
		ajTab->ajDownloadWidget->mutex.unlock();
	}
	return TRUE;
}

bool QXMLModule::endElement( const QString& s1, const QString& s2, const QString& qName )
{
	if( qName == "share" )
		active[SHARE] = false;
	else if( qName == "removed" )
		active[REMOVED] = false;
	else if( qName == "shares" )
		active[SHARES] = false;
	else if( qName == "ids" )
		active[IDS] = false;
	else if( qName == "applejuice" )
	{
		active[APPLEJUICE] = false;
	}
	name = "dummy";
	
	// release the cpu
	//usleep( 50000 );
	
	return TRUE;
}

bool QXMLModule::characters( const QString& s1 )
{
	if( name == "version" )
		ajQtGUI->setCoreVersion( s1 );
	else if( name == "time" )
	{
		*currTimeStamp = s1;
		//if( currTimeStamp == &timeStamp )
		{
			downTimeStamp = upTimeStamp = infoTimeStamp = searchTimeStamp = serverTimeStamp = s1;//timeStamp;
		}
	}
	return true;
}

int QXMLModule::get( int getCode, QString param )
{
	QString get = "";
	QString partListId;
	switch(getCode)
	{
		case GET_SESSION_XML:
			session = "";
			request = "getsession";
			break;
		case GET_SETTINGS_XML:
			request = "settings";
			break;
		case MODIFIED_XML:
			request = "modified";
			get = "&session=" + session + "&timestamp=" + timeStamp;
			currTimeStamp = &timeStamp;
			break;
		case INFORMATION_XML:
			request = "modified";
			get = "&session=" + session + "&filter=informations;ids" + "&timestamp="  + infoTimeStamp;
			currTimeStamp = &infoTimeStamp;
			break;
		case EVERYTHING_XML:
			request = "modified";
			get = "&filter=down;uploads;user;server;search;informations";
			currTimeStamp = &timeStamp;
			break;
		case IDS_XML:
			objectCnt = 0;
			request = "modified";
			get = "&filter=ids";
			break;
		case DOWNLOAD_XML:
			request = "modified";
			get = "&session=" + session + "&filter=user;down;informations;ids" + "&timestamp=" + downTimeStamp;
			currTimeStamp = &downTimeStamp;
			break;
		case USER_XML:
			request = "modified";
			get = "&session=" + session + "&filter=user";
			break;
		case UPLOAD_XML:
			request = "modified";
			get = "&session=" + session + "&filter=uploads;informations;ids" + "&timestamp=" + upTimeStamp;
			currTimeStamp = &upTimeStamp;
			break;
		case SERVER_XML:
			request = "modified";
			get = "&session=" + session + "&filter=server;informations;ids" + "&timestamp=" + serverTimeStamp;
			currTimeStamp = &serverTimeStamp;
			break;
		case SEARCH_XML:
			request = "modified";
			get = "&session=" + session + "&filter=search;informations;ids" + "&timestamp=" + searchTimeStamp;
			currTimeStamp = &searchTimeStamp;
			break;
		case GET_OBJECT_XML:
			request = "getobject";
			get = /*"&session=" + session +*/ "&Id=" + param;
			break;
		case SHARE_XML:
			request = "share";
			break;
		case EXTRA_INFORMATION_XML:
			request = "information";
			break;
		case DOWNLOAD_PARTLIST_XML:
			request = "downloadpartlist";
			partListId = param.split( "=" )[1];
			get = param;
			break;
		case DOWNLOAD_PARTLIST_SIMPLE_XML:
			request = "downloadpartlist";
			partListId = param.split( "=" )[1];
			get = param;
			break;
	}
	QString file = "/xml/" + request + ".xml?password=" + passwordMD5 + get;
//	if( getCode == GET_OBJECT_XML )
//		printf("%s\n", file.toAscii().data());
		
	int httpRequest;
//	if( getCode != DOWNLOAD_PARTLIST_XML )
		httpRequest = QHttp::get(file);//, buffer);
	
	if( getCode == DOWNLOAD_PARTLIST_XML )
	{
		partListRequests[ httpRequest ] = partListId;
	}
	else if( getCode == DOWNLOAD_PARTLIST_SIMPLE_XML )
	{
		partListSimpleRequests[ httpRequest ] = partListId;
	}

	return httpRequest;
}

int QXMLModule::set( int setCode, QString param )
{
	switch( setCode )
	{
		case SET_POWER_XML:
			request = "setpowerdownload";
			break;
		case PROCESS_LINK_XML:
			request = "processlink";
			break;
		case SET_SETTINGS_XML:
			request = "setsettings";
			break;
		case CANCEL_DOWNLOAD_XML:
			request = "canceldownload";
			break;
		case CLEAN_DOWNLOAD_XML:
			request = "cleandownloadlist";
			break;
		case PAUSE_DOWNLOAD_XML:
			request = "pausedownload";
			break;
		case RESUME_DOWNLOAD_XML:
			request = "resumedownload";
			break;
		case RENAME_DOWNLOAD_XML:
			request = "renamedownload";
			break;
		case REMOVE_SERVER_XML:
			request = "removeserver";
			break;
		case CONNECT_SERVER_XML:
			request = "serverlogin";
			break;
		case EXIT_CORE_XML:
			request = "exitcore";
			break;
		case SEARCH_XML:
			request = "search";
			break;
		case CANCEL_SEARCH_XML:
			request = "cancelsearch";
			break;
	}
	QString action = "/function/" + request + "?password=" + passwordMD5 + param;
//	printf("set: %s\n", action.toAscii().data() );
	return QHttp::get( action );
}

void QXMLModule::requestFinished( int id, bool error )
{
// 	printf("%d finished\n", id);
// 	printf("bytes available: %d\n", bytesAvailable() );
	if( ! error )
	{
//		printf( "%s\n=======================================\n", QString( readAll() ).ascii() );

		QByteArray x = readAll();

		QDomDocument doc;
		doc.setContent(x);

		QDomElement root = doc.documentElement();
		QDomNode n;

		if(root.tagName().toLower() == "applejuice") {
			
			for(n = root.firstChild(); !n.isNull(); n = n.nextSibling()){
				QDomElement e = n.toElement();
				if(!e.isNull()) {
					if( e.tagName().toLower() == "session" ) {
						session = e.attribute("id");
					}
					if( e.tagName().toLower() == "shares" ) {
						QDomNode shareNode;
						for(shareNode=n.firstChild();!shareNode.isNull();shareNode=shareNode.nextSibling()) {
							QDomElement shareElement = shareNode.toElement();
							
						}
					}

				}
			}
		} else if(root.tagName().toLower() == "settings" ) {
			AjSettings settings;
			for(n = root.firstChild(); !n.isNull(); n = n.nextSibling()){
				QDomElement e = n.toElement();
				if(!e.isNull()) {
					if( e.tagName().toLower() == "nick" )
						settings.nick = e.text();
					if( e.tagName().toLower() == "port" )
						settings.tcpPort = e.text();
					if( e.tagName().toLower() == "xmlport" )
						settings.xmlPort = e.text();
					if( e.tagName().toLower() == "autoconnect" )
						settings.autoconnect = e.text().toLower() == "true";
					if( e.tagName().toLower() == "maxupload" )
						settings.maxUp = e.text();
					if( e.tagName().toLower() == "maxdownload" )
						settings.maxDown = e.text();
					if( e.tagName().toLower() == "maxconnections" )
						settings.maxCon = e.text();
					if( e.tagName().toLower() == "maxsourcesperfile" )
						settings.maxSources = e.text();
					if( e.tagName().toLower() == "speedperslot" )
						settings.maxSlot = e.text();
					if( e.tagName().toLower() == "maxnewconnectionsperturn" )
						settings.maxNewCon = e.text();
					if( e.tagName().toLower() == "incomingdirectory" )
						settings.incomingDir = e.text();
					if( e.tagName().toLower() == "temporarydirectory" )
						settings.tempDir = e.text();
				}
			}
			settingsReady(settings);
		}

// 		QFile file("xml.xml");
// 		file.open(QIODevice::Append);
// 
// 		file.write(x);
// 		file.write("\n==========================\n");
// 		file.close();

		inputSource->setData(x);
		simpleReader->parse(inputSource);
		
		// process part list request result
		if( partList.size() > 0 )
		{
			map<int, QString>::iterator part = partListRequests.find( id );
			
			if( part != partListRequests.end() )
			{
				QAjDownloadItem* item = ajTab->ajDownloadWidget->findDownload( part->second );
				if( item != NULL )
					item->getPartListWidget()->update( partsSize, &partList );
				
				partListRequests.erase( part );
			}
			else
			{
				part = partListSimpleRequests.find( id );
				if( part != partListSimpleRequests.end() )
				{
					QAjDownloadItem* item = ajTab->ajDownloadWidget->findDownload( part->second );
					if( item != NULL )
						item->setParts( partsSize, &partList );
				}
			}
			partList.clear();
		}

	}
	userCnt = 0;
}

void QXMLModule::requestStarted(int id)
{
//	printf("%d startet: %s\n", id, currentRequest().path().latin1() );
}
void QXMLModule::responseHeaderReceived ( const QHttpResponseHeader & resp )
{
//	http header ausgeben
/*	int i;
	QStringList list = resp.keys();
	for(i=0; i<list.size(); i++)
	{
		QString key = list[i];
		QString value = resp.value( list[i] );
		printf("%s: %s\n", key.latin1(), value.latin1() );
	}
*/
	if( resp.statusCode() != 200 )
	{
		error( resp.statusCode() );
	}
}

void QXMLModule::dataReadProgress( int done, int total )
{
	printf("%d of %d\n", done, total );
}

void QXMLModule::readyRead( const QHttpResponseHeader & resp )
{

//	printf("code: %d\n", resp.statusCode() );
}

/*void QXMLModule::saveSettings( AjSettings settings )
{
	QString get = "/function/setsettings?password=" + passwordMD5;
	get += "&nick=" + settings.nick;
	get += "&Port=" + settings.tcpPort;
	get += "&XMLPort=" + settings.xmlPort;
	get += "&MaxDownload=" + settings.maxDown;
	get += "&MaxUpload=" + settings.maxUp;
	get += "&MaxConnections=" + settings.maxCon;
	get += "&AutoConnect=" + settings.autoconnect;
	get += "&MaxSourcesPerFile=" + settings.maxSources;
	get += "&Speedperslot=" + settings.maxSlot;
	get += "&Incomingdirectory=" + settings.incomingDir;
	get += "&Temporarydirectory=" + settings.tempDir;
	get += "&MaxNewConnectionsPerTurn=" + settings.maxNewCon;
	get += "&AllowBrowse=" + settings.allowBrowse;
	QHttp::get(get);
}*/

void QXMLModule::setPassword( QString password )
{
	CMD5 md5( password.toAscii().data() );
	passwordMD5 = md5.getMD5Digest();
}

int QXMLModule::countElements( QString xmlString )
{
	int cnt = 0;
	int start = 0;
	QRegExp regExp("<[^/]", Qt::CaseSensitive );
	regExp.setMinimal(true );
	while( ( start = xmlString.indexOf( regExp, start ) ) != -1 )
	{
		start++;
		cnt++;
	}
	return cnt;
}
