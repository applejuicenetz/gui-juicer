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

#ifndef QAJAPPLICATION_H
#define QAJAPPLICATION_H

#include <qglobal.h>
#include <QApplication>
#include <QSplashScreen>
#include <QMessageBox>
#include <QFileInfo>

#include "serversocket.h"
#include "socket.h"
#include "juicer.h"
#include "autoupdate.h"

/**
@author Matthias Reif
*/
class Application : public QApplication
{
    Q_OBJECT
public:
    Application( int & argc, char ** argv );
    ~Application();
    int exec();
    static const int APP_PORT = 21456;
    QString appPath;
public slots:
    void start();
private:
    AutoUpdate* autoUpdate;
    Socket *socket;
    QStringList argList;
};

#endif
