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

#ifndef QAJSOCKET_H
#define QAJSOCKET_H

#include <QTcpSocket>
#include <QStringList>
#include <QCoreApplication>
#include <QHostAddress>

/**
@author Matthias Reif
*/
class Socket : public QTcpSocket
{
    Q_OBJECT
public:
    Socket(int appPort, const QStringList& argList, QObject *parent = 0);
    ~Socket();
    void start();

private:
    int appPort;
    QStringList argList;

private slots:
    void connected();
};

#endif
