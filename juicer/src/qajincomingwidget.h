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

#include "qajoptionsdialog.h"
#include "qajincomingitem.h"
#include "qajlistwidget.h"
#include "ftp.h"

class Juicer;

/**
	@author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class QAjIncomingWidget : public QAjListWidget
{
Q_OBJECT
public:
    QAjIncomingWidget( QXMLModule* xml, QWidget *parent = 0 );

    ~QAjIncomingWidget();
    void setDir( QString dir );

private:
    void initToolBar();
    QString dir;
    QAction *reloadButton, *openButton, *saveButton;
    void storeFtp();
    void reloadFtp();
    void initPopup();
    AjSettings::LOCATION getLocation();
    QFtp* ftp;

public slots:
    void reload();
    void open();
    void save();
    void insert( QUrlInfo info );
    void selectionChanged( bool oneSelected );

private:
    class CopyThread : public QThread
    {
        public:
        CopyThread(QString oldFilename, QString newFilename)
        {
            this->oldFilename = oldFilename;
            this->newFilename = newFilename;
        }
        QString oldFilename, newFilename;
        void run()
        {
            if(!QFile::copy(oldFilename, newFilename))
            {
                QMessageBox::critical(NULL, "error", "copy process failed");
            }
        }
    };
};

#endif
