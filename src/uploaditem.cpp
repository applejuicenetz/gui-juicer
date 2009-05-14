/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#include "uploaditem.h"
#include "juicer.h"

UploadItem::UploadItem( const QString& id, const QString& shareId, QTreeWidget *parent )
  : Item( parent, id )
  , shareId( shareId )
{
    status_ = NEW_UPLOAD;
    speed = 0.0;

    progressChunk_.setMaximumHeight(18);
    initProgressBar( progressChunk_, CHUNK_COL );
    progressLoaded_.setMaximumHeight(18);
    initProgressBar( progressLoaded_, LOADED_COL );
}


UploadItem::~UploadItem() {
}


bool UploadItem::operator<( const QTreeWidgetItem & other ) const
{
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    const UploadItem* upItem = dynamic_cast<const UploadItem*>(&other);
    if ( upItem ) {
        switch ( sortIndex ) {
            case SPEED_COL :
                return speed < upItem->speed;
            case CHUNK_COL :
                return ( progressPercentualValue( PRGB_CHUNKS ) < upItem->progressPercentualValue( PRGB_CHUNKS ) );
            case LOADED_COL :
                return ( progressPercentualValue( PRGB_LOADED ) < upItem->progressPercentualValue( PRGB_LOADED ) );
            default:
                return this->text(sortIndex) < other.text(sortIndex);
        }
    }
    return false;
}

/*!
    \fn UploadItem::update(const QIcon& osIcon,const QString& status,
        const QString& statusDescr, const QString& directState,
        const QString& priority, const QString& nick, const QString& speed,
        const QString& loaded, const QString& chunkStart,
        const QString& chunkEnd, const QString& chunkPos, bool newUpload)
 */
bool UploadItem::update(const QIcon& osIcon,const QString& status,
        const QString& statusDescr, const QString& directState,
        const QString& priority, const QString& nick, const QString& speed,
        const QString& version, const QString& loaded, const QString& chunkStart,
        const QString& chunkEnd, const QString& chunkPos, const QString& lastConnected, bool newUpload)
{
    this->speed = speed.toDouble();
    bool newStatus = setStatus( status );
    if( newUpload ) {
        setText(NICK_COL, nick);
        setIcon(OS_COL, osIcon);
        setText(VERSION_COL, version);
    }
    setText(SPEED_COL, Convert::bytes(speed) + "/s" );
    setText(STATUS_COL, statusDescr);
    setText(DIRECTSTATE_COL, directState);
    setText(PRIORITY_COL, priority );

    updateLoadedProgress( loaded );
    updateChunkProgress( chunkStart.toInt(), chunkEnd.toInt(), chunkPos.toInt() );
    updateLastSeen( lastConnected );
    return newStatus;
}

void UploadItem::initProgressBar( QProgressBar& progressBar, column col )
{
    QWidget* progressBarWidget = new QWidget( treeWidget() );
    progressBarWidget->setContentsMargins(5, 0, 5, 0);
    QVBoxLayout* l = new QVBoxLayout();
    l->setSpacing(0);
    l->setMargin(1);
    l->addWidget(&progressBar);
    progressBarWidget->setLayout( l );

    treeWidget()->setItemWidget( this, col, progressBarWidget );
}

void UploadItem::updateChunkProgress( int uploadFrom, int uploadTo, int uploadCurrent )
{
    if ( getStatus() != ACTIVE_UPLOAD ) {
        progressChunk_.hide();
        return;
    }
    if ( false == progressChunk_.isVisible() && !isHidden()) {
        progressChunk_.setVisible( true );
    }
    if ( uploadFrom  != progressChunk_.minimum() ) {
        progressChunk_.setMinimum( uploadFrom );
    }
    if ( uploadTo    != progressChunk_.maximum() ) {
        progressChunk_.setMaximum( uploadTo );
    }
    progressChunk_.setValue( uploadCurrent );
}

void UploadItem::updateLoadedProgress( const QString& loaded )
{
    if ( loaded.toInt() == -1 ) {
        progressLoaded_.hide();
        return;
    }
    if ( false == progressLoaded_.isVisible() && !isHidden()) {
        progressLoaded_.setVisible( true );
    }
    progressLoaded_.setValue( loaded.toDouble() * 100 );
}

void UploadItem::updateLastSeen( const QString& lastConnected )
{
    if ( getStatus() != ACTIVE_UPLOAD ) {
        if ( false == lastConnected.isEmpty() ) {
            // lastConnected in milli seconds, fromTime_t needs seconds since 1.1.1970
            unsigned int seconds = lastConnected.toULongLong() / 1000;
            setText( LASTSEEN_COL, QDateTime::fromTime_t( seconds ).toString("dd.MM.yyyy, hh:mm") );
        }
        else setText( LASTSEEN_COL, tr("unknown") );
    }
    else setText( LASTSEEN_COL, tr("current") );
}

int UploadItem::progressPercentualValue( progressBars p ) const
{
    int s, e, v;
    s = e = v = 0;
    switch ( p ) {
    case PRGB_CHUNKS :
        if ( getStatus() != ACTIVE_UPLOAD ) return 0;
        s = progressChunk_.minimum();
        e = progressChunk_.maximum();
        v = progressChunk_.value();
        break;
    case PRGB_LOADED :
        s = progressLoaded_.minimum();
        e = progressLoaded_.maximum();
        v = progressLoaded_.value();
        break;
    }
    if ( e == s ) return 0;
    if ( v < s )  return 0;
    return ( ((v-s) *100)/(e-s) );
}
