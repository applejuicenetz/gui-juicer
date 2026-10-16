#include "serverlist.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QTimer>
#include <cstdio>
#include <cstring>

class Collector : public QObject
{
    Q_OBJECT
public:
    QEventLoop loop;
    QStringList links;
    QString message;
public slots:
    void ready(const QString& text) { links = ServerList::parseLinks(text); loop.quit(); }
    void error(const QString& m) { message = m; loop.quit(); }
};

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #cond); failures++; } } while (0)

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    CHECK(ServerList::defaultUrl() == "http://www.applejuicenet.cc/serverlist/xmllist.php");
    CHECK(ServerList::effectiveUrl("") == ServerList::defaultUrl());
    CHECK(ServerList::effectiveUrl("http://www.applejuicenet.de/18.0.html") == ServerList::defaultUrl());
    CHECK(ServerList::effectiveUrl("https://example.org/list.txt") == "https://example.org/list.txt");

    const QString xml = QString::fromUtf8(
        "<?xml version=\"1.0\"?><serverlist>"
        "<server lastseen=\"x\" link=\"ajfsp://server|aj-server.ddnss.de|7001/\" name=\"a &amp; b\"/>"
        "<server link=\"ajfsp://server|aj-server.ddnss.de|7001/\" name=\"dup\"/>"
        "<server link=\"ajfsp://server|apple-deluxe.ddns.net|9855/\" name=\"c\"/>"
        "<server link=\"ajfsp://file|x|y|1/\" name=\"no\"/></serverlist>");
    QStringList links = ServerList::parseLinks(xml);
    CHECK(links.size() == 2);
    CHECK(links.value(0) == "ajfsp://server|aj-server.ddnss.de|7001/");
    CHECK(links.value(1) == "ajfsp://server|apple-deluxe.ddns.net|9855/");
    CHECK(ServerList::parseLinks("ajfsp://server|h|1/\najfsp://server|h2|2/").size() == 2);
    CHECK(ServerList::parseLinks("").isEmpty());

    // Echter Abruf nur mit "--live URL"; das Netzwerk ist kein Pflichtbestandteil der CI.
    if (argc == 3 && std::strcmp(argv[1], "--live") == 0) {
        ServerList list;
        Collector collector;
        QObject::connect(&list, SIGNAL(linksReady(QString)), &collector, SLOT(ready(QString)));
        QObject::connect(&list, SIGNAL(failed(QString)), &collector, SLOT(error(QString)));
        QTimer::singleShot(20000, &collector.loop, SLOT(quit()));
        list.fetch(argv[2]);
        collector.loop.exec();
        std::printf("live links: %d %s\n", (int)collector.links.size(), qPrintable(collector.message));
        CHECK(collector.links.size() > 0);
    }

    if (failures == 0) std::puts("PASS: server list");
    return failures ? 1 : 0;
}

#include "serverlist_test.moc"
