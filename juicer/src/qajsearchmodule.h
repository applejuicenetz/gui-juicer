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
// #include <QMessageBox>
#include <QApplication>
#include <QClipboard>

#include "qajmodulebase.h"
#include "qajsearchitem.h"
#include "qajsearchentryitem.h"
#include "qconvert.h"

#define MAX_SEARCH_ENTRIES 100000

/**
@author Matthias Reif
*/
class QAjSearchModule : public QAjModuleBase {
    Q_OBJECT
public:
    QAjSearchModule(Juicer* juicer);
    ~QAjSearchModule();

    void insertSearch(const QString& id, const QString& searchText, const QString& running, const QString& foundFiles );
    void insertSearchEntry(const QString& id, const QString& searchId, const QString& size, const QString& checksum, const QStringList& filenames );
    bool remove(const QString& id);
    QAjSearchItem* findSearch(const QString& id);
    QAjSearchEntryItem* findSearchEntry(const QString& id);

protected:
    bool removeSearch(const QString& id);
    bool removeSearchEntry(const QString& id);
    QHash<QString, QAjSearchItem*> searches;
    QHash<QString, QAjSearchEntryItem*> searchEntries;

    QLineEdit *searchEdit;

private slots:
    void removeSlot();
    void downloadSlot();
    void searchSlot();
    void linkSlot();
    void selectionChanged();
};

#endif
