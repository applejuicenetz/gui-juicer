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
#include "qajsearchwidget.h"

QAjSearchWidget::QAjSearchWidget( QWidget *parent ) : QAjListWidget( parent )
{
    setColumnCount( NUM_SEARCH_COL );
    QStringList headers;
    int i;
    for ( i=0; i<NUM_SEARCH_COL; i++)
    {
        switch (i)
        {
        case TEXT_SEARCH_INDEX:
            headers.append( tr("search") );
            break;
        case SIZE_SEARCH_INDEX:
            headers.append( tr("size") );
            break;
        case COUNT_SEARCH_INDEX:
            headers.append( tr("hits") );
            break;
        }
    }
    setHeaderLabels( headers );
    header()->resizeSection( TEXT_SEARCH_INDEX, 300 );
    header()->resizeSection( SIZE_SEARCH_INDEX, 100 );
    header()->resizeSection( COUNT_SEARCH_INDEX, 50 );

    popup->setTitle( tr("&Search") );
    popup->addAction( QIcon(":/small/save.png"), "download", this, SLOT(downloadSlot()) );
    popup->addAction( QIcon(":/small/cancel.png"), "remove", this, SLOT(removeSlot()) );
}

QAjSearchWidget::~QAjSearchWidget()
{}

void QAjSearchWidget::insertSearch( QString id, QString searchText, QString running, QString foundFiles )
{
    QAjSearchItem *item = findSearch( id );
    if ( item == NULL )
    {
        item = new QAjSearchItem( id, this );
        searches[id] = item;
        item->setText( TEXT_SEARCH_INDEX, searchText );
        item->setText( COUNT_SEARCH_INDEX, "0" );
    }
    item->setText( TEXT_SEARCH_INDEX, searchText + " (" + foundFiles + ")" );
    if ( running == "true" )
        item->setIcon( TEXT_SEARCH_INDEX, QIcon(":/small/searching.png") );
    else
        item->setIcon( TEXT_SEARCH_INDEX, QIcon(":/small/ok.png") );

}

void QAjSearchWidget::insertSearchEntry( QString id, QString searchId, QString size, QString checksum, QStringList filenames )
{
    QAjSearchItem *searchItem = findSearch( searchId );
    if ( searchItem == NULL )
    {
        searchItem = new QAjSearchItem( searchId, this );
        searches[searchId] = searchItem;
    }

    if ( searchItem->entriesCount < MAX_SEARCH_ENTRIES )
    {
        if ( searchItem->findSearchEntry( id ) == NULL )
        {
            QAjSearchEntryItem *searchEntryItem = new QAjSearchEntryItem( id, searchItem, checksum, size, searchItem );
            searchItem->entries[ id ] = searchEntryItem;
            searchEntries[ id ] = searchEntryItem;
            searchEntryItem->setText( SIZE_SEARCH_INDEX, QConvert::bytes(size) );
            QStringListIterator it(filenames);
            while (it.hasNext())
            {
                QString filename = it.next();
                if ( searchEntryItem->text( TEXT_SEARCH_INDEX ) == "" )
                {
                    searchEntryItem->setText( TEXT_SEARCH_INDEX, filename);
                }
/*                else
                {
                    QAjSearchItem *searchSubEntryItem = new QAjSearchItem( id, searchEntryItem );
                    searchSubEntryItem->setText( TEXT_SEARCH_INDEX, filename);
                }*/
            }
            searchItem->hits++;
            searchItem->setText( COUNT_SEARCH_INDEX, QString::number(searchItem->hits) );
            searchItem->entriesCount++;
        }
    }
}

bool QAjSearchWidget::removeSearch( QString id )
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


bool QAjSearchWidget::removeSearchEntry( QString id )
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


bool QAjSearchWidget::remove( QString id )
{
    if(removeSearch( id ))
        return true;
    else if(removeSearchEntry( id ))
        return true;
    else
        return false;
}

void QAjSearchWidget::removeSlot()
{
    remove();
}

void QAjSearchWidget::downloadSlot()
{
    download();
}

QAjSearchItem* QAjSearchWidget::findSearch( QString id )
{
    if (searches.contains( id ))
        return searches[ id ];
    else
        return NULL;
}

QAjSearchEntryItem* QAjSearchWidget::findSearchEntry( QString id )
{
    if (searchEntries.contains( id ))
        return searchEntries[ id ];
    else
        return NULL;
}
