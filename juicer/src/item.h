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
class Item : public QTreeWidgetItem, public QObject
{
// Q_OBJECT
public:
    Item( QTreeWidget *parent, const QString& id="generic" );
    Item( QTreeWidgetItem *parent, const QString& id="generic" );

    virtual ~Item();

    QString getStatus() const {
        return status_;
    }
    void setStatus( const QString& newStatus )  {
        status_ = newStatus;
    }
    QString getId() const {
        return id_;
    }
    QString getHash() const {
        return hash_;
    }
    QString getFilename() const {
        return filename_;
    }
    double getSize() const {
        return size_;
    }

protected:
    QString id_;
    QString status_;

    double  size_;
    QString hash_;
    QString filename_;
};

#endif
