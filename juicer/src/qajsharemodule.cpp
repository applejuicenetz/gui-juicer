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

#include <qglobal.h>

QAjShareModule::QAjShareModule(Juicer* juicer) 
  : QAjModuleBase(juicer, juicer->sharesTreeWidget, juicer->shareToolBar)
  , prio_(1)
{
    this->shareSelectionDialog = new QAjShareSelectionDialog(juicer->xml, juicer);
    this->filesystemSeparator = filesystemSeparator;
    changed_ = false;

    prioLabel = new QLabel(tr("Priority:"), juicer->shareToolBar);
    juicer->shareToolBar->addWidget(prioLabel);
    prioSpin = new QSpinBox(juicer->shareToolBar);
    prioSpin->setRange(1, 250);
    juicer->shareToolBar->addWidget(prioSpin);
    prioButton = new QPushButton( tr("SET"), juicer->shareToolBar);
    juicer->shareToolBar->addWidget(prioButton);

    connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
    connect(juicer->actionAdd_Share, SIGNAL(triggered()), this, SLOT(insertSlot()));
    connect(juicer->actionRemove_Share, SIGNAL(triggered()), this, SLOT(removeSlot()));
    connect(juicer->actionReload_Share, SIGNAL(triggered()), this, SLOT(reloadSlot()));
    connect(juicer->actionCommit_Share, SIGNAL(triggered()), this, SLOT(commitSlot()));
    connect(juicer->actionCopy_Link_Share, SIGNAL(triggered()), this, SLOT(linkSlot()));
    connect(juicer->actionCreate_Link_List_Share, SIGNAL(triggered()), this, SLOT(linkListSlot()));

//    connect(prioSpin, SIGNAL(valueChanged(int)), this, SLOT(setPriority(int)));
    connect(prioSpin, SIGNAL(valueChanged(int)), this, SLOT(setTmpPriority(int)));
    connect(prioButton, SIGNAL(clicked()), this, SLOT(setPriority()));

    selectionChanged();
}


QAjShareModule::~QAjShareModule()
{}

void QAjShareModule::insertShare( const QString& path, const QString& shareMode) {
    new QAjShareItem(treeWidget, path, (shareMode == "subdirectory"));
}

void QAjShareModule::insertSlot() {
    xml->get("directory");
    if(shareSelectionDialog->exec() == QDialog::Accepted) {
        QString path =  shareSelectionDialog->selectedPath();
        if(!path.isEmpty()) {
            QString mode = "directory";
            if(QMessageBox::question( juicer, "question", tr("Share subdirectories?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
                mode = "subdirectory";
            }
            insertShare(path, mode);
            changed_ = true;
            juicer->actionCommit_Share->setEnabled(true);
        }
    }
}

void QAjShareModule::removeSlot()
{
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++ ) {
        //delete it.current();
        selectedItems[i]->setFlags( 0 );
    }
    changed_ = true;
    juicer->actionCommit_Share->setEnabled( true );
}

void QAjShareModule::reloadSlot()
{
    treeWidget->clear();
    sharedFiles.clear();
    xml->get( "settings" );
    xml->get( "share" );

    juicer->actionCommit_Share->setEnabled( false );
}

void QAjShareModule::commitSlot()
{
    QString sharesString;
    int cnt = 1;
    for(int i=0; i<treeWidget->topLevelItemCount() ; i++) {
        QAjShareItem* item = (QAjShareItem*)treeWidget->topLevelItem(i);
        if ( item->flags() & Qt::ItemIsEnabled ) {
            sharesString += "&sharedirectory" + QString::number(cnt) + "=" + item->getPath();
            sharesString += "&sharesub" + QString::number(cnt) + "=";
            QAjShareItem* shareItem = dynamic_cast<QAjShareItem*>(item);
            if ( shareItem == NULL ) {
              qFatal( "QAjShareModule::commitSlot: shareItem is NULL" );
              return;
            }
            sharesString += shareItem->isRecursive() ? "true" : "false";
            cnt++;
        }
    }
    sharesString += "&countshares=" + QString::number( cnt-1 );
    xml->set( "setsettings", sharesString );
//    xml->get( "settings" );
    changed_ = false;
    juicer->actionCommit_Share->setEnabled( false );

    updateSharedFilesList();
}

void QAjShareModule::linkSlot() {
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        if(selectedItems[i]->parent() != NULL) {
            QAjShareFileItem* shareItem = dynamic_cast<QAjShareFileItem*>(selectedItems[0]);
            if ( shareItem != NULL ) {
                QApplication::clipboard()->setText( shareItem->getLinkAJFSP() );
            }
        }
    }
}

QAjShareItem* QAjShareModule::findShare(const QString& fileName) {
    for(int i=0; i<treeWidget->topLevelItemCount() ; i++) {
        QAjShareItem* item = dynamic_cast<QAjShareItem*>(treeWidget->topLevelItem(i));
        if ( item == NULL ) continue;
        if(fileName.startsWith(item->getPath())) {
            return item;
        }
    }
    return NULL;
}

void QAjShareModule::insertFile( const QString& id,
                                 const QString& hash,
                                 const QString& fileName,
                                 const QString& size,
                                 const QString& priority,
                                 const QString& filesystemSeperator ) 
{
    QAjShareFileItem *shareFileItem = findFile(id);
    if( shareFileItem == NULL ) {
        QAjShareItem* parentItem = findShare(fileName);
        if ( parentItem != NULL ) {
            shareFileItem = new QAjShareFileItem( id, parentItem );
            if ( shareFileItem == NULL ) return;  // new failed?
            parentItem->insertSharedFile(shareFileItem);
            sharedFiles[ id ] = shareFileItem;
        }
        else {
//            qDebug() << fileName << endl;
            // file in share.xml which doesn't belong to any shared directory
            return;
        }
    }
    shareFileItem->update(hash, fileName, size, priority, filesystemSeperator);
    adjustSizeOfColumns();
}

QAjShareFileItem* QAjShareModule::findFile( const QString& id )
{
    if ( sharedFiles.contains( id ) ) {
        return sharedFiles[id];
    }
    else {
        return NULL;
    }
}

void QAjShareModule::updateSharedFilesList() 
{
    treeWidget->clear();
    sharedFiles.clear();
    xml->get( "settings" );
    xml->get( "share" );
}

void QAjShareModule::setTmpPriority(int prio) 
{
    prio_ = prio;
}

void QAjShareModule::setPriority() 
{
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        if(selectedItems[i]->parent() != NULL) {
            QAjShareFileItem* ajShareItem = dynamic_cast<QAjShareFileItem*>(selectedItems[i]);
            if ( ajShareItem != NULL) {
              xml->set("setpriority", "&priority=" + QString::number(prio_) + "&id=" + ajShareItem->getId());
              ajShareItem->updatePrio(prio_);
            }
            else return;
        }
    }
//    xml->get("share");
//    updateSharedFilesList();
}


/*!
    \fn QAjShareModule::findFile(const QString& size, const QString& hash)
 */
QAjShareFileItem* QAjShareModule::findFile(const QString& size, const QString& hash) 
{
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
