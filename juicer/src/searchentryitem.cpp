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
#include "searchentryitem.h"

#include "searchitem.h"

SearchEntryItem::SearchEntryItem( QString id, SearchItem* search, QString checksum, QString size, QTreeWidgetItem* parent ) 
  : Item( parent, id )
{
    this->search = search;
    if ( hash_.isEmpty() ) {
        hash_ = checksum;
    }
    size_ = size.toDouble();
}

SearchEntryItem::~SearchEntryItem()
{
}

bool SearchEntryItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    SearchEntryItem* searchItem = (SearchEntryItem*)&other;
    switch ( sortIndex )
    {
    case SearchItem::SIZE_COL:
        return size_ < searchItem->getSize();
    default:
        return this->text( sortIndex ) < other.text( sortIndex );
    }
}

void SearchEntryItem::setFilename( QString filename ) {
    filename_ = filename;
}

