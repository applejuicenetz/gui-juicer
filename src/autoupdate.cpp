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
    updateFilename = "update-windows.zip";
#endif
#ifdef Q_OS_MAC
    updateFilename = "update-mac.zip";
#endif
#ifdef Q_OS_LINUX
    updateFilename = "update-linux.zip";
#endif
    updatePossible = !updateFilename.isEmpty();

    // -- disabled because insufficiently tested --
    updatePossible = false;
    // --------------------------------------------

    if(updatePossible) {
        updateDialog = new UpdateDialog(parent);
        checkId = getId = -1;
        compileTime = QDateTime::fromString(QString(__DATE__) + " " + QString(__TIME__), "MMM dd yyyy hh:mm:ss");
        connect(&http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
        connect(&http, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataReadProgress(int, int)));
        http.setHost("applejuicer.net");
    }
}


AutoUpdate::~AutoUpdate() {
}


/*!
    \fn AutoUpdate::check()
 */
void AutoUpdate::check() {
    if(updatePossible) {
        checkId = http.get("/release_time");
    }
}


/*!
    \fn AutoUpdate::requestFinished(int id, bool error)
 */
void AutoUpdate::requestFinished(int id, bool error) {
    if(error) {
        fprintf(stderr, "AutoUpdate: %s\n", http.errorString().toLatin1().data());
        return;
    }
    // -- got file with release time, check if newer and request update file if necessary --
    if(id == checkId) {
        ulong releaseTime = QString(http.readAll()).toULong();
        if(releaseTime < compileTime.toTime_t()) {
            printf("newer version available\n");

            updateDialog->progressBar->setValue(0);
            updateDialog->textEdit->clear();
            updateDialog->show();
            updateDialog->textEdit->append("download update archive...");

            getId = http.get("/"+updateFilename, &file);
        }
    // -- got update file, unzip it --
    } else if(id == getId) {
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
            }
            QFile::copy(newFile, origFile);
            updateDialog->textEdit->append(newFile + " => " + origFile);
        }
        updateDialog->textEdit->append("done");
    }
}


/*!
    \fn AutoUpdate::dataReadProgress(int done, int total)
 */
void AutoUpdate::dataReadProgress(int done, int total) {
    updateDialog->progressBar->setMaximum(total);
    updateDialog->progressBar->setValue(done);
}
