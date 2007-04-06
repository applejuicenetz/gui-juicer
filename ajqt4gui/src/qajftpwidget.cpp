/***************************************************************************
 *   Copyright (C) 2005 by Matthias Reif   *
 *   matthias.reif@informatik.tu-chemnitz.de   *
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
#include "qajftpwidget.h"
#include "iostream.h"

QAjFtpWidget::QAjFtpWidget( QAjIcons *icons, QWidget *parent ) : QAjListWidget( icons, ID_FTP_INDEX, parent )
{
	QStringList headers;
	int i;
	for( i=0; i<NUM_FTP_COL; i++)
	{
		switch(i)
		{
			case ID_FTP_INDEX:
				headers.append( tr("id") );
				break;
			case FILENAME_FTP_INDEX:
				headers.append( tr("filename") );
				break;
			case SIZE_FTP_INDEX:
				headers.append( tr("size") );
				break;
		}
	}
	setHeaderLabels( headers );
	setColumnHidden( ID_FTP_INDEX, true );
}


QAjFtpWidget::~QAjFtpWidget()
{
}


/*!
    \fn QAjFtpWidget::insert( QUrlInfo info )
 */
void QAjFtpWidget::insert( QUrlInfo info )
{
	if( info.isFile() )
	{
		QAjItem *item = new QAjItem( GENERIC, this );
		item->setText( FILENAME_FTP_INDEX, info.name() );
		item->setText( SIZE_FTP_INDEX, QConvert::bytes( (double)info.size() ) );
		adjustSizeOfColumns();
	}
}
