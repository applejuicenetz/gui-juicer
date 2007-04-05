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
#include "qajtab.h"

QAjTab::QAjTab( QString* filesystemSeparator, QAjIcons *icons, bool special, QWidget *parent, const char *name) : QTabWidget( parent )
{
	this->special = special;
	ajDownloadWidget = new QAjDownloadWidget( icons, NULL );
	ajUploadWidget = new QAjUploadWidget( icons, NULL );
	ajSearchWidget = new QAjSearchWidget( icons, NULL );
	ajServerWidget = new QAjServerWidget( icons, NULL );
	ajShareWidget = new QAjShareWidget( filesystemSeparator, icons, NULL );
	
	setTabToolTip( addTab( ajDownloadWidget, *icons->downloadSmallIcon, "Downloads" ), "dowloads" );
	setTabToolTip( addTab( ajUploadWidget, *icons->uploadSmallIcon, "Uploads" ), "uploads" );
	setTabToolTip( addTab( ajSearchWidget, *icons->serverConnectSmallIcon, "Search" ), "servers" );
	setTabToolTip( addTab( ajServerWidget, *icons->downloadSearchingIcon, "Server" ), "searches" );
	setTabToolTip( addTab( ajShareWidget, *icons->shareSmallIcon, "Shares" ), "shares" );

	if( special )
	{
		ajFtpWidget = new QAjFtpWidget( icons, NULL );
		setTabToolTip( addTab( ajFtpWidget, *icons->ftpSmallIcon, "Ftp" ), "ftp" );
	}

}

QAjTab::~QAjTab()
{
	delete ajDownloadWidget;
	delete ajUploadWidget;
	delete ajSearchWidget;
	delete ajServerWidget;
	delete ajShareWidget;
}

bool QAjTab::oneSelected( QTreeWidget* list )
{
	return ! list->selectedItems().empty();
}
