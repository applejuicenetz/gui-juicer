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
#ifndef QAJITEM_H
#define QAJITEM_H

#include <stdio.h>

#include <QTreeWidget>

typedef enum { DOWN, UP, USER, SEARCH, SHARE, SHARED_FILE, GENERIC } ItemType;

/**
@author Matthias Reif
*/
class QAjItem : public QTreeWidgetItem
{
public:
	QAjItem( ItemType type, QTreeWidget *parent = 0, const char *name = 0 );
	QAjItem( ItemType type, QTreeWidgetItem *parent = 0, const char *name = 0 );

	virtual ~QAjItem();
	
	int getStatus() { return status; }
	void setStatus( int newStatus ) { status = newStatus; }
	int getType();
	
//	virtual bool operator<( const QAjItem & other ){ return false; };

protected:
    ItemType type;
    int status;
};

#endif
