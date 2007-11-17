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
#include "qajapplication.h"

QAjApplication::QAjApplication( int & argc, char ** argv ) : QApplication( argc, argv )
{
    QCoreApplication::setOrganizationName("progeln.de");
    QCoreApplication::setOrganizationDomain("progeln.de");
    QCoreApplication::setApplicationName("Juicer");
    setQuitOnLastWindowClosed( false );
    socket = NULL;
    *argv++;
    while (argc-- > 1)
    {
        argList << QString( *argv++ );
    }
    if( ! argList.isEmpty() )
    {
        socket = new QAjSocket( APP_PORT, argList, this );
        connect( socket, SIGNAL( error( QAbstractSocket::SocketError ) ), this, SLOT( start() ) );
    }
}


QAjApplication::~QAjApplication()
{
}

int QAjApplication::exec()
{
    if ( socket != NULL )
        socket->start();
    else
        start();
    return QApplication::exec();
}

void QAjApplication::start()
{
    QSplashScreen *splash = NULL;
    if ( QAjOptionsDialog::getSetting( "showSplash", true ).toBool() )
    {
        splash = new QSplashScreen(QPixmap(":/splash.png"));
        splash->show();
    }

    Juicer* juicer = new Juicer( argList );
    juicer->setWindowTitle( "Juicer" );
    //setQuitOnLastWindowClosed( true );

    if ( splash != NULL )
    {
        splash->finish( juicer );
        delete splash;
    }

}
