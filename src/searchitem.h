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

#ifndef QAJSEARCHITEM_H
#define QAJSEARCHITEM_H

#include <QHash>
#include <QHeaderView>

#include "item.h"
#include "searchentryitem.h"

/**
@author Matthias Reif
*/
class SearchItem : public Item
{
public:
    SearchItem( const QString& id, QTreeWidget* parent );

    ~SearchItem();

    enum {TEXT_COL, SIZE_COL, COUNT_COL};

    SearchEntryItem* findSearchEntry( const QString& id );

    int hits;
    int entriesCount;

    QHash<QString, SearchEntryItem*> entries;

    virtual bool operator<( const QTreeWidgetItem & other ) const;

    int getHits() const {
      return hits;
    }

};

#endif
