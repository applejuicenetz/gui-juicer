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

#include "item.h"

Item::Item( QTreeWidget *parent, const QString& id ) 
  : QTreeWidgetItem( parent ), QObject()
  , id_(id)
  , status_("-1")
  , size_(0.0)
{
}

Item::Item( QTreeWidgetItem *parent, const QString& id ) 
  : QTreeWidgetItem( parent ), QObject()
  , id_(id)
  , status_("-1")
  , size_(0.0)
{
}


Item::~Item() {
    setHiddenSave(false);
}


/*!
    \fn Item::setHiddenSave(bool hidden)
 */
bool Item::setHiddenSave(bool hidden) {
    bool r = isHidden();
    if(r != hidden) {
        qApp->processEvents();
        setHidden(hidden);
        qApp->processEvents();
    }
    return r;
}

void Item::setFileIcon(int column) {
    setIcon(column, QIcon(Convert::getFileIcon(text(column))));
}

bool Item::setStatus(const QString& status) {
    bool newStatus = status_ != status;
    if(newStatus) {
        status_ = status;
    }
    return newStatus;
}
