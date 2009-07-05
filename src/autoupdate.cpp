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

#include "autoupdate.h"
#include <QDebug>

AutoUpdate::AutoUpdate(const QString& appPath, QWidget *parent) : QObject(parent) {
    this->appPath = appPath;
#ifdef Q_OS_WIN32
    os = "windows";
#endif
#ifdef Q_OS_MAC
    os = "mac";
#endif
#ifdef Q_OS_LINUX
    os = "linux";
#endif
    // -- 32 or 64bit? --
    if(!os.isEmpty()) {
        if(sizeof(void*) == 4) {
            os += "-32";
        } else if(sizeof(void*) == 8) {
            os += "-64";
        } else {
            os = "unsupported";
        }
    }
    updateFolder = "/update/";
    updateXML = "update.xml";
    updateDialog = new UpdateDialog(parent);
    checkId = getId = -1;
    connect(&http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
    connect(&http, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataReadProgress(int, int)));
    connect(updateDialog->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));
    http.setHost("applejuicer.net");
}


AutoUpdate::~AutoUpdate() {
    http.abort();
}


/*!
    \fn AutoUpdate::check()
 */
void AutoUpdate::check() {
    checkId = http.get(updateFolder + updateXML);
}


/*!
    \fn AutoUpdate::requestFinished(int id, bool error)
 */
void AutoUpdate::requestFinished(int id, bool error) {
    if(!error) {
        // -- got file with release time, check if newer and request update file if necessary --
        if(id == checkId) {
            checkVersion();
        // -- got update file, unzip it --
        } else if(id == getId) {
            update();
        }
    }
}


/*!
    \fn AutoUpdate::dataReadProgress(int done, int total)
 */
void AutoUpdate::dataReadProgress(int done, int total) {
    updateDialog->progressBar->setMaximum(total);
    updateDialog->progressBar->setValue(done);
}


/*!
    \fn AutoUpdate::readXML()
 */
bool AutoUpdate::readXML() {
    QDomDocument doc;
    doc.setContent(http.readAll());
    QDomElement root = doc.documentElement();
    QDomNode n;
    if(root.tagName() == "update") {
        for(n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
            QDomElement e = n.toElement();
            if(!e.isNull() && e.tagName() == "package" && e.attribute("os") == os) {
                updateVersion = e.attribute("version");
                updateFile = updateFolder + e.attribute("file");
                return true;
            }
        }
    }
    return false;
}


/*!
    \fn AutoUpdate::clicked(QAbstractButton * button)
 */
void AutoUpdate::clicked(QAbstractButton * button) {
    if(button == updateDialog->buttonBox->button(QDialogButtonBox::Yes)) {
        updateDialog->buttonBox->clear();
        updateDialog->buttonBox->addButton(QDialogButtonBox::Cancel);
        updateDialog->progressBar->setEnabled(true);
        updateDialog->textEdit->append("download update archive...");
        getId = http.get(updateFile, &file);
    } else if(button == updateDialog->buttonBox->button(QDialogButtonBox::No)) {
        updateDialog->hide();
    } else if(button == updateDialog->buttonBox->button(QDialogButtonBox::Cancel)) {
        http.abort();
    }
}


/*!
    \fn AutoUpdate::update()
 */
void AutoUpdate::update() {
    updateDialog->textEdit->append("extract update archive...");
    QDir outDir(QDir::tempPath() + QDir::separator() + "juicer_update" + QDateTime::currentDateTime().toString());
    for(int i=0; i<3 && !outDir.exists(); i++) {
        outDir.setPath(QDir::tempPath() + QDir::separator() + "juicer_update" + QDateTime::currentDateTime().toString());
    }
    if(outDir.exists()) {
        updateDialog->textEdit->append("Update failed, could not create a temporary directory.");
        return;
    }
    QString fileName = file.fileName();
    file.close();
    QuaZip zip(fileName);
    zip.open(QuaZip::mdUnzip);
    updateDialog->progressBar->setMaximum(zip.getEntriesCount());
    QuaZipFile file(&zip);
    QList<QFileInfo> files;
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile()) {
        files.append(file.getActualFileName());
        QFileInfo& info = files.last();
        updateDialog->textEdit->append(info.filePath());
        if(!outDir.mkpath(info.path())) {
            updateDialog->textEdit->append("error during extracting archive");
            return;
        }
        file.open(QIODevice::ReadOnly);
        QFile outFile(outDir.absolutePath()+QDir::separator()+info.filePath());
        outFile.open(QIODevice::WriteOnly);
        outFile.write(file.readAll());
        outFile.close();
        file.close();
        updateDialog->progressBar->setValue(files.size());
    }
    zip.close();
    // -- rename old files and copy new files in --
    updateDialog->textEdit->append("replace old files...");
    for(int i=0; i<files.size(); i++) {
        QString origFile = appPath + QDir::separator() + files.at(i).filePath();
        QString newFile = outDir.absolutePath() + QDir::separator() + files.at(i).filePath();
        QString bakFile = origFile + ".bak";
        if(QFile::exists(origFile)) {
            QFile::rename(origFile, bakFile);
            QFile::setPermissions(newFile, QFile::permissions(bakFile));
        }
        QFile::copy(newFile, origFile);
        updateDialog->textEdit->append(newFile + " => " + origFile);
    }
    updateDialog->textEdit->append("done");
    updateDialog->textEdit->append("Please restart Juicer in order to finish the update process.");
    updateDialog->buttonBox->button(QDialogButtonBox::Cancel)->setText("done");
}


/*!
    \fn AutoUpdate::checkVersion()
 */
void AutoUpdate::checkVersion() {
    if(readXML()) {
        if(Convert::compareVersion(updateVersion, qApp->applicationVersion()) > 0) {
            updateDialog->buttonBox->clear();
            updateDialog->buttonBox->addButton(QDialogButtonBox::Yes);
            updateDialog->buttonBox->addButton(QDialogButtonBox::No);
            updateDialog->progressBar->setValue(0);
            updateDialog->progressBar->setEnabled(false);
            updateDialog->textEdit->clear();
            updateDialog->textEdit->append("An update for Juicer is available.");
            updateDialog->textEdit->append("Would you like to update Juicer now?");
            updateDialog->show();
        }
    }
}
