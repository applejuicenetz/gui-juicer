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
#ifndef QAJSEARCHITEM_H
#define QAJSEARCHITEM_H

using namespace std;
#include <map>

#include "qajitem.h"
#include "types.h"

/**
@author Matthias Reif
*/
class QAjSearchItem : public QAjItem
{
public:
    QAjSearchItem( QTreeWidget* parent );
	 QAjSearchItem( QTreeWidgetItem* parent );

	~QAjSearchItem();
	
	int compare( QTreeWidgetItem * i, int col, bool ascending ) const;
	QAjSearchItem* find( int id );
	
	int hits;
	int entriesCount;
	QString size;

	map<int, QAjSearchItem*> results;

};

#endif