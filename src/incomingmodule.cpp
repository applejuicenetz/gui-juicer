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

#include "incomingmodule.h"
#include "juicer.h"

IncomingModule::IncomingModule(Juicer* juicer) : ModuleBase(juicer, juicer->incomingTreeWidget, juicer->incomingToolBar) {
    ftp = new QFtp(this);
    waitLabel = new QLabel(tr("please wait..."), treeWidget);
    connect(ftp, SIGNAL(listInfo (QUrlInfo)), this, SLOT(insert(QUrlInfo)));
    connect(treeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem*, int)), this, SLOT(open()));
    connect(juicer->actionOpen_Incoming, SIGNAL(triggered()), this, SLOT(open()));
    connect(juicer->actionCopy_Incoming, SIGNAL(triggered()), this, SLOT(copy()));
    connect(juicer->actionDelete_Incoming, SIGNAL(triggered()), this, SLOT(remove()));
    connect(juicer->actionReload_Incoming, SIGNAL(triggered()), this, SLOT(reload()));
    selectionChanged();
}


IncomingModule::~IncomingModule()
{
}


/*!
    \fn IncomingModule::reloadFtp()
 */
void IncomingModule::reloadFtp() {
    treeWidget->clear();
    waitLabel->move((treeWidget->width() - treeWidget->width()) /2, (treeWidget->height() - waitLabel->height()) / 2);
    waitLabel->show();

    QString server = OptionsDialog::getSetting("ftp", "server", "localhost").toString();
    int port = OptionsDialog::getSetting("ftp", "port", "21").toInt();
    QString user = OptionsDialog::getSetting("ftp", "user", "anonymous").toString();
    QString password = OptionsDialog::getSetting("ftp", "password", "").toString();
    QString dir = OptionsDialog::getSetting("ftp", "inDir", "/").toString();
    QFtp::TransferMode mode = (QFtp::TransferMode)OptionsDialog::getSetting("ftp", "mode", QFtp::Active).toInt();

    if(ftp->state() != QFtp::Unconnected) {
        ftp->close();
    }
    ftp->connectToHost(server, port);
    ftp->login(user, password);
    ftp->setTransferMode(mode);
    ftp->list(dir);
}


/*!
    \fn IncomingModule::copy()
 */
void IncomingModule::copy() {
    QString actDir;
    // determine the path
    AjSettings::LOCATION location = getLocation();
    if(location == AjSettings::FTP) {
        storeFtp();
    } else {
        if(location == AjSettings::SPECIFIC) {
            actDir = OptionsDialog::getSetting("incomingDirSpecific", "/").toString() + QDir::separator();
        } else if(location == AjSettings::SAME) {
            actDir = this->dir + QDir::separator();
        }
        QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
        for(int i=0; i<selectedItems.size(); i++) {
            QString newDir = QFileDialog::getExistingDirectory(juicer, tr("copy to"), actDir)
                    + QDir::separator();
            if(!newDir.isEmpty()) {
                QString newFilename = selectedItems[i]->text(IncomingItem::FILENAME_COL);
                while(!newFilename.isEmpty() && QFile::exists(newDir + newFilename)) {
                    newFilename = QInputDialog::getText(juicer, tr("file already exists"),
                            "filename", QLineEdit::Normal, selectedItems[i]->text(IncomingItem::FILENAME_COL));
                }
                if(!newFilename.isEmpty()) {
                    (new CopyThread(actDir + selectedItems[i]->text(IncomingItem::FILENAME_COL),
                                newDir + newFilename))->start();
                }
            }
        }
    }
}


/*!
    \fn IncomingModule::removeFtp()
 */
void IncomingModule::removeFtp() {
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    if(confirmRemove(selectedItems)) {
        QFtp* ftp = new QFtp(this);
        QString server = OptionsDialog::getSetting("ftp", "server", "localhost").toString();
        int port = OptionsDialog::getSetting("ftp", "port", "21").toInt();
        QString user = OptionsDialog::getSetting("ftp", "user", "anonymous").toString();
        QString password = OptionsDialog::getSetting("ftp", "password", "").toString();
        QString dir = OptionsDialog::getSetting("ftp", "inDir", "/").toString();
        QFtp::TransferMode mode = (QFtp::TransferMode)OptionsDialog::getSetting("ftp", "mode", QFtp::Active).toInt();
        if (! dir.endsWith('/'))
            dir += '/';
        ftp->connectToHost(server, port);
        ftp->login(user, password);
        ftp->setTransferMode(mode);
        for(int i=0; i<selectedItems.size(); i++) {
            ftp->remove(dir + selectedItems[i]->text(IncomingItem::FILENAME_COL));
        }
    }
}


/*!
    \fn IncomingModule::remove()
 */
void IncomingModule::remove() {
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    if(confirmRemove(selectedItems)) {
        QString actDir;
        // determine the path
        AjSettings::LOCATION location = getLocation();
        if(location == AjSettings::FTP) {
            removeFtp();
        } else {
            if(location == AjSettings::SPECIFIC) {
                actDir = OptionsDialog::getSetting("incomingDirSpecific", "/").toString() + QDir::separator();
            } else if(location == AjSettings::SAME) {
                actDir = this->dir + QDir::separator();
            }
            for(int i=0; i<selectedItems.size(); i++) {
                if(!QFile::remove(actDir + selectedItems[i]->text(IncomingItem::FILENAME_COL))) {
                    QMessageBox::critical(juicer, "Error", tr("Could not remove %1.").arg(actDir + selectedItems[i]->text(IncomingItem::FILENAME_COL)));
                }
            }
            reload();
        }
    }
}


/*!
    \fn IncomingModule::storeFtp()
 */
void IncomingModule::storeFtp() {
    QString filename, localDir;
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    QString dir = OptionsDialog::getSetting("ftp", "inDir", "/").toString();
    // TODO: check if the core filesystem separator is a better choice
    if(! dir.endsWith('/')) {
        dir += '/';
    }
    FTP* ftp = new FTP(this);
    for(int  i=0; i<selectedItems.size(); i++) {
        filename = selectedItems.at(i)->text(IncomingItem::FILENAME_COL);
        localDir = QFileDialog::getExistingDirectory(juicer, tr("copy")+"\"" + filename + "\" "+tr("to"));
        if(localDir != "") {
            if(!localDir.endsWith(QDir::separator())) {
                localDir += QDir::separator();
            }
            QFile* dstFile = new QFile(localDir + filename);
            if(!dstFile->exists()) {
                dstFile->open(QIODevice::WriteOnly);
                ftp->add(dir + filename, dstFile);
            } else {
                QMessageBox::critical(juicer, tr("error"), tr("\"%1\" already exists").arg(dstFile->fileName()), QMessageBox::Ok, QMessageBox::NoButton);
            }
        }
    }
    ftp->start();
}


/*!
    \fn IncomingModule::openFtp()
 */
void IncomingModule::openFtp() {
    QString filename;
    QString dir = OptionsDialog::getSetting("ftp", "inDir", "/").toString();
    // TODO: check if the core filesystem separator is a better choice
    if (! dir.endsWith('/')) {
        dir += '/';
    }
    FTP* ftp = new FTP(this);
    if(OptionsDialog::getSetting("ftp", "full", false).toBool()) {
        connect(ftp, SIGNAL(downloadFinished(QFile*, FTP*)), this, SLOT(ftpReadyRead(QFile*, FTP*)));
    } else {
        connect(ftp, SIGNAL(readyRead(QFile*, FTP*)), this, SLOT(ftpReadyRead(QFile*, FTP*)));
    }
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        filename = selectedItems.at(i)->text(IncomingItem::FILENAME_COL);
        ftp->add(dir + filename);
    }
    ftp->start();
}


/*!
    \fn IncomingModule::reload()
 */
void IncomingModule::reload() {
    treeWidget->clear();
    waitLabel->setVisible(false);
    AjSettings::LOCATION location = getLocation();
    if(location == AjSettings::FTP) {
        reloadFtp();
    } else {
        QString actDir;
        if(location == AjSettings::SAME) {
            if(this->dir.isEmpty()) {
                QLabel* l = new QLabel(tr("There is no connection to the core.\nYou may want to define the incoming directory manually at the options dialog."), treeWidget);
                l->adjustSize();
                l->move((treeWidget->width() - l->width()) / 2, (treeWidget->height() - l->height()) / 2);
                l->show();
                return;
            }
            actDir = this->dir;
        } else {
            actDir = OptionsDialog::getSetting("incomingDirSpecific", "/").toString();
        }
        QDir dirDir(actDir);
        if(dirDir.exists()) {
            QFileIconProvider fileIconProvider;
            QFileInfoList list = dirDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
            for(int i=0; i<list.size(); i++) {
                QFileInfo& file = list[i];
                IncomingItem* item = new IncomingItem(file, treeWidget);
                treeWidget->addTopLevelItem(item);
            }
        }
    }
    adjustSizeOfColumns();
}


/*!
    \fn IncomingModule::setDir(const QString& dir)
 */
void IncomingModule::setDir(const QString& dir) {
    this->dir = dir;
}


/*!
    \fn IncomingModule::open()
 */
void IncomingModule::open() {
    QStringList args = Juicer::getExec();
    QString exec = args.takeFirst();
    QString actDir;
    // determine the path
    AjSettings::LOCATION location = getLocation();
    if(location == AjSettings::SPECIFIC) {
        actDir = OptionsDialog::getSetting("incomingDirSpecific", "/").toString() + QDir::separator();
    } else if(location == AjSettings::SAME) {
        actDir = this->dir + QDir::separator();
    } else {  // ftp
        return openFtp();
    }
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++) {
        args <<  actDir + selectedItems[i]->text(IncomingItem::FILENAME_COL);
        QProcess::startDetached(exec, args);
        args.removeLast();
    }
}

/*!
    \fn IncomingModule::insert(QUrlInfo info)
 */
void IncomingModule::insert(QUrlInfo info) {
    waitLabel->setVisible(false);
    if(info.isFile() || info.isDir()) {
        new IncomingItem(info, treeWidget);
        adjustSizeOfColumns();
    }
}


/*!
    \fn IncomingModule::getLocation()
 */
AjSettings::LOCATION IncomingModule::getLocation() {
    return (AjSettings::LOCATION)OptionsDialog::getSetting("location", AjSettings::SAME).toInt();
}


/*!
    \fn IncomingModule::ftpReadyRead(QFile* dstFile, FTP* ftp)
 */
void IncomingModule::ftpReadyRead(QFile* dstFile, FTP* ftp) {
    ftp = ftp;
    QStringList args = Juicer::getExec();
    QString exec = args.takeFirst();
    args << dstFile->fileName();
    QProcess::startDetached(exec, args);
}

/*!
    \fn IncomingModule::confirmRemove(QList<QTreeWidgetItem *>& items)
 */
bool IncomingModule::confirmRemove(QList<QTreeWidgetItem *>& items) {
    int maxFilesToShow = 10;
    QString list = "<b>"+tr("Delete %n file(s)?", "", items.size()) + "</b><br>";
    for(int i=0; i<items.size() && i<maxFilesToShow; i++) {
        list += "<br>" + items[i]->text(IncomingItem::FILENAME_COL);
    }
    if(items.size() > maxFilesToShow) {
        list += "<br>(" +tr("%1 more").arg(QString::number(items.size() - maxFilesToShow))+")";
    }
    return QMessageBox::question(juicer, "Confirmation", list, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes;
}

void IncomingModule::selectionChanged() {
    bool oneSelected = !treeWidget->selectedItems().empty();
    juicer->actionOpen_Incoming->setEnabled(oneSelected);
    juicer->actionCopy_Incoming->setEnabled(oneSelected);
    juicer->actionDelete_Incoming->setEnabled(oneSelected);
}
