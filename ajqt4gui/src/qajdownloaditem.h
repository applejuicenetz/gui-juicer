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

#include "qajuseritem.h"
#include "qajdownloaditem.h"

#include "qajlistwidget.h"
#include "qajitem.h"

#include "types.h"

#include "qconvert.h"

#include "qajpartlistwidget.h"

class QAjUserItem;

/**
@author Matthias Reif
*/
class QAjDownloadItem : public QAjItem
{
//Q_OBJECT
public:
    QAjDownloadItem( QAjListWidget *parent = 0, const char *name = 0 );

    ~QAjDownloadItem();

    void moveItem( QAjUserItem *userItem, QString oldStatus );
    void update( QString fileName, QString status, QString size, QString ready, QString power, QString tempNumber );
    void updateUser( QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString statusString, QIcon *osIcon );

    void decSources( QString type );
    void incSources( QString type );

    QAjUserItem* findUser( QString id );
    void removeUser( QString id );

    QTreeWidgetItem* activeSourcesItem;
    QTreeWidgetItem* queuedSourcesItem;
    QTreeWidgetItem* otherSourcesItem;
    void incActiveSources()
    {
        activeSources++;
    }
    void incQueuedSources()
    {
        queuedSources++;
    }
    void incOtherSources()
    {
        otherSources++;
    }

    void decActiveSources()
    {
        activeSources--;
    } //  = (activeSources>0)?activeSources-1:0; }
    void decQueuedSources()
    {
        queuedSources--;
    } // = (queuedSources>0)?queuedSources-1:0; }
    void decOtherSources()
    {
        otherSources--;
    } // = (otherSources>0)?otherSources-1:0; }

    int getActiveSources()
    {
        return activeSources;
    }

    void setFinishedPixmap(int newWidth, int newHeight, double newReady);


    QString getSourcesString();

    void updateView( QHash<QString, QString>* downloadStatusDescr );

    void deleteUsers();

    int compare( QTreeWidgetItem * i, int col, bool ascending ) const;
    void showWidget( const QPoint &p );

    QHash<QString, QAjUserItem*> users;

    double getSize()
    {
        return size;
    }
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
    void setParts( qulonglong size, QLinkedList<Part> partList );

    QString getTempNumber()
    {
        return tempNumber;
    }

    virtual bool operator<( const QTreeWidgetItem & other ) const;

protected:
    //Q_ULLONG
    double size, ready, remainingSize;
    double speed;
    double finished;
    int percent;
    int width, height;
    double missing;
    long int remainingSec;

    bool finishedChanged;
    bool first;

    int activeSources, queuedSources, otherSources;
    QAjListWidget *parentWidget;
    QPixmap *pixmap;

    QAjPartListWidget* partListWidget;

    QString tempNumber;
};

#endif
