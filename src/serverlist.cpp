#include "serverlist.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>

static const char* const DEFAULT_URL = "http://www.applejuicenet.cc/serverlist/xmllist.php";
static const int MAX_RESPONSE = 1024 * 1024;
static const int MAX_REDIRECTS = 3;

ServerList::ServerList(QObject* parent) : QObject(parent), reply(0), redirects(0) {
}

QString ServerList::defaultUrl() {
    return QString::fromLatin1(DEFAULT_URL);
}

QString ServerList::effectiveUrl(const QString& stored) {
    QString url = stored.trimmed();
    if (url.isEmpty() || url.contains("applejuicenet.de/18.0.html", Qt::CaseInsensitive)) {
        return defaultUrl();
    }
    return url;
}

QStringList ServerList::parseLinks(const QString& text) {
    QStringList result;
    QRegExp re("ajfsp://server[|][^/\"'<>\\s]*[/]");
    int d = 0;
    while ((d = re.indexIn(text, d)) >= 0) {
        QString link = text.mid(d, re.matchedLength());
        if (!result.contains(link)) {
            result.append(link);
        }
        d += re.matchedLength();
    }
    return result;
}

void ServerList::fetch(const QString& url) {
    if (reply != 0) {
        return;
    }
    QString target = effectiveUrl(url);
    // Eingaben ohne Schema wie "host/pfad" wie bisher als HTTP behandeln.
    if (!target.contains("://")) {
        target = "http://" + target;
    }
    redirects = 0;
    QNetworkRequest request((QUrl(target)));
    request.setRawHeader("User-Agent", "Juicer");
    reply = manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void ServerList::finished() {
    QNetworkReply* r = reply;
    reply = 0;
    r->deleteLater();

    QUrl redirect = r->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirect.isValid() && redirects < MAX_REDIRECTS) {
        redirect = r->url().resolved(redirect);
        if (redirect.scheme() == "http" || redirect.scheme() == "https") {
            redirects++;
            QNetworkRequest request(redirect);
            request.setRawHeader("User-Agent", "Juicer");
            reply = manager.get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(finished()));
            return;
        }
    }
    if (r->error() != QNetworkReply::NoError) {
        emit failed(r->errorString());
        return;
    }
    QByteArray data = r->read(MAX_RESPONSE + 1);
    if (data.size() > MAX_RESPONSE) {
        emit failed(tr("The response is too large."));
        return;
    }
    emit linksReady(QString::fromUtf8(data.constData(), data.size()));
}
