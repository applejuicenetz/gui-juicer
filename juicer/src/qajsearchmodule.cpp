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

#include "qajsearchmodule.h"
#include "juicer.h"

QAjSearchModule::QAjSearchModule(Juicer* juicer) : QAjModuleBase(juicer, juicer->searchsTreeWidget, juicer->searchToolBar) {
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

QAjSearchModule::~QAjSearchModule()
{}


void QAjSearchModule::insertSearch( const QString& id, const QString& searchText, const QString& running, const QString& foundFiles )
{
    QAjSearchItem *item = findSearch( id );
    if ( item == NULL )
    {
        item = new QAjSearchItem(id, treeWidget);
        searches[id] = item;
        item->setText( QAjSearchItem::TEXT_COL, searchText );
        item->setText( QAjSearchItem::COUNT_COL, "0" );
    }
    item->setText( QAjSearchItem::TEXT_COL, searchText + " (" + foundFiles + ")" );
    if ( running == "true" )
        item->setIcon( QAjSearchItem::TEXT_COL, QIcon(":/small/searching.png") );
    else
        item->setIcon( QAjSearchItem::TEXT_COL, QIcon(":/small/ok.png") );

}

void QAjSearchModule::insertSearchEntry( const QString& id, const QString& searchId, const QString& size, const QString& checksum, const QStringList& filenames )
{
    QAjSearchItem *searchItem = findSearch( searchId );
    if ( searchItem == NULL )
    {
        searchItem = new QAjSearchItem( searchId, treeWidget );
        searches[searchId] = searchItem;
    }

    if ( searchItem->entriesCount < MAX_SEARCH_ENTRIES )
    {
        if ( searchItem->findSearchEntry( id ) == NULL )
        {
            QAjSearchEntryItem *searchEntryItem = new QAjSearchEntryItem( id, searchItem, checksum, size, searchItem );
            searchItem->entries[ id ] = searchEntryItem;
            searchEntries[ id ] = searchEntryItem;
            searchEntryItem->setText( QAjSearchItem::SIZE_COL, QConvert::bytes(size) );
            QStringListIterator it(filenames);
            while (it.hasNext())
            {
                QString filename = it.next();
                if ( searchEntryItem->text( QAjSearchItem::TEXT_COL ) == "" )
                {
                    searchEntryItem->setText( QAjSearchItem::TEXT_COL, filename);
                    searchEntryItem->setFilename( filename );
                }
            }
            searchItem->hits++;
            searchItem->setText( QAjSearchItem::COUNT_COL, QString::number(searchItem->hits) );
            searchItem->entriesCount++;
        }
    }
}

bool QAjSearchModule::removeSearch( const QString& id )
{
    QAjSearchItem* item = findSearch( id );
    if( item != NULL )
    {
        searches.remove( id );

        // -- delete all corresponding search entries --
        QList<QString> entryIds = item->entries.keys();
        while(!entryIds.isEmpty())
        {
            QString entryId = entryIds.takeFirst();
            searchEntries.remove( entryId );
            delete item->entries[ entryId ];
        }

        delete item;
        return true;
    }
    return false;
}


bool QAjSearchModule::removeSearchEntry( const QString& id )
{
    QAjSearchEntryItem* item = findSearchEntry( id );
    if( item != NULL )
    {
        searchEntries.remove( id );
        // -- delete this entry in the parent search
        item->search->entries.remove( id );
        delete item;
        return true;
    }
    return false;
}


bool QAjSearchModule::remove( const QString& id )
{
    if(removeSearch( id ))
        return true;
    else if(removeSearchEntry( id ))
        return true;
    else
        return false;
}


void QAjSearchModule::searchSlot()
{
    QString text( QUrl::toPercentEncoding( searchEdit->text() ) );
    xml->set( "search", "&search=" + text );
    searchEdit->clear();
}


void QAjSearchModule::removeSlot() {
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        xml->set("cancelsearch", "&id=" + ((QAjSearchItem*)selectedItems[i])->getId());
    }
}

void QAjSearchModule::downloadSlot() {
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        QAjSearchEntryItem *searchEntryItem = findSearchEntry(((QAjSearchItem*)selectedItems[i])->getId());
        if( searchEntryItem != NULL ) {
            QString link = "ajfsp://file|";
            link += searchEntryItem->text( QAjSearchItem::TEXT_COL );
            link += "|" + searchEntryItem->getHash();
            link += "|" + QString::number( (int)searchEntryItem->getSize() ) + "/";
            link = QString( QUrl::toPercentEncoding(link) );
            xml->set( "processlink", "&link=" +link );
        }
    }
}

void QAjSearchModule::selectionChanged() {
    bool searchSelected = false;
    bool entrySelected = false;
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size() && (!searchSelected || !entrySelected); i++) {
        searchSelected |= searches.contains(((QAjSearchItem*)selectedItems[i])->getId());
        entrySelected |= searchEntries.contains(((QAjSearchItem*)selectedItems[i])->getId());
    }
    juicer->actionDownload->setEnabled(entrySelected);
    juicer->actionCancel_Search->setEnabled(searchSelected);
    juicer->actionCopy_Link_Search->setEnabled(entrySelected);
    juicer->actionCreate_Link_List_Search->setEnabled(entrySelected);
}

void QAjSearchModule::linkSlot()
{
    QString link;
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    QAjSearchEntryItem *searchEntryItem = findSearchEntry(((QAjSearchItem*)selectedItems[0])->getId() );
    if( searchEntryItem != NULL ) {
        link += "ajfsp://file|";
        link += searchEntryItem->text( QAjSearchItem::TEXT_COL );
        link += "|" + searchEntryItem->getHash();
        link += "|" + QString::number( (int)searchEntryItem->getSize() ) + "/";
    }
    QApplication::clipboard()->setText(link);
}

QAjSearchItem* QAjSearchModule::findSearch( const QString& id ) {
    if (searches.contains( id )) {
        return searches[ id ];
    }
    return NULL;
}

QAjSearchEntryItem* QAjSearchModule::findSearchEntry( const QString& id ) {
    if (searchEntries.contains( id )) {
        return searchEntries[ id ];
    }
    return NULL;
}