#ifndef NEWSFEED_H
#define NEWSFEED_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>

class QNetworkReply;
class QTextBrowser;

/**
    Lädt die News-Seite zur Core-Version (https://applejuicenetz.github.io/news/VERSION.html)
    und zeigt sie auf der Startseite an. Ohne Version oder bei ungültiger Version wird 404.html
    geladen, wie bei den anderen GUIs.
*/
class NewsFeed : public QObject
{
    Q_OBJECT
public:
    NewsFeed(QTextBrowser* browser, QObject* parent = 0);

    /// News-URL zur Core-Version; ungültige oder leere Versionen ergeben 404.html.
    static QUrl newsUrl(const QString& version);
    /// Wandelt die Antwort per Charset aus Meta-Tag in Unicode, nimmt nur den Body und entfernt aktive Inhalte.
    static QString toHtml(const QByteArray& body);

public slots:
    /// Lädt die News neu, wenn sich die Version geändert hat oder noch nichts geladen wurde.
    void load(const QString& coreVersion);

private slots:
    void finished();

private:
    void showFallback();

    QTextBrowser* browser;
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QString loadedVersion;
    bool loaded;
};

#endif
