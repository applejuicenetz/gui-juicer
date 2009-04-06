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

#include "searchmodule.h"
#include "juicer.h"

SearchModule::SearchModule(Juicer* juicer) : ModuleBase(juicer, juicer->searchsTreeWidget, juicer->searchToolBar) {
    connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(downloadSlot()));

    connect(juicer->actionDownload, SIGNAL(triggered()), this, SLOT(downloadSlot()));
    connect(juicer->actionCancel_Search, SIGNAL(triggered()), this, SLOT(removeSlot()));
    connect(juicer->actionCopy_Link_Search, SIGNAL(triggered()), this, SLOT(linkSlot()));
    connect(juicer->actionCreate_Link_List_Search, SIGNAL(triggered()), this, SLOT(linkListSlot()));

    juicer->searchToolBar->addSeparator();
    juicer->searchToolBar->addWidget(new QLabel(tr("search for:"), juicer->searchToolBar));
    searchEdit = new QLineEdit(juicer->searchToolBar);
    searchEdit->setMinimumWidth( 200 );
    juicer->searchToolBar->addWidget(searchEdit);
    connect(searchEdit, SIGNAL(returnPressed()), this, SLOT(searchSlot()));
    juicer->searchToolBar->addAction(tr("search"), this, SLOT(searchSlot()));

    selectionChanged();
}

SearchModule::~SearchModule()
{}


void SearchModule::insertSearch(const QString& id, const QString& searchText, const QString& running, const QString& foundFiles) {
    SearchItem *item = findSearch(id);
    if(item == NULL) {
        item = new SearchItem(id, treeWidget);
        searches[id] = item;
        item->setText(SearchItem::TEXT_COL, searchText);
        item->setText(SearchItem::COUNT_COL, "0");
    }
    item->setText(SearchItem::TEXT_COL, searchText + " (" + foundFiles + ")");
    if(running == "true") {
        item->setIcon(SearchItem::TEXT_COL, QIcon(":/small/searching.png"));
    } else {
        item->setIcon(SearchItem::TEXT_COL, QIcon(":/small/ok.png"));
    }

}

void SearchModule::insertSearchEntry(const QString& id, const QString& searchId, const QString& size, const QString& checksum, const QStringList& filenames) {
    SearchItem *searchItem = findSearch(searchId);
    if(searchItem == NULL) {
        searchItem = new SearchItem( searchId, treeWidget );
        searches[searchId] = searchItem;
    }
    if(searchItem->entriesCount < MAX_SEARCH_ENTRIES) {
        if(searchItem->findSearchEntry( id ) == NULL) {
            SearchEntryItem *searchEntryItem = new SearchEntryItem(id, searchItem, checksum, size, searchItem);
            searchItem->entries[ id ] = searchEntryItem;
            searchEntries[ id ] = searchEntryItem;
            searchEntryItem->setText( SearchItem::SIZE_COL, QConvert::bytes(size) );
            QStringListIterator it(filenames);
            while(it.hasNext()) {
                QString filename = it.next();
                if(searchEntryItem->text( SearchItem::TEXT_COL ) == "") {
                    searchEntryItem->setText( SearchItem::TEXT_COL, filename);
                    searchEntryItem->setFilename( filename );
                }
            }
            searchItem->hits++;
            searchItem->setText(SearchItem::COUNT_COL, QString::number(searchItem->hits));
            searchItem->entriesCount++;
        }
    }
}

bool SearchModule::removeSearch(const QString& id) {
    SearchItem* item = findSearch(id);
    if(item != NULL) {
        searches.remove(id);
        // -- delete all corresponding search entries --
        QList<QString> entryIds = item->entries.keys();
        while(!entryIds.isEmpty()) {
            QString entryId = entryIds.takeFirst();
            searchEntries.remove(entryId);
            delete item->entries[entryId];
        }
        delete item;
        return true;
    }
    return false;
}


bool SearchModule::removeSearchEntry(const QString& id) {
    SearchEntryItem* item = findSearchEntry(id);
    if(item != NULL) {
        searchEntries.remove(id);
        // -- delete this entry in the parent search
        item->search->entries.remove(id);
        delete item;
        return true;
    }
    return false;
}


bool SearchModule::remove(const QString& id) {
    return removeSearch(id) || removeSearchEntry( id );
}


void SearchModule::searchSlot() {
    QString text(QUrl::toPercentEncoding(searchEdit->text()));
    xml->set("search", "&search=" + text);
    searchEdit->clear();
}


void SearchModule::removeSlot() {
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        xml->set("cancelsearch", "&id=" + ((SearchItem*)selectedItems[i])->getId());
    }
}

void SearchModule::downloadSlot() {
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        SearchEntryItem *searchEntryItem = findSearchEntry(((SearchItem*)selectedItems[i])->getId());
        if( searchEntryItem != NULL ) {
            QString link = "ajfsp://file|";
            link += searchEntryItem->text(SearchItem::TEXT_COL);
            link += "|" + searchEntryItem->getHash();
            link += "|" + QString::number((int)searchEntryItem->getSize()) + "/";
            link = QString(QUrl::toPercentEncoding(link));
            xml->set("processlink", "&link=" +link);
        }
    }
}

void SearchModule::selectionChanged() {
    bool searchSelected = false;
    bool entrySelected = false;
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size() && (!searchSelected || !entrySelected); i++) {
        searchSelected |= searches.contains(((SearchItem*)selectedItems[i])->getId());
        entrySelected |= searchEntries.contains(((SearchItem*)selectedItems[i])->getId());
    }
    juicer->actionDownload->setEnabled(entrySelected);
    juicer->actionCancel_Search->setEnabled(searchSelected);
    juicer->actionCopy_Link_Search->setEnabled(entrySelected);
    juicer->actionCreate_Link_List_Search->setEnabled(entrySelected);
}

void SearchModule::linkSlot() {
    QString link;
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    SearchEntryItem *searchEntryItem = findSearchEntry(((SearchItem*)selectedItems[0])->getId());
    if(searchEntryItem != NULL) {
        link += "ajfsp://file|";
        link += searchEntryItem->text(SearchItem::TEXT_COL);
        link += "|" + searchEntryItem->getHash();
        link += "|" + QString::number((int)searchEntryItem->getSize()) + "/";
    }
    QApplication::clipboard()->setText(link);
}

SearchItem* SearchModule::findSearch(const QString& id) {
    if(searches.contains(id)) {
        return searches[id];
    }
    return NULL;
}

SearchEntryItem* SearchModule::findSearchEntry(const QString& id) {
    if(searchEntries.contains(id)) {
        return searchEntries[id];
    }
    return NULL;
}
