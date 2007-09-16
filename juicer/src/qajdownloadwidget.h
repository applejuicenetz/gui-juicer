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
#ifndef QAJDOWNLOADWIDGET_H
#define QAJDOWNLOADWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QPainter>
#include <QMenu>
#include <QHash>
#include <QList>
#include <QIcon>
#include <QSettings>
#include <QFileDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QInputDialog>
#include <QApplication>
#include <QClipboard>

#include "qajlistwidget.h"
#include "qajdownloaditem.h"
#include "qajuseritem.h"
#include "qconvert.h"
#include "ftp.h"
#include "qajoptionsdialog.h"

#include "types.h"

/**
@author Matthias Reif
*/

class Juicer;

class QAjDownloadWidget : public QAjListWidget
{
    Q_OBJECT
public:
    QAjDownloadWidget( QXMLModule* xml, QWidget *parent = 0 );

    ~QAjDownloadWidget();

    void insertDownload(QString id, QString hash, QString fileName, QString status, QString size, QString ready, QString power, QString tempNumber);
    void insertUser(QString downloadId, QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString os, QTime time);
    bool remove( QString id );

    QAjDownloadItem* findDownload( QString id );
    bool removeDownload( QString id );
    DownloadUser findParent( QString id );

    QString getNextIdRoundRobin();
    QString findDownloadByTempNum( QFileInfo tempFile );
    void initToolBar();
    void setDirs( QFileInfo tmpDir, QFileInfo inDir );
    void initPopup();
    QAjDownloadItem* findDownload( QString size, QString hash );

    QAction *powerUpButton, *powerDownButton, *powerOkButton, *powerMaxButton;
    QAction *pauseDownloadButton, *resumeDownloadButton;
    QAction *cancelDownloadButton, *clearDownloadButton,  *partListButton;
    QAction *renameDownloadButton, *renamePlusDownloadButton;
    QAction *openDownloadButton;
    QAction *copyLinkButton;

    QDoubleSpinBox* powerSpin;
    QCheckBox *powerCheck;

private:

    QHash<QString, QAjDownloadItem*> downloads;
    int currIdRoundRobin;

    QIcon *linuxIcon, *windowsIcon, *macIcon, *solarisIcon, *freeBsdIcon, *netwareIcon, *otherOsIcon;

    QHash<QString, QString> userStatusDescr;
    QHash<QString, QString> downloadStatusDescr;

    QFileInfo tempDir, incomingDir;

private slots:
    void selectionChanged( bool oneSelected );
    void pauseSlot();
    void resumeSlot();
    void cancelSlot();
    void cleanSlot();
    void partListSlot();
    void renameSlot();
    void renamePlusSlot();
    void openSlot();
    void linkSlot();

    void processSelected( QString request, QString para = "" );
    void requestSelected( QString request, QString para = "" );
    void applyPowerDownload();
    void applyPowerDownload(QString id, double value);
    void maxPowerDownload();
    void itemChanged( QTreeWidgetItem* item, int column );


public slots:
    void updateView( bool force = false );
    void storeDownloadFtp();

signals:
    void downloadsFinished( QList<QAjDownloadItem*> downloads );
};

#endif
