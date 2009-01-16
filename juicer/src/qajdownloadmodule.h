//
// C++ Interface: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef QAJDOWNLOADMODULE_H
#define QAJDOWNLOADMODULE_H

#include <QObject>
#include <QInputDialog>
#include <QClipboard>

#include "qxmlmodule.h"
#include "qajmodulebase.h"
#include "qajdownloaditem.h"
#include "qajuseritem.h"
#include "qconvert.h"
#include "ftp.h"
#include "qajoptionsdialog.h"


class QAjDownloadModule : public QAjModuleBase {
  Q_OBJECT
public:
    QAjDownloadModule(Juicer* juicer);
    ~QAjDownloadModule();
    void insertDownload(const QString& id, const QString& hash, const QString& fileName, const QString& status,
                        const QString& size, const QString& ready, const QString& power, const QString& tempNumber);
    void insertUser(const QString& downloadId, const QString& id, const QString& fileName, const QString& speed,
                        const QString& status, const QString& power, const QString& queuePos, const QString& os, QTime& time);
    bool remove(const QString& id);
    bool removeDownload(const QString& id);
    QAjDownloadItem* findDownload(const QString& id);
    QAjDownloadItem* findDownload(const QString& size, const QString& hash);
    QMenu *popup;
    QString getNextIdRoundRobin();
    void setDirs(const QFileInfo& tmpDir, const QFileInfo& inDir);
    QString findDownloadByTempNum(const QFileInfo& tempFile);
public slots:
    void updateView(bool force = false);
protected:
    void processSelected(QXMLModule::Type type, const QString& request, const QString& para = "");
    void getSelected(const QString& request, const QString& para = "");
    void setSelected(const QString& request, const QString& para = "");

    QHash<QString, QAjDownloadItem*> downloads;
    int currIdRoundRobin;
    QDoubleSpinBox* powerSpin;
    QCheckBox *powerCheck;
    QHash<QString, QString> userStatusDescr;
    QHash<QString, QString> downloadStatusDescr;
    QFileInfo tempDir, incomingDir;

    class DownloadUser {
        public:
        DownloadUser() {download = NULL; user = NULL; }
        QAjDownloadItem* download;
        QAjUserItem* user;
    };
    QAjDownloadModule::DownloadUser findParent(const QString& id);

protected slots:
    void cancelSlot();
    void cleanSlot();
    void resumeSlot();
    void pauseSlot();
    void partListSlot();
    void renameSlot();
    void renamePlusSlot();
    void openSlot();
    void linkSlot();
    void applyPowerDownload();
    void applyPowerDownload(const QString& id, double value);
    void maxPowerDownload();
    void selectionChanged();
    void storeDownloadFtp();
    void hidePausedSlot(bool checked);
signals:
    void downloadsFinished( const QList<QAjDownloadItem*>& downloads );
};


#endif

