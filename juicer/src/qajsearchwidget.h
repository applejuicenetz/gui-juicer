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
#ifndef QAJSEARCHWIDGET_H
#define QAJSEARCHWIDGET_H

#include <QHash>

#include "qajlistwidget.h"
#include "qajsearchitem.h"
#include "types.h"
#include "qconvert.h"

#define MAX_SEARCH_ENTRIES 100000

/**
@author Matthias Reif
*/
class QAjSearchWidget : public QAjListWidget
{
    Q_OBJECT
public:
    QAjSearchWidget( QWidget *parent = 0 );

    ~QAjSearchWidget();

    void insertSearch( QString id, QString searchText, QString running, QString foundFiles );
    void insertSearchEntry( QString id, QString searchId, QString size, QString checksum, QStringList filenames );
    bool remove( QString id );
    QAjSearchItem* findSearch( QString id );
    QAjSearchItem* findSearchEntry( QString id );

protected:
    QHash<QString, QAjSearchItem*> searches;
    QHash<QString, QAjSearchItem*> searchEntries;

public slots:
    void removeSlot();
    void downloadSlot();
signals:
    void remove();
    void download();
};

#endif
