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

#include <QTreeWidgetItem>

#include "xmlmodule.h"

#include "dirselectiondialog.h"

QList<DirSelectionDialog::Dir> DirSelectionDialog::rootDir = QList<DirSelectionDialog::Dir>();

DirSelectionDialog::DirSelectionDialog(const QString& label, const QString& startDir, bool hideNew, XMLModule * const xml, QWidget* parent, Qt::WFlags fl )
  : QDialog( parent, fl )
  , Ui::DirSelectionDialogBase()
  , xml_( xml )
  , expandedItem_( 0 )
{
    setupUi( this );
    textLabel->setText(label);

    QFileIconProvider fileIconProvider;
    shareIcons_[WORKSTATION] = fileIconProvider.icon(QFileIconProvider::Computer);
    shareIcons_[DRIVE]       = fileIconProvider.icon(QFileIconProvider::Drive);
    shareIcons_[DISKDRIVE]   = fileIconProvider.icon(QFileIconProvider::Drive);
    shareIcons_[FOLDER]      = fileIconProvider.icon(QFileIconProvider::Folder);
    shareIcons_[DESKTOP]     = fileIconProvider.icon(QFileIconProvider::Desktop);

    treeWidget->sortItems( 0, Qt::AscendingOrder );

    leNewFolder->setHidden(hideNew);
    labelNewFolder->setHidden(hideNew);

    connect( treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)),
             this, SLOT(updateSubDirectoriesSlot(QTreeWidgetItem*)) );
    connect( xml_,  SIGNAL(requestFinished(int,bool)),
             this, SLOT(requestFinished(int,bool)) );

    requestSubdirsFromDir(startDir);
}


DirSelectionDialog::~DirSelectionDialog() {
}

void DirSelectionDialog::reject()
{
    QDialog::reject();
}

void DirSelectionDialog::accept()
{
    QString newFolder = leNewFolder->displayText();
    QString path      = getSelectedPath();

    if ( newFolder.isEmpty() && path.isEmpty() ) {
        return;   // no dir selected and no new dir inserted
    }
    else {
        QDialog::accept();
    }
}

QString DirSelectionDialog::getPath() const
{
    QString newFolder = leNewFolder->displayText();
    QString path      = getSelectedPath();
    if ( ! newFolder.isEmpty() ) {
        if ( ! path.isEmpty() ) path += filesystemSeperator_ + newFolder;
        else path = newFolder;
    }
    return path;
}

void DirSelectionDialog::requestSubdirsFromDir(const QString& dir /* = QString::Null() */) {
    if(!dir.isNull()){
        xmlIds << xml_->get("directory", "&directory=" + dir);
    } else {
        if(rootDir.isEmpty()) {
            rootDirRequestId = xml_->get("directory");
            xmlIds << rootDirRequestId;
        } else {
            rootDirRequestId = -1;
            for(int i=0; i<rootDir.size(); i++) {
                insertDirectory(rootDir.at(i).name, rootDir.at(i).path, rootDir.at(i).type);
            }
        }
    }
}

void DirSelectionDialog::requestFinished( int id, bool error )
{
    if(xmlIds.contains(id)) {
        if( ! error ) {
            QDomElement root = xml_->getContent().documentElement();
            if( root.tagName() == "applejuice" ) {
                for( QDomNode n = root.firstChild(); ! n.isNull(); n = n.nextSibling() ) {
                    QDomElement e = n.toElement();
                    if( ! e.isNull() ) {
                        if( e.tagName() == "dir" ) {
                            if(id == rootDirRequestId) {
                                DirSelectionDialog::rootDir << Dir(e.attribute("name"), e.attribute("path"), e.attribute("type"));
                            }
                            insertDirectory(
                                e.attribute("name"),
                                e.attribute("path"),
                                (DirType)e.attribute("type").toInt());
                        } else if( e.tagName() == "filesystem" ) {
                            insertSeperator( e.attribute("seperator") );
                        }
                    }
                }
            }
        }
        xmlIds.removeOne(id);
    }
}

// <2DO> path needed anymore?
void DirSelectionDialog::insertDirectory( const QString& dir, const QString& /*path*/, DirType type )
{
    if( type != DISKDRIVE ) {
        QTreeWidgetItem * item = NULL;
        if( expandedItem_ == NULL ) {
            item = new QTreeWidgetItem(treeWidget);
        } else {
            bool present = false;
            for( int i=0; i<expandedItem_->childCount() && !present; ++i ) {
                present = expandedItem_->child( i )->text( 0 ) == dir;
            }
            if( ! present ) {
                item = new QTreeWidgetItem( expandedItem_ );
            }
        }
        if( item != NULL ) {
            item->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
            item->setText( 0, dir );
            item->setIcon( 0, shareIcons_[type] );
        }
    }
}

void DirSelectionDialog::insertSeperator( const QString& seperator )
{
    filesystemSeperator_ = seperator;
}


/*!
    \fn DirSelectionDialog::updateSubDirectoriesSlot(QTreeWidgetItem* item)
 */
void DirSelectionDialog::updateSubDirectoriesSlot( QTreeWidgetItem * item )
{
    if( item->childCount() == 0 ) {
        expandedItem_ = item;
        xmlIds << xml_->get( "directory", "&directory=" + getTreePath(item) );
    }
}


/*!
    \fn DirSelectionDialog::getPath(QTreeWidgetItem* item)
 */
QString DirSelectionDialog::getTreePath( QTreeWidgetItem * const item ) const
{
    if ( ! item ) return "";
    QString path = item->text( 0 );
    QTreeWidgetItem * curItem;
    for ( curItem = item->parent(); curItem != NULL; curItem = curItem->parent() ) {
        path = curItem->text( 0 ) + filesystemSeperator_ + path;
    }
    return path;
}


/*!
    \fn DirSelectionDialog::selectedPath()
 */
QString DirSelectionDialog::getSelectedPath() const
{
    QItemList selectedItems = treeWidget->selectedItems();
    if( ! selectedItems.empty() ) {
        return getTreePath( selectedItems.first() );
    } else {
        return QString::Null();
    }
}

