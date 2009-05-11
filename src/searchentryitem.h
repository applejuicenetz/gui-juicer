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

#ifndef QAJSEARCHENTRYITEM_H
#define QAJSEARCHENTRYITEM_H

#include "item.h"

class SearchItem;

/**
	@author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class SearchEntryItem : public Item
{
public:
    SearchEntryItem( const QString& id, SearchItem* search, const QString& checksum, const QString& size, QTreeWidgetItem* parent );
    ~SearchEntryItem();

    SearchItem* search;
    virtual bool operator<( const QTreeWidgetItem & other ) const;
    void setFilename( const QString& filename );

    class Filter {
    public:
        double minSize, maxSize;
        QRegExp type, custom;
        
    };
    void setFilter(Filter& filter);
};

#endif
