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

#include "./xpm/windows.xpm"
#include "./xpm/linux.xpm"


QAjUploadWidget::QAjUploadWidget( QWidget *parent, const char *name) : QAjListWidget( ID_UP_INDEX, parent, name)
{
	uploadStatusDescr["1"] = "active";
	uploadStatusDescr["2"] = "queueing";
	uploadStatusDescr["5"] = "connecting";
	uploadStatusDescr["6"] = "connecting indirect";
	uploadStatusDescr["7"] = "connection failed";

    linuxIcon = new QIcon(QPixmap(linux_xpm));
    windowsIcon = new QIcon(QPixmap(windows_xpm));
	
	QStringList headers;
	int i;
	for( i=0; i<NUM_UP_COL; i++)
	{
		switch(i)
		{
			case ID_UP_INDEX:
				headers.append( tr("id") );
				break;
			case SHAREID_UP_INDEX:
				headers.append( tr("share id") );
				break;
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
	
	setColumnHidden( ID_UP_INDEX, true );
	setColumnHidden( SHAREID_UP_INDEX, true);
	
	activeUpload = new QTreeWidgetItem( this );
	activeUpload->setText( FILENAME_UP_INDEX, "1. active" );
	queuedUpload = new QTreeWidgetItem( this );
	queuedUpload->setText( FILENAME_UP_INDEX, "2. queueing" );
	otherUpload = new QTreeWidgetItem( this );
	otherUpload->setText( FILENAME_UP_INDEX, "3. other" );
	
	expandItem( activeUpload );
}


QAjUploadWidget::~QAjUploadWidget()
{
}

bool QAjUploadWidget::insertUpload(QString id, QString shareId, QString version, QString os, QString status, QString directState, QString priority, QString nick, QString speed)
{
    QAjUploadItem *uploadItem = findUpload( id );

    if( uploadItem == NULL )
    {
        uploadItem = new QAjUploadItem( NULL );
        uploads[ id ] = uploadItem;
        uploadItem->setText( ID_UP_INDEX, id);
        uploadItem->setText( SHAREID_UP_INDEX, shareId);
        uploadItem->setText( NICK_UP_INDEX, nick );
        uploadItem->setText( FILENAME_UP_INDEX, "" );
        if( os == WINDOWS )
            uploadItem->setIcon( OS_UP_INDEX, *windowsIcon );
        else if( os == LINUX )
            uploadItem->setIcon( OS_UP_INDEX, *linuxIcon );
    }

    QTreeWidgetItem* takeFromItem;
    QTreeWidgetItem* insertIntoItem;

    QString oldStatus = uploadItem->getStatus();

    if( ( oldStatus != status ) && ( !isOtherUpload( oldStatus ) || !isOtherUpload( status ) ) )
    {
        if( oldStatus == NEW_UPLOAD )
            takeFromItem = NULL;
        else if( oldStatus == ACTIVE_UPLOAD )
            takeFromItem = activeUpload;
        else if( oldStatus == QUEUEING_UPLOAD )
            takeFromItem = queuedUpload;
        else
            takeFromItem = otherUpload;

        if( status == ACTIVE_UPLOAD )
            insertIntoItem = activeUpload;
        else if( status == QUEUEING_UPLOAD )
            insertIntoItem = queuedUpload;
        else
            insertIntoItem = otherUpload;

        if( takeFromItem != NULL )
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
    if( item != NULL )
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
    if( uploads.contains( id ) )
        return uploads[ id ];
    else
        return NULL;
}


/*!
    \fn QAjUploadWidget::setFilename( QString shareId, QString filename )
 */
void QAjUploadWidget::setFilename( QString shareId, QString filename )
{
    int i;
    for(i=0;i<activeUpload->childCount(); i++)
    {
        QTreeWidgetItem* item = activeUpload->child(i);
        if( item->text( SHAREID_UP_INDEX ) == shareId )
        {
            item->setText( FILENAME_UP_INDEX, filename );
        }
    }
    for(i=0;i<queuedUpload->childCount(); i++)
    {
        QTreeWidgetItem* item = queuedUpload->child(i);
        if( item->text( SHAREID_UP_INDEX ) == shareId )
        {
            item->setText( FILENAME_UP_INDEX, filename );
        }
    }
    for(i=0;i<otherUpload->childCount(); i++)
    {
        QTreeWidgetItem* item = otherUpload->child(i);
        if( item->text( SHAREID_UP_INDEX ) == shareId )
        {
            item->setText( FILENAME_UP_INDEX, filename );
        }
    }
}
