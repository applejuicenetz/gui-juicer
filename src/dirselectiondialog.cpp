//
// C++ Implementation: dirselectionbase
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QTreeWidgetItem>

#include "xmlmodule.h"

#include "dirselectiondialog.h"

QList<DirSelectionDialog::Dir> DirSelectionDialog::rootDir = QList<DirSelectionDialog::Dir>();

DirSelectionDialog::DirSelectionDialog(const QString& label, const QString& startDir, bool hideNew, XMLModule * const xml, QWidget* parent, Qt::WFlags fl )
  : QDialog( parent, fl )
  , Ui::targetFolderDialogBase()
  , xml_( xml )
  , expandedItem_( 0 )
{
    setupUi( this );
    textLabel->setText(label);

    shareIcons_[WORKSTATION] = QIcon(":/small/system.png");
    shareIcons_[DRIVE]       = QIcon(":/small/hdd.png");
    shareIcons_[DISKDRIVE]   = QIcon(":/small/floppy.png");
    shareIcons_[FOLDER]      = QIcon(":/small/folder.png");
    shareIcons_[DESKTOP]     = QIcon(":/small/desktop.png");

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
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    if( ! selectedItems.empty() ) {
        return getTreePath( selectedItems.first() );
    } else {
        return QString::Null();
    }
}

