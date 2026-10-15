#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>

class QNetworkReply;

/**
    Prüft über die GitHub-Release-API, ob eine neuere Juicer-Version existiert,
    und verweist auf die Release-Seite. Es wird nichts heruntergeladen oder installiert.
*/
class UpdateCheck : public QObject
{
    Q_OBJECT
public:
    UpdateCheck(QWidget* parent = 0);

    /// Tag aus der Antwort von /releases/latest; leer bei ungültiger Antwort.
    static QString parseTagName(const QByteArray& json);
    /// true, wenn tag (mit optionalem "v") neuer als current ist.
    static bool isNewer(const QString& tag, const QString& current);
    static QUrl releaseUrl(const QString& tag);

public slots:
    /// silent: keine Meldung, wenn aktuell oder bei Fehlern (Start-Prüfung).
    void check(bool silent = true);

private slots:
    void finished();

private:
    QWidget* parentWidget;
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    bool silent;
};

#endif
