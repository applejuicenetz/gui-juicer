//
// C++ Implementation: qajsharefileswidget
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qajsharefileswidget.h"

QAjShareFilesWidget::QAjShareFilesWidget( QXMLModule* xml, QWidget *parent ) : QAjListWidget( xml, parent )
{
    QStringList headers;
    for ( int i = 0; i < NUM_SHARED_FILES_OVERVIEW_COL; i++ )
    {
        switch (i)
        {
        case FILENAME_SHARED_FILE_INDEX:
              headers.append( tr("filename") );
              break;
        case SIZE_SHARED_FILE_INDEX:
              headers.append( tr("size") );
              break;
        case PRIORITY_SHARED_FILE_INDEX:
              headers.append( tr("priority") );
              break;
        }
    }
    setHeaderLabels( headers );
}

QAjShareFilesWidget::~QAjShareFilesWidget()
{
}

void QAjShareFilesWidget::insertFile( QString id, QString hash, QString fileName, QString size, QString priority, QString filesystemSeperator )
{
    QAjShareFileItem *shareFileItem = findFile( id );
    if ( shareFileItem == NULL )
    {
        shareFileItem = new QAjShareFileItem( id, this );
        sharedFiles[ id ] = shareFileItem;
        shareFileItem->setHidden(true);
    }
    shareFileItem->update( hash, fileName, size, priority, filesystemSeperator );
    this->adjustSizeOfColumns();
}

QAjShareFileItem* QAjShareFilesWidget::findFile( QString id )
{
    if (sharedFiles.contains( id ))
        return sharedFiles[id];
    else
        return NULL;
}

void QAjShareFilesWidget::updateSharedFilesList() {
    xml->get("share");
}

/*!
 * show only the files in the choosen directory
 */
void QAjShareFilesWidget::updateVisibleFiles( QString path ) {

    if (this->sharedFiles.isEmpty()) return;

    QList<QAjShareFileItem*> files = sharedFiles.values();
    for ( int i = 0; i < files.size(); i++ ) {
        files.at(i)->setHidden(!files.at(i)->getPath().startsWith( path ));
    }
    this->adjustSizeOfColumns();
}

void QAjShareFilesWidget::linkSlot() {

    QString link;

    QList<QAjItem *>  selectedItems = selectedAjItems();
    QAjShareFileItem* ajShareItem = (QAjShareFileItem*)selectedItems[0];
    link = ajShareItem->getLinkAJFSP();

    QApplication::clipboard()->setText(link);
}

void QAjShareFilesWidget::setPriority( int prio ) {
    QList<QTreeWidgetItem *>  selectedItems = this->selectedItems();

    for ( int i = 0; i < selectedItems.size(); i++ ) {
        QAjShareFileItem* ajShareItem = (QAjShareFileItem*)selectedItems.at(i);
        xml->set( "setpriority", "&priority=" + QString::number(prio) + "&id=" + ajShareItem->getId() );
    }

    xml->get( "share" );
}


/*!
    \fn QAjShareFilesWidget::findFile( QString size, QString hash )
 */
QAjShareFileItem* QAjShareFilesWidget::findFile( QString size, QString hash )
{
    QList<QAjShareFileItem*> list = sharedFiles.values();
    int i;
    for( i=0; i<list.size(); i++ )
    {
        if( list[i]->getSize() == size.toDouble() && list[i]->getHash() == hash )
            return list[i];
    }
    return NULL;
}
