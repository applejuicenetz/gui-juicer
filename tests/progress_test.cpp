#include "item.h"
#include <QApplication>
#include <cstdio>
class Probe : public Item {
public:
    Probe(QTreeWidget* tree) : Item(tree) {}
    using Item::updateProgressBar;
    using Item::progressPercentualValue;
    void setSize(qulonglong size) { size_ = size; }
};
int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QProgressBar bar;
    Probe::updateProgressBar(bar, QString("4294967296"), QString("8589934592"), QString("6442450944"));
    if (Probe::progressPercentualValue(bar) != 50) {
        std::fprintf(stderr, "FAIL: 64-bit byte-position progress must be 50 percent, got %d\n", Probe::progressPercentualValue(bar));
        return 1;
    }
    const qulonglong maximum = Q_UINT64_C(9223372036854775807);
    Probe::updateProgressBar(bar, maximum - 100, maximum, maximum - 50);
    if (Probe::progressPercentualValue(bar) != 50) return 2;
    Probe::updateProgressBar(bar, 10, 10, 10);
    if (Probe::progressPercentualValue(bar) != 0) return 3;
    Probe::updateProgressBar(bar, 10, 100, 0);
    if (Probe::progressPercentualValue(bar) != 0) return 4;
    Probe::updateProgressBar(bar, 10, 100, 200);
    if (Probe::progressPercentualValue(bar) != 100) return 5;
    QTreeWidget tree;
    Probe* item = new Probe(&tree);
    const qulonglong exact = Q_UINT64_C(9007199254740993);
    item->setSize(exact);
    if (static_cast<qulonglong>(item->getSize()) != exact) {
        std::fprintf(stderr, "FAIL: file size above 2^53 lost precision\n");
        return 1;
    }
    std::puts("PASS: 64-bit byte-position progress and exact file sizes");
    return 0;
}
