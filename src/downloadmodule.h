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

#include "xmlmodule.h"
#include "modulebase.h"
#include "downloaditem.h"
#include "useritem.h"
#include "convert.h"
#include "ftp.h"
#include "optionsdialog.h"
#include "targetfolderdialog.h"

class DownloadModule : public ModuleBase {
  Q_OBJECT

public:
    DownloadModule(Juicer* juicer);
    ~DownloadModule();
    void insertDownload(const QString& id, const QString& hash, const QString& fileName,
                        const QString& status, const QString& size, const QString& ready,
                        const QString& power, const QString& tempNumber,
                        const QString& targetDir);
    void insertUser(  const QString& downloadId, const QString& id, const QString& fileName,
                      const QString& nickname, const QString& speed, const QString& status,
                      const QString& power, const QString& queuePos, const QString& os,
                      const QString& downloadfrom, const QString& downloadto,
                      const QString& actualdownloadposition, QTime& time);
    bool remove(const QString& id);
    bool removeDownload(const QString& id);
    DownloadItem* findDownload(const QString& id);
    DownloadItem* findDownload(const QString& size, const QString& hash);
    QMenu *popup;
    QString getNextIdRoundRobin();
    void setDirs(const QString& tmpDir, const QString& inDir);
    QString findDownloadByTempNum(const QString& tempFile);
    void setPartList( const QString& id, qulonglong size,
                      QLinkedList<PartsWidget::Part>& partList);
public slots:
    void updateView(bool force = false);
    void partListSlot();
protected:
    void processSelected( XMLModule::Type type, const QString& request,
                          const QString& para = "");
    void getSelected(const QString& request, const QString& para = "");
    void setSelected(const QString& request, const QString& para = "");

    QHash<QString, DownloadItem*> downloads;
    int currIdRoundRobin;
    QDoubleSpinBox* powerSpin;
    QCheckBox *powerCheck;
    QAction *powerButtonAction;
    QHash<QString, QString> userStatusDescr;
    QHash<QString, QString> downloadStatusDescr;
    QString tempDir, incomingDir;
    bool hidePaused;

    class DownloadUser {
        public:
        DownloadUser() {download = NULL; user = NULL; }
        DownloadItem* download;
        UserItem* user;
    };
    DownloadModule::DownloadUser findParent(const QString& id);

protected slots:
    void cancelSlot();
    void cleanSlot();
    void resumeSlot();
    void pauseSlot();
    void renameSlot();
    void renamePlusSlot();
    void openSlot();
    void linkSlot();
    void setMultiPowerDownload();
    void applyPowerDownload();
    void applyPowerDownload(const QString& id, double value);
    void maxPowerDownload();
    void selectionChanged();
    void storeDownloadFtp();
    void hidePausedSlot(bool checked);
    void partListWidgetSlot();
    void targetFolder();
signals:
    void downloadsFinished( const QList<DownloadItem*>& downloads );
};


#endif

