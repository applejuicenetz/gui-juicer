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
#ifndef QAJLISTWIDGET_H
#define QAJLISTWIDGET_H

#include <QHeaderView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QMutex>
#include <QMutexLocker>

#include "qconvert.h"
#include "qajitem.h"
#include "types.h"

/**
@author Matthias Reif
*/
class QAjListWidget : public QTreeWidget
{
    Q_OBJECT
public:
    QAjListWidget( int idIndex, QWidget *parent = 0, const char *name = 0);

    ~QAjListWidget();

    bool wasChanged()
    {
        return changed;
    }
    void setChanged( bool changed = true )
    {
        this->changed = changed;
    }

    QMenu *popup;
    void adjustSizeOfColumns();

    QMutex mutex;
protected:
    bool isOneSelected();
    bool changed;
    QAjItem *popupItem;
    int idIndex;

private slots:
    void selectionChanged();
    void contextMenuRequested( const QPoint &pos );
signals:
    void newSelection( bool oneSelected );
};

#endif
