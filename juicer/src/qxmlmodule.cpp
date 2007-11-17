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

#include "juicer.h"

QXMLModule::QXMLModule( Juicer* juicer, QObject *parent ) : QHttp( parent )
{
    this->juicer = juicer;
    timeStamp = "0";
    QObject::connect(this, SIGNAL(requestFinished( int , bool )), this, SLOT(requestFinished(int, bool )));
    QObject::connect(this, SIGNAL(responseHeaderReceived( const QHttpResponseHeader&)), this, SLOT(responseHeaderReceived( const QHttpResponseHeader&)));
}

QXMLModule::~QXMLModule()
{
}

int QXMLModule::setHost( const QString & hostname, quint16 portnumber )
{
    host = hostname;
    port = portnumber;
    return QHttp::setHost( hostname, portnumber );
}

int QXMLModule::exec( const QString request, int nErrors ) {
    int id = QHttp::get(request);
    requests[id] = request;
    errors[id] = nErrors;
    return id;
}

int QXMLModule::get( QString request, QString param )
{
    if(request == "modified")
    {
        param += "&session=" + session + "&timestamp=" + timeStamp;
        param += "&filter=down;uploads;user;server;search;informations;ids";
    }

    int httpRequest = exec("/xml/" + request + ".xml?password=" + passwordMD5 + param);

    if(request == "downloadpartlist")
    {
        if(!param.contains("simple"))
            partListRequests[ httpRequest ] = param.split( "=" )[1];
        else
            partListSimpleRequests[ httpRequest ] = param.split( "=" )[1];
    }

    return httpRequest;
}

int QXMLModule::set( QString request, QString param )
{
    return exec( "/function/" + request + "?password=" + passwordMD5 + param );
}

void QXMLModule::requestFinished( int id, bool error )
{
    if ( ! error )
    {
        QTime now = QTime::currentTime();
        doc.setContent(readAll());

        QDomElement root = doc.documentElement();
        QDomNode n;

        if (root.tagName() == "applejuice")
        {
            for (n = root.firstChild(); !n.isNull(); n = n.nextSibling())
            {
                QDomElement e = n.toElement();
                if (!e.isNull())
                {
                    if ( e.tagName() == "session" )
                    {
                        session = e.attribute("id");
                        gotSession();
                    }
                    else if ( e.tagName() == "time" )
                    {
                        timeStamp = e.text();
                    }
                    else if ( e.tagName() == "ids" )
                    {
                        handleIds(n);
                    }
                    else if ( e.tagName() == "share" )
                    {
                        handleShare(e);
                    }
                    else if ( e.tagName() == "removed" )
                    {
                        handleRemoved(e);
                    }
                    else if ( e.tagName() == "generalinformation" )
                    {
                        handleGeneralInformation(n);
                    }
                    else if ( e.tagName() == "information" )
                    {
                        juicer->setStatusBarText(
                            e.attribute("downloadspeed"),
                            e.attribute("uploadspeed"),
                            e.attribute("credits"),
                            e.attribute("sessiondownload"),
                            e.attribute("sessionupload")
                        );
                    }
                    else if ( e.tagName() == "networkinfo" )
                    {
                        handleNetworkInfo( e );
                    }
                    else if ( e.tagName() == "upload" )
                    {
                        handleUpload( e );
                    }
                    else if ( e.tagName() == "download" )
                    {
                        handleDownload( e );
                    }
                    else if ( e.tagName() == "user" )
                    {
                        handleUser( e, now );
                    }
                    else if ( e.tagName() == "server" )
                    {
                        handleServer( e );
                    }
                    else if ( e.tagName() == "search" )
                    {
                        handleSearch( e );
                    }
                    else if ( e.tagName() == "searchentry" )
                    {
                        handleSearchEntry( e );
                    }
                    else if ( e.tagName() == "part" )
                    {
                        handlePart( e );
                    }
                    else if ( e.tagName() == "fileinformation" )
                    {
                        partsSize = e.attribute("filesize").toULongLong();
                    }
                    else if ( e.tagName() == "shares" )
                    {
                        handleShares( e );
                    }
                    else if ( e.tagName() == "dir" )
                    {
                        if ( juicer->ajShareWidget->fileSystem != NULL ) {
                        juicer->ajShareWidget->fileSystem->insertDirectory(
                              e.attribute("name"),
                              e.attribute("path"),
                              e.attribute("type").toInt() );
                        }
                    }
                    else if ( e.tagName() == "filesystem" )
                    {
                        if ( juicer->ajShareWidget->fileSystem != NULL ) {
                        juicer->ajShareWidget->fileSystem->insertSeperator(e.attribute("seperator"));
                        }
                    }
                    else
                    {
                        fprintf(stderr, "unhandled element: %s\n", e.tagName().toLatin1().data());
                    }
                }
            }
        }
        else if ( root.tagName() == "settings" )
        {
            handleSettings(root);
        }
// this does not work. documentation false?
//         else if ( root.tagName() == "shares" )
//         {
//             handleShares(root);
//         }
        handlePartList(id);
        modifiedDone();
    } else if(errors[id] < 3) {
        exec(requests[id], errors[id] + 1);
    } else {
        QXMLModule::error(-1);
    }
}


void QXMLModule::responseHeaderReceived ( const QHttpResponseHeader & resp )
{
    if( resp.statusCode() != 200 )
    {
        error( resp.statusCode() );
    }
}

void QXMLModule::setPassword( QString password )
{
    CMD5 md5( password.toAscii().data() );
    passwordMD5 = md5.getMD5Digest();
}


/*!
    \fn QXMLModule::handleSettings( QDomElement e )
 */
void QXMLModule::handleSettings( QDomElement e )
{
    AjSettings settings;
    settings.nick = e.firstChildElement("nick").text();
    settings.tcpPort = e.firstChildElement("port").text();
    settings.xmlPort = e.firstChildElement("xmlport").text();
    settings.autoconnect = e.firstChildElement("autoconnect").text().toLower() == "true";
    settings.maxUp = e.firstChildElement("maxupload").text();
    settings.maxDown = e.firstChildElement("maxdownload").text();
    settings.maxCon = e.firstChildElement("maxconnections").text();
    settings.maxSources = e.firstChildElement("maxsourcesperfile").text();
    settings.maxSlot = e.firstChildElement("speedperslot").text();
    settings.maxNewCon = e.firstChildElement("maxnewconnectionsperturn").text();
    settings.incomingDir = e.firstChildElement("incomingdirectory").text();
    settings.tempDir = e.firstChildElement("temporarydirectory").text();
    settingsReady(settings);

    juicer->ajShareFilesWidget->setTmpDir(settings.tempDir);

    juicer->ajShareWidget->clear();
    QDomElement shareE;
    for(shareE=e.firstChildElement("share").firstChildElement("directory");
        !shareE.isNull(); shareE = shareE.nextSiblingElement("directory"))
    {
        juicer->ajShareWidget->insertShare(
            shareE.attribute("name"), shareE.attribute("sharemode"),
            juicer->getFilesystemSeparator());
    }
}


/*!
    \fn QXMLModule::handleShare( QDomElement e )
 */
void QXMLModule::handleShare( QDomElement e )
{
    juicer->setUploadFilename( e.attribute("id"), e.attribute("filename") );

}


/*!
    \fn QXMLModule::handleShares( QDomElement e )
 */
void QXMLModule::handleShares( QDomElement e )
{
    QDomNode n;
    {
        for (n = e.firstChild(); !n.isNull(); n = n.nextSibling())
        {
            QDomElement shareE = n.toElement();
            if (!shareE.isNull() &&
                !shareE.attribute("filename").contains(juicer->ajShareFilesWidget->getTmpDir()))
            {
            juicer->ajShareFilesWidget->insertFile(
            shareE.attribute("id"),
            shareE.attribute("checksum"),
            shareE.attribute("filename"),
            shareE.attribute("size"),
            shareE.attribute("priority"),
            juicer->getFilesystemSeparator() );
            }
        }
    }
}


/*!
    \fn QXMLModule::handleIds( QDomNode node )
 */
void QXMLModule::handleIds( QDomNode node )
{
    // TODO
    /*
    QDomNode childNode;
    for (childNode=node.firstChild();!childNode.isNull();childNode=childNode.nextSibling())
    {
        QDomElement childElement = childNode.toElement();
        if(childElement.tagName().toLower() == "serverid")
        {
            // TODO
        }
        else if(childElement.tagName().toLower() == "uploadid")
        {
            // TODO
        }
        else if(childElement.tagName().toLower() == "downloadid")
        {
            // TODO
        }
        else if(childElement.tagName().toLower() == "userid")
        {
            // TODO
        }
    }
    */
}


/*!
    \fn QXMLModule::handleNetworkInfo( QDomElement e )
 */
void QXMLModule::handleNetworkInfo( QDomElement e )
{
    juicer->networkDialog->setValues(
        e.attribute("users"),
        e.attribute("files"),
        QConvert::bytesLong( e.attribute("filesize")),
        e.attribute("ip"),
        e.attribute("firewalled")=="true"?tr("yes"):tr("no"));
    juicer->ajServerWidget->connectedWith( e.attribute("connectedwithserverid") );
    juicer->ajServerWidget->connectingTo( e.attribute("tryconnecttoserver") );
    juicer->connectedSince( e.attribute("connectedsince") );
    juicer->ajServerMetaWidget->welcomeMessage->setHtml( e.firstChildElement("welcomemessage").text().trimmed() );
}


/*!
    \fn QXMLModule::handleUpload( QDomElement e )
 */
void QXMLModule::handleUpload( QDomElement e )
{
    if( ! juicer->ajUploadWidget->insertUpload(
            e.attribute("id"),
            e.attribute("shareid"),
            e.attribute("version"),
            e.attribute("operatingsystem"),
            e.attribute("status"),
            e.attribute("directstate"),
            e.attribute("priority"),
            e.attribute("nick"),
            e.attribute("speed")) )
    {
        this->get("getobject", "&Id="+e.attribute("shareid"));
    }
}


/*!
    \fn QXMLModule::handleUser( QDomElement e, QTime time )
 */
void QXMLModule::handleUser( QDomElement e, QTime time )
{
//     juicer->ajDownloadWidget->mutex.lock();
    juicer->ajDownloadWidget->insertUser(
        e.attribute("downloadid"),
        e.attribute("id"),
        e.attribute("filename"),
        e.attribute("speed"),
        e.attribute("status"),
        e.attribute("powerdownload"),
        e.attribute("queueposition"),
        e.attribute("operatingsystem"),
        time);
//     juicer->ajDownloadWidget->mutex.unlock();
}


/*!
    \fn QXMLModule::handleDownload( QDomElement e )
 */
void QXMLModule::handleDownload( QDomElement e )
{
//     juicer->ajDownloadWidget->mutex.lock();
    juicer->ajDownloadWidget->insertDownload(
        e.attribute("id"),
        e.attribute("hash"),
        e.attribute("filename"),
        e.attribute("status"),
        e.attribute("size"),
        e.attribute("ready"),
        e.attribute("powerdownload"),
        e.attribute("temporaryfilenumber"));
//     juicer->ajDownloadWidget->mutex.unlock();
}


/*!
    \fn QXMLModule::handleServer( QDomElement e )
 */
void QXMLModule::handleServer( QDomElement e )
{
    juicer->ajServerWidget->insertServer(
        e.attribute("id"),
        e.attribute("name"),
        e.attribute("host"),
        e.attribute("port"),
        e.attribute("lastseen"),
        e.attribute("connectiontry"));
}


/*!
    \fn QXMLModule::handleSearch( QDomElement e )
 */
void QXMLModule::handleSearch( QDomElement e )
{
    juicer->ajSearchWidget->insertSearch(
        e.attribute("id"),
        e.attribute("searchtext"),
        e.attribute("running"),
        e.attribute("foundfiles"));
}


/*!
    \fn QXMLModule::handleSearchEntry( QDomElement e )
 */
void QXMLModule::handleSearchEntry( QDomElement e )
{
    QStringList filenames;
    QDomElement fileE;
    for(fileE=e.firstChildElement("filename"); !fileE.isNull(); fileE = fileE.nextSiblingElement("filename")) {
        filenames.append(fileE.attribute("name"));
    }
    juicer->ajSearchWidget->insertSearchEntry(
        e.attribute("id"),
        e.attribute("searchid"),
        e.attribute("size"),
        e.attribute("checksum"),
        filenames);
}


/*!
    \fn QXMLModule::handleGeneralInformation( QDomNode node )
 */
void QXMLModule::handleGeneralInformation( QDomNode node )
{
    juicer->setFilesystemSeparator(
        node.firstChildElement("filesystem").attribute("seperator"));
    juicer->setCoreVersion(node.firstChildElement("version").text());
}


/*!
    \fn QXMLModule::handleRemoved( QDomElement e )
 */
void QXMLModule::handleRemoved( QDomElement e )
{
    QDomElement objectE;
    for(objectE = e.firstChildElement("object");
        !objectE.isNull(); objectE = objectE.nextSiblingElement("object"))
    {
        QString id = objectE.attribute("id");
//         juicer->ajDownloadWidget->mutex.lock();
        if ( ! juicer->ajDownloadWidget->remove( id ) )
            if ( ! juicer->ajUploadWidget->remove( id ) )
                if ( ! juicer->ajServerWidget->remove( id ) )
                    juicer->ajSearchWidget->remove( id );
//         juicer->ajDownloadWidget->mutex.unlock();
    }
}

/*!
    \fn QXMLModule::handlePart( QDomElement e )
 */
void QXMLModule::handlePart( QDomElement e )
{
    Part part;
    part.type = e.attribute("type").toInt();
    part.fromPosition = e.attribute("fromposition").toULongLong();
    partList.push_back(part);
}


/*!
    \fn QXMLModule::handlePartList( int id )
 */
void QXMLModule::handlePartList( int id )
{
    if ( partList.size() > 0 )
    {
        if( partListRequests.contains( id ) )
        {
            QAjDownloadItem* item = juicer->ajDownloadWidget->findDownload( partListRequests[id] );
            if( item != NULL )
            {
                item->getPartListWidget()->update( partsSize, partList );
            }
            partListRequests.remove( id );
        }
        else if( partListSimpleRequests.contains( id ) )
        {
            QAjDownloadItem* item = juicer->ajDownloadWidget->findDownload( partListSimpleRequests[id] );
            if ( item != NULL )
            {
                item->setParts( partsSize, partList );
            }
        }
        partList.clear();
    }
}
