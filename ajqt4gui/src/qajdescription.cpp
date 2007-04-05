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
#include "qajdescription.h"

QAjDescription::QAjDescription()
{
	userStatusDescr[0] = QObject::tr("unknown ");
	userStatusDescr[1] = QObject::tr("unasked ");
	userStatusDescr[2] = QObject::tr("try to connect ");
	userStatusDescr[3] = QObject::tr("partner have to old vers. ");
	userStatusDescr[4] = QObject::tr("partner can't	 open file ");
	userStatusDescr[5] = QObject::tr("queueing ");
	userStatusDescr[6] = QObject::tr("no usable parts ");
	userStatusDescr[7] = QObject::tr("loading ");
	userStatusDescr[8] = QObject::tr("not enough disk space ");
	userStatusDescr[9] = QObject::tr("finished ");
	userStatusDescr[10] = QObject::tr("unknown ");
	userStatusDescr[11] = QObject::tr("no connection possible ");
	userStatusDescr[12] = QObject::tr("try indirect ");
	userStatusDescr[13] = QObject::tr("paused ");
	userStatusDescr[14] = QObject::tr("queue full ");
	userStatusDescr[15] = QObject::tr("own limit reached ");
	userStatusDescr[16] = QObject::tr("indirect conn. rejected ");
	
	downloadStatusDescr[0] = QObject::tr("searching ");
	downloadStatusDescr[1] = QObject::tr("error at creating ");
	downloadStatusDescr[2] = QObject::tr("unknown ");
	downloadStatusDescr[3] = QObject::tr("unknown ");
	downloadStatusDescr[4] = QObject::tr("loading ");
	downloadStatusDescr[5] = QObject::tr("unknown ");
	downloadStatusDescr[6] = QObject::tr("unknown ");
	downloadStatusDescr[7] = QObject::tr("unknown ");
	downloadStatusDescr[8] = QObject::tr("unknown ");
	downloadStatusDescr[9] = QObject::tr("unknown ");
	downloadStatusDescr[10] = QObject::tr("unknown ");
	downloadStatusDescr[11] = QObject::tr("unknown ");
	downloadStatusDescr[12] = QObject::tr("finishing ");
	downloadStatusDescr[13] = QObject::tr("error at finishing ");
	downloadStatusDescr[14] = QObject::tr("finished ");
	downloadStatusDescr[15] = QObject::tr("canceling ");
	downloadStatusDescr[16] = QObject::tr("creating .dat ");
	downloadStatusDescr[17] = QObject::tr("canceled ");
	downloadStatusDescr[18] = QObject::tr("paused ");
}

QAjDescription::~QAjDescription()
{
}

QString QAjDescription::getUserStatusDescription(int num)
{
	if( (num >= NUM_USER_STATES) || (num < 0) )
		num = 0;
	return userStatusDescr[num];
}

QString QAjDescription::getDownloadStatusDescription(int num)
{
	if( (num >= NUM_DOWNLOAD_STATES) || (num < 0) )
		num = 0;
	return downloadStatusDescr[num];
}



