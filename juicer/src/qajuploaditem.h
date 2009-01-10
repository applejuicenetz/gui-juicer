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
#ifndef QAJUPLOADITEM_H
#define QAJUPLOADITEM_H

#include "qajitem.h"

static const QString ACTIVE_UPLOAD = "1";
static const QString QUEUEING_UPLOAD = "2";
static const QString NEW_UPLOAD = "-1";

/**
@author Matthias Reif
*/
class QAjUploadItem : public QAjItem
{
public:
    QAjUploadItem( QString id, QString shareId, QTreeWidget *parent );
    ~QAjUploadItem();

    QString shareId;

    enum {FILENAME_COL, NICK_COL, SPEED_COL, STATUS_COL,
          PRIORITY_COL, OS_COL, DIRECTSTATE_COL};

};

#endif
