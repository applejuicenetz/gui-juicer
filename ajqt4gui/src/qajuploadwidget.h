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
#ifndef QAJUPLOADWIDGET_H
#define QAJUPLOADWIDGET_H

using namespace std;
#include <map>

#include "qajlistwidget.h"
#include "qajuploaditem.h"
#include "qconvert.h"
#include "types.h"

#define NUM_UPLOAD_STATES 8

/**
@author Matthias Reif
*/
class QAjUploadWidget : public QAjListWidget
{
Q_OBJECT
public:
	QAjUploadWidget( QAjIcons *icons, QWidget *parent = 0, const char *name = 0 );

	~QAjUploadWidget();

	bool insertUpload( QString id, QString shareId, QString version, QString os, QString status, QString directState, QString priority, QString nick, QString speed );
	
	bool setFileName( QString shareId, QString fileName );
	
	bool remove( QString id );
	
	QTreeWidgetItem *activeUpload;
	QTreeWidgetItem *queuedUpload;
	QTreeWidgetItem *otherUpload;
private:
	QAjUploadItem* findUpload( QString id );
	QAjUploadItem* removeUpload( QString id );
	bool isOtherUpload( int status );
	QString uploadStatusDescr[NUM_UPLOAD_STATES];
	map<unsigned long, QAjUploadItem*> uploads;
	map<unsigned long, QAjUploadItem*>::iterator uploadsIt;
	
};

#endif
