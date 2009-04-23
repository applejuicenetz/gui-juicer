//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "shareselectiondialog.h"

ShareSelectionDialog::ShareSelectionDialog(XMLModule* xml, QWidget* parent, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::Dialog() {
    setupUi(this);
    this->xml = xml;
    expandedItem = NULL;
    shareIcons[WORKSTATION] = QIcon(":/small/system.png");
    shareIcons[DRIVE] = QIcon(":/small/hdd.png");
    shareIcons[DISKDRIVE] = QIcon(":/small/floppy.png");
    shareIcons[FOLDER] = QIcon(":/small/folder.png");
    shareIcons[DESKTOP] = QIcon(":/small/desktop.png");
    treeWidget->sortItems(0, Qt::AscendingOrder);
    connect(treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
        this, SLOT(updateSubDirectoriesSlot(QTreeWidgetItem*)));
}

ShareSelectionDialog::~ShareSelectionDialog() {
}

void ShareSelectionDialog::insertDirectory(const QString& dir, const QString& path, int type) {
    if(type != DISKDRIVE) {
        QTreeWidgetItem* item = NULL;
        if(expandedItem == NULL) {
            item = new QTreeWidgetItem(treeWidget);
        } else {
            bool present = false;
            for(int i=0; i<expandedItem->childCount() && !present; i++) {
                present = expandedItem->child(i)->text(0) == dir;
            }
            if(!present) {
                item = new QTreeWidgetItem(expandedItem);
            }
        }
        if(item != NULL) {
            item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
            item->setText(0, dir);
            item->setIcon(0, shareIcons[type]);
        }
    }
}

void ShareSelectionDialog::insertSeperator(const QString& seperator) {
    this->filesystemSeperator = seperator;
}


/*!
    \fn ShareSelectionDialog::updateSubDirectoriesSlot(QTreeWidgetItem* item)
 */
void ShareSelectionDialog::updateSubDirectoriesSlot(QTreeWidgetItem* item) {
    if(item->childCount() == 0) {
        expandedItem = item;
        xml->get( "directory", "&directory="+getPath(item));
    }
}


/*!
    \fn ShareSelectionDialog::getPath(QTreeWidgetItem* item)
 */
QString ShareSelectionDialog::getPath(QTreeWidgetItem* item) {
    QString path = "";
    while(item != NULL) {
        path = item->text(0) + filesystemSeperator + path;
        item = item->parent();
    }
    return path;
}


/*!
    \fn ShareSelectionDialog::selectedPath()
 */
QString ShareSelectionDialog::selectedPath() {
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    if(!selectedItems.empty()) {
        return getPath(selectedItems.first());
    } else {
        return "";
    }
}
