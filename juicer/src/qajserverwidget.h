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
#ifndef QAJSERVERWIDGET_H
#define QAJSERVERWIDGET_H

#include <QHash>

#include <time.h>

#include "qajlistwidget.h"

/**
@author Matthias Reif
*/
class QAjServerWidget : public QAjListWidget
{
Q_OBJECT
public:
	QAjServerWidget( QWidget *parent = 0, const char *name = 0);

	~QAjServerWidget();
	
	void insertServer( QString id, QString name, QString host, QString port, QString lastseen, QString tests );
	
	void connectedWith( QString id );
   void connectingTo( QString id );
	QTreeWidgetItem* findServer( QString id );
	bool remove( QString id );

private:
	QString connectedWithId, connectingToId;
	QAction* removeId;
	QAction* connectId;
	QHash<QString, QTreeWidgetItem*> servers;

public slots:
	void selectionChanged1( bool isOneSelected );
	void removeSlot();
	void connectSlot();
	void findSlot();
signals:
	void remove();
	void connect();
	void find();
	void connected( QString server );
};

#endif
