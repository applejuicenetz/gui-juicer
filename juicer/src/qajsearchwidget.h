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
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

#include "qajlistwidget.h"
#include "qajsearchitem.h"
#include "qajsearchentryitem.h"
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
    QAjSearchWidget( QXMLModule* xml, QWidget *parent = 0 );

    ~QAjSearchWidget();

    void insertSearch( QString id, QString searchText, QString running, QString foundFiles );
    void insertSearchEntry( QString id, QString searchId, QString size, QString checksum, QStringList filenames );
    bool remove( QString id );
    QAjSearchItem* findSearch( QString id );
    QAjSearchEntryItem* findSearchEntry( QString id );
    void initToolBar();

protected:
    bool removeSearch( QString id );
    bool removeSearchEntry( QString id );
    QHash<QString, QAjSearchItem*> searches;
    QHash<QString, QAjSearchEntryItem*> searchEntries;

    QLabel *searchLabel;
    QLineEdit *searchEdit;
    QAction *searchButton;


private slots:
    void removeSlot();
    void downloadSlot();
    void searchSlot();
    void linkSlot();
};

#endif
