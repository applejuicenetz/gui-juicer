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

    enum Type {GET, SET};
    void resetHttp();
    int exec(const QString & request);
    int make(Type type, const QString & request, QString param = "");
    int get(const QString & request, QString param = "");
    int set(const QString & request, QString param = "");
    void setPassword(const QString & password);
    void setPasswordMD5(const QString & passwordMD5);
    void sendToTray(const QString & message1, const QString & message2);
    static void printAllAttributes(QDomElement& e);

    QString getRecentTime() const { return timeStamp; }
    const QDomDocument& getContent() const { return doc; }
    int setHost(const QString& hostName, quint16 port = 80);

protected:
    Juicer *juicer;
    QString timeStamp;
    QString passwordMD5;
    QString session;

    QDomDocument doc;

    PartsWidget::PartList partList;
    QHash<int, QString> requests;
    QHash<int, int> errors;
    QHash<int, QString> partListRequests;
    QHash<int, QString> partListSimpleRequests;

signals:
    void settingsReady(const AjSettings& settings);
    void error(const QString& message);
    void gotSession();
    void modifiedDone();
private:
    void handleSettings(QDomElement& e);
    void handleShare(QDomElement& e);
    void handleShares(QDomElement& e);
    void handleIds(QDomNode& node);
    void handleNetworkInfo(QDomElement& e);
    void handleInformation(QDomElement& e);
    void handleUpload(QDomElement& e);
    void handleDownload(QDomElement& e);
    void handleUser(QDomElement& e, QTime& time);
    void handleServer(QDomElement& e);
    void handleSearch(QDomElement& e);
    void handleSearchEntry(QDomElement& e);
    void handleGeneralInformation(QDomNode& node);
    void handleRemoved(QDomElement& e);
    void handlePart(QDomElement& e);
    void handlePartList(int id);
    void processUsers();
    QList<QDomElement> users;
    QList<QTime> userTimes;
private slots:
    void responseHeaderReceived(const QHttpResponseHeader & resp);
    void requestFinished(int id, bool error);
    void networkErrorSlot();
    void httpErrorSlot();
};

#endif
