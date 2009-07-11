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

#ifndef QAJSERVERWIDGET_H
#define QAJSERVERWIDGET_H

#include <QHash>
#include <QHttp>
#include <QSettings>
#include <QMessageBox>

#include "modulebase.h"
#include "serveritem.h"
#include "optionsdialog.h"

/**
@author Matthias Reif
*/
class ServerModule : public ModuleBase
{
    Q_OBJECT
public:
    ServerModule(Juicer* juicer);

    ~ServerModule();
    void insertServer(const QString& id, const QString& name, const QString& host, const QString& port, const QString& lastseen, const QString& tests);
    void connectedWith( const QString& id );
    void connectingTo( const QString& id );
    ServerItem* findServer( const QString& id );
    bool remove( const QString& id );
    QDateTime& setConnectedSince(const QString& time);

private:
    QString connectedWithId, connectingToId;
    QHash<QString, ServerItem*> servers;
    QHttp *serverHttp;
    QDateTime connectedSince;

public slots:
    void connectSlot();
    void removeSlot();
    void searchSlot();
    void gotServer( int id, bool error );
    void selectionChanged();
};

#endif
