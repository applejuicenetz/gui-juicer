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
#include "qajsharewidget.h"

#include "./xpm/cancel_small.xpm"
#include "./xpm/reload_small.xpm"
#include "./xpm/insert_small.xpm"
#include "./xpm/ok.xpm"
#include "./xpm/cancel.xpm"
#include "./xpm/shared_files.xpm"

QAjShareWidget::QAjShareWidget( QString filesystemSeparator, QWidget *parent, const char *name) : QAjListWidget( ID_SHARE_INDEX, parent, name )
{
	this->filesystemSeparator = filesystemSeparator;
	setColumnCount( NUM_SHARE_COL );
	QStringList headers;
	int i;
	for( i=0; i<NUM_SHARE_COL; i++)
	{
		switch(i)
		{
			case ID_SHARE_INDEX:
				headers.append( tr("id") );
				break;
			case PATH_SHARE_INDEX:
				headers.append( tr("path") );
				break;
			case MODE_SHARE_INDEX:
				headers.append( tr("recursive") );
				break;
		}
	}
	setHeaderLabels( headers );

	//setColumnWidthMode(ID_SHARE_INDEX, Q3ListView::Manual );
	setColumnHidden( ID_SHARE_INDEX, true );
	//setColumnAlignment( MODE_SHARE_INDEX, Qt::AlignRight );

	removeId = popup->addAction( QIcon(QPixmap(cancel_small_xpm)), "remove", this, SLOT(removeSlot()) );
	popup->addSeparator();
	popup->addAction( QIcon(QPixmap(insert_small_xpm)), "insert new", this, SLOT(insertSlot()) );
	popup->addAction( QIcon(QPixmap(reload_small_xpm)), "reload view", this, SLOT(reloadSlot()) );
	removeId->setEnabled( false );
	
	connect( this, SIGNAL( newSelection( bool ) ) , this, SLOT( newSelection( bool ) ) );
}


QAjShareWidget::~QAjShareWidget()
{
}

void QAjShareWidget::insertShare( QString path, QString shareMode )
{
	if( !path.endsWith( filesystemSeparator ) )
		path += filesystemSeparator;
	QAjShareItem *item = new QAjShareItem( this );
	item->path = path;
	item->setText( PATH_SHARE_INDEX, path );
	item->setIcon( PATH_SHARE_INDEX, QIcon(QPixmap(shared_files_xpm)) );
	if( shareMode == "subdirectory" )
	{
		item->recursiv = "true";
		item->setIcon( MODE_SHARE_INDEX, QIcon(QPixmap(ok_xpm)) );
	}
	else
	{
		item->recursiv = "false";
		item->setIcon( MODE_SHARE_INDEX, QIcon(QPixmap(cancel_xpm)) );
	}
}

void QAjShareWidget::insertSlot()
{
	insert();
}
void QAjShareWidget::removeSlot()
{
	remove();
}
void QAjShareWidget::reloadSlot()
{
	reload();
}

void QAjShareWidget::newSelection( bool oneSelected )
{
	removeId->setEnabled( oneSelected );
}

void QAjShareWidget::insertDirList( QTreeWidgetItem* parent, QStringList* dirList )
{
	if( dirList->size() <= 0 )
		return;
	if( dirList->size() == 1 )
	{
		QAjItem* newItem = new QAjItem( SHARED_FILE, parent );
		newItem->setFlags( Qt::ItemIsEnabled );
		parent->addChild( newItem );
		newItem->setText( PATH_SHARE_INDEX, dirList->front() );
	}
	else
	{
		QTreeWidgetItem* currChild = parent->child( 0 );
		int i = 1;
		while( ( currChild != NULL ) && ( currChild->text( PATH_SHARE_INDEX ) != dirList->front() ) )
		{
			currChild = parent->child( i++ );
		}
		// nicht gefunden
		if( currChild == NULL )
		{
			QAjItem* newItem = new QAjItem( SHARED_FILE, parent );
			newItem->setFlags( Qt::ItemIsEnabled );
			parent->addChild( newItem );
			newItem->setText( PATH_SHARE_INDEX, dirList->front() );
			newItem->setIcon( PATH_SHARE_INDEX, QIcon(QPixmap(shared_files_xpm)) );
			dirList->pop_front();
			insertDirList( newItem, dirList );
		}
		else
		{
			dirList->pop_front();
			insertDirList( currChild, dirList );
		}
	}
}

