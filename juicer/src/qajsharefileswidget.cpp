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

//     currIdRoundRobin = -1;
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

void QAjShareFilesWidget::insertFile(QString id, QString hash, QString fileName, QString size, QString priority)
{
    QAjShareFileItem *shareFileItem = findFile( id );
    if ( shareFileItem == NULL )
    {
        shareFileItem = new QAjShareFileItem( id, this );
        sharedFiles[ id ] = shareFileItem;
    }
    shareFileItem->update( hash, fileName, size, priority );
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

// show only the files in the choosen directory
void QAjShareFilesWidget::updateVisibleFiles( QString path ) {

    if (this->sharedFiles.isEmpty()) return;

    QList<QString>  ids = this->sharedFiles.keys();

    for ( int i = 0; i < ids.size(); i++ ) {
        QAjShareFileItem *shareFileItem = findFile( ids[i] );
        if ( shareFileItem != NULL )
        {
            if ( shareFileItem->getPath().contains( path ) ) {
                sharedFiles[ ids[i] ]->setHidden( false );
            }
            else {
                sharedFiles[ ids[i] ]->setHidden( true );
            }
        }
        else {
            fprintf(stderr, "shared file with id %s not found\n", ids[i].toLatin1().data());
        }
    }

    repaint();
}

void QAjShareFilesWidget::linkSlot() {

    QString link;

    QList<QAjItem *>  selectedItems = selectedAjItems();
    QAjShareFileItem* ajShareItem = (QAjShareFileItem*)selectedItems[0];
    link = ajShareItem->getLinkAJFSP();

    QApplication::clipboard()->setText(link);
}


