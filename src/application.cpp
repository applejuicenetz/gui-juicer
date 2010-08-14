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

#include "application.h"

#include "askagaindialog.h"


#include "convert.h"
bool less(const QString &s1, const QString &s2) {
    return Convert::compareVersion(s1, s2) == -1;
}

Application::Application(int & argc, char ** argv) : QApplication( argc, argv ) {
    QCoreApplication::setOrganizationName("progeln.de");
    QCoreApplication::setOrganizationDomain("progeln.de");
    QCoreApplication::setApplicationName("Juicer");
    QCoreApplication::setApplicationVersion("1.2");
    setQuitOnLastWindowClosed(false);
    socket = NULL;

    appFileInfo.setFile(argv[0]);

    // -- check if juicer is default application for ajfsp links --
    this->setAjfspHandler(false);

    *argv++;
    while(argc-- > 1) {
        QString arg(*argv++);
        if(arg == "--reset") {
            QSettings lokalSettings;
            lokalSettings.clear();
        } else {
            argList << QString(arg);
        }
    }
    if(!argList.isEmpty()) {
        socket = new Socket(APP_PORT, argList, this);
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(start()));
    }
}


Application::~Application() {
}

int Application::exec() {
    if(socket != NULL) {
        socket->start();
    } else {
        start();
    }
    return QApplication::exec();
}

void Application::start() {
    QSplashScreen *splash = new QSplashScreen(QPixmap(":/splash.png"));
    splash->setVisible(OptionsDialog::getSetting( "showSplash", true ).toBool());
    Juicer* juicer = new Juicer(argList, splash, appFileInfo);
    connect(juicer->optionsDialog->handlerPushButton, SIGNAL(clicked()), this, SLOT(setAjfspHandler()));

}


/*!
    \fn Application::setAjfspHandler(bool force)
 */
void Application::setAjfspHandler(bool force) {
    #ifdef Q_WS_WIN
        QString appCmd = appFileInfo.absoluteFilePath().replace("/","\\");
        appCmd = "\"" + appCmd + "\" \"%1\"";
        QSettings settings("HKEY_CLASSES_ROOT\\ajfsp", QSettings::NativeFormat);
        settings.setValue("Default","URL:ajfsp Protocol");
        settings.setValue("URL Protocol","");
        if(settings.value("shell/open/command/Default") != appCmd) {

            HandlerDialog handlerDialog(
                    tr( "ajfsp Protocol Handler" ),
                    tr("Juicer seems not to be the default application for ajfsp:// links.\nWould you like to change this?"),
                    QDialogButtonBox::Yes | QDialogButtonBox::No,
                    QStyle::SP_MessageBoxQuestion);
            if(force || handlerDialog.exec("handler") == QDialog::Accepted) {
                settings.setValue("shell/open/command/Default",appCmd);
            }
        }
    #endif
}
