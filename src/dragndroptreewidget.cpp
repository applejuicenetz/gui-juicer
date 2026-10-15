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

#include "dragndroptreewidget.h"

DragNDropTreeWidget::DragNDropTreeWidget(QWidget *parent) : QTreeWidget(parent) {
    this->incomingModule = NULL;
}

DragNDropTreeWidget::~DragNDropTreeWidget()
{
}

void DragNDropTreeWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPosition = event->pos();
    }
    QTreeWidget::mousePressEvent(event);
}

void DragNDropTreeWidget::mouseMoveEvent(QMouseEvent *event) {
    QTreeWidget::mouseMoveEvent(event);

    if(!(event->buttons() & Qt::LeftButton))
        return;
    if((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;
    if(incomingModule != NULL) {
        QString actDir = incomingModule->getActualIncomingDir();
        if(!actDir.isEmpty()) {
            QItemList items = selectedItems();
            QList<QUrl> urls;
            for(int i=0; i<items.size(); i++) {
                urls << QUrl("file://" + actDir + items[i]->text(IncomingItem::FILENAME_COL));
            }
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            mimeData->setUrls(urls);
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction | Qt::MoveAction | Qt::LinkAction);
            incomingModule->reload();
        }
    }
}


/*!
    \fn DragNDropTreeWidget::setIncomingModule(IncomingModule* incomingModule)
 */
void DragNDropTreeWidget::setIncomingModule(IncomingModule* incomingModule) {
    this->incomingModule = incomingModule;
}
