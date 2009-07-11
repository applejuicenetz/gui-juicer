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

#ifndef QAJINCOMINGWIDGET_H
#define QAJINCOMINGWIDGET_H

#include <QFileDialog>
#include <QMessageBox>

#include "optionsdialog.h"
#include "incomingitem.h"
#include "modulebase.h"
#include "ftp.h"

class Juicer;

/**
	@author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class IncomingModule : public ModuleBase
{
Q_OBJECT
public:
    IncomingModule(Juicer* juicer);
    ~IncomingModule();
    void setDir(const QString& dir);

private:
    void initToolBar();
    QString dir;
    void storeFtp();
    void reloadFtp();
    void openFtp();
    void removeFtp();
    void initPopup();
    AjSettings::LOCATION getLocation();
    bool confirmRemove(QItemList& items);
    QFtp* ftp;
    QLabel* waitLabel;

public slots:
    void reload();
    void open();
    void copy();
    void remove();
    void insert(QUrlInfo info);
    void selectionChanged();
private:
    class CopyThread : public QThread {
        public:
        CopyThread(const QString& oldFilename, const QString& newFilename) {
            this->oldFilename = oldFilename;
            this->newFilename = newFilename;
        }
        QString oldFilename, newFilename;
        void run() {
            if(!QFile::copy(oldFilename, newFilename)) {
                QMessageBox::critical(NULL, tr("error"), tr("copy process failed"));
            }
        }
    };

private slots:
    void ftpReadyRead(QFile* dstFile, FTP* ftp);
};

#endif
