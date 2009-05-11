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

#ifndef QAJSERVERITEM_H
#define QAJSERVERITEM_H

#include <QHeaderView>
#include <QDateTime>

#include "item.h"

/**
    @author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class ServerItem : public Item
{
public:
    ServerItem( const QString& id, QTreeWidget *parent );

    ~ServerItem();

    enum {NAME_COL, HOST_COL, PORT_COL, LASTSEEN_COL, TESTS_COL};
    virtual bool operator<( const QTreeWidgetItem & other ) const;
    void update(const QString& name, const QString& host, const QString& port, const QString& lastseen, const QString& tests, const QString& time);
};

#endif
