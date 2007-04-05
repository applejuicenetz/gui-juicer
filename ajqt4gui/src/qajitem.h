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

/**
@author Matthias Reif
*/
class QAjItem : public QTreeWidgetItem
{
public:
	QAjItem( int type, QTreeWidget *parent = 0, const char *name = 0 );
	QAjItem( int type, QTreeWidgetItem *parent = 0, const char *name = 0 );

	virtual ~QAjItem();
	
	int getStatus() { return status; }
	void setStatus( int newStatus ) { status = newStatus; }
	int getType();
	
	static const int DOWN_TYPE = 0;
	static const int UP_TYPE = 1;
	static const int USER_TYPE = 2;
	static const int SEARCH_TYPE = 3;
	static const int SHARE_TYPE = 4;
	static const int SHARED_FILE_TYPE = 5;
	static const int GENERIC_TYPE = 6;
	
//	virtual bool operator<( const QAjItem & other ){ return false; };
	
protected:
	int type;
	int status;

};

#endif
