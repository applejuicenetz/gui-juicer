/***************************************************************************
 *   Copyright (C) 2004 by Matthias Reif                                   *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
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
#ifndef QAJUSERITEM_H
#define QAJUSERITEM_H

#include <QTreeWidget>
#include <QList>

#include "item.h"
#include "convert.h"
#include "optionsdialog.h"
#include "downloaditem.h"

static const QString ACTIVE_SOURCE = "7";
static const QString QUEUED_SOURCE = "5";
static const QString NEW_SOURCE = "-1";

/**
@author Matthias Reif
*/
class UserItem : public Item
{
public:
    UserItem( const QString& id, QTreeWidget *parent );
    UserItem( const QString& id, QTreeWidgetItem *parent );

    ~UserItem();

    void update( const QString& fileName, const QString& nickname, const QString& speed, const QString& status, const QString& power, const QString& queuePos, const QString& statusString, QIcon& osIcon, const QString& downloadfrom, const QString& downloadto, const QString& actualdownloadposition, const QTime& time );

    void setSpeed( const QString& newSpeedString, const QTime& time );

    int getSpeed()
    {
        return speed;
    }
    int getQueuePos()
    {
        return queuePos;
    }
    QString getId()
    {
        return id;
    }
    QString getPower()
    {
        return power;
    }
    void setAvgSpeed();
private:
    void init();
protected:
    int speed;
    QList<int> speeds;
    QList<QTime> times;
    QString id;
    int queuePos;
    QString power;
    QString fileName;
    bool fileNameSet;
};

#endif
