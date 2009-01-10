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
#ifndef QCONVERT_H
#define QCONVERT_H

#include <stdlib.h>

#include <QObject>
#include <QString>
#include <QFile>

/**
@author Matthias Reif
*/
class QConvert : public QObject
{
    Q_OBJECT
public:
    QConvert(QObject *parent = 0);

    ~QConvert();

    static QString num( long int num );
    static QString bytes( QString x );
    static QString bytes( long int x );
    static QString bytes( double x, int precision = 2 );
    static QString bytes( qulonglong x );
    static QString bytes( QString x, QString y );
    static QString bytesLong( QString x );
    static QString bytesExtra( QString x );
    static QString power( QString x );
    static float powerValue( QString x );
    static QString power( float power );
    static QString time( long int seconds );
    static QString getFileErrorString(QFile::FileError error);

    static const int ONE_DAY = 86400;
    static const int ONE_HOUR = 3600;
    static const int ONE_MINUTE = 60;

    static const int ONE_GIG = 1073741824;
    static const int ONE_MEG = 1048576;
    static const int ONE_KILO = 1024;
};

#endif
