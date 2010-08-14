/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#include "sharefileitem.h"
#include "shareitem.h"

QString ShareFileItem::filesystemSeperator = "";

ShareFileItem::ShareFileItem(const QString& id, ShareItem *parent) : Item(parent, id) {
    prioritySpin = new QSpinBox();
    prioritySpin->setMaximumHeight(20);
    prioritySpin->setRange(1, 250);
    QTimer::singleShot(100, this, SLOT(initPrioritySpin()));
}


ShareFileItem::~ShareFileItem() {
}


void ShareFileItem::initPrioritySpin() {
    treeWidget()->setItemWidget(this, ShareItem::PRIORITY_COL, prioritySpin);
    connect(prioritySpin, SIGNAL(valueChanged(const QString&)), this, SLOT(priorityChanged(const QString &)));
}


void ShareFileItem::update(const QString& hash,
                           const QString& fileName,
                           const QString& size,
                           const QString& priority,
                           const QString& lastAsked,
                           const QString& askCount,
                           const QString& searchCount,
                           const QString& filesystemSeperator) {
    setText(ShareItem::PATH_COL, fileName.split(filesystemSeperator).last());
    setText(ShareItem::SIZE_COL, Convert::bytesExtra(size));
    prioritySpin->setValue(priority.toInt());
    setText(ShareItem::LAST_ASKED_COL, Convert::dateString(lastAsked));
    setText(ShareItem::ASK_COUNT_COL, askCount);
    setText(ShareItem::SEARCH_COUNT_COL, searchCount);

    if (hash_.isEmpty() && !hash.isEmpty()) {
        hash_ = hash;
    }
    if (filename_.isEmpty() && !fileName.isEmpty()) {
        filename_ = fileName;
        setFileIcon(ShareItem::PATH_COL);
    }
    if (this->filesystemSeperator.isEmpty() && !filesystemSeperator.isEmpty()) {
        this->filesystemSeperator = filesystemSeperator;
    }

    size_ = size.toDouble();
    this->lastAsked = lastAsked.toULongLong();

    ShareItem* parentItem = dynamic_cast<ShareItem*>(QTreeWidgetItem::parent());
    if (parentItem != NULL) parentItem->update();
}

void ShareFileItem::updatePrio(int prio) {
    setText(ShareItem::PRIORITY_COL, QString::number(prio));
}


bool ShareFileItem::operator<(const QTreeWidgetItem & other) const {
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    const ShareFileItem* shareFileItem = dynamic_cast<const ShareFileItem*>(&other);
    if (shareFileItem == NULL) {
        return false;
    }
    switch (sortIndex) {
    case ShareItem::PATH_COL:
        return text(ShareItem::PATH_COL) < other.text(ShareItem::PATH_COL);
    case ShareItem::SIZE_COL:
        return size_ < shareFileItem->getSize();
    case ShareItem::LAST_ASKED_COL:
        return lastAsked < shareFileItem->lastAsked;
    case ShareItem::ASK_COUNT_COL:
        return text(ShareItem::ASK_COUNT_COL).toInt() < other.text(ShareItem::ASK_COUNT_COL).toInt();
    case ShareItem::SEARCH_COUNT_COL:
        return text(ShareItem::SEARCH_COUNT_COL).toInt() < other.text(ShareItem::SEARCH_COUNT_COL).toInt();
    default:
        return text(sortIndex) < other.text(sortIndex);
    }
}

QString ShareFileItem::getLinkAJFSP() {
    QString ajfspLink = "ajfsp://file|%1|%2|%3/";
    QString file = filename_.split(filesystemSeperator).last();
    return ajfspLink.arg(file).arg(hash_).arg((unsigned long)size_);
}


/*!
    \fn ShareFileItem::priorityChanged(const QString& priority)
 */
void ShareFileItem::priorityChanged(const QString& priority) {
    priorityChanged(id_, priority);
}
