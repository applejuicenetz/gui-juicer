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
    updatePossible = !os.isEmpty();
    updateFolder = "/update/";
    updateXML = "update.xml";
    // -- disabled because insufficiently tested --
      updatePossible = false;
    // --------------------------------------------

    if(updatePossible) {
        updateDialog = new UpdateDialog(parent);
        checkId = getId = -1;
        connect(&http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
        connect(&http, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataReadProgress(int, int)));
        http.setHost("applejuicer.net");
    }
}


AutoUpdate::~AutoUpdate() {
    http.abort();
}


/*!
    \fn AutoUpdate::check()
 */
void AutoUpdate::check() {
    if(updatePossible) {
        checkId = http.get(updateFolder + updateXML);
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
        QString updateVersion, updateFile;
        if(readXML(updateVersion, updateFile)) {
            qDebug() << "current version: " << qApp->applicationVersion();
            qDebug() << "update version:  " << updateVersion;

            if(Convert::compareVersion(updateVersion, qApp->applicationVersion()) > 0) {
                qDebug() << "new!";
                updateDialog->progressBar->setValue(0);
                updateDialog->textEdit->clear();
                updateDialog->show();
                updateDialog->textEdit->append("download update archive...");
                getId = http.get(updateFile, &file);
            }
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
        updateDialog->textEdit->append("Please restart Juicer in order to finish the update process.");
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
    \fn AutoUpdate::readXML(QString& version, QString& file)
 */
bool AutoUpdate::readXML(QString& version, QString& file) {
    QDomDocument doc;
    doc.setContent(http.readAll());
    QDomElement root = doc.documentElement();
    QDomNode n;
    if(root.tagName() == "update") {
        for(n = root.firstChild(); !n.isNull(); n = n.nextSibling()) {
            QDomElement e = n.toElement();
            if(!e.isNull() && e.tagName() == "package" && e.attribute("os") == os) {
                version = e.attribute("version");
                file = updateFolder + e.attribute("file");
                return true;
            }
        }
    }
    return false;
}
