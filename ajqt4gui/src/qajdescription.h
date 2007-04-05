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
#ifndef QAJDESCRIPTION_H
#define QAJDESCRIPTION_H

/**
@author Matthias Reif
*/

#include <QString>
#include <QObject>

#define NUM_USER_STATES 17
#define NUM_DOWNLOAD_STATES 19

class QAjDescription{
public:
	QAjDescription();

	~QAjDescription();

	QString getUserStatusDescription(int num);
	QString getDownloadStatusDescription(int num);
 
private:
	QString userStatusDescr[NUM_USER_STATES];
	QString downloadStatusDescr[NUM_DOWNLOAD_STATES];
};

#endif
