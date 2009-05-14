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

#ifndef QAJUPLOADITEM_H
#define QAJUPLOADITEM_H

#include <QHeaderView>
#include <QProgressBar>

#include "item.h"

static const QString ACTIVE_UPLOAD    = "1";
static const QString QUEUEING_UPLOAD  = "2";
static const QString NEW_UPLOAD       = "-1";

/**
@author Matthias Reif
*/
class UploadItem : public Item
{
    QProgressBar progressChunk_, progressLoaded_;
    QString shareId;
    double speed;

public:
    UploadItem( const QString& id, const QString& shareId, QTreeWidget *parent );
    ~UploadItem();

    enum column { FILENAME_COL    = 0,
                  CHUNK_COL       = 1,
                  NICK_COL        = 2,
                  SPEED_COL       = 3,
                  STATUS_COL      = 4,
                  PRIORITY_COL    = 5,
                  OS_COL          = 6,
                  DIRECTSTATE_COL = 7,
                  VERSION_COL     = 8,
                  LOADED_COL      = 9,
                  LASTSEEN_COL    = 10 };

    enum progressBars {
                  PRGB_CHUNKS,
                  PRGB_LOADED
                };

    virtual bool operator<( const QTreeWidgetItem & other ) const;
    bool update(const QIcon& osIcon, const QString& status, const QString& statusDescr,
                const QString& directState, const QString& priority, const QString& nick,
                const QString& speed, const QString& version, const QString& loaded,
                const QString& chunkStart, const QString& chunkEnd, const QString& chunkPos,
                const QString& lastConnected, bool newUpload);

    QString getShareID() const {
        return shareId;
    }

    int progressPercentualValue( progressBars p ) const;
private:
    void initProgressBar( QProgressBar& progressBar, column col );
    void updateChunkProgress( int uploadFrom, int uploadTo, int uploadCurrent );
    void updateLoadedProgress( const QString& loaded );
    void updateLastSeen( const QString& lastSeen );
};

#endif
