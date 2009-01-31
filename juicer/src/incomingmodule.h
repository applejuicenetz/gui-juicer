/***************************************************************************
 *   Copyright (C) 2007 by Matthias Reif   *
 *   matthias.reif@informatik.tu-chemnitz.de   *
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
    void setDir(QString dir);

private:
    void initToolBar();
    QString dir;
    void storeFtp();
    void reloadFtp();
    void openFtp();
    void removeFtp();
    void initPopup();
    AjSettings::LOCATION getLocation();
    bool confirmRemove(QList<QTreeWidgetItem *>& items);
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
        CopyThread(QString oldFilename, QString newFilename) {
            this->oldFilename = oldFilename;
            this->newFilename = newFilename;
        }
        QString oldFilename, newFilename;
        void run() {
            if(!QFile::copy(oldFilename, newFilename)) {
                QMessageBox::critical(NULL, "error", "copy process failed");
            }
        }
    };

private slots:
    void ftpReadyRead( QFile* dstFile, FTP* ftp );
};

#endif
