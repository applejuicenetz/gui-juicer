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

#include "qajitem.h"
#include "qconvert.h"
#include "types.h"
#include "qajoptionsdialog.h"

/**
@author Matthias Reif
*/
class QAjUserItem : public QAjItem
{
public:
    QAjUserItem( QString id, QTreeWidget *parent );
    QAjUserItem( QString id, QTreeWidgetItem *parent );

    ~QAjUserItem();

    void update( QString fileName, QString speed, QString status, QString power, QString queuePos, QString statusString, QIcon* osIcon, QTime time );

    void setSpeed( QString newSpeedString, QTime time );

    int getSpeedDif()
    {
        return speedDif;
    }
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
    int speed, newSpeed, speedDif;
    QList<int> speeds;
    QList<QTime> times;
    QString id;
    int queuePos;
    QString power;
    QString fileName;
    bool fileNameSet;
};

#endif
