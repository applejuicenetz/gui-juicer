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
#ifndef QAJDOWNLOADWIDGET_H
#define QAJDOWNLOADWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QPixmap>
#include <QPainter>
#include <QMenu>
#include <QHash>
#include <QList>
#include <QIcon>

#include "qajlistwidget.h"
#include "qajdownloaditem.h"
#include "qajuseritem.h"
#include "qconvert.h"

#include "types.h"

/**
@author Matthias Reif
*/

class QAjDownloadItem;

typedef struct {
	QAjDownloadItem* download;
	QAjUserItem* user;
} DownloadUser;

class QAjDownloadWidget : public QAjListWidget
{
Q_OBJECT
public:
	QAjDownloadWidget( QWidget *parent = 0, const char *name = 0);

	~QAjDownloadWidget();
	
	int insertDownload(QString id, QString fileName, QString status, QString size, QString ready, QString power, QString tempNumber);
	int insertUser(QString downloadId, QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString os);
	bool remove( QString id );
	
	QAjDownloadItem* findDownload( QString id );
	QAjDownloadItem* removeDownload( QString id );
	DownloadUser findParent( QString id );
	
    QString getNextIdRoundRobin();
    QList<QString> getIds() { return downloads.keys(); }
    QAjDownloadItem* findDownloadByTempNum( QString tempNum );

private:

	QHash<QString, QAjDownloadItem*> downloads;
   int currIdRoundRobin;

	QAction* pauseId;
	QAction* resumeId;
	QAction* cancelId;
	QAction* partListId;
	QAction* renameId;
   QAction* renamePlusId;

	QIcon *linuxIcon, *windowsIcon, *otherOsIcon;

    QHash<QString, QString> userStatusDescr;
    QHash<QString, QString> downloadStatusDescr;

	
private slots:
	void selectionChanged1( bool oneSelected );
	void pauseSlot();
	void resumeSlot();
	void cancelSlot();
	void cleanSlot();
	void partListSlot();
	void renameSlot();
   void renamePlusSlot();

public slots:
	void updateView();

signals:
	void cancel( );
	void clean( );
	void pause( );
	void resume( );
	void partListRequest( );
	void rename( );
    void renamePlus( );
};

#endif
