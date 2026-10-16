#ifndef SERVERLIST_H
#define SERVERLIST_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QNetworkAccessManager>

class QNetworkReply;

/**
    Lädt die Server-Quelle (Standard wie JavaGUI und phpGUI:
    http://www.applejuicenet.cc/serverlist/xmllist.php) und liefert die ajfsp-Server-Links.
*/
class ServerList : public QObject
{
    Q_OBJECT
public:
    ServerList(QObject* parent = 0);

    static QString defaultUrl();
    /// Leere oder frühere Standard-URLs (applejuicenet.de/18.0.html) werden durch den aktuellen Standard ersetzt.
    static QString effectiveUrl(const QString& stored);
    /// Eindeutige ajfsp://server|host|port/-Links aus der Antwort, in Reihenfolge der Quelle.
    static QStringList parseLinks(const QString& text);

public slots:
    void fetch(const QString& url);

signals:
    void linksReady(const QString& text);
    void failed(const QString& message);

private slots:
    void finished();

private:
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    int redirects;
};

#endif
