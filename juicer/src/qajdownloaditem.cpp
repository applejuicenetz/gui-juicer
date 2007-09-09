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


QAjDownloadItem::QAjDownloadItem( QString id, QAjListWidget *parent ) : QAjItem( parent, id )
{
    this->setFlags( Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable );

    int i;
    for ( i=1; i<NUM_DOWN_COL; i++ )
    {
        setTextAlignment( i, Qt::AlignRight );
    }
    parentWidget = parent;
    activeSources = queuedSources = otherSources = 0;

    size = 0.0;
    ready = 0.0;
    speed = 0.0;
    pixmap = new QPixmap(1, 1);

    finishedChanged = true;
    firstFinished = false;
    first = true;

    setText( SOURCES_DOWN_INDEX, getSourcesString() );
    setText( SPEED_DOWN_INDEX, QString("0 b/s") );
    setText( REMAIN_TIME_DOWN_INDEX, QString( "n.a." ) );

    activeSourcesItem = new QAjItem( this );
    activeSourcesItem->setText(FILENAME_DOWN_INDEX, QObject::tr("1. active"));
    activeSourcesItem->setFlags( Qt::ItemIsEnabled );

    queuedSourcesItem = new QAjItem( this );
    queuedSourcesItem->setText(FILENAME_DOWN_INDEX, QObject::tr("2. queueing"));
    queuedSourcesItem->setFlags( Qt::ItemIsEnabled );

    otherSourcesItem = new QAjItem( this );
    otherSourcesItem->setText(FILENAME_DOWN_INDEX, QObject::tr("3. others"));
    otherSourcesItem->setFlags( Qt::ItemIsEnabled );

    partListWidget = new QAjPartListWidget();
    partListWidget->hide();

    powerSpin = new QAjPowerSpin(id, parent);
    parent->setItemWidget(this, POWER_DOWN_INDEX, powerSpin);

    QSize sizeH = powerSpin->sizeHint();
    sizeH.setHeight(sizeHint(FILENAME_DOWN_INDEX).height());
    setSizeHint(POWER_DOWN_INDEX, sizeH);
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
    if ( partListWidget != NULL )
        delete partListWidget;
}

void QAjDownloadItem::moveItem( QAjUserItem *userItem, QString oldStatus )
{
    if ( oldStatus != userItem->getStatus() )
    {

        if ( userItem->parent() && ( oldStatus != NEW_SOURCE ) )
            userItem->parent()->takeChild( userItem->parent()->indexOfChild( userItem ) );

        if ( oldStatus == ACTIVE_SOURCE )
        {
            if ( userItem->getStatus() == QUEUED_SOURCE )
            {
// 				activeSourcesItem->takeChild( indexOfChild( userItem ) );
                queuedSourcesItem->addChild( userItem );
            }
            else if ( (userItem->getStatus() != ACTIVE_SOURCE) &&  (userItem->getStatus() != QUEUED_SOURCE) )
            {
// 				activeSourcesItem->takeChild( indexOfChild( userItem ) );
                otherSourcesItem->addChild( userItem );
            }
        }
        else if ( oldStatus == QUEUED_SOURCE )
        {
            if ( userItem->getStatus() == ACTIVE_SOURCE )
            {
// 				queuedSourcesItem->takeChild( indexOfChild( userItem ) );
                activeSourcesItem->addChild( userItem );
            }
            else if ( (userItem->getStatus() != ACTIVE_SOURCE) &&  (userItem->getStatus() != QUEUED_SOURCE) )
            {
// 				queuedSourcesItem->takeChild( indexOfChild( userItem ) );
                otherSourcesItem->addChild( userItem );
            }
        }
        else if ( oldStatus == NEW_SOURCE )
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
            if ( userItem->getStatus() == ACTIVE_SOURCE )
            {
// 				otherSourcesItem->takeChild( indexOfChild( userItem ) );
                activeSourcesItem->addChild( userItem );
            }
            else if ( userItem->getStatus() == QUEUED_SOURCE )
            {
// 				otherSourcesItem->takeChild( indexOfChild( userItem ) );
                queuedSourcesItem->addChild( userItem );
            }
        }
    }
}


void QAjDownloadItem::decSources( QString type )
{
    if ( type == NEW_SOURCE );
    else if ( type == ACTIVE_SOURCE )
        decActiveSources();
    else if ( type == QUEUED_SOURCE )
        decQueuedSources();
    else
        decOtherSources();
}

void QAjDownloadItem::incSources( QString type )
{
    if ( type == NEW_SOURCE );
    else if ( type == ACTIVE_SOURCE )
        incActiveSources();
    else if ( type == QUEUED_SOURCE )
        incQueuedSources();
    else
        incOtherSources();
}

QAjUserItem* QAjDownloadItem::findUser( QString id )
{
    if ( users.contains( id ) )
        return users[ id ];
    else
        return NULL;
}

void QAjDownloadItem::removeUser( QString id )
{
    QAjUserItem* item = users[ id ];
    users.remove( id );
    delete item;
}

void QAjDownloadItem::updateUser( QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString statusString, QIcon* osIcon )
{
    QAjUserItem* userItem = findUser( id );
    if ( userItem == NULL )
    {
        if ( status == ACTIVE_SOURCE )
            userItem = new QAjUserItem( id, activeSourcesItem );
        else if ( status == QUEUED_SOURCE )
            userItem = new QAjUserItem( id, queuedSourcesItem );
        else
            userItem = new QAjUserItem( id, otherSourcesItem );
        users[ id ] = userItem;
    }
    else
    {
        decSources( userItem->getStatus() );
    }
    QString oldStatus = userItem->getStatus();

    userItem->update( fileName, speed, status, power, queuePos, statusString, osIcon );

    incSources( userItem->getStatus() );

    this->speed += userItem->getSpeedDif();

    moveItem( userItem, oldStatus );

}

void QAjDownloadItem::update( QString hash, QString fileName, QString status, QString size, QString ready, QString power, QString tempNumber )
{
    this->tempNumber = tempNumber;
    if ( this->size == 0.0 )
    {
        this->size = size.toDouble();//ULongLong();
    }
    double readyNew = ready.toDouble();//ULongLong();

    // save hash number for recovering the ajfsp link
    if ( this->hash.isEmpty() )
    {
        this->hash = hash;
    }

    if ( status == DOWN_FINISHED )
    {
        readyNew = this->size;
        this->remainingSize = 0.0;
    }

    if ( (readyNew != this->ready) || this->first )
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

    if ( this->text( SIZE_DOWN_INDEX ).isEmpty() )
        this->setText( SIZE_DOWN_INDEX, " " + QConvert::bytes( this->size ) + " " );
    this->setText( FILENAME_DOWN_INDEX, fileName );
    partListWidget->setFilename( fileName );

//     this->setText( POWER_DOWN_INDEX, " " + QConvert::power( power ) + " " );

    float p = QConvert::powerValue( power );
    if(p>1.0) {
        this->powerSpin->spin->setValue( p );
    }
    this->powerSpin->check->setChecked(p>1.0);
    this->powerSpin->spin->setEnabled(p>1.0);

    firstFinished = false;
    if ( status != this->status )
    {
        if ( status == DOWN_PAUSED )
            this->setTextColor( FILENAME_DOWN_INDEX, Qt::darkGray );
        else if ( status == DOWN_FINISHED )
        {
            this->setTextColor( FILENAME_DOWN_INDEX, Qt::darkGreen );
            firstFinished = true;
        }
        else if ( status == DOWN_CANCELD )
            this->setTextColor( FILENAME_DOWN_INDEX, Qt::red );
        else
            this->setTextColor( FILENAME_DOWN_INDEX, Qt::black );
        this->status = status;
    }
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
    if ( ready > 0.0 )
    {
        if ( finished >= 1.0 )
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

bool QAjDownloadItem::updateView( QHash<QString, QString>* downloadStatusDescr )
{
    if( ( status == DOWN_SEARCHING ) || ( status == DOWN_LOADING ) )
    {
        if ( getActiveSources() > 0 )
        {
            setTextColor( FILENAME_DOWN_INDEX, Qt::darkBlue );
            setText( STATUS_DOWN_INDEX, downloadStatusDescr->value( "-1", "unknown" ) );
            status = DOWN_LOADING;
        }
        else if ( getActiveSources() <= 0 )
        {
            setTextColor( FILENAME_DOWN_INDEX, Qt::black );
            setText( STATUS_DOWN_INDEX, downloadStatusDescr->value( status, "unknown" ) );
            status = DOWN_SEARCHING;
        }
    }
    else
    {
        setText( STATUS_DOWN_INDEX, downloadStatusDescr->value( status, "unknown" ) );
    }

    setText( SOURCES_DOWN_INDEX, getSourcesString() );

    if ( finishedChanged )
    {
        setFinishedPixmap( 100, 10, ready );
        setText( FINISHED_SIZE_DOWN_INDEX, " " + QConvert::bytes( ready ) + " " );
        setText( REMAIN_SIZE_DOWN_INDEX, " " + QConvert::bytes( remainingSize ) + " " );
    }
    
    if( status == DOWN_FINISHED )
    {
        speed = 0.0;
        remainingSec = 0.0;
    }

    setText( SPEED_DOWN_INDEX, " " + QConvert::bytes( speed, 1 ) + "/s ");

    if ( speed > 0 )
    {
        remainingSec = (long int)(remainingSize / speed);
        setText( REMAIN_TIME_DOWN_INDEX, " " + QConvert::time( remainingSec ) + " " );
    }
    else
    {
        remainingSec = LONG_MAX;
        setText( REMAIN_TIME_DOWN_INDEX, QString( " n.a. " ) );
    }

    bool ret = firstFinished;
    firstFinished = false;
    return ret;
}


void QAjDownloadItem::showWidget( const QPoint & p )
{
    if ( partListWidget == NULL )
    {
        partListWidget = new QAjPartListWidget();
    }
    partListWidget->move( p );
    partListWidget->show();
}

void QAjDownloadItem::deleteUsers()
{
    QList<QString> userIds = users.keys();
    int i;
    for ( i=0; i<userIds.size(); i++ )
        delete users[userIds[i]];
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
    if ( partListWidget->isVisible() )
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

    while ( it.hasNext() )
    {
        fromPart = toPart;
        toPart = it.next();

        partSize = toPart.fromPosition - fromPart.fromPosition;

        switch ( fromPart.type )
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
    if ( missing < 1.0 && missing > 0.0 )
        setText( MISSING_DOWN_INDEX, QString::number( missing, 'f', 1 ) + "%" );
    else
        setText( MISSING_DOWN_INDEX, QString::number( missing, 'f', 0 ) + "%" );
    if ( missing > 0.0 )
        setTextColor( MISSING_DOWN_INDEX, Qt::darkRed );
    else
        setTextColor( MISSING_DOWN_INDEX, Qt::darkGreen );
}

bool QAjDownloadItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    QAjDownloadItem* downItem = (QAjDownloadItem*)&other;
    switch ( sortIndex )
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

QString QAjDownloadItem::getLinkAJFSP() {
    return "ajfsp://file|"
        + this->text(FILENAME_DOWN_INDEX) + "|"
        + this->hash + "|"
        + QString::number( (int)this->size ) + "/";
}
