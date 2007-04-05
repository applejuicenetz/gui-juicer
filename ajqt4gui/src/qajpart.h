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
#ifndef QAJPART_H
#define QAJPART_H

#include <QObject>  // Q_ULLONG

/**
@author Matthias Reif
*/
class QAjPart{
public:
    QAjPart( qulonglong fromPosition, int type );

    ~QAjPart();
    qulonglong getFromPosition();
    int getType();

protected:
	qulonglong fromPosition;
	int type;
};

#endif
