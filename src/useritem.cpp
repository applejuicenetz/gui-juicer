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
#include "useritem.h"

UserItem::UserItem( const QString& id, QTreeWidget *parent ) 
  : Item( parent, id )
{
    init();
}

UserItem::UserItem( const QString& id, QTreeWidgetItem *parent ) 
  : Item( parent, id )
{
    init();
}

UserItem::~UserItem()
{}

void UserItem::init()
{
    speed = 0;
    fileNameSet = false;
    status_ = NEW_SOURCE;
    setFlags( Qt::ItemIsEnabled );
}

void UserItem::setSpeed( const QString& newSpeedString, const QTime& /*time*/ )
{
    speed = newSpeedString.toInt();
    if ( status_ == ACTIVE_SOURCE )
        setText( DownloadItem::SPEED_COL, Convert::bytes(newSpeedString) + "/s" );
    else
        setText( DownloadItem::SPEED_COL, "" );
}

void UserItem::update( const QString& fileName,
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
    this->fileName = fileName;
    status_ = status;
    this->power = power;
    this->queuePos = queuePos.toInt();
    setSpeed( speed, time );

    if ( status_ == QUEUED_SOURCE ) { // queueing? print position
        setText( DownloadItem::STATUS_COL,  statusString + " (" + queuePos + ")" );
    } else {
        setText( DownloadItem::STATUS_COL,  statusString );
    }
    setText( DownloadItem::POWER_COL,  Convert::power( power ) );
    setText( DownloadItem::SOURCES_COL,  nickname );
    if ( !fileNameSet && !fileName.isEmpty() )
    {
        setText( DownloadItem::FILENAME_COL, fileName );
        fileNameSet = true;
    }
    if ( this->icon(DownloadItem::SOURCES_COL).isNull() )
    {
        setIcon( DownloadItem::SOURCES_COL, osIcon );
    }
    if(actualdownloadposition != "-1") {
        double dFrom = downloadfrom.toULongLong();
        double dTo = downloadto.toULongLong();
        double dPosition = actualdownloadposition.toULongLong();
        setText(DownloadItem::SIZE_COL, Convert::bytes(dTo - dFrom + 1.0, 2));
        setText(DownloadItem::REMAIN_SIZE_COL, Convert::bytes(dTo - dPosition + 1.0, 2));
        setText(DownloadItem::FINISHED_SIZE_COL, Convert::bytes(dPosition - dFrom, 2));
    } else {
        setText(DownloadItem::SIZE_COL, "");
        setText(DownloadItem::REMAIN_SIZE_COL, "");
        setText(DownloadItem::FINISHED_SIZE_COL, "");
    }
}

