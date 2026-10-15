#include "updatecheck.h"

#include <QApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>
#include <QSslSocket>

#include "convert.h"

static const char* const LATEST_API =
    "https://api.github.com/repos/applejuicenetz/gui-juicer/releases/latest";
static const char* const RELEASE_PAGE =
    "https://github.com/applejuicenetz/gui-juicer/releases/tag/";
static const int MAX_RESPONSE = 256 * 1024;

UpdateCheck::UpdateCheck(QWidget* parent)
    : QObject(parent), parentWidget(parent), reply(0), silent(true)
{
}

QString UpdateCheck::parseTagName(const QByteArray& json) {
    QRegExp tag("\"tag_name\"\\s*:\\s*\"([^\"]*)\"");
    if (tag.indexIn(QString::fromUtf8(json)) < 0) {
        return QString();
    }
    QString name = tag.cap(1);
    // Nur Versionsnummern wie v1.4 oder 1.10.2 zulassen; der Wert landet in einer URL.
    if (!QRegExp("v?[0-9]+(\\.[0-9]+)*").exactMatch(name)) {
        return QString();
    }
    return name;
}

static QString stripV(const QString& tag) {
    return tag.startsWith('v') ? tag.mid(1) : tag;
}

bool UpdateCheck::isNewer(const QString& tag, const QString& current) {
    if (tag.isEmpty()) {
        return false;
    }
    return Convert::compareVersion(stripV(tag), stripV(current)) > 0;
}

QUrl UpdateCheck::releaseUrl(const QString& tag) {
    return QUrl(QString(RELEASE_PAGE) + tag);
}

void UpdateCheck::check(bool silent) {
    if (reply != 0) {
        return;
    }
    this->silent = silent;
    QNetworkRequest request(QUrl(QString::fromLatin1(LATEST_API)));
    request.setRawHeader("User-Agent", "Juicer");
    request.setRawHeader("Accept", "application/vnd.github+json");
    reply = manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void UpdateCheck::finished() {
    QNetworkReply* r = reply;
    reply = 0;
    r->deleteLater();
    if (r->error() != QNetworkReply::NoError || r->bytesAvailable() > MAX_RESPONSE) {
        if (!silent) {
            QMessageBox::warning(parentWidget, tr("Update check"),
                tr("Could not check for updates:\n%1").arg(r->errorString()));
        }
        return;
    }
    QString tag = parseTagName(r->readAll());
    if (tag.isEmpty()) {
        if (!silent) {
            QMessageBox::warning(parentWidget, tr("Update check"),
                tr("Unexpected response from the update server."));
        }
        return;
    }
    QString current = qApp->applicationVersion();
    if (isNewer(tag, current)) {
        QMessageBox box(QMessageBox::Information, tr("Update check"),
            tr("A new version of Juicer is available: %1 (installed: %2).\n"
               "Open the release page to download it?").arg(stripV(tag), current),
            QMessageBox::Yes | QMessageBox::No, parentWidget);
        if (box.exec() == QMessageBox::Yes) {
            QDesktopServices::openUrl(releaseUrl(tag));
        }
    } else if (!silent) {
        QMessageBox::information(parentWidget, tr("Update check"),
            tr("Juicer %1 is up to date.").arg(current));
    }
}
