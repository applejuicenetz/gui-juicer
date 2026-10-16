#include "newsfeed.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>
#include <QTextBrowser>
#include <QTextCodec>

static const char* const NEWS_URL = "https://applejuicenetz.github.io/news/%1.html";
static const int MAX_RESPONSE = 256 * 1024;

NewsFeed::NewsFeed(QTextBrowser* browser, QObject* parent)
    : QObject(parent), browser(browser), reply(0), loaded(false)
{
    browser->setOpenExternalLinks(true);
}

QUrl NewsFeed::newsUrl(const QString& version) {
    // Der Wert landet in einer URL: nur Versionsnummern wie 0.35.185.93 zulassen.
    QString name = QRegExp("[0-9]+(\\.[0-9]+)*").exactMatch(version) ? version : QString("404");
    return QUrl(QString(NEWS_URL).arg(name));
}

QString NewsFeed::toHtml(const QByteArray& body) {
    QTextCodec* codec = 0;
    QRegExp charset("charset\\s*=\\s*[\"']?([A-Za-z0-9_\\-]+)", Qt::CaseInsensitive);
    QString head = QString::fromLatin1(body.left(2048));
    if (charset.indexIn(head) >= 0) {
        codec = QTextCodec::codecForName(charset.cap(1).toLatin1());
    }
    QString html = codec ? codec->toUnicode(body) : QString::fromUtf8(body.constData(), body.size());

    QRegExp bodyTag("<body\\b[^>]*>(.*)</body\\s*>", Qt::CaseInsensitive);
    bodyTag.setMinimal(true);
    if (bodyTag.indexIn(html) >= 0) {
        html = bodyTag.cap(1);
    }
    QRegExp active("<(script|style|iframe|object|embed)\\b.*</\\1\\s*>", Qt::CaseInsensitive);
    active.setMinimal(true);
    html.remove(active);
    return html.trimmed();
}

void NewsFeed::load(const QString& coreVersion) {
    if (reply != 0 || (loaded && coreVersion == loadedVersion)) {
        return;
    }
    loadedVersion = coreVersion;
    QNetworkRequest request(newsUrl(coreVersion));
    request.setRawHeader("User-Agent", "Juicer");
    reply = manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void NewsFeed::finished() {
    QNetworkReply* r = reply;
    reply = 0;
    r->deleteLater();
    // GitHub Pages liefert für fehlende Versionen die 404.html mit Status 404.
    bool usable = r->error() == QNetworkReply::NoError
        || r->error() == QNetworkReply::ContentNotFoundError;
    QByteArray data = usable ? r->read(MAX_RESPONSE + 1) : QByteArray();
    QString html = data.size() > MAX_RESPONSE ? QString() : toHtml(data);
    if (html.isEmpty()) {
        loaded = false;
        showFallback();
        return;
    }
    loaded = true;
    browser->setHtml(html);
}

void NewsFeed::showFallback() {
    browser->setHtml(QString("<p>%1</p><p><a href=\"https://applejuicenetz.github.io/faq/\">appleJuice FAQ</a></p>")
        .arg(tr("The news could not be loaded.")));
}
