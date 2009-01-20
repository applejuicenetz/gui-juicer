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

#include <QMessageBox>
#include <QFileInfo>
#include "qajhandlerdialog.h"

QAjApplication::QAjApplication( int & argc, char ** argv ) 
    : QApplication( argc, argv )
{
    QCoreApplication::setOrganizationName("progeln.de");
    QCoreApplication::setOrganizationDomain("progeln.de");
    QCoreApplication::setApplicationName("Juicer");
    setQuitOnLastWindowClosed( false );
    socket = NULL;

    QFileInfo appFileInfo( argv[0] );
    appPath = appFileInfo.absoluteFilePath();    // contains path and filename
    // -- check if juicer is default application for ajfsp links --
    #ifdef Q_WS_WIN
        QString appCmd = appPath.replace("/","\\");
        appCmd = "\"" + appCmd + "\" \"%1\"";
        QSettings settings("HKEY_CLASSES_ROOT\\ajfsp", QSettings::NativeFormat);
        settings.setValue("Default","URL:ajfsp Protocol");
        settings.setValue("URL Protocol","");
        if(settings.value("shell/open/command/Default") != appCmd) {
            if(QAjOptionsDialog::hasSetting("handler")) {
                if(QAjOptionsDialog::getSetting("handler", false).toBool()) {
                    settings.setValue("shell/open/command/Default",appCmd);
                }
            } else {
                bool accepted = false;
                QAjHandlerDialog handlerDialog( tr( "ajfsp Protecol Handler" ),
                                                this,
                                                tr( "Yes" ),
                                                tr( "No" ) );
                handlerDialog.setText( tr("Juicer seems not to be the default application for ajfsp:// links.\nWould you like to change this?") );
                accepted = ( handlerDialog.exec() == QDialog::Accepted );
                if( accepted ) {
                    settings.setValue("shell/open/command/Default",appCmd);
                }
                if ( handlerDialog.dontAskAgain() ) {
                    QAjOptionsDialog::setSetting("handler", accepted);
                } else {
                    QAjOptionsDialog::removeSetting("handler");
                }
            }
        }
    #endif

    *argv++;
    while (argc-- > 1)
    {
        QString arg(*argv++);
        if(arg == "--reset") {
            QSettings lokalSettings;
            lokalSettings.clear();
        } else {
            argList << QString(arg);
        }
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
    if(socket != NULL) {
        socket->start();
    } else {
        start();
    }
    return QApplication::exec();
}

void QAjApplication::start() 
{
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/splash.png"));
    splash->setVisible(QAjOptionsDialog::getSetting( "showSplash", true ).toBool());

    Juicer* juicer = new Juicer(argList, splash);
    juicer->setWindowTitle("Juicer");
}

