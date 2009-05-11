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

#include "serversocket.h"

ServerSocket::ServerSocket(quint16 port, QObject *parent) : QTcpServer(parent) {
    listen(QHostAddress::Any, port);
    clientSocket = NULL;
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
}

ServerSocket::~ServerSocket() {
    if(clientSocket != NULL) {
        clientSocket->close();
        delete clientSocket;
    }
}

void ServerSocket::readLine() {
    QString line;
    while((line = clientSocket->readLine()) != "") {
        lineReady(line);
    }
}


/*!
    \fn ServerSocket::newConnectionSlot()
 */
void ServerSocket::newConnectionSlot() {
    clientSocket = nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readLine()));
}
