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

#include "searchmodule.h"
#include "juicer.h"

SearchModule::SearchModule(Juicer* juicer) : ModuleBase(juicer, juicer->searchTreeWidget, juicer->searchToolBar) {
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

    connect(juicer->minSizeCheckBox, SIGNAL(clicked()), this, SLOT(filterResults()));
    connect(juicer->maxSizeCheckBox, SIGNAL(clicked()), this, SLOT(filterResults()));
    connect(juicer->videoCheckBox, SIGNAL(clicked()), this, SLOT(filterResults()));
    connect(juicer->audioCheckBox, SIGNAL(clicked()), this, SLOT(filterResults()));
    connect(juicer->imageCheckBox, SIGNAL(clicked()), this, SLOT(filterResults()));
    connect(juicer->archiveCheckBox, SIGNAL(clicked()), this, SLOT(filterResults()));
    connect(juicer->textCheckBox, SIGNAL(clicked()), this, SLOT(filterResults()));
    connect(juicer->cddvdCheckBox, SIGNAL(clicked()), this, SLOT(filterResults()));
    connect(juicer->fileTypeEdit, SIGNAL(textChanged(const QString &)), this, SLOT(filterResults()));

    connect(juicer->minSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(filterResults()));
    connect(juicer->maxSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(filterResults()));

    selectionChanged();
    filterResults();
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
            searchEntryItem->setText( SearchItem::SIZE_COL, Convert::bytes(size) );
            QStringListIterator it(filenames);
            while(it.hasNext()) {
                QString filename = it.next();
                if(searchEntryItem->text( SearchItem::TEXT_COL ) == "") {
                    searchEntryItem->setText( SearchItem::TEXT_COL, filename);
                    searchEntryItem->setFilename( filename );
                    searchEntryItem->setFileIcon(SearchItem::TEXT_COL);
                }
            }
            searchEntryItem->setFilter(filter);
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


/*!
    \fn SearchModule::filterResults()
 */
void SearchModule::filterResults() {
    filter = getFilter();
    treeWidget->setUpdatesEnabled(false);
    QHash<QString, SearchEntryItem*>::const_iterator i;
    for(i = searchEntries.constBegin(); i != searchEntries.constEnd(); i++) {
        (*i)->setFilter(filter);
    }
    treeWidget->setUpdatesEnabled(true);
}


/*!
    \fn SearchModule::listToPattern(QString& pattern, const QString& list)
 */
void SearchModule::listToPattern(QString& pattern, const QString& list) {
    QStringList list2 = list.split(" ");
    for(int i=0; i<list2.size(); i++) {
        if(!pattern.isEmpty()) {
            pattern += "|";
        }
        pattern += "\\." + list2.at(i).trimmed() + "$";
    }
}

/*!
    \fn SearchModule::getFilter()
 */
SearchEntryItem::Filter SearchModule::getFilter() {
    SearchEntryItem::Filter filter;
    filter.minSize = juicer->minSizeCheckBox->isChecked()?juicer->minSizeSpinBox->value():0.0;
    filter.maxSize = juicer->maxSizeCheckBox->isChecked()?juicer->maxSizeSpinBox->value():999999.9;
    filter.minSize *= 1024 * 1024;
    filter.maxSize *= 1024 * 1024;

    QString pattern;
    if(juicer->videoCheckBox->isChecked()) {
        listToPattern(pattern, OptionsDialog::video());
    }
    if(juicer->audioCheckBox->isChecked()) {
        listToPattern(pattern, OptionsDialog::audio());
    }
    if(juicer->imageCheckBox->isChecked()) {
        listToPattern(pattern, OptionsDialog::image());
    }
    if(juicer->textCheckBox->isChecked()) {
        listToPattern(pattern, OptionsDialog::text());
    }
    if(juicer->archiveCheckBox->isChecked()) {
        listToPattern(pattern, OptionsDialog::archive());
    }
    if(juicer->cddvdCheckBox->isChecked()) {
        listToPattern(pattern, OptionsDialog::cddvd());
    }
    if(!juicer->fileTypeEdit->text().isEmpty()) {
        listToPattern(pattern, juicer->fileTypeEdit->text());
    }
    filter.type.setPattern(pattern);
    filter.type.setCaseSensitivity(Qt::CaseInsensitive);
    return filter;
}
