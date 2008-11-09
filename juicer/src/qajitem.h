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
#ifndef QAJITEM_H
#define QAJITEM_H

#include <QObject>
#include <QTreeWidget>

/**
@author Matthias Reif
*/
class QAjItem : public QTreeWidgetItem, public QObject
{
// Q_OBJECT
public:
    QAjItem( QTreeWidget *parent, const QString& id="generic" );
    QAjItem( QTreeWidgetItem *parent, const QString& id="generic" );

    virtual ~QAjItem();

    QString getStatus()
    {
        return status;
    }
    void setStatus( const QString& newStatus )
    {
        status = newStatus;
    }

    QString getId()
    {
        return id;
    }

    QString getHash()
    {
        return hash;
    }

    QString getFilename()
    {
        return filename;
    }

    double getSize()
    {
        return size;
    }

protected:
    QString status;
    QString id;

    QString hash;
    QString filename;
    double size;
};

#endif
