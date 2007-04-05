/***************************************************************************
 *   Copyright (C) 2005 by Matthias Reif                                   *
 *   matthas.reif@informatik.tu-emnitz.de                                  *
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
#include "qajpartswidget.h"

QAjPartsWidget::QAjPartsWidget( QWidget *parent, const char *name ) : QWidget( parent )
{
	setFixedSize( 560, 200 );
	move( 20, 20 );
	show();
	partList = NULL;
	blockHeight = this->height() / BLOCK_ROWS;
	numPixels = this->width() * BLOCK_ROWS;
}


QAjPartsWidget::~QAjPartsWidget()
{
}




/*!
    \fn QAjPartsWidget::paintEvent( QPaintEvent* )
 */
void QAjPartsWidget::paintEvent( QPaintEvent* )
{
	if( partList == NULL )
		return;
	
	list<QAjPart*>::iterator from = partList->begin();
	list<QAjPart*>::iterator to = ++partList->begin();
	
	QPainter painter( this );
	
	int fromPixel, toPixel;
	QColor color;
	
	int fromRow;
	int toRow;
	
	qulonglong bytesReady, bytesAvailable, bytesMissing;
	qulonglong partSize;
	bytesReady = bytesAvailable = bytesMissing = 0;
	
	while( to != partList->end() )
	{
		fromPixel = (*from)->getFromPosition() * pixelPerByte;
		toPixel = ( (*to)->getFromPosition() - 1 ) * pixelPerByte;
		partSize = (*to)->getFromPosition() - (*from)->getFromPosition();

		if( (*from)->getFromPosition() != (*to)->getFromPosition() )
		{
		
//		printf("(%d, %d)\t%d\n", fromPixel, toPixel, (*from)->getType() );

			switch( (*from)->getType() )
			{
				case -1:
					color.setRgb( 0, 220, 0 );
					bytesReady += partSize;
					break;
				case 0:
					color.setRgb( 240, 0, 0 );
					bytesMissing += partSize;
					break;
				default:
					color.setRgb( 0, 0, 220/*255 - (*from)->getType() * 5*/ );
					bytesAvailable += partSize;
					break;
			}
			
			fromRow = fromPixel / width();
			fromPixel = fromPixel % width();
			toRow = toPixel / width();
			toPixel = toPixel % width();
			
	/*		printf("fromrow: %d   torow: %d\n", fromRow, toRow );
			printf("frompix: %d   topix: %d\n", fromPixel, toPixel );
			printf("type: %d\n==============\n", (*from)->getType() );
		*/	
			while( fromRow < toRow )
			{
				// fill until the end of the col
				painter.fillRect( fromPixel, fromRow*blockHeight, width() - fromPixel, blockHeight, QBrush( color ) );
				fromPixel = 0;
				fromRow++;
			}
			painter.fillRect( fromPixel, fromRow*blockHeight, toPixel - fromPixel, blockHeight, QBrush( color ) );
		}
		
		from = to;
		to++;
		
	}
	
	painter.end();
	
	ready = (double)bytesReady / (double)size * 100.0;
	available = (double)bytesAvailable / (double)size * 100.0;
	missing = (double)bytesMissing / (double)size * 100.0;
}


/*!
    \fn QAjPartsWidget::update( qulonglong size, list<QAjPart*> *partList )
 */
void QAjPartsWidget::update( qulonglong size, list<QAjPart*> *partList )
{
	this->size = size;
	this->partList = partList;
	pixelPerByte = (float)numPixels / (float)size;
}
