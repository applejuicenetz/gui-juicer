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
#ifndef QAJPARTSWIDGET_H
#define QAJPARTSWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QLinkedList>

#include "types.h"
/**
@author Matthias Reif
*/
class QAjPartsWidget : public QWidget
{
Q_OBJECT
public:
    QAjPartsWidget( QWidget *parent = 0 );
    
    ~QAjPartsWidget();
    void update( qulonglong size, QLinkedList<Part>& partList );
    
    double ready;
    double available;
    double lessSources;
    double missing;
    qulonglong size;

    static const int READY_COLOR = 0x00DC00;
    static const int MISSING_COLOR = 0xF00000;
    static const int AVAILABLE_COLOR = 0x0000DC;
    static const int RARE_COLOR =  0xDCDC00;

protected:
    void paintEvent( QPaintEvent* );
    QLinkedList<Part> partList;
    static const int BLOCK_ROWS = 6;
    int blockHeight;
    int numPixels;
    float pixelPerByte;
signals:
    void painted();
};

#endif
