#include "newsfeed.h"
#include <QTextCodec>
#include <QSslSocket>
#include <cstdio>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #cond); failures++; } } while (0)

int main() {
    CHECK(NewsFeed::newsUrl("0.35.185.93").toString() == "https://applejuicenetz.github.io/news/0.35.185.93.html");
    CHECK(NewsFeed::newsUrl("").toString() == "https://applejuicenetz.github.io/news/404.html");
    CHECK(NewsFeed::newsUrl("../x").toString() == "https://applejuicenetz.github.io/news/404.html");
    CHECK(NewsFeed::newsUrl("1.2/a?b").toString() == "https://applejuicenetz.github.io/news/404.html");
    CHECK(NewsFeed::newsUrl("1..2").toString() == "https://applejuicenetz.github.io/news/404.html");

    QTextCodec* latin = QTextCodec::codecForName("ISO-8859-15");
    QByteArray page = latin->fromUnicode(QString::fromUtf8(
        "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-15\"/>"
        "<title>t</title><style>p{}</style></head><body>\n<p>Über <b>uns</b></p>"
        "<script>alert(1)</script></body></html>"));
    QString html = NewsFeed::toHtml(page);
    CHECK(html == QString::fromUtf8("<p>Über <b>uns</b></p>"));
    CHECK(NewsFeed::toHtml("").isEmpty());
    CHECK(NewsFeed::toHtml("<body>  </body>").isEmpty());
    CHECK(NewsFeed::toHtml("<p>ohne body</p>") == "<p>ohne body</p>");

    // Qt4 braucht OpenSSL 1.0.x zur Laufzeit; ohne HTTPS bleiben News und Update-Prüfung leer.
    CHECK(QSslSocket::supportsSsl());

    if (failures == 0) std::puts("PASS: news feed");
    return failures ? 1 : 0;
}
