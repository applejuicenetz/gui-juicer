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
#ifndef QAJDOWNLOADITEM_H
#define QAJDOWNLOADITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <QHash>

#include <QHeaderView>
#include <QLinkedList>
#include <QProgressBar>

#include "useritem.h"

#include "item.h"

#include "qconvert.h"

#include "partlistdialog.h"
#include "powerspin.h"

class UserItem;


static const QString DOWN_PAUSED    = "18";
static const QString DOWN_FINISHED  = "14";
static const QString DOWN_CANCELD   = "17";
static const QString DOWN_FINISHING = "12";
static const QString DOWN_SEARCHING = "0";
static const QString DOWN_LOADING   = "-1";

/**
@author Matthias Reif
*/
class DownloadItem : public Item
{
Q_OBJECT
public:
    DownloadItem( const QString& id, QTreeWidget *parent = 0 );
    ~DownloadItem();

    enum {FILENAME_COL, SOURCES_COL, SPEED_COL, STATUS_COL, FINISHED_COL, POWER_COL,
          SIZE_COL, FINISHED_SIZE_COL, REMAIN_SIZE_COL, REMAIN_TIME_COL, MISSING_COL};

    void moveItem( UserItem *userItem, const QString& oldStatus );
    bool update( const QString& hash, const QString& fileName, const QString& status, const QString& size, const QString& ready, const QString& power, const QString& tempNumber );
    void updateUser( const QString& id, const QString& fileName, const QString& nickname, const QString& speed, const QString& status, const QString& power, const QString& queuePos, const QString& statusString, QIcon& osIcon, const QString& downloadfrom, const QString& downloadto, const QString& actualdownloadposition, const QTime& time );

    UserItem* findUser( const QString& id );
    void removeUser( const QString& id );

    QString getLinkAJFSP();

    QTreeWidgetItem* activeSourcesItem;
    QTreeWidgetItem* queuedSourcesItem;
    QTreeWidgetItem* otherSourcesItem;

    QString getSourcesString();
    bool updateView(const QHash<QString, QString>& downloadStatusDescr);
    void deleteUsers();
    PartListDialog* getPartListDialog();
    void setParts( qulonglong size, QLinkedList<PartsWidget::Part>& partList );
    void showWidget( const QPoint &p );

    QHash<QString, UserItem*> users;

    int getActiveSources() const  {
        return activeSourcesItem->childCount();
    }
    double getReady() const {
        return ready;
    }
    double getRemainingSize() const {
        return remainingSize;
    }
    double getRemainingSec() const {
        return remainingSec;
    }
    double getSpeed() const {
        return speed;
    }
    double getMissing() const {
        return missing;
    }
    double getFinished() const {
        return finished;
    }
    int getPercent() const {
        return percent;
    }
    QString getTempNumber() {
        return tempNumber;
    }
    // Powerdownload for this Item
    bool powerDownloadActive() const {
        return (powerSpin->check->checkState() == Qt::Checked) ? true : false;
    }
    float powerDownloadValue() const {
        return powerSpin->spin->value();
    }

    virtual bool operator<( const QTreeWidgetItem & other ) const;
    QProgressBar* progressBar;
    PowerSpin* powerSpin;

public slots:
    void initPowerSpin();

protected:
    double ready, remainingSize;
    double speed;
    double finished;
    int percent;
    double missing;
    long int remainingSec;
    /// used for notifications about finished downloads
    bool firstFinished;
    PartListDialog* partListDialog;
    QString tempNumber;
};

#endif
