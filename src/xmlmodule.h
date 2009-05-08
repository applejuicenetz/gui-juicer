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

#include "optionsdialog.h"
#include "partswidget.h"
#include "md5class.h"
#include "item.h"

/**
@author Matthias Reif
*/

class Juicer;
class QXMLHandler;

class XMLModule : public QHttp
{
    Q_OBJECT
public:
    XMLModule(Juicer *juicer, QObject *parent = 0);

    ~XMLModule();

    void resetHttp();

    enum Type {GET, SET};
    int exec( const QString & request, int nErrors = 0 );
    int make( Type type, const QString & request, QString param = "" );
    int get( const QString & request, QString param = "" );
    int set( const QString & request, QString param = "" );

    QString session;

    void setPassword( const QString & password );
    void setPasswordMD5( const QString & passwordMD5 )
    {
        this->passwordMD5 = passwordMD5;
    }
    void sendToTray(const QString & message1, const QString & message2 );
    QString getRecentTime() { return timeStamp; }
    static void printAllAttributes(QDomElement& e);

    const QDomDocument& getContent() const {
        return doc;
    }

protected:
    Juicer *juicer;
    QString timeStamp;
    QString passwordMD5;

    QDomDocument doc;

    QLinkedList<PartsWidget::Part> partList;
    qulonglong partsSize;
    QHash<int, QString> requests;
    QHash<int, int> errors;
    QHash<int, QString> partListRequests;
    QHash<int, QString> partListSimpleRequests;

public slots:
    void responseHeaderReceived ( const QHttpResponseHeader & resp );
    void requestFinished(int id, bool error);
signals:
    void settingsReady( const AjSettings& settings );
    void error(const QString& message );
    void gotSession();
    void modifiedDone();
private:
    void handleSettings( QDomElement& e );
    void handleShare( QDomElement& e );
    void handleShares( QDomElement& e );
    void handleIds( QDomNode& node );
    void handleNetworkInfo( QDomElement& e );
    void handleUpload( QDomElement& e );
    void handleDownload( QDomElement& e );
    void handleUser( QDomElement& e, QTime& time );
    void handleServer( QDomElement& e );
    void handleSearch( QDomElement& e );
    void handleSearchEntry( QDomElement& e );
    void handleGeneralInformation( QDomNode& node );
    void handleRemoved( QDomElement& e );
    void handlePart( QDomElement& e );
    void handlePartList( int id );
    void processUsers();
    QList<QDomElement> users;
    QList<QTime> userTimes;
private slots:
    void networkErrorSlot();
    void httpErrorSlot();
};

#endif
