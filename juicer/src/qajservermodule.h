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
#ifndef QAJSERVERWIDGET_H
#define QAJSERVERWIDGET_H

#include <QHash>
#include <QHttp>
#include <QSettings>
#include <QMessageBox>

#include "qajmodulebase.h"
#include "qajserveritem.h"
#include "qajoptionsdialog.h"

/**
@author Matthias Reif
*/
class QAjServerModule : public QAjModuleBase
{
    Q_OBJECT
public:
    QAjServerModule(Juicer* juicer);

    ~QAjServerModule();
    void insertServer(const QString& id, const QString& name, const QString& host, const QString& port, const QString& lastseen, const QString& tests);
    void connectedWith( QString id );
    void connectingTo( QString id );
    QAjServerItem* findServer( QString id );
    bool remove( QString id );
    QDateTime& setConnectedSince(const QString& time);

private:
    QString connectedWithId, connectingToId;
    QHash<QString, QAjServerItem*> servers;
    QHttp *serverHttp;
    QDateTime zeroTime, connectedSince;

public slots:
    void connectSlot();
    void removeSlot();
    void searchSlot();
    void gotServer( int id, bool error );
    void selectionChanged();
    void welcomeDockVisible(bool visible);
signals:
    void remove();
    void connect();
    void find();
};

#endif
