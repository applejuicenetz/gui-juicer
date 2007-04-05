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
#include "qajserversocket.h"

QAjServerSocket::QAjServerSocket( quint16 port, int backlog, QObject *parent, const char *name) : QTcpServer()
{
	listen( QHostAddress::Any, port  );
	clientSocket = NULL;
	connect( this, SIGNAL( newConnection() ), this, SLOT( newConnectionSlot() ) );
}

QAjServerSocket::~QAjServerSocket()
{
	if( clientSocket != NULL )
	{
		clientSocket->close();
		delete clientSocket;
	}
}

void QAjServerSocket::readLine()
{
	QString line;
	while( (line = clientSocket->readLine())  != "" )
	{
		lineReady( line );
	}
}


/*!
    \fn QAjServerSocket::newConnectionSlot()
 */
void QAjServerSocket::newConnectionSlot()
{
	clientSocket = nextPendingConnection();
	connect( clientSocket, SIGNAL( readyRead() ), this, SLOT( readLine() ) );
}
