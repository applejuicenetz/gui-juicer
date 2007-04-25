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
#include "qajsharewidget.h"

QAjShareWidget::QAjShareWidget( QString filesystemSeparator, QXMLModule* xml, QWidget *parent ) : QAjListWidget( xml, parent )
{
    this->fileSystem = NULL;

    this->filesystemSeparator = filesystemSeparator;
    changed = false;
    setColumnCount( NUM_SHARE_COL );
    QStringList headers;
    int i;
    for ( i=0; i<NUM_SHARE_COL; i++)
    {
        switch (i)
        {
        case PATH_SHARE_INDEX:
            headers.append( tr("path") );
            break;
        case MODE_SHARE_INDEX:
            headers.append( tr("with subdirectories") );
            break;
        }
    }
    setHeaderLabels( headers );

    connect( this, SIGNAL( newSelection( bool ) ) , this, SLOT( selectionChanged( bool ) ) );
    connect( this, SIGNAL( newSelection( bool ) ) , parent, SLOT( selectionChanged( bool ) ) );

    initToolBar();
    initPopup();
    selectionChanged( false );
}


QAjShareWidget::~QAjShareWidget()
{}


/*!
    \fn QAjShareWidget::initToolBar()
 */
void QAjShareWidget::initToolBar()
{
    toolBar = new QToolBar( "share operations", this );

    insertButton = toolBar->addAction( QIcon(":/add.png"), "add share", this, SLOT( insertSlot() ) );
    removeButton = toolBar->addAction( QIcon(":/remove.png"), "remove share", this, SLOT( removeSlot() ) );
    reloadButton = toolBar->addAction( QIcon(":/update.png"), "reload shared files", this, SLOT( reloadSlot() ) );
    applyButton = toolBar->addAction( QIcon(":/commit.png"), "commit changes to the core", this, SLOT( commitSlot() ) );
}


void QAjShareWidget::insertShare( QString path, QString shareMode )
{
    if ( !path.endsWith( filesystemSeparator ) )
        path += filesystemSeparator;
    QAjShareItem *item = new QAjShareItem( this );
    item->path = path;
    item->setText( PATH_SHARE_INDEX, path );
    item->setIcon( PATH_SHARE_INDEX, QIcon(":/small/shares.png") );

    if( (item->recursive=(shareMode == "subdirectory")) )
        item->setIcon( MODE_SHARE_INDEX, QIcon(":/small/ok.png") );
    else
        item->setIcon( MODE_SHARE_INDEX, QIcon(":/small/cancel.png") );
}

void QAjShareWidget::insertSlot()
{

    if ( fileSystem == NULL) {
        fileSystem = new QAjFileDialog(xml, this);
    }

    fileSystem->exec();

    QString dir = fileSystem->getDirectory();

    if ( !dir.isEmpty() )
    {
        int result = QMessageBox::question( this, "question", tr("Share subdirectories?"), QMessageBox::Yes, QMessageBox::No );
        QString mode;
        if ( result == QMessageBox::Yes )
            mode = "subdirectory";
        else
            mode = "directory";
        insertShare( dir, mode );

        changed = true;
        applyButton->setEnabled( true );
    }

}

void QAjShareWidget::removeSlot()
{
    QList<QTreeWidgetItem *>  selectedItems = QTreeWidget::selectedItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        //delete it.current();
        selectedItems[i]->setFlags( 0 );
    }
    changed = true;
    applyButton->setEnabled( true );
}

void QAjShareWidget::reloadSlot()
{
    clear();
    xml->get( "settings" );
    xml->get( "share" );
}

void QAjShareWidget::commitSlot()
{
    QString sharesString;
    int i;
    int cnt = 1;
    for ( i=0; i<topLevelItemCount() ; i++)
    {
        QTreeWidgetItem* item = topLevelItem( i );
        if ( item->flags() & Qt::ItemIsEnabled )
        {
            sharesString += "&sharedirectory" + QString::number(cnt) + "=" + item->text( PATH_SHARE_INDEX );
            sharesString += "&sharesub" + QString::number(cnt) + "=";
            sharesString += ((QAjShareItem*)item)->recursive?"true":"false";
            cnt++;
        }
    }
    sharesString += "&countshares=" + QString::number( cnt-1 );
    xml->set( "setsettings", sharesString );
    xml->get( "settings" );
    changed = false;
    applyButton->setEnabled( false );
}

void QAjShareWidget::selectionChanged( bool oneSelected )
{
    removeButton->setEnabled( oneSelected );
}

void QAjShareWidget::insertDirList( QTreeWidgetItem* parent, QStringList* dirList )
{
    if ( dirList->size() <= 0 )
        return;
    if ( dirList->size() == 1 )
    {
        QAjItem* newItem = new QAjItem( parent );
        newItem->setFlags( Qt::ItemIsEnabled );
        parent->addChild( newItem );
        newItem->setText( PATH_SHARE_INDEX, dirList->front() );
    }
    else
    {
        QTreeWidgetItem* currChild = parent->child( 0 );
        int i = 1;
        while ( ( currChild != NULL ) && ( currChild->text( PATH_SHARE_INDEX ) != dirList->front() ) )
        {
            currChild = parent->child( i++ );
        }
        // nicht gefunden
        if ( currChild == NULL )
        {
            QAjItem* newItem = new QAjItem( parent );
            newItem->setFlags( Qt::ItemIsEnabled );
            parent->addChild( newItem );
            newItem->setText( PATH_SHARE_INDEX, dirList->front() );
            newItem->setIcon( PATH_SHARE_INDEX, QIcon(":/small/shares.png") );
            dirList->pop_front();
            insertDirList( newItem, dirList );
        }
        else
        {
            dirList->pop_front();
            insertDirList( currChild, dirList );
        }
    }
}


/*!
    \fn QAjShareWidget::initPopup()
 */
void QAjShareWidget::initPopup()
{
    popup->setTitle( tr("S&hare") );
    popup->addAction( removeButton );
    popup->addSeparator();
    popup->addAction( insertButton );
    popup->addAction( reloadButton );
    popup->addAction( applyButton );
}
