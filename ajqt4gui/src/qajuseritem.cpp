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
#include "qajuseritem.h"

QAjUserItem::QAjUserItem( QAjDescription *description, QAjIcons *icons, QTreeWidget *parent ) : QAjItem(USER_TYPE, parent)
{
	this->description = description;
	this->icons = icons;
	init();
}

QAjUserItem::QAjUserItem( QAjDescription *description, QAjIcons *icons, QTreeWidgetItem *parent ) : QAjItem(USER_TYPE, parent)
{
	this->description = description;
	this->icons = icons;
	init();
}

QAjUserItem::~QAjUserItem()
{
}

void QAjUserItem::init()
{
	os = -1;
	speed = 0;
	fileNameSet = false;
	status = NEW_SOURCE;
	setFlags( Qt::ItemIsEnabled );
}

void QAjUserItem::setSpeed( QString newSpeedString )
{
	newSpeed = newSpeedString.toInt();
	speedDif = newSpeed - speed;
	speed = newSpeed;
	if( status == ACTIVE_SOURCE )
		setText( SPEED_DOWN_INDEX, QConvert::bytes(newSpeedString) + "/s" );
	else
		setText( SPEED_DOWN_INDEX, "" );
}

void QAjUserItem::update( QString fileName, QString speed, QString status, QString power, QString queuePos, QString os )
{
	this->fileName = fileName;
	this->status = status.toInt();
	this->power = power;
	this->queuePos = queuePos.toInt();
	setSpeed( speed );
	
	QString statusString = description->getUserStatusDescription( this->status );
	if( this->status == 5 )  // queueing? print position
		statusString += " (" + queuePos + ")";
	setText( STATUS_DOWN_INDEX,  statusString );
	setText( POWER_DOWN_INDEX,  QConvert::power( power ) );
	if( !fileNameSet && !fileName.isEmpty() )
	{
		setText( FILENAME_DOWN_INDEX, fileName );
		fileNameSet = true;
	}
	if( this->os == -1 )
	{
		this->os = os.toInt();
		if( this->os == WINDOWS_INT )
			setIcon( SOURCES_DOWN_INDEX, *icons->windowsIcon );
		else if( this->os == LINUX_INT )
			setIcon( SOURCES_DOWN_INDEX, *icons->linuxIcon );
	}
}
