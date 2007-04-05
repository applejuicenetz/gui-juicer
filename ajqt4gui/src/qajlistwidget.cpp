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
#include "qajlistwidget.h"


QAjListWidget::QAjListWidget( QAjIcons *icons, int idIndex, QWidget *parent, const char *name ) : QTreeWidget( parent )
{
	this->icons = icons;
	this->idIndex = idIndex;
	changed = false;
	sizeAdjusted = false;
	setContextMenuPolicy( Qt::CustomContextMenu );

	popup = new QMenu( this );
	
	setSelectionBehavior( QAbstractItemView::SelectRows );
	setSelectionMode( QAbstractItemView::ExtendedSelection );
	setRootIsDecorated( true );
	setSortingEnabled( true );
	header()->setStretchLastSection( false );
	
	connect( this, SIGNAL( itemSelectionChanged() ) , this, SLOT( selectionChanged1() ) );
	connect( this, SIGNAL( customContextMenuRequested( const QPoint & ) ), this, SLOT( contextMenuRequested( const QPoint & ) ) );
}


QAjListWidget::~QAjListWidget()
{
	delete popup;
}

void QAjListWidget::selectionChanged1()
{
	newSelection( isOneSelected() );
}

void QAjListWidget::contextMenuRequested( const QPoint &pos )
{
	QAjItem* item = (QAjItem*)this->itemAt( pos );
	popupItem = item;
	popup->exec( QCursor::pos() );
}

bool QAjListWidget::isOneSelected()
{
	return  !selectedItems().empty();
}

/*!
    \fn QAjListWidget::adjustSizeOfColumns()
 */
void QAjListWidget::adjustSizeOfColumns()
{
	int i;
	for( i=0; i<this->columnCount(); i++ )
	{
		resizeColumnToContents( i );
	}
	sizeAdjusted = true;
}


/*!
    \fn QAjListWidget::wasSizeAdjusted()
 */
bool QAjListWidget::wasSizeAdjusted()
{
	return sizeAdjusted;
}
