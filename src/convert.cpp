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
#include "convert.h"

Convert::Convert(QObject *parent) : QObject( parent )
{}

Convert::~Convert()
{}

QString Convert::bytes( const QString& x )
{
    return bytes( x.toDouble() );
}

QString Convert::bytes( long int x )
{
    if ( x >= ONE_GIG )
        return QString::number( x / ( ONE_GIG ) ) + " Gb";
    else if ( x >= ONE_MEG )
        return QString::number( x / ( ONE_MEG ) ) + " Mb";
    else if ( x >= ONE_KILO )
        return QString::number( x / ( ONE_KILO ) ) + " kb";
    else
        return QString::number(x) + " b";
}

QString Convert::bytes( double x, int precision )
{
    double absX = x>0.0?x:-x;

    if ( absX >= ONE_GIG )
        return QString::number( x / ( ONE_GIG ), 'f', precision ) + " Gb";
    else if ( absX >= ONE_MEG )
        return QString::number( x / ( ONE_MEG ), 'f', precision ) + " Mb";
    else if ( absX >= ONE_KILO )
        return QString::number( x / ( ONE_KILO ), 'f', precision ) + " kb";
    else
        return QString::number(x) + " b";
}

QString Convert::bytes( qulonglong x )
{
    if ( x >= ONE_GIG )
        return QString::number( (x / ( ONE_GIG )) ) + " Gb";
    else if ( x >= ONE_MEG )
        return QString::number( (x / ( ONE_MEG )) ) + " Mb";
    else if ( x >= ONE_KILO )
        return QString::number( (x / ( ONE_KILO )) ) + " kb";
    else
        return QString::number(x) + " b";
}

QString Convert::bytes( qulonglong x, int precision )
{
    if ( x >= ONE_GIG )
        return QString::number( (x / (float)( ONE_GIG )), 'f', precision ) + " Gb";
    else if ( x >= ONE_MEG )
        return QString::number( (x / (float)( ONE_MEG )), 'f', precision ) + " Mb";
    else if ( x >= ONE_KILO )
        return QString::number( (x / (float)( ONE_KILO )), 'f', precision ) + " kb";
    else
        return QString::number(x) + " b";
}


QString Convert::bytes( const QString& x, const QString& y )
{
    double x1 = x.toDouble() - y.toDouble();
    return Convert::bytes( x1, 2 );
}

QString Convert::bytesLong( const QString& x )
{
    // x in Mbyte
    double x1 = x.section(',',0,0).toDouble();
    if ( x1 >= ONE_MEG )
        return QString::number( x1 / ( ONE_MEG ), 'f', 2 ) + " Terrabyte";
    else if ( x1 >= ONE_KILO )
        return QString::number( x1 / ( ONE_KILO ), 'f', 2 ) + " Gigabyte";
    else
        return x + " Mbyte";
}

QString Convert::bytesExtra( const QString& x )
{
    bool negative = (x[0] == '-');
    qlonglong x1 = qAbs(x.toLongLong());
    int base = 1;
    QString unit;
    if ( x1 >= ONE_GIG )
    {
        base = ONE_GIG;
        unit = " Gb";
    }
    else if ( x1 >= ONE_MEG )
    {
        base = ONE_MEG;
        unit = " Mb";
    }
    else if ( x1 >= ONE_KILO )
    {
        base = ONE_KILO;
        unit = " kb";
    }
    else
    {
        base = 1;
        unit = " b";
    }

    double r = (double)( x1 / ( base ) );
    r += (double)( ( x1 % ( base ) ) ) / base ;

    if ( negative )
        r *= -1;
    return QString::number( r , 'f', 2 ) + unit;
}

QString Convert::num( long int num )
{
    if ( num < 10 )
        return QString("0" + QString::number( num ) );
    else
        return QString::number( num );
}

float Convert::powerValue( const QString& x )
{
    float x1 = x.toFloat();
    return (x1 + 10) / 10.0;
}

QString Convert::power( const QString& x )
{
    float x1 = x.toFloat();
    x1 = (x1 + 10) / 10.0;
    return "1:"+QString::number(x1, 'f', 1);
}

QString Convert::power( float power )
{
    return QString::number( power*10 - 10,'f',0 );
}

QString Convert::time( long int seconds )
{
    int days, hours, minutes;
    days = hours = minutes = 0;
    if ( seconds >= ONE_DAY )
    {
        days = seconds / ONE_DAY;
        seconds %= ONE_DAY;
    }
    if ( seconds >= ONE_HOUR )
    {
        hours = seconds / ONE_HOUR;
        seconds %= ONE_HOUR;
    }
    if ( seconds >= ONE_MINUTE )
    {
        minutes = seconds / ONE_MINUTE;
        seconds %= ONE_MINUTE;
    }
    QString time ;
    if ( days == 1 )
        time = "> 1 day";
    else if ( days > 1 )
        time = "> " + QString::number( days ) + " days";
    else
        time = num( hours ) + ":" + num( minutes ) + ":" + num( seconds );
    return time;
}


/*!
    \fn Convert::getFileErrorString(QFile::FileError error)
 */
QString Convert::getFileErrorString(QFile::FileError error)
{
    switch ( error ) {
        case QFile::ReadError: return tr("An error occurred when reading from the file.");
        break;
        case QFile::WriteError: return tr("An error occurred when writing to the file.");
        break;
        case QFile::FatalError: return tr("A fatal error occurred.");
        break;
        case QFile::ResourceError: return tr("Not enough disk space.");
        break;
        case QFile::OpenError: return tr("The file could not be opened.");
        break;
        case QFile::AbortError: return tr("The operation was aborted.");
        break;
        case QFile::TimeOutError: return tr("A timeout occurred.");
        break;
        case QFile::PermissionsError: return tr("The file could not be accessed.");
        break;
        default: return tr("An unspecified error occurred.");
    }
}
