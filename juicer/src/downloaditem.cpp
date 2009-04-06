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
#include "downloaditem.h"


DownloadItem::DownloadItem( const QString& id, QTreeWidget *parent )
  : Item( parent, id )
  , activeSourcesItem( 0 )
  , queuedSourcesItem( 0 )
  , otherSourcesItem( 0 )
  , progressBar( 0 )
  , powerSpin( 0 )
  , partListDialog( 0 )
{
    size_ = 0.0;
    ready = 0.0;
    speed = 0.0;

    firstFinished = false;

    activeSourcesItem = new Item( this );
    activeSourcesItem->setText(FILENAME_COL, QObject::tr("1. active"));
    activeSourcesItem->setFlags( Qt::ItemIsEnabled );

    queuedSourcesItem = new Item( this );
    queuedSourcesItem->setText(FILENAME_COL, QObject::tr("2. queueing"));
    queuedSourcesItem->setFlags( Qt::ItemIsEnabled );

    otherSourcesItem = new Item( this );
    otherSourcesItem->setText(FILENAME_COL, QObject::tr("3. others"));
    otherSourcesItem->setFlags( Qt::ItemIsEnabled );

    partListDialog = new PartListDialog();
    partListDialog->hide();

    powerSpin = new PowerSpin(id);
    powerSpin->setMaximumHeight(20);

    progressBar = new QProgressBar();

    setText( SOURCES_COL, getSourcesString() );
    setText( SPEED_COL, QString("0 b/s") );
    setText( REMAIN_TIME_COL, QString( "n.a." ) );

    QTimer::singleShot(100, this, SLOT(initPowerSpin()));
}

DownloadItem::~DownloadItem()
{
    takeChild( indexOfChild( activeSourcesItem ) );
    takeChild( indexOfChild( queuedSourcesItem ) );
    takeChild( indexOfChild( otherSourcesItem ) );
    deleteUsers();
    if ( activeSourcesItem )  delete activeSourcesItem;
    if ( queuedSourcesItem )  delete queuedSourcesItem;
    if ( otherSourcesItem )   delete otherSourcesItem;
    if ( partListDialog )     delete partListDialog;
}

void DownloadItem::initPowerSpin()
{
    QWidget* progressBarWidget = new QWidget(treeWidget());
    progressBarWidget->setContentsMargins(5, 0, 5, 0);
    QVBoxLayout* l = new QVBoxLayout();
    l->setSpacing(0);
    l->setMargin(1);
    l->addWidget(progressBar);
    progressBarWidget->setLayout(l);

    treeWidget()->setItemWidget(this, POWER_COL, powerSpin);
    treeWidget()->setItemWidget(this, FINISHED_COL, progressBarWidget);

//     connect( powerSpin, SIGNAL( valueChanged( const QString&) ), this, SLOT( applyPowerDownload() ) );
//     connect( powerSpin, SIGNAL( valueChanged( double ) ), this, SLOT( applyPowerDownload() ) );
}

void DownloadItem::moveItem( UserItem *userItem, const QString& oldStatus )
{
    if ( oldStatus != userItem->getStatus() ) {
        if ( userItem->QTreeWidgetItem::parent() && ( oldStatus != NEW_SOURCE ) ) {
            userItem->QTreeWidgetItem::parent()->takeChild( userItem->QTreeWidgetItem::parent()->indexOfChild( userItem ) );
        }
        if ( oldStatus == ACTIVE_SOURCE ) {
            if ( userItem->getStatus() == QUEUED_SOURCE ) {
                queuedSourcesItem->addChild( userItem );
            } else if ( (userItem->getStatus() != ACTIVE_SOURCE) &&  (userItem->getStatus() != QUEUED_SOURCE) ) {
                otherSourcesItem->addChild( userItem );
            }
        } else if ( oldStatus == QUEUED_SOURCE ) {
            if ( userItem->getStatus() == ACTIVE_SOURCE ) {
                activeSourcesItem->addChild( userItem );
            } else if ( (userItem->getStatus() != ACTIVE_SOURCE) &&  (userItem->getStatus() != QUEUED_SOURCE) ) {
                otherSourcesItem->addChild( userItem );
            }
        } else if ( oldStatus == NEW_SOURCE ) {
            // -- pass --
        } else {
            if ( userItem->getStatus() == ACTIVE_SOURCE ) {
                activeSourcesItem->addChild( userItem );
            } else if ( userItem->getStatus() == QUEUED_SOURCE ) {
                queuedSourcesItem->addChild( userItem );
            }
        }
    }
}

UserItem* DownloadItem::findUser( const QString& id ) 
{
    if ( users.contains( id ) ) {
        return users[ id ];
    }
    return NULL;
}

void DownloadItem::removeUser( const QString& id ) 
{
    UserItem* item = users[ id ];
    users.remove( id );
    delete item;
}

void DownloadItem::updateUser( const QString& id,
                                  const QString& fileName,
                                  const QString& nickname,
                                  const QString& speed,
                                  const QString& status,
                                  const QString& power,
                                  const QString& queuePos,
                                  const QString& statusString,
                                  QIcon& osIcon,
                                  const QString& downloadfrom,
                                  const QString& downloadto,
                                  const QString& actualdownloadposition,
                                  const QTime& time )
{
    UserItem* userItem = findUser( id );
    if ( userItem == NULL ) {
        if ( status == ACTIVE_SOURCE ) {
            userItem = new UserItem( id, activeSourcesItem );
        } else if ( status == QUEUED_SOURCE ) {
            userItem = new UserItem( id, queuedSourcesItem );
        } else {
            userItem = new UserItem( id, otherSourcesItem );
        }
        users[ id ] = userItem;
    }
    QString oldStatus = userItem->getStatus();
    userItem->update( fileName, nickname, speed, status, power, queuePos, statusString, osIcon, downloadfrom, downloadto, actualdownloadposition, time );
    moveItem( userItem, oldStatus );
}

bool DownloadItem::update( const QString& hash,
                              const QString& fileName,
                              const QString& status,
                              const QString& size,
                              const QString& ready,
                              const QString& power,
                              const QString& tempNumber )
{
    bool newStatus = false;
    if ( status != status_ ) {
        if ( status == DOWN_PAUSED ) {
            this->setTextColor( FILENAME_COL, Qt::darkGray );
        } else if ( status == DOWN_FINISHED ) {
            this->setTextColor( FILENAME_COL, Qt::darkGreen );
            firstFinished = true;
        } else if ( status == DOWN_CANCELD ) {
            this->setTextColor( FILENAME_COL, Qt::red );
        } else {
            this->setTextColor( FILENAME_COL, Qt::black );
        }
        status_ = status;
        newStatus = true;
    }

    this->tempNumber = tempNumber;
    if ( size_ == 0.0 && !size.isEmpty()) {
        size_ = size.toDouble();
    }

    // -- save hash number for recovering the ajfsp link --
    if ( hash_.isEmpty() ) {
        hash_ = hash;
    }
    if ( filename_.isEmpty() ) {
        filename_ = fileName;
    }
    if ( status_ == DOWN_FINISHED ) {
        this->ready = size_;
        this->remainingSize = 0.0;
    } else {
        this->ready = ready.toDouble();
        this->remainingSize = size_ - this->ready;
    }

    if ( this->text( SIZE_COL ).isEmpty() ) {
        this->setText( SIZE_COL, " " + QConvert::bytes( size_ ) + " " );
    }
    this->setText( FILENAME_COL, fileName );
    partListDialog->setFilename( fileName );

    float p = QConvert::powerValue( power );
    if( p > 1.0 ) {
        this->powerSpin->spin->setValue( p );
    }
    this->powerSpin->check->setChecked( p > 1.0 );
    this->powerSpin->spin->setEnabled( p > 1.0 );

    firstFinished = false;
    return newStatus;
}


QString DownloadItem::getSourcesString() {
    //return QString( " " + QString::number(activeSources) + "-" + QString::number(queuedSources) + "-" + QString::number(otherSources) + " " );
    QString sources = " " + QString::number(activeSourcesItem->childCount());
    sources += "-" + QString::number(queuedSourcesItem->childCount());
    sources += "-" + QString::number(otherSourcesItem->childCount());
    return sources;
}

bool DownloadItem::updateView(const QHash<QString, QString>& downloadStatusDescr ) 
{
    if( ( status_ == DOWN_SEARCHING ) || ( status_ == DOWN_LOADING ) )     {
        if ( getActiveSources() > 0 )         {
            setTextColor( FILENAME_COL, Qt::darkBlue );
            setText( STATUS_COL, downloadStatusDescr.value( "-1", "unknown" ) );
            status_ = DOWN_LOADING;
        } else if ( getActiveSources() <= 0 ) {
            setTextColor( FILENAME_COL, Qt::black );
            setText( STATUS_COL, downloadStatusDescr.value( status_, "unknown" ) );
            status_ = DOWN_SEARCHING;
        }
    } else {
        setText( STATUS_COL, downloadStatusDescr.value( status_, "unknown" ) );
    }

    setText( SOURCES_COL, getSourcesString() );
    finished = ready / size_;
    percent = (int)(finished * 100.0);
    progressBar->setValue(percent);

    setText( FINISHED_SIZE_COL, " " + QConvert::bytes( ready ) + " " );
    setText( REMAIN_SIZE_COL, " " + QConvert::bytes( remainingSize ) + " " );


    speed = 0.0;
    if( status_ == DOWN_LOADING ) {
        QList<UserItem*> userItems = users.values();
        for(int i=0; i<userItems.size(); i++) {
            speed += userItems[i]->getSpeed();
        }
    }

    setText( SPEED_COL, " " + QConvert::bytes( speed, 1 ) + "/s ");

    if( speed > 0 ) {
        remainingSec = (long int)(remainingSize / speed);
        setText( REMAIN_TIME_COL, " " + QConvert::time( remainingSec ) + " " );
    } else {
        remainingSec = LONG_MAX;
        setText( REMAIN_TIME_COL, QString( " n.a. " ) );
    }

    bool returnValue = firstFinished;
    firstFinished = false;
    return returnValue;
}


void DownloadItem::showWidget( const QPoint & p ) 
{
    if ( partListDialog == NULL ) {
        partListDialog = new PartListDialog();
    }
    partListDialog->move( p );
    partListDialog->show();
}

void DownloadItem::deleteUsers() 
{
    QList<QString> userIds = users.keys();
    for (int i=0; i<userIds.size(); i++ ) {
        delete users[userIds[i]];
    }
    users.clear();
}


/*!
    \fn DownloadItem::getPartListDialog()
 */
PartListDialog* DownloadItem::getPartListDialog() 
{
    return partListDialog;
}


/*!
    \fn DownloadItem::setParts( Q_ULLONG size, QLinkedList<PartsWidget::Part>& partList )
 */
void DownloadItem::setParts( qulonglong size, QLinkedList<PartsWidget::Part>& partList ) 
{
    if ( partListDialog->isVisible() ) {
        partListDialog->update( size, partList );
    }
    PartsWidget::Part closePart;
    closePart.type = -10;
    closePart.fromPosition = size;
    partList.push_back( closePart );

    qulonglong bytesReady, bytesAvailable, bytesMissing;
    qulonglong partSize;
    bytesReady = bytesAvailable = bytesMissing = 0;

    PartsWidget::Part fromPart, toPart;
    QLinkedListIterator<PartsWidget::Part> it(partList);
    toPart = it.next();

    while ( it.hasNext() ) {
        fromPart = toPart;
        toPart = it.next();

        partSize = toPart.fromPosition - fromPart.fromPosition;

        switch ( fromPart.type ) {
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
    if ( missing < 1.0 && missing > 0.0 ) {
        setText( MISSING_COL, QString::number( missing, 'f', 1 ) + "%" );
    } else {
        setText( MISSING_COL, QString::number( missing, 'f', 0 ) + "%" );
    }
    if ( missing > 0.0 ) {
        setTextColor( MISSING_COL, Qt::darkRed );
    } else {
        setTextColor( MISSING_COL, Qt::darkGreen );
    }
}

bool DownloadItem::operator<( const QTreeWidgetItem & other ) const 
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    DownloadItem* downItem = (DownloadItem*)&other;
    switch ( sortIndex ) {
        case FILENAME_COL:
            return this->text( FILENAME_COL ) < other.text( FILENAME_COL );
        case SIZE_COL:
            return size_ < downItem->getSize();
        case FINISHED_SIZE_COL:
            if(ready == downItem->getReady()) {
                return this->text( FILENAME_COL ) < other.text( FILENAME_COL );
            } else {
                return ready < downItem->getReady();
            }
        case REMAIN_SIZE_COL:
            return remainingSize < downItem->getRemainingSize();
        case REMAIN_TIME_COL:
            return remainingSec < downItem->getRemainingSec();
        case SPEED_COL:
            return this->speed < downItem->getSpeed();
        case MISSING_COL:
            return this->missing < downItem->getMissing();
        case FINISHED_COL:
            if(this->finished == downItem->getFinished()) {
                return this->text( FILENAME_COL ) < other.text( FILENAME_COL );
            } else {
                return this->finished < downItem->getFinished();
            }
        default:
            return this->text( sortIndex ) < other.text( sortIndex );
    }
}

QString DownloadItem::getLinkAJFSP() 
{
    return "ajfsp://file|"
        + text(FILENAME_COL) + "|"
        + hash_ + "|"
        + QString::number( (int)size_ ) + "/";
}
