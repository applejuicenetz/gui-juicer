/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#include "xmlmodule.h"

#include "juicer.h"

XMLModule::XMLModule(Juicer* juicer, QObject *parent) : QHttp(parent) {
    this->juicer = juicer;
    timeStamp = "0";
    QObject::connect(this, SIGNAL(requestFinished( int , bool )), this, SLOT(requestFinished(int, bool )));
    QObject::connect(this, SIGNAL(responseHeaderReceived( const QHttpResponseHeader&)), this, SLOT(responseHeaderReceived( const QHttpResponseHeader&)));
}

XMLModule::~XMLModule() {
}

int XMLModule::exec(const QString & request, int nErrors) {
    int id = QHttp::get(request);
    requests[id] = request;
    errors[id] = nErrors;
    return id;
}

int XMLModule::make(Type type, const QString & request, QString param) {
    if(type == GET) {
        return get(request, param);
    } else {
        return set(request, param);
    }
}

int XMLModule::get(const QString & request, QString param) {
    if(request == "modified") {
        param += "&session=" + session + "&timestamp=" + timeStamp;
        param += "&filter=down;uploads;user;server;search;informations;ids";
    }
    int httpRequest = exec("/xml/" + request + ".xml?password=" + passwordMD5 + param);
    if(request == "downloadpartlist") {
        if(!param.contains("simple")) {
            partListRequests[ httpRequest ] = param.split( "=" )[1];
        } else {
            partListSimpleRequests[ httpRequest ] = param.split( "=" )[1];
        }
    }
    return httpRequest;
}

int XMLModule::set(const QString & request, QString param) {
    return exec("/function/" + request + "?password=" + passwordMD5 + param);
}

void XMLModule::requestFinished(int id, bool error) {
    if(!error) {
        QTime now = QTime::currentTime();
        doc.setContent(readAll());
        QDomElement root = doc.documentElement();
        QDomNode n;
        if(root.tagName() == "applejuice") {
            juicer->downloads->setUpdatesEnabled(false);
            juicer->uploads->setUpdatesEnabled(false);
            juicer->search->setUpdatesEnabled(false);
            juicer->server->setUpdatesEnabled(false);
            juicer->shares->setUpdatesEnabled(false);
            for(n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                    if(e.tagName() == "session") {
                        session = e.attribute("id");
                        gotSession();
                    } else if(e.tagName() == "time") {
                        timeStamp = e.text();
                    } else if(e.tagName() == "ids") {
                        handleIds(n);
                    } else if(e.tagName() == "share") {
                        handleShare(e);
                    } else if(e.tagName() == "removed") {
                        handleRemoved(e);
                    } else if(e.tagName() == "generalinformation") {
                        handleGeneralInformation(n);
                    } else if(e.tagName() == "information") {
                        juicer->setStatusBarText(
                            e.attribute("downloadspeed"),
                            e.attribute("uploadspeed"),
                            e.attribute("credits"),
                            e.attribute("sessiondownload"),
                            e.attribute("sessionupload")
                        );
                    } else if(e.tagName() == "networkinfo") {
                        handleNetworkInfo( e );
                    } else if(e.tagName() == "upload") {
                        handleUpload( e );
                    } else if(e.tagName() == "download") {
                        handleDownload( e );
                    } else if(e.tagName() == "user") {
                        handleUser( e, now );
                    } else if(e.tagName() == "server") {
                        handleServer( e );
                    } else if(e.tagName() == "search") {
                         handleSearch( e );
                    } else if(e.tagName() == "searchentry") {
                         handleSearchEntry( e );
                    } else if(e.tagName() == "part") {
                        handlePart( e );
                    } else if(e.tagName() == "fileinformation") {
                        partsSize = e.attribute("filesize").toULongLong();
                    } else if(e.tagName() == "shares") {
                        handleShares( e );
                    } else {
                        if ( ! (e.tagName() == "dir") &&
                             ! (e.tagName() == "filesystem") )
                        {
                            fprintf(stderr, "unhandled element: %s\n", e.tagName().toLatin1().data());
                        }
                    }
                }
            }
            juicer->uploads->setUpdatesEnabled(true);
            juicer->search->setUpdatesEnabled(true);
            juicer->server->setUpdatesEnabled(true);
            juicer->shares->setUpdatesEnabled(true);
            processUsers();
            handlePartList(id);
            juicer->downloads->setUpdatesEnabled(true);
            modifiedDone();
        } else if(root.tagName() == "settings") {
            handleSettings(root);
        }
// this does not work. documentation false?
//         else if ( root.tagName() == "shares" )
//         {
//             handleShares(root);
//         }

/*    } else if(errors[id] < 1) {
        exec("requests[id]", errors[id] + 1);*/
    } else if(QHttp::error() != QHttp::Aborted) {
        // -- use a timer (it does NOT work calling it directly) --
        QTimer::singleShot(0, this, SLOT(networkErrorSlot()));
    }
}


/*!
    \fn XMLModule::networkErrorSlot()
 */
void XMLModule::networkErrorSlot() {
    abort();
    XMLModule::error(errorString());
}

/*!
    \fn XMLModule::httpErrorSlot()
 */
void XMLModule::httpErrorSlot() {
    abort();
    XMLModule::error("Either wrong password or connection lost.");
}

void XMLModule::responseHeaderReceived (const QHttpResponseHeader & resp) {
    if(resp.statusCode() != 200) {
        // -- use a timer (it does NOT work calling it directly) --
        QTimer::singleShot(0, this, SLOT(httpErrorSlot()));
    }
}

void XMLModule::setPassword(const QString & password) {
    CMD5 md5(password.toAscii().data());
    passwordMD5 = md5.getMD5Digest();
}


/*!
    \fn XMLModule::handleSettings(QDomElement& e)
 */
void XMLModule::handleSettings(QDomElement& e) {
    AjSettings settings;
    settings.nick         = e.firstChildElement("nick").text();
    settings.tcpPort      = e.firstChildElement("port").text();
    settings.xmlPort      = e.firstChildElement("xmlport").text();
    settings.autoconnect  = e.firstChildElement("autoconnect").text();
    settings.maxUp        = e.firstChildElement("maxupload").text();
    settings.maxDown      = e.firstChildElement("maxdownload").text();
    settings.maxCon       = e.firstChildElement("maxconnections").text();
    settings.maxSources   = e.firstChildElement("maxsourcesperfile").text();
    settings.maxSlot      = e.firstChildElement("speedperslot").text();
    settings.maxNewCon    = e.firstChildElement("maxnewconnectionsperturn").text();
    settings.incomingDir  = e.firstChildElement("incomingdirectory").text();
    settings.tempDir      = e.firstChildElement("temporarydirectory").text();
    settingsReady(settings);

    juicer->setIncomingDirectory(settings.incomingDir);
    juicer->setTempDirectory(settings.tempDir);

    juicer->sharesTreeWidget->clear();
    QDomElement shareE;
    for(shareE=e.firstChildElement("share").firstChildElement("directory");
        !shareE.isNull(); shareE = shareE.nextSiblingElement("directory")) {
        juicer->shareModule->insertShare(
            shareE.attribute("name"), shareE.attribute("sharemode"));
    }
}


/*!
    \fn XMLModule::handleShare( QDomElement& e )
 */
void XMLModule::handleShare( QDomElement& e ) {
    juicer->setUploadFilename( e.attribute("id"), e.attribute("filename") );

}


/*!
    \fn XMLModule::handleShares( QDomElement& e )
 */
void XMLModule::handleShares( QDomElement& e )
{
    QDomNode n;
    {
        for (n = e.firstChild(); !n.isNull(); n = n.nextSibling())
        {
            QDomElement shareE = n.toElement();
            if (!shareE.isNull() &&
                !shareE.attribute("filename").contains(juicer->getTempDirectory()))
            {
              juicer->shareModule->insertFile(
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
    \fn XMLModule::handleIds( QDomNode& node )
 */
void XMLModule::handleIds( QDomNode& node )
{
    node = node;
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
    \fn XMLModule::handleNetworkInfo( QDomElement& e )
 */
void XMLModule::handleNetworkInfo( QDomElement& e )
{
    bool firewalled = e.attribute("firewalled")=="true";
    juicer->networkDialog->setValues(
        e.attribute("users"),
        e.attribute("files"),
        Convert::bytesLong( e.attribute("filesize")),
        e.attribute("ip"),
        firewalled?tr("yes"):tr("no")
        );
    juicer->serverModule->connectedWith( e.attribute("connectedwithserverid") );
    juicer->serverModule->connectingTo( e.attribute("tryconnecttoserver") );
    juicer->connectedSince( e.attribute("connectedsince") );
    juicer->setFirewalled(firewalled);
    juicer->welcomeEdit->setHtml( e.firstChildElement("welcomemessage").text().trimmed() );
}


/*!
    \fn XMLModule::handleUpload( QDomElement& e )
 */
void XMLModule::handleUpload( QDomElement& e )
{
    if( ! juicer->uploadModule->insertUpload(
            e.attribute("id"),
            e.attribute("shareid"),
            e.attribute("version"),
            e.attribute("operatingsystem"),
            e.attribute("status"),
            e.attribute("directstate"),
            e.attribute("priority"),
            e.attribute("nick"),
            e.attribute("speed"),
            e.attribute("loaded"),
            e.attribute("uploadfrom"),
            e.attribute("uploadto"),
            e.attribute("actualuploadposition"),
            e.attribute("lastconnection")
      ) )
    {
        this->get("getobject", "&Id="+e.attribute("shareid"));
    }
}


/*!
    \fn XMLModule::handleUser( QDomElement& e, QTime& time )
 */
void XMLModule::handleUser( QDomElement& e, QTime& time )
{
    // -- try to insert users after down- and uploads --
    // -- store them in a lists and process them when the end of the xml was reached --
    users.append(e);
    userTimes.append(time);
}

void XMLModule::processUsers() {
    while(!users.empty()) {
        QDomElement e = users.takeFirst();
        QTime time = userTimes.takeFirst();
        juicer->downloadModule->insertUser(
            e.attribute("downloadid"),
            e.attribute("id"),
            e.attribute("filename"),
            e.attribute("nickname"),
            e.attribute("speed"),
            e.attribute("status"),
            e.attribute("powerdownload"),
            e.attribute("queueposition"),
            e.attribute("operatingsystem"),
            e.attribute("downloadfrom"),
            e.attribute("downloadto"),
            e.attribute("actualdownloadposition"),
            time);
    }
// version
}

/*!
    \fn XMLModule::handleDownload( QDomElement& e )
 */
void XMLModule::handleDownload( QDomElement& e )
{
    juicer->downloadModule->insertDownload(
        e.attribute("id"),
        e.attribute("hash"),
        e.attribute("filename"),
        e.attribute("status"),
        e.attribute("size"),
        e.attribute("ready"),
        e.attribute("powerdownload"),
        e.attribute("temporaryfilenumber"),
        e.attribute("targetdirectory"));
}


/*!
    \fn XMLModule::handleServer( QDomElement& e )
 */
void XMLModule::handleServer( QDomElement& e )
{
    juicer->serverModule->insertServer(
        e.attribute("id"),
        e.attribute("name"),
        e.attribute("host"),
        e.attribute("port"),
        e.attribute("lastseen"),
        e.attribute("connectiontry"));
}


/*!
    \fn XMLModule::handleSearch( QDomElement& e )
 */
void XMLModule::handleSearch( QDomElement& e )
{
    juicer->searchModule->insertSearch(
        e.attribute("id"),
        e.attribute("searchtext"),
        e.attribute("running"),
        e.attribute("foundfiles"));
}


/*!
    \fn XMLModule::handleSearchEntry( QDomElement& e )
 */
void XMLModule::handleSearchEntry( QDomElement& e )
{
    QStringList filenames;
    QDomElement fileE;
    for(fileE=e.firstChildElement("filename"); !fileE.isNull(); fileE = fileE.nextSiblingElement("filename")) {
        filenames.append(fileE.attribute("name"));
    }
    juicer->searchModule->insertSearchEntry(
        e.attribute("id"),
        e.attribute("searchid"),
        e.attribute("size"),
        e.attribute("checksum"),
        filenames);
}


/*!
    \fn XMLModule::handleGeneralInformation( QDomNode& node )
 */
void XMLModule::handleGeneralInformation( QDomNode& node )
{
    juicer->setFilesystemSeparator(
        node.firstChildElement("filesystem").attribute("seperator"));
    juicer->setCoreVersion(node.firstChildElement("version").text());
}


/*!
    \fn XMLModule::handleRemoved( QDomElement& e )
 */
void XMLModule::handleRemoved( QDomElement& e )
{
    QDomElement objectE;
    for(objectE = e.firstChildElement("object");
        !objectE.isNull(); objectE = objectE.nextSiblingElement("object"))
    {
        QString id = objectE.attribute("id");
        if ( ! juicer->downloadModule->remove( id ) )
            if ( ! juicer->uploadModule->remove( id ) )
                if ( ! juicer->serverModule->remove( id ) )
                    juicer->searchModule->remove( id );
    }
}

/*!
    \fn XMLModule::handlePart( QDomElement& e )
 */
void XMLModule::handlePart( QDomElement& e )
{
    PartsWidget::Part part;
    part.type = e.attribute("type").toInt();
    part.fromPosition = e.attribute("fromposition").toULongLong();
    partList.push_back(part);
}


/*!
    \fn XMLModule::handlePartList( int id )
 */
void XMLModule::handlePartList( int id )
{
    if ( !partList.empty() )
    {
        if( partListRequests.contains( id ) )
        {
            juicer->downloadModule->setPartList(partListRequests[id], partsSize, partList);
            partListRequests.remove( id );
        }
        else if( partListSimpleRequests.contains( id ) )
        {
            DownloadItem* item = juicer->downloadModule->findDownload( partListSimpleRequests[id] );
            if ( item != NULL )
            {
                item->setParts( partsSize, partList );
            }
        }
        partList.clear();
    }
}

void XMLModule::sendToTray(const QString & message1, const QString & message2 ) {
    juicer->sendToTray( message1, message2 );
}


/*!
    \fn XMLModule::printAllAttributes(QDomElement& e)
 */
void XMLModule::printAllAttributes(QDomElement& e)
{
    QDomNamedNodeMap a = e.attributes();
    printf("%d\n", a.length());
    for( unsigned int i=0; i<a.length(); i++ ) {
        QDomNode item = a.item(i);
        printf("%s\t\t%s\n", item.nodeName().toLatin1().data(), item.nodeValue().toLatin1().data());
    }
}
