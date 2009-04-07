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
#include "serveritem.h"

ServerItem::ServerItem( const QString& id, QTreeWidget *parent ) : Item( parent, id )
{
}


ServerItem::~ServerItem()
{
}

bool ServerItem::operator<( const QTreeWidgetItem & other ) const {
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    switch ( sortIndex ) {
        case PORT_COL:
            return this->text(PORT_COL).toInt() < other.text(PORT_COL).toInt();
        case TESTS_COL:
            return this->text(TESTS_COL).toInt() < other.text(TESTS_COL).toInt();
        case LASTSEEN_COL:
            return QDateTime::fromString(this->text(LASTSEEN_COL), Qt::LocalDate) < QDateTime::fromString(other.text(LASTSEEN_COL), Qt::LocalDate);
        default:
            return this->text(sortIndex) < other.text(sortIndex);
    }
}



/*!
    \fn ServerItem::update(const QString& name, const QString& host, const QString& port, const QString& lastseen, const QString& tests, const QString& time)
 */
void ServerItem::update(const QString& name, const QString& host, const QString& port, const QString& lastseen, const QString& tests, const QString& time) {
    setText(NAME_COL, name);
    setText(HOST_COL, host);
    setText(PORT_COL, port);
    setText(LASTSEEN_COL, time);
    setText(TESTS_COL, tests);
}
