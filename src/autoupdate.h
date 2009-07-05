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

#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <QObject>
#include <QHttp>
#include <QDateTime>
#include <QProgressBar>
#include <QDialog>
#include <QFile>
#include <QBuffer>
#include <QHttpRequestHeader>
#include <QDir>
#include <QUrl>
#include <QTemporaryFile>
#include <QMessageBox>
#include <QtXml>
#include <QPushButton>

#include "quazip.h"
#include "quazipfile.h"
#include "updatedialog.h"
#include "convert.h"

/**
    @author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class AutoUpdate : public QObject
{
Q_OBJECT
public:
    AutoUpdate(const QString& appPath, QWidget *parent = 0);
    ~AutoUpdate();
    bool readXML();
    void update();
    void checkVersion();
public slots:
    void check();
private:
    QHttp http;
    int checkId, getId;
    UpdateDialog* updateDialog;
    QTemporaryFile file;
    QString appPath;
    QString os, updateFolder, updateXML;
    QString updateVersion, updateFile;
private slots:
    void requestFinished(int id, bool error);
    void dataReadProgress(int done, int total);
    void clicked(QAbstractButton * button);
};

#endif
