#include "updatecheck.h"
#include <cstdio>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #cond); failures++; } } while (0)

int main() {
    const QByteArray release(
        "{\"url\":\"https://api.github.com/repos/applejuicenetz/gui-juicer/releases/1\","
        "\"html_url\":\"https://github.com/applejuicenetz/gui-juicer/releases/tag/v1.4\","
        "\"author\":{\"login\":\"red171\",\"html_url\":\"https://github.com/red171\"},"
        "\"tag_name\":\"v1.4\",\"name\":\"Juicer 1.4\",\"draft\":false,\"prerelease\":false}");
    CHECK(UpdateCheck::parseTagName(release) == "v1.4");
    CHECK(UpdateCheck::parseTagName("{\"tag_name\" : \"1.10.2\"}") == "1.10.2");
    CHECK(UpdateCheck::parseTagName("{\"message\":\"Not Found\"}").isEmpty());
    CHECK(UpdateCheck::parseTagName("").isEmpty());
    CHECK(UpdateCheck::parseTagName("{\"tag_name\":\"v1.4<b>x</b>\"}").isEmpty());
    CHECK(UpdateCheck::parseTagName("{\"tag_name\":\"nightly\"}").isEmpty());

    CHECK(UpdateCheck::isNewer("v1.4", "1.3"));
    CHECK(UpdateCheck::isNewer("1.3.1", "1.3"));
    CHECK(UpdateCheck::isNewer("1.10", "1.9"));
    CHECK(!UpdateCheck::isNewer("v1.3", "1.3"));
    CHECK(!UpdateCheck::isNewer("1.2", "1.3"));
    CHECK(!UpdateCheck::isNewer("", "1.3"));

    CHECK(UpdateCheck::releaseUrl("v1.4").toString() ==
          "https://github.com/applejuicenetz/gui-juicer/releases/tag/v1.4");

    if (failures == 0) std::puts("PASS: update check");
    return failures ? 1 : 0;
}
