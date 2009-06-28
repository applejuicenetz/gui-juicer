/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#include "partswidget.h"

PartsWidget::PartsWidget( QWidget *parent ) : QWidget( parent )
{

}


PartsWidget::~PartsWidget()
{
}


/*!
    \fn PartsWidget::paintEvent( QPaintEvent* )
 */
void PartsWidget::paintEvent( QPaintEvent* )
{
    if ( partList.empty() ) {
        return;
    }
    int BLOCK_ROWS = Convert::max(1,height() / 30);
    double size = partList.getSize();

    blockHeight = this->height() / BLOCK_ROWS;
    numPixels = this->width() * BLOCK_ROWS;
    pixelPerByte = numPixels / size;
    
    QListIterator<Part> it ( partList );
    
    QPainter painter ( this );
    
    int fromPixel, toPixel;
    Part fromPart, toPart;
    
    QColor color;
    
    int fromRow;
    int toRow;
    
    qulonglong bytesReady, bytesAvailable, bytesMissing, bytesLessSources;
    qulonglong partSize;
    bytesReady = bytesAvailable = bytesMissing = bytesLessSources = 0;

    toPart = it.next();
    while ( it.hasNext() )
    {
        fromPart = toPart;
        toPart = it.next();

        fromPixel = ( int ) ( fromPart.fromPosition * pixelPerByte );
        toPixel = ( int ) ( (toPart.fromPosition) * pixelPerByte );
        partSize = (toPart.fromPosition) - fromPart.fromPosition;
    
        if ( fromPart.fromPosition != toPart.fromPosition )
        {
            switch ( fromPart.type )
            {
                case -1:
                    color.setRgb(READY_COLOR);
                    bytesReady += partSize;
                    break;
                case 0:
                    color.setRgb(MISSING_COLOR);
                    bytesMissing += partSize;
                    break;
                default:
                    if ( fromPart.type < 5 )
                    {
                        color.setRgb ( 255 - fromPart.type * 20, 255 - fromPart.type * 20, fromPart.type * 20 );
                        bytesLessSources += partSize;
                    }
                    else
                    {
                        int blue = 100 + fromPart.type * 10;
                        if ( blue > 255 ) {
                            blue = 255;
                        }
                        color.setRgb ( 0, 0, blue );
                    }
                    bytesAvailable += partSize;
                    break;
            }
    
            fromRow = fromPixel / width();
            fromPixel = fromPixel % width();
            toRow = toPixel / width();
            toPixel = toPixel % width();
            
            while ( fromRow < toRow )
            {
                // -- fill until the end of the col --
                painter.fillRect ( fromPixel, fromRow*blockHeight, width() - fromPixel, blockHeight, QBrush ( color ) );
                fromPixel = 0;
                fromRow++;
            }
            painter.fillRect ( fromPixel, fromRow*blockHeight, toPixel - fromPixel, blockHeight, QBrush ( color ) );
        }
    }
    painter.end();
    ready = bytesReady / size * 100.0;
    available = bytesAvailable / size * 100.0;
    lessSources = bytesLessSources / size * 100.0;
    missing = bytesMissing / size * 100.0;
    painted();
}


/*!
    \fn PartsWidget::update(PartList& partList)
 */
void PartsWidget::update(PartList& partList)
{
    this->partList = partList;
    PartsWidget::Part closePart;
    closePart.fromPosition = partList.getSize();
    closePart.type = -10;
    this->partList.push_back(closePart);
    QWidget::update();
}


/*!
    \fn PartsWidget::clear()
 */
void PartsWidget::clear() {
    partList.clear();
    QWidget::update();
}
