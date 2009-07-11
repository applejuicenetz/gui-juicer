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

#include "incomingitem.h"

IncomingItem::IncomingItem(QFileInfo& fileInfo, QTreeWidget* parent) : Item(parent) {
    size = fileInfo.size();
    date = fileInfo.lastModified().toLocalTime();
    isFile = fileInfo.isFile();
    setText(IncomingItem::FILENAME_COL, fileInfo.fileName());
    setText(IncomingItem::SIZE_COL, Convert::bytes((double)size, 2));
    setText(IncomingItem::DATE_COL, date.toString(Qt::LocalDate));
    setFileIcon(IncomingItem::FILENAME_COL, fileInfo);
}

IncomingItem::IncomingItem(QUrlInfo& urlInfo, QTreeWidget* parent) : Item(parent) {
    size = urlInfo.size();
    date = urlInfo.lastModified().toLocalTime();
    isFile = urlInfo.isFile();
    setText(IncomingItem::FILENAME_COL, urlInfo.name());
    setText(IncomingItem::SIZE_COL, Convert::bytes((double)size, 2));
    setText(IncomingItem::DATE_COL, date.toString(Qt::LocalDate));
    //setFileIcon(IncomingItem::FILENAME_COL, urlInfo);
}

IncomingItem::~IncomingItem() {
}

bool IncomingItem::operator<(const QTreeWidgetItem & other) const {
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    Qt::SortOrder order = treeWidget()->header()->sortIndicatorOrder();
    IncomingItem* incomingItem = (IncomingItem*)&other;
    switch(sortIndex) {
        case SIZE_COL:
            return this->size < incomingItem->size;
            break;
        case DATE_COL:
            return this->date < incomingItem->date;
            break;
        case FILENAME_COL:
            if(isFile && !incomingItem->isFile) {
                return order == Qt::DescendingOrder;
            } else if(!isFile && incomingItem->isFile) {
                return order == Qt::AscendingOrder;
            } else {
                return this->text(FILENAME_COL) < other.text(FILENAME_COL);
            }
        default:
            return this->text(sortIndex) < other.text(sortIndex);
    }
}
