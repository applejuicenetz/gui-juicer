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

#ifndef QAJSHAREFILEITEM_H
#define QAJSHAREFILEITEM_H

#include <QString>
#include <QObject>

#include "useritem.h"

#include "item.h"

class ShareItem;

/**
	@author Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>
*/
class ShareFileItem : public Item
{
public:
    ShareFileItem( const QString& id, ShareItem *parent = 0 );
    virtual ~ShareFileItem();

    void update(const QString& hash, const QString& fileName, const QString& size,
                const QString& priority, const QString& lastAsked, const QString& askCount,
                const QString& searchCount, const QString& filesystemSeperator );

    QString getLinkAJFSP();

    void updatePrio( int prio );
    virtual bool operator<(const QTreeWidgetItem & other) const;
    qulonglong lastAsked;

protected:
    static QString filesystemSeperator;
};

#endif
