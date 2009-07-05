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

#ifndef QCONVERT_H
#define QCONVERT_H

#include <stdlib.h>

#include <QObject>
#include <QString>
#include <QFile>
#include <QPixmap>

/**
@author Matthias Reif
*/
class Convert : public QObject
{
    Q_OBJECT
public:
    Convert(QObject *parent = 0);

    ~Convert();

    static QString num(long int num);
    static QString bytes(const QString& x);
    static QString bytes(long int x);
    static QString bytes(double x, int precision = 2);
    static QString bytes(qulonglong x);
    static QString bytes(const QString& x, const QString& y);
    static QString bytes(qulonglong x, int precision);
    static QString bytesLong(const QString& x);
    static QString bytesExtra(const QString& x);
    static QString power(const QString& x);
    static float powerValue(const QString& x);
    static QString power(float power);
    static QString time(long int seconds);
    static QString getFileErrorString(QFile::FileError error);

    static const long ONE_DAY = 86400;
    static const long ONE_HOUR = 3600;
    static const long ONE_MINUTE = 60;

    static const long ONE_GIG = 1073741824;
    static const long ONE_MEG = 1048576;
    static const long ONE_KILO = 1024;

    template<class X1> static const X1& min(const X1& a, const X1& b) {
        if(a < b) {
            return a;
        }
        return b;
    }
    template<class X>
    static const X& max(const X& a, const X& b) {
        if(a > b) {
            return a;
        }
        return b;
    }

    static QPixmap getFileIcon(const QString &path);
    static int compareVersion(const QString& v1, const QString& v2);
    #ifdef Q_WS_WIN
    static QPixmap convertHIconToPixmap(const HICON icon);
    static QImage qt_fromWinHBITMAP(HDC hdc, HBITMAP bitmap, int w, int h);
    #endif

};

#endif
