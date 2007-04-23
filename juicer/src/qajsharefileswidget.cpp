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

    currIdRoundRobin = -1;
    QStringList headers;
    headers.append( tr("filename") );
    headers.append( tr("size") );
    setHeaderLabels( headers );

}

QAjShareFilesWidget::~QAjShareFilesWidget()
{
}

void QAjShareFilesWidget::insertFile(QString id, QString hash, QString fileName, QString size)
{
    QAjShareFileItem *shareFileItem = findFile( id );
    if ( shareFileItem == NULL )
    {
        shareFileItem = new QAjShareFileItem( id, this );
        sharedFiles[ id ] = shareFileItem;
    }
    shareFileItem->update( hash, fileName, size );
}

QAjShareFileItem* QAjShareFilesWidget::findFile( QString id )
{
    if (sharedFiles.contains( id ))
        return sharedFiles[id];
    else
        return NULL;
}

