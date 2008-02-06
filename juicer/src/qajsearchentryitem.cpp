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

QAjSearchEntryItem::QAjSearchEntryItem( QString id, QAjSearchItem* search, QString checksum, QString size, QTreeWidgetItem* parent ) : QAjItem( parent, id )
{
    this->search = search;
    if ( this->hash.isEmpty() ) {
        this->hash = checksum;
    }
    this->size = size.toDouble();
}

QAjSearchEntryItem::~QAjSearchEntryItem()
{
}

bool QAjSearchEntryItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    QAjSearchEntryItem* searchItem = (QAjSearchEntryItem*)&other;
    switch ( sortIndex )
    {
    case SIZE_SEARCH_INDEX:
        return this->size < searchItem->size;
    default:
        return this->text( sortIndex ) < other.text( sortIndex );
    }
}

void QAjSearchEntryItem::setFilename( QString filename ) {
    this->filename = filename;
}

