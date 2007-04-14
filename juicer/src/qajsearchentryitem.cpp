/***************************************************************************
 *   Copyright (C) 2007 by Matthias Reif   *
 *   matthias.reif@informatik.tu-chemnitz.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "qajsearchentryitem.h"

#include "qajsearchitem.h"

QAjSearchEntryItem::QAjSearchEntryItem( QString id, QAjSearchItem* search, QString checksum, QString size, QTreeWidget* parent ) : QAjItem( parent, id )
{
    this->search = search;
    this->checksum = checksum;
    this->size = size;
}

QAjSearchEntryItem::QAjSearchEntryItem( QString id, QAjSearchItem* search, QString checksum, QString size, QTreeWidgetItem* parent ) : QAjItem( parent, id )
{
    this->search = search;
    this->checksum = checksum;
    this->size = size;
}

QAjSearchEntryItem::~QAjSearchEntryItem()
{
}

int QAjSearchEntryItem::compare( QTreeWidgetItem* i, int col, bool ) const
{
    if ( col == SIZE_SEARCH_INDEX )
    {
        qulonglong a = size.toULongLong();
        qulonglong b = ((QAjSearchEntryItem*)i)->size.toULongLong();
        if ( a > b )
            return 1;
        else if ( a < b )
            return -1;
        else
            return 0;
    }
    else
    {
        if ( text( col ) > i->text( col ) )
            return 1;
        else if ( text( col ) < i->text( col ) )
            return -1;
        else
            return 0;
    }
}

