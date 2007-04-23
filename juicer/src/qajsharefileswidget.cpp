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
    xml->get("shares");
}

