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
#include "qajsharemodule.h"
#include "juicer.h"

QAjShareModule::QAjShareModule(Juicer* juicer) : QAjModuleBase(juicer, juicer->sharesTreeWidget, juicer->shareToolBar)
{
    this->fileSystem = NULL;
    this->filesystemSeparator = filesystemSeparator;
    changed = false;

    prioLabel = new QLabel("Priority:", juicer->shareToolBar);
    juicer->shareToolBar->addWidget(prioLabel);
    prioSpin = new QSpinBox(juicer->shareToolBar);
    prioSpin->setRange(1, 250);
    juicer->shareToolBar->addWidget(prioSpin);

    connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
    connect(juicer->actionAdd_Share, SIGNAL(triggered()), this, SLOT(insertSlot()));
    connect(juicer->actionRemove_Share, SIGNAL(triggered()), this, SLOT(removeSlot()));
    connect(juicer->actionReload_Share, SIGNAL(triggered()), this, SLOT(reloadSlot()));
    connect(juicer->actionCommit_Share, SIGNAL(triggered()), this, SLOT(commitSlot()));
    connect(juicer->actionCopy_Link_Share, SIGNAL(triggered()), this, SLOT(linkSlot()));

    connect(prioSpin, SIGNAL(valueChanged(int)), this, SLOT(setPriority(int)));

    selectionChanged();
}


QAjShareModule::~QAjShareModule()
{}


void QAjShareModule::insertShare( QString path, QString shareMode, QString filesystemSeperator )
{
    if(!path.endsWith(filesystemSeparator.data()[0])) {
        path += filesystemSeperator;
    }
    QAjShareItem *item = new QAjShareItem(treeWidget, path, (shareMode == "subdirectory"));
    item->setText(QAjShareItem::PATH_COL, path);
    if(item->isRecursive()) {
        item->setIcon(QAjShareItem::PATH_COL, QIcon(":/small/recursive.png"));
    } else {
        item->setIcon(QAjShareItem::PATH_COL, QIcon(":/small/not_recursive.png"));
    }
}

void QAjShareModule::insertSlot()
{
    if ( fileSystem == NULL) {
        fileSystem = new QAjFileDialog(xml, juicer);
    }
    fileSystem->exec();
    QString dir = fileSystem->getDirectory();
    if ( !dir.isEmpty() )
    {
        int result = QMessageBox::question( juicer, "question", tr("Share subdirectories?"), QMessageBox::Yes, QMessageBox::No );
        QString mode;
        if ( result == QMessageBox::Yes )
            mode = "subdirectory";
        else
            mode = "directory";
        insertShare( dir, mode, fileSystem->getSeperator() );

        changed = true;
        juicer->actionCommit_Share->setEnabled(true);
    }
}

void QAjShareModule::removeSlot()
{
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++ ) {
        //delete it.current();
        selectedItems[i]->setFlags( 0 );
    }
    changed = true;
    juicer->actionCommit_Share->setEnabled( true );
}

void QAjShareModule::reloadSlot()
{
    treeWidget->clear();
    xml->get( "settings" );
    xml->get( "share" );

    juicer->actionCommit_Share->setEnabled( false );
}

void QAjShareModule::commitSlot()
{
    QString sharesString;
    int cnt = 1;
    for(int i=0; i<treeWidget->topLevelItemCount() ; i++) {
        QTreeWidgetItem* item = treeWidget->topLevelItem(i);
        if ( item->flags() & Qt::ItemIsEnabled ) {
            sharesString += "&sharedirectory" + QString::number(cnt) + "=" + item->text( QAjShareItem::PATH_COL );
            sharesString += "&sharesub" + QString::number(cnt) + "=";
            sharesString += ((QAjShareItem*)item)->isRecursive()?"true":"false";
            cnt++;
        }
    }
    sharesString += "&countshares=" + QString::number( cnt-1 );
    xml->set( "setsettings", sharesString );
    xml->get( "settings" );
    changed = false;
    juicer->actionCommit_Share->setEnabled( false );
}

void QAjShareModule::linkSlot() {
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        if(selectedItems[i]->parent() != NULL) {
            QApplication::clipboard()->setText(((QAjShareFileItem*)selectedItems[0])->getLinkAJFSP());
        }
    }
}

void QAjShareModule::insertDirList( QTreeWidgetItem* parent, QStringList* dirList )
{
    if(dirList->empty())
        return;
    if ( dirList->size() == 1 )
    {
        QAjItem* newItem = new QAjItem( parent );
        newItem->setFlags( Qt::ItemIsEnabled );
        parent->addChild( newItem );
        newItem->setText( QAjShareItem::PATH_COL, dirList->front() );
    }
    else
    {
        QTreeWidgetItem* currChild = parent->child( 0 );
        int i = 1;
        while ( ( currChild != NULL ) && ( currChild->text( QAjShareItem::PATH_COL ) != dirList->front() ) )
        {
            currChild = parent->child( i++ );
        }
        // nicht gefunden
        if ( currChild == NULL )
        {
            QAjItem* newItem = new QAjItem( parent );
            newItem->setFlags( Qt::ItemIsEnabled );
            parent->addChild( newItem );
            newItem->setText( QAjShareItem::PATH_COL, dirList->front() );
            newItem->setIcon( QAjShareItem::PATH_COL, QIcon(":/small/shares.png") );
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

QAjShareItem* QAjShareModule::findShare(const QString& fileName) {
    for(int i=0; i<treeWidget->topLevelItemCount() ; i++) {
        QAjShareItem* item = (QAjShareItem*)treeWidget->topLevelItem(i);
        if(fileName.startsWith(item->getPath())) {
            return item;
        }
    }
    return NULL;
}

void QAjShareModule::insertFile( QString id, QString hash, QString fileName, QString size, QString priority, QString filesystemSeperator ) {
    QAjShareFileItem *shareFileItem = findFile(id);
    if(shareFileItem == NULL) {
        shareFileItem = new QAjShareFileItem(id, findShare(fileName));
        sharedFiles[ id ] = shareFileItem;
    }
    shareFileItem->update(hash, fileName, size, priority, filesystemSeperator);
    this->adjustSizeOfColumns();
}

QAjShareFileItem* QAjShareModule::findFile( QString id )
{
    if (sharedFiles.contains( id ))
        return sharedFiles[id];
    else
        return NULL;
}

void QAjShareModule::updateSharedFilesList() {
    xml->get("share");
}

void QAjShareModule::setPriority(int prio) {
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        if(selectedItems[i]->parent() != NULL) {
            QAjShareFileItem* ajShareItem = (QAjShareFileItem*)selectedItems[i];
            xml->set("setpriority", "&priority=" + QString::number(prio) + "&id=" + ajShareItem->getId());
        }
    }
    xml->get("share");
}


/*!
    \fn QAjShareModule::findFile(QString size, QString hash)
 */
QAjShareFileItem* QAjShareModule::findFile(QString size, QString hash) {
    QList<QAjShareFileItem*> list = sharedFiles.values();
    for(int i=0; i<list.size(); i++) {
        if(list[i]->getSize() == size.toDouble() && list[i]->getHash() == hash) {
            return list[i];
        }
    }
    return NULL;
}

void QAjShareModule::selectionChanged() {
    bool shareSelected = false;
    bool fileSelected = false;
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++ ) {
        if(treeWidget->selectedItems()[i]->parent() == NULL) {
            shareSelected = true;
        } else {
            fileSelected = true;
        }
        if(shareSelected && fileSelected) {
            break;
        }
    }
    juicer->actionRemove_Share->setEnabled(shareSelected);
    juicer->actionCopy_Link_Share->setEnabled(fileSelected);
    juicer->actionCreate_Link_List_Share->setEnabled(fileSelected);
    prioLabel->setEnabled(fileSelected);
    prioSpin->setEnabled(fileSelected);
}
