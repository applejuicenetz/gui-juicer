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

QAjUploadWidget::QAjUploadWidget( QAjIcons *icons, QWidget *parent, const char *name) : QAjListWidget(icons, ID_UP_INDEX, parent, name)
{
	uploadStatusDescr[0] = "unknown";
	uploadStatusDescr[1] = "active";
	uploadStatusDescr[2] = "queueing";
	uploadStatusDescr[3] = "unknown";
	uploadStatusDescr[4] = "unknown";
	uploadStatusDescr[5] = "connecting";
	uploadStatusDescr[6] = "connecting indirect";
	uploadStatusDescr[7] = "connection failed";
	
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
	int newStatus = status.toInt();
	
	QTreeWidgetItem* takeFromItem;
	QTreeWidgetItem* insertIntoItem;
	
	int oldStatus;
	if( uploadItem != NULL )
		oldStatus = uploadItem->getStatus();
	else
		oldStatus = NEW_UPLOAD;

	if( ( oldStatus != newStatus ) &&  ( !isOtherUpload( oldStatus ) || !isOtherUpload( newStatus ) ) )
	{
		if( oldStatus == ACTIVE_UPLOAD )
			takeFromItem = activeUpload;
		else if( oldStatus == QUEUEING_UPLOAD )
			takeFromItem = queuedUpload;
		else
			takeFromItem = otherUpload;
		
		if( newStatus == ACTIVE_UPLOAD )
			insertIntoItem = activeUpload;
		else if( newStatus == QUEUEING_UPLOAD )
			insertIntoItem = queuedUpload;
		else
			insertIntoItem = otherUpload;
	
		if( uploadItem == NULL )
		{
			uploadItem = new QAjUploadItem( insertIntoItem );
			uploads[ id.toULong() ] = uploadItem;
			uploadItem->setText( ID_UP_INDEX, id);
			uploadItem->setText( SHAREID_UP_INDEX, shareId);
			uploadItem->setText( NICK_UP_INDEX, nick );
			uploadItem->setText( FILENAME_UP_INDEX, "" );
			if( os == WINDOWS )
				uploadItem->setIcon( OS_UP_INDEX, *icons->windowsIcon );
			else if( os == LINUX )
				uploadItem->setIcon( OS_UP_INDEX, *icons->linuxIcon );
		}
		else
		{
			takeFromItem->takeChild( takeFromItem->indexOfChild( uploadItem ) );
			insertIntoItem->insertChild( 0, uploadItem );
		}
	}

	uploadItem->setStatus( newStatus );

	uploadItem->setText(SPEED_UP_INDEX, QConvert::bytes(speed) + "/s" );
	uploadItem->setText(STATUS_UP_INDEX, uploadStatusDescr[status.toInt()] );
	uploadItem->setText(PRIORITY_UP_INDEX, priority );
	
	if( uploadItem->text( FILENAME_UP_INDEX ) != "" )
		return true;

	return false;
}

bool QAjUploadWidget::remove( QString id )
{
	QTreeWidgetItem *item = removeUpload( id );
	if( item != NULL )
	{
		delete item;
		return true;
	}
	return false;
}

bool QAjUploadWidget::setFileName( QString shareId, QString fileName )
{
	uploadsIt = uploads.begin();
	while ( uploadsIt != uploads.end() )
	{
		if ( uploadsIt->second->text( SHAREID_UP_INDEX ) == shareId )
		{
			uploadsIt->second->setText( FILENAME_UP_INDEX, fileName );
		}
		uploadsIt++;
	}
	return true;
}

bool QAjUploadWidget::isOtherUpload( int status )
{
	return ( ( status != ACTIVE_UPLOAD ) && ( status != QUEUEING_UPLOAD ) && ( status != NEW_UPLOAD) );
}

QAjUploadItem* QAjUploadWidget::findUpload( QString id )
{
	uploadsIt = uploads.find( id.toULong() );
	if( uploadsIt != uploads.end() )
		return uploadsIt->second;
	else
		return NULL;
}

QAjUploadItem* QAjUploadWidget::removeUpload( QString id )
{
	uploadsIt = uploads.find( id.toULong() );
	if( uploadsIt != uploads.end() )
	{
		uploads.erase( uploadsIt );
		return uploadsIt->second;
	}
	else
		return NULL;
}
