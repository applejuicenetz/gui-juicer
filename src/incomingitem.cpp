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

IncomingItem::IncomingItem( qint64 size, QDateTime date, QTreeWidget* parent ) : Item( parent )
{
    this->size = size;
    this->date = date;
}


IncomingItem::~IncomingItem()
{
}

bool IncomingItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    IncomingItem* incomingItem = (IncomingItem*)&other;
    switch ( sortIndex )
    {
        case SIZE_COL:
            return this->size < incomingItem->size;
            break;
        case DATE_COL:
            return this->date < incomingItem->date;
            break;
        default:
            return this->text( sortIndex ) < other.text( sortIndex );
    }
}

