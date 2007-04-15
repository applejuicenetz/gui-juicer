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
#include "qajuploadwidget.h"

QAjUploadWidget::QAjUploadWidget( QXMLModule* xml, QWidget *parent ) : QAjListWidget( xml, parent )
{
    uploadStatusDescr["1"] = "active";
    uploadStatusDescr["2"] = "queueing";
    uploadStatusDescr["5"] = "connecting";
    uploadStatusDescr["6"] = "connecting indirect";
    uploadStatusDescr["7"] = "connection failed";

    linuxIcon = new QIcon(":/small/linux.png");
    windowsIcon = new QIcon(":/small/windows.png");
    macIcon = new QIcon(":/small/mac.png");
    solarisIcon = new QIcon(":/small/solaris.png");
    freeBsdIcon = new QIcon(":/small/freebsd.png");
    netwareIcon = new QIcon(":/small/netware.png");

    QStringList headers;
    int i;
    for ( i=0; i<NUM_UP_COL; i++)
    {
        switch (i)
        {
        case FILENAME_UP_INDEX:
            headers.append( tr("filename") );
            break;
        case SPEED_UP_INDEX:
            headers.append( tr("speed") );
            break;
        case STATUS_UP_INDEX:
            headers.append( tr("status") );
            break;
        case PRIORITY_UP_INDEX:
            headers.append( tr("priority") );
            break;
        case NICK_UP_INDEX:
            headers.append( tr("nick") );
            break;
        case OS_UP_INDEX:
            headers.append( tr("os") );
            break;
        case DIRECTSTATE_UP_INDEX:
            headers.append( tr("direct state") );
            break;
        }
    }
    setHeaderLabels( headers );

    activeUpload = new QTreeWidgetItem( this );
    activeUpload->setText( FILENAME_UP_INDEX, "1. active" );
    queuedUpload = new QTreeWidgetItem( this );
    queuedUpload->setText( FILENAME_UP_INDEX, "2. queueing" );
    otherUpload = new QTreeWidgetItem( this );
    otherUpload->setText( FILENAME_UP_INDEX, "3. other" );

    expandItem( activeUpload );

    initToolBar();
}


QAjUploadWidget::~QAjUploadWidget()
{}


/*!
    \fn QAjUploadWidget::initToolBar()
 */
void QAjUploadWidget::initToolBar()
{
    toolBar = new QToolBar( "upload operations", this );
    toolBar->addAction( QIcon(":/dummy.png"), "" )->setDisabled( true );
    toolBar->hide();
}


bool QAjUploadWidget::insertUpload(QString id, QString shareId, QString version, QString os, QString status, QString directState, QString priority, QString nick, QString speed)
{
    QAjUploadItem *uploadItem = findUpload( id );

    if ( uploadItem == NULL )
    {
        uploadItem = new QAjUploadItem( id, shareId, 0 );
        uploads[ id ] = uploadItem;
        uploadItem->setText( NICK_UP_INDEX, nick );
        uploadItem->setText( FILENAME_UP_INDEX, "" );
        if ( os == WINDOWS )
            uploadItem->setIcon( OS_UP_INDEX, *windowsIcon );
        else if ( os == LINUX )
            uploadItem->setIcon( OS_UP_INDEX, *linuxIcon );
        else if ( os == MAC )
            uploadItem->setIcon( OS_UP_INDEX, *macIcon );
        else if ( os == NETWARE )
            uploadItem->setIcon( OS_UP_INDEX, *netwareIcon );
        else if ( os == SOLARIS )
            uploadItem->setIcon( OS_UP_INDEX, *solarisIcon );
        else if ( os == FREEBSD )
            uploadItem->setIcon( OS_UP_INDEX, *freeBsdIcon );
    }

    QTreeWidgetItem* takeFromItem;
    QTreeWidgetItem* insertIntoItem;

    QString oldStatus = uploadItem->getStatus();

    if ( ( oldStatus != status ) && ( !isOtherUpload( oldStatus ) || !isOtherUpload( status ) ) )
    {
        if ( oldStatus == NEW_UPLOAD )
            takeFromItem = NULL;
        else if ( oldStatus == ACTIVE_UPLOAD )
            takeFromItem = activeUpload;
        else if ( oldStatus == QUEUEING_UPLOAD )
            takeFromItem = queuedUpload;
        else
            takeFromItem = otherUpload;

        if ( status == ACTIVE_UPLOAD )
            insertIntoItem = activeUpload;
        else if ( status == QUEUEING_UPLOAD )
            insertIntoItem = queuedUpload;
        else
            insertIntoItem = otherUpload;

        if ( takeFromItem != NULL )
            takeFromItem->takeChild( takeFromItem->indexOfChild( uploadItem ) );
        insertIntoItem->insertChild( 0, uploadItem );
    }

    uploadItem->setStatus( status );

    uploadItem->setText(SPEED_UP_INDEX, QConvert::bytes(speed) + "/s" );
    uploadItem->setText(STATUS_UP_INDEX, uploadStatusDescr[status]);
    uploadItem->setText(PRIORITY_UP_INDEX, priority );

    return ( uploadItem->text( FILENAME_UP_INDEX ) != "" );
}

bool QAjUploadWidget::remove( QString id )
{
    QTreeWidgetItem *item = findUpload( id );
    if ( item != NULL )
    {
        uploads.remove( id );
        delete item;
        return true;
    }
    return false;
}

bool QAjUploadWidget::isOtherUpload( QString status )
{
    return ( ( status != ACTIVE_UPLOAD ) && ( status != QUEUEING_UPLOAD ) && ( status != NEW_UPLOAD) );
}

QAjUploadItem* QAjUploadWidget::findUpload( QString id )
{
    if ( uploads.contains( id ) )
        return uploads[ id ];
    else
        return NULL;
}


/*!
    \fn QAjUploadWidget::setFilename( QString shareId, QString filename )
 */
void QAjUploadWidget::setFilename( QString shareId, QString filename )
{
    setFilename(activeUpload, shareId, filename);
    setFilename(queuedUpload, shareId, filename);
    setFilename(otherUpload, shareId, filename);
}


/*!
    \fn QAjUploadWidget::setFilename( QTreeWidgetItem* uploadState, QString shareId, QString filename )
 */
void QAjUploadWidget::setFilename( QTreeWidgetItem* uploadState, QString shareId, QString filename )
{
    int i;
    for( i=0; i<uploadState->childCount(); i++ )
    {
        QAjUploadItem* item = (QAjUploadItem*)uploadState->child(i);
        if( item != NULL && item->shareId == shareId )
        {
            item->setText( FILENAME_UP_INDEX, filename );
        }
    }
}

