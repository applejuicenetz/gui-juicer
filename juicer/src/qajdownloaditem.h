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

#include "qajuseritem.h"
#include "qajdownloaditem.h"

#include "qajlistwidget.h"
#include "qajitem.h"

#include "types.h"

#include "qconvert.h"

#include "qajpartlistwidget.h"
#include "qajpowerspin.h"

class QAjUserItem;

/**
@author Matthias Reif
*/
class QAjDownloadItem : public QAjItem
{
Q_OBJECT
public:
    QAjDownloadItem( QString id, QAjListWidget *parent = 0 );

    ~QAjDownloadItem();

    void moveItem( QAjUserItem *userItem, QString oldStatus );
    void update( const QString& hash, const QString& fileName, const QString& status, const QString& size, const QString& ready, const QString& power, const QString& tempNumber );
    void updateUser( const QString& id, const QString& fileName, const QString& speed, const QString& status, const QString& power, const QString& queuePos, const QString& statusString, QIcon *osIcon, const QTime& time );

    QAjUserItem* findUser( const QString& id );
    void removeUser( const QString& id );

    QString getLinkAJFSP();

    QTreeWidgetItem* activeSourcesItem;
    QTreeWidgetItem* queuedSourcesItem;
    QTreeWidgetItem* otherSourcesItem;

    int getActiveSources()
    {
        return activeSourcesItem->childCount();
    }

    void setFinishedPixmap(int newWidth, int newHeight, double newReady);


    QString getSourcesString();

    bool updateView( QHash<QString, QString>* downloadStatusDescr );

    void deleteUsers();

//     int compare( QTreeWidgetItem * i, int col, bool ) const;
    void showWidget( const QPoint &p );

    QHash<QString, QAjUserItem*> users;

    double getReady()
    {
        return ready;
    }
    double getRemainingSize()
    {
        return remainingSize;
    }
    double getRemainingSec()
    {
        return remainingSec;
    }
    double getSpeed()
    {
        return speed;
    }
    double getMissing()
    {
        return missing;
    }
    double getFinished()
    {
        return finished;
    }
    int getPercent()
    {
        return percent;
    }
    QAjPartListWidget* getPartListWidget();
    void setParts( qulonglong size, QLinkedList<Part>& partList );

    QString getTempNumber()
    {
        return tempNumber;
    }

    virtual bool operator<( const QTreeWidgetItem & other ) const;
    QAjPowerSpin* powerSpin;
public slots:
    void initPowerSpin();

protected:
    //Q_ULLONG
    double ready, remainingSize;
    double speed;
    QLinkedList<double> speeds;
    double finished;
    int percent;
    int width, height;
    double missing;
    long int remainingSec;

    bool finishedChanged;
    bool first;
    bool firstFinished;

    QAjListWidget *parentWidget;
    QPixmap *pixmap;

    QAjPartListWidget* partListWidget;

    QString tempNumber;
};

#endif
