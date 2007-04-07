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
#include "qajdownloaditem.h"


QAjDownloadItem::QAjDownloadItem( QAjDescription *description, QAjIcons *icons,  QAjListWidget *parent, const char *name) : QAjItem( DOWN, parent )
{
	int i;
	for( i=1; i<NUM_DOWN_COL; i++ )
	{
		setTextAlignment( i, Qt::AlignRight );
	}
	parentWidget = parent;
	activeSources = queuedSources = otherSources = 0;
	this->description = description;
	this->icons = icons;
	
	size = 0.0;
	ready = 0.0;
	speed = 0.0;
	pixmap = new QPixmap(1, 1);
	
	finishedChanged = true;
	first = true;

	setText( SOURCES_DOWN_INDEX, getSourcesString() );
	setText( SPEED_DOWN_INDEX, QString("0 b/s") );
	setText( REMAIN_TIME_DOWN_INDEX, QString( "n.a." ) );

	activeSourcesItem = new QAjItem( GENERIC, this );
	activeSourcesItem->setText(FILENAME_DOWN_INDEX, QObject::tr("1. active"));
	activeSourcesItem->setFlags( Qt::ItemIsEnabled );
	
	queuedSourcesItem = new QAjItem( GENERIC, this );
	queuedSourcesItem->setText(FILENAME_DOWN_INDEX, QObject::tr("2. queueing"));
	queuedSourcesItem->setFlags( Qt::ItemIsEnabled );

	otherSourcesItem = new QAjItem( GENERIC, this );
	otherSourcesItem->setText(FILENAME_DOWN_INDEX, QObject::tr("3. others"));
	otherSourcesItem->setFlags( Qt::ItemIsEnabled );

	partListWidget = new QAjPartListWidget( icons );
	partListWidget->hide();
}

QAjDownloadItem::~QAjDownloadItem()
{
	takeChild( indexOfChild( activeSourcesItem ) );
	takeChild( indexOfChild( queuedSourcesItem ) );
	takeChild( indexOfChild( otherSourcesItem ) );
	deleteUsers();
	delete activeSourcesItem;
	delete queuedSourcesItem;
	delete otherSourcesItem;
	delete pixmap;
	if( partListWidget != NULL )
		delete partListWidget;
}

void QAjDownloadItem::moveItem( QAjUserItem *userItem, int oldStatus )
{
	if( oldStatus != userItem->getStatus() )
	{

		if( userItem->parent() && ( oldStatus != NEW_SOURCE ) )
			userItem->parent()->takeChild( userItem->parent()->indexOfChild( userItem ) );
	
		if( oldStatus == ACTIVE_SOURCE )
		{
			if( userItem->getStatus() == QUEUED_SOURCE )
			{
// 				activeSourcesItem->takeChild( indexOfChild( userItem ) );
				queuedSourcesItem->addChild( userItem );
			}
			else if( (userItem->getStatus() != ACTIVE_SOURCE) &&  (userItem->getStatus() != QUEUED_SOURCE) )
			{
// 				activeSourcesItem->takeChild( indexOfChild( userItem ) );
				otherSourcesItem->addChild( userItem );
			}
		}
		else if( oldStatus == QUEUED_SOURCE )
		{
			if( userItem->getStatus() == ACTIVE_SOURCE )
			{
// 				queuedSourcesItem->takeChild( indexOfChild( userItem ) );
				activeSourcesItem->addChild( userItem );
			}
			else if( (userItem->getStatus() != ACTIVE_SOURCE) &&  (userItem->getStatus() != QUEUED_SOURCE) )
			{
// 				queuedSourcesItem->takeChild( indexOfChild( userItem ) );
				otherSourcesItem->addChild( userItem );
			}
		}
		else if( oldStatus == NEW_SOURCE )
		{
/*			if( userItem->getStatus() == ACTIVE_SOURCE )
			{
				activeSourcesItem->addChild( userItem );
			}
			else if( userItem->getStatus() == QUEUED_SOURCE )
			{
				queuedSourcesItem->addChild( userItem );
			}
			else if( (userItem->getStatus() != ACTIVE_SOURCE) &&  (userItem->getStatus() != QUEUED_SOURCE) )
			{
				otherSourcesItem->addChild( userItem );
			}*/
		}
		else
		{
			if( userItem->getStatus() == ACTIVE_SOURCE )
			{
// 				otherSourcesItem->takeChild( indexOfChild( userItem ) );
				activeSourcesItem->addChild( userItem );
			}
			else if( userItem->getStatus() == QUEUED_SOURCE )
			{
// 				otherSourcesItem->takeChild( indexOfChild( userItem ) );
				queuedSourcesItem->addChild( userItem );
			}
		}
	}
}


void QAjDownloadItem::decSources( int type )
{
	switch( type )
	{
		case NEW_SOURCE:
			break;
		case ACTIVE_SOURCE:
			decActiveSources();
			break;
		case QUEUED_SOURCE:
			decQueuedSources();
			break;
		default:
			decOtherSources();
			break;
	}
}

void QAjDownloadItem::incSources( int type )
{
	switch( type )
	{
		case NEW_SOURCE:
			break;
		case ACTIVE_SOURCE:
			incActiveSources();
			break;
		case QUEUED_SOURCE:
			incQueuedSources();
			break;
		default:
			incOtherSources();
			break;
	}
}

QAjUserItem* QAjDownloadItem::findUser( QString id )
{
	usersIt = users.find( id.toULong() );
	if( usersIt != users.end() )
		return usersIt->second;
	else
		return NULL;
}

QAjUserItem* QAjDownloadItem::removeUser( QString id )
{
	usersIt = users.find( id.toULong() );
	if( usersIt != users.end() )
	{
		users.erase( usersIt );
		return usersIt->second;
	}
	else
		return NULL;
}

void QAjDownloadItem::updateUser( QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString os )
{
	QAjUserItem* userItem = findUser( id );
	if( userItem == NULL )
	{
		int newStatus = status.toInt();
		switch( newStatus )
		{
			case ACTIVE_SOURCE:
				userItem = new QAjUserItem( description, icons, activeSourcesItem );
				break;
			case QUEUED_SOURCE:
				userItem = new QAjUserItem( description, icons, queuedSourcesItem );
				break;
			default:
				userItem = new QAjUserItem( description, icons, otherSourcesItem );
				break;
		}
		users[ id.toULong() ] = userItem;
	}
	else
	{
		decSources( userItem->getStatus() );
	}
	int oldStatus = userItem->getStatus();
	userItem->update( fileName, speed, status, power, queuePos, os );
	
	incSources( userItem->getStatus() );

	this->speed += userItem->getSpeedDif();

	moveItem( userItem, oldStatus );

}

double QAjDownloadItem::calculateSpeed()
{
	speed = 0.0;
	usersIt  = users.begin();
	while( usersIt != users.end() )
	{
		speed += usersIt->second->getSpeed();
		++usersIt;
	}
	return speed;
}

void QAjDownloadItem::update( QString fileName, QString status, QString size, QString ready, QString power )
{
    
	int newStatus = status.toInt();
	if( this->size == 0.0 )
	{
		this->size = size.toDouble();//ULongLong();
	}
	double readyNew = ready.toDouble();//ULongLong();

	if( newStatus == DOWN_FINISHED )
	{
		readyNew = this->size;
		this->remainingSize = 0.0;
	}
	
	if( (readyNew != this->ready) || this->first )
	{
		this->ready = readyNew;
		this->remainingSize = this->size - this->ready;
		this->first = false;
		this->finishedChanged = true;
	}
	else
	{
		this->finishedChanged = false;
	}

	if( this->text( SIZE_DOWN_INDEX ).isEmpty() )
		this->setText( SIZE_DOWN_INDEX, " " + QConvert::bytes( this->size ) + " " );
//	if( this->text( FILENAME_DOWN_INDEX ).isEmpty() )
	{
		this->setText( FILENAME_DOWN_INDEX, fileName );
		
	}
	partListWidget->setFilename( fileName );

	this->setText( POWER_DOWN_INDEX, " " + QConvert::power( power ) + " " );

/*	if( (newStatus == DOWN_SEARCHING) && (item->getActiveSources() > 0) )
		newStatus = DOWN_LOADING;*/
	if( newStatus != this->status )
	{
		switch ( newStatus )
		{
/*			case DOWN_LOADING:
				item->setPixmap( FILENAME_DOWN_INDEX, *icons->downloadLoadingPixmap );
				break;
			case DOWN_SEARCHING:
				item->setPixmap( FILENAME_DOWN_INDEX, *icons->downloadSearchingPixmap );
				break;*/
			case DOWN_PAUSED:
				//this->setIcon( FILENAME_DOWN_INDEX, *icons->downloadPausedIcon );
				this->setTextColor( FILENAME_DOWN_INDEX, Qt::darkGray );
				break;
			case DOWN_FINISHED:
				//this->setIcon( FILENAME_DOWN_INDEX, *icons->downloadFinishedIcon );
				this->setTextColor( FILENAME_DOWN_INDEX, Qt::darkGreen );
				break;
			case DOWN_CANCELD:
				//this->setIcon( FILENAME_DOWN_INDEX, *icons->downloadCanceldIcon );
				this->setTextColor( FILENAME_DOWN_INDEX, Qt::red );
				break;
			default:
				//this->setIcon( FILENAME_DOWN_INDEX, *icons->dummyIcon );
				this->setTextColor( FILENAME_DOWN_INDEX, Qt::black );
				break;
		}
		this->status = newStatus;
	}
	//item->setText( _DOWN_INDEX, descriptions.getDownloadStatusDescription(newStatus) );
}

void QAjDownloadItem::setFinishedPixmap(int newWidth, int newHeight, double newReady)
{
	ready = newReady;
	width = newWidth;
	height = newHeight;

	finished = ready / size;
	percent = (int)(finished * 100.0);
	delete pixmap;
	pixmap = new QPixmap(width, height);
	pixmap->fill(Qt::white);

	QPainter p(pixmap);
	if( ready > 0.0 )
	{
		if( finished >= 1.0 )
		{
			p.setPen(Qt::darkGreen);
			p.setBrush(Qt::darkGreen);
		}
		else
		{
			p.setPen(Qt::darkBlue);
			p.setBrush(Qt::darkBlue);
		}
		p.drawRect(0, 0, (int)(width*finished), height);
	}
	p.setPen(Qt::gray);
	p.setBrush(Qt::gray);
	p.drawRect((int)(width*finished+1), 2, (int)(width-(width*finished+1)), height-4);
	p.end();

	setIcon( FINISHED_DOWN_INDEX, QIcon(*pixmap) );
	setText( FINISHED_DOWN_INDEX, QString::number((int)(finished*100)) + "%" );
}

QString QAjDownloadItem::getSourcesString()
{
	return QString( " " + QString::number(activeSources) + "-" + QString::number(queuedSources) + "-" + QString::number(otherSources) + " " );
}

void QAjDownloadItem::updateView()
{
	setText( SPEED_DOWN_INDEX, " " + QConvert::bytes( speed, 1 ) + "/s ");
	
	if( speed > 0 )
	{
		remainingSec = remainingSize / speed;
		setText( REMAIN_TIME_DOWN_INDEX, " " + QConvert::time( remainingSec ) + " " );
	}
	else
	{
		remainingSec = LONG_MAX;
		setText( REMAIN_TIME_DOWN_INDEX, QString( " n.a. " ) );
	}

	if( ( status == DOWN_SEARCHING ) || ( status == DOWN_LOADING ) )
	{
		if( getActiveSources() > 0 ) 
		{
			setTextColor( FILENAME_DOWN_INDEX, Qt::darkBlue );
			setText( STATUS_DOWN_INDEX, description->getDownloadStatusDescription( DOWN_LOADING ) );
			status = DOWN_LOADING;
		}
		else if( getActiveSources() <= 0 )
		{
			setTextColor( FILENAME_DOWN_INDEX, Qt::black );
			setText( STATUS_DOWN_INDEX, description->getDownloadStatusDescription( DOWN_SEARCHING ) );
			status = DOWN_SEARCHING;
		}
	}
	else
	{
		setText( STATUS_DOWN_INDEX, description->getDownloadStatusDescription( status ) );
	}

	setText( SOURCES_DOWN_INDEX, getSourcesString() );

	if( finishedChanged )
	{
		setFinishedPixmap( 100, 10, ready );
		setText( FINISHED_SIZE_DOWN_INDEX, " " + QConvert::bytes( ready ) + " " );
		setText( REMAIN_SIZE_DOWN_INDEX, " " + QConvert::bytes( remainingSize ) + " " );
	}
}


int QAjDownloadItem::compare( QTreeWidgetItem * i, int col, bool ascending ) const
{
	double a, b;
	if( col == SIZE_DOWN_INDEX )
	{
		a = size;
		b = ((QAjDownloadItem*)i)->getSize(); 
	}
	else if( col == FINISHED_SIZE_DOWN_INDEX )
	{
		a = ready;
		b = ((QAjDownloadItem*)i)->getReady();
	}
	else if( col == REMAIN_SIZE_DOWN_INDEX )
	{
		a = remainingSize;
		b = ((QAjDownloadItem*)i)->getRemainingSize();
	}
	else if( col == SPEED_DOWN_INDEX )
	{
		a = speed;
		b = ((QAjDownloadItem*)i)->getSpeed();
	}
	else if( col == FINISHED_DOWN_INDEX )
	{
		a = percent;
		b = ((QAjDownloadItem*)i)->getPercent();
	}
	else
	{
		if( text( col ) > i->text(col) )
			return 1;
		else if( text( col ) < i->text(col) )
			return -1;
		else
			return 0;
		//return QTreeWidgetItem::compare( i, col, ascending );
	}

	if( a > b )
		return 1;
	else if( a < b )
		return -1;
	else
		return 0;
}

void QAjDownloadItem::showWidget( const QPoint & p )
{
	if( partListWidget == NULL )
	{
		partListWidget = new QAjPartListWidget( icons );
	}
	partListWidget->move( p );
	partListWidget->show();
}

void QAjDownloadItem::deleteUsers()
{
	usersIt = users.begin();
	while( usersIt != users.end() )
	{
		delete usersIt->second;
		usersIt++;
	}
	users.clear();
}


/*!
    \fn QAjDownloadItem::getPartListWidget()
 */
QAjPartListWidget* QAjDownloadItem::getPartListWidget()
{
	return partListWidget;
}


/*!
    \fn QAjDownloadItem::setParts( Q_ULLONG size, QLinkedList<Part> partList )
 */
void QAjDownloadItem::setParts( qulonglong size, QLinkedList<Part> partList )
{
	if( partListWidget->isVisible() )
		partListWidget->update( size, partList );

   Part closePart;
   closePart.type = -10;
   closePart.fromPosition = size;
	partList.push_back( closePart );

	qulonglong bytesReady, bytesAvailable, bytesMissing;
	qulonglong partSize;
	bytesReady = bytesAvailable = bytesMissing = 0;

    Part fromPart, toPart;
    QLinkedListIterator<Part> it(partList);
    toPart = it.next();

    while( it.hasNext() )
    {
        fromPart = toPart;
        toPart = it.next();

        partSize = toPart.fromPosition - fromPart.fromPosition;

		switch( fromPart.type )
		{
			case -1:
				bytesReady += partSize;
				break;
			case 0:
				bytesMissing += partSize;
				break;
			default:
				bytesAvailable += partSize;
				break;
		}
	}
	missing = (double)bytesMissing / (double)size * 100.0;
	if( missing < 1.0 && missing > 0.0 )
		setText( MISSING_DOWN_INDEX, QString::number( missing, 'f', 1 ) + "%" );
	else
		setText( MISSING_DOWN_INDEX, QString::number( missing, 'f', 0 ) + "%" );
	if( missing > 0.0 )
		setTextColor( MISSING_DOWN_INDEX, Qt::darkRed );
	else
		setTextColor( MISSING_DOWN_INDEX, Qt::darkGreen );
}

bool QAjDownloadItem::operator<( const QTreeWidgetItem & other ) const
{
	int sortIndex = treeWidget()->header()->sortIndicatorSection();
	Qt::SortOrder sortOrder = treeWidget()->header()->sortIndicatorOrder();
// 	QMutexLocker(&((QAjListWidget*)treeWidget())->mutex);
	QAjItem* item = (QAjItem*)&other;
	if( item->getType() ==  DOWN )
	{
		QAjDownloadItem* downItem = (QAjDownloadItem*)item;
		switch( sortIndex )
		{
			case FILENAME_DOWN_INDEX:
				return this->text( FILENAME_DOWN_INDEX ) < other.text( FILENAME_DOWN_INDEX );
			case SIZE_DOWN_INDEX:
				return size < downItem->getSize();
			case FINISHED_SIZE_DOWN_INDEX:
				return ready < downItem->getReady();
			case REMAIN_SIZE_DOWN_INDEX:
				return remainingSize < downItem->getRemainingSize();
			case REMAIN_TIME_DOWN_INDEX:
				return remainingSec < downItem->getRemainingSec();
			case SPEED_DOWN_INDEX:
				return this->speed < downItem->getSpeed();
			case MISSING_DOWN_INDEX:
				return this->missing < downItem->getMissing();
			case FINISHED_DOWN_INDEX:
				return this->finished < downItem->getFinished();
			default:
				return this->text( sortIndex ) < other.text( sortIndex );
		}
	}
	else
	{
		return this->text( sortIndex ) < other.text( sortIndex );
		return false;
	}
}
