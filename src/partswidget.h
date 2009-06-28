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

#ifndef QAJPARTSWIDGET_H
#define QAJPARTSWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QLinkedList>
#include <QDebug>

#include "convert.h"

/**
@author Matthias Reif
*/
class PartsWidget : public QWidget
{
Q_OBJECT
public:
    PartsWidget( QWidget *parent = 0 );
    
    ~PartsWidget();

    class Part {
        public:
        Part() {}
        Part(const QString& from, const QString& type) {
            this->type = type.toInt();
            this->fromPosition = from.toULongLong();
        }
        qulonglong fromPosition;
        int type;
    };

    class PartList : public QList<Part> {
        qulonglong bytesMissing, size;
        double missing;
        public:
        PartList() {
            clear();
        }
        void clear() {
            size = 1;    // -- avoid division by zero, but should not happen --
            bytesMissing = 0;
            missing = 0.0;
            QList<Part>::clear();
        }
        void setSize(qulonglong size) {
            this->size = size;
        }
        qulonglong getSize() {
            return this->size;
        }
        void close() {
            if(!empty() && last().type == 0) {
                bytesMissing += size - last().fromPosition;
            }
            missing = (double)bytesMissing / (double)size * 100.0;
        }
        double getMissing() {
            return missing;
        }
        void push_back(const Part& part) {
            if(!empty() && last().type == 0) {
                bytesMissing += part.fromPosition - last().fromPosition;
            }
            QList<Part>::push_back(part);
        }
    };

    void update(PartList& partList );
    void clear();
    
    double ready;
    double available;
    double lessSources;
    double missing;

    static const int READY_COLOR = 0x00DC00;
    static const int MISSING_COLOR = 0xF00000;
    static const int AVAILABLE_COLOR = 0x0000DC;
    static const int RARE_COLOR =  0xDCDC00;
    PartList partList;

protected:
    void paintEvent( QPaintEvent* );
    int blockHeight;
    int numPixels;
    float pixelPerByte;
signals:
    void painted();
};

#endif
