//
// C++ Interface: autoupdate
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
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

#include "quazip.h"
#include "quazipfile.h"
#include "updatedialog.h"

/**
    @author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class AutoUpdate : public QObject
{
Q_OBJECT
public:
    AutoUpdate(const QString& appPath, QWidget *parent = 0);

    ~AutoUpdate();
    void check();
private:
    QDateTime compileTime, releaseTime;
    QHttp http;
    int checkId, getId;
    UpdateDialog* updateDialog;
    QTemporaryFile file;
    QString appPath;
    QString updateFilename;
    bool updatePossible;
private slots:
    void requestFinished(int id, bool error);
    void dataReadProgress(int done, int total);
};

#endif
