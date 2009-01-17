//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "qajdownloadmodule.h"
#include "juicer.h"

QAjDownloadModule::QAjDownloadModule(Juicer* juicer) : QAjModuleBase(juicer, juicer->downloadsTreeWidget, juicer->downloadToolBar) {
    userStatusDescr["1"] = QObject::tr("unasked");
    userStatusDescr["2"] = QObject::tr("try to connect");
    userStatusDescr["3"] = QObject::tr("partner have to old vers.");
    userStatusDescr["4"] = QObject::tr("partner can't    open file");
    userStatusDescr["5"] = QObject::tr("queueing");
    userStatusDescr["6"] = QObject::tr("no usable parts");
    userStatusDescr["7"] = QObject::tr("loading");
    userStatusDescr["8"] = QObject::tr("not enough disk space");
    userStatusDescr["9"] = QObject::tr("finished");
    userStatusDescr["11"] = QObject::tr("no connection possible");
    userStatusDescr["12"] = QObject::tr("try indirect");
    userStatusDescr["13"] = QObject::tr("paused");
    userStatusDescr["14"] = QObject::tr("queue full");
    userStatusDescr["15"] = QObject::tr("own limit reached");
    userStatusDescr["16"] = QObject::tr("indirect conn. rejected");
    downloadStatusDescr["-1"] = QObject::tr("loading");
    downloadStatusDescr["0"] = QObject::tr("searching");
    downloadStatusDescr["1"] = QObject::tr("error at creating");
    downloadStatusDescr["12"] = QObject::tr("finishing");
    downloadStatusDescr["13"] = QObject::tr("error at finishing");
    downloadStatusDescr["14"] = QObject::tr("finished");
    downloadStatusDescr["15"] = QObject::tr("canceling");
    downloadStatusDescr["16"] = QObject::tr("creating .dat");
    downloadStatusDescr["17"] = QObject::tr("canceled");
    downloadStatusDescr["18"] = QObject::tr("paused");

    currIdRoundRobin = -1;

    powerCheck = new QCheckBox(tr("Power Download:"), juicer->downloadToolBar);
    powerCheck->setChecked(false);
    juicer->downloadToolBar->addWidget(powerCheck);

    powerSpin = new QDoubleSpinBox(juicer->downloadToolBar);
    powerSpin->setRange( 2.2, 50.0 );
    powerSpin->setSingleStep( 0.1 );
    powerSpin->setDecimals( 1 );
    juicer->downloadToolBar->addWidget(powerSpin);

    connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
    connect(powerSpin, SIGNAL(valueChanged(const QString&)), this, SLOT(applyPowerDownload()));
    connect(powerSpin, SIGNAL(valueChanged(double)), this, SLOT(applyPowerDownload()));
    connect(juicer->actionPause, SIGNAL(triggered()), this, SLOT(pauseSlot()));
    connect(juicer->actionResume, SIGNAL(triggered()), this, SLOT(resumeSlot()));
    connect(juicer->actionCancel, SIGNAL(triggered()), this, SLOT(cancelSlot()));
    connect(juicer->actionShow_Part_List, SIGNAL(triggered()), this, SLOT(partListWidgetSlot()));
    connect(juicer->actionShow_Part_List, SIGNAL(triggered()), this, SLOT(partListSlot()));
    connect(juicer->actionRename, SIGNAL(triggered()), this, SLOT(renameSlot()));
    connect(juicer->actionRename_By_Clipboard, SIGNAL(triggered()), this, SLOT(renamePlusSlot()));
    connect(juicer->actionOpen, SIGNAL(triggered()), this, SLOT(openSlot()));
    connect(juicer->actionCopy_Link, SIGNAL(triggered()), this, SLOT(linkSlot()));
    connect(juicer->actionRemove_Finished, SIGNAL(triggered()), this, SLOT(cleanSlot()));
    connect(juicer->actionCreate_Link_List, SIGNAL(triggered()), this, SLOT(linkListSlot()));
    connect(juicer->actionHide_Paused, SIGNAL(triggered(bool)), this, SLOT(hidePausedSlot(bool)));
    connect(juicer->actionMaximal_Power, SIGNAL(triggered()), this, SLOT(maxPowerDownload()));

    connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(partListSlot()));

    connect(juicer->actionShow_Part_List_Dock, SIGNAL(triggered(bool)), juicer->partListDock, SLOT(setVisible(bool)));
    connect(juicer->partListDock, SIGNAL(visibilityChanged(bool)), juicer->actionShow_Part_List_Dock, SLOT(setChecked(bool)));
    connect(juicer->actionShow_Part_List_Dock, SIGNAL(triggered(bool)), this, SLOT(partListDockVisible(bool)));
    connect(juicer->partListDock->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(partListDockVisible(bool)));


    #ifdef AJQTGUI_MODE_SPECIAL
        juicer->actionMaximal_Power->setVisible(true);
    #else
        char* mode = getenv("AJQTGUI_MODE");
        juicer->actionMaximal_Power->setVisible(((mode != NULL)) && (strcmp(mode, "SPECIAL") == 0 ));
    #endif

    selectionChanged();
}

QAjDownloadModule::~QAjDownloadModule() {
}

/*!
    \fn QAjDownloadModule::insertDownload(const QString& id, const QString& hash, const QString& fileName, const QString& status, const QString& size, const QString& ready, const QString& power, const QString& tempNumber)
 */
void QAjDownloadModule::insertDownload(const QString& id, const QString& hash, const QString& fileName, const QString& status,
                        const QString& size, const QString& ready, const QString& power, const QString& tempNumber) {
    QAjDownloadItem *downloadItem = findDownload( id );
    if(downloadItem == NULL) {
        downloadItem = new QAjDownloadItem( id, treeWidget );
        downloads[id] = downloadItem;
        if(juicer->actionMaximal_Power->isVisible()) {
            xml->set("setpowerdownload", "&Powerdownload="+QConvert::power(50)+"&id="+id);
        }
        downloadItem->update(hash, fileName, status, size, ready, power, tempNumber);
        updateView(true);
        connect(downloadItem->powerSpin, SIGNAL(powerChanged(QString, double)), this, SLOT(applyPowerDownload(const QString &, double)));
    } else {
        downloadItem->update(hash, fileName, status, size, ready, power, tempNumber);
    }
}


/*!
    \fn QAjDownloadModule::insertUser(const QString& downloadId, const QString& id, const QString& fileName, const QString& speed, const QString& status, const QString& power, const QString& queuePos, const QString& os, QTime& time)
 */
void QAjDownloadModule::insertUser(const QString& downloadId, const QString& id, const QString& fileName, const QString& speed,
                        const QString& status, const QString& power, const QString& queuePos, const QString& os, QTime& time) {
    QAjDownloadItem *downloadItem = findDownload(downloadId);
    if(downloadItem == NULL) {  // -- this shouldn't happen, just in case... --
        insertDownload(downloadId, "", "", "", "", "", "", "");
        downloadItem = findDownload(downloadId);
    }
    downloadItem->updateUser(id, fileName, speed, status, power, queuePos, userStatusDescr[status], juicer->osIcons[os], time);
}

/*!
    \fn QAjDownloadModule::updateView( bool force )
 */
void QAjDownloadModule::updateView( bool force ) {
    QList<QAjDownloadItem*> finished;
    if( force || treeWidget->isVisible() )
    {
        int i;
        for (i=0; i < treeWidget->topLevelItemCount(); i++)
        {
            QAjDownloadItem* item = ((QAjDownloadItem*)treeWidget->topLevelItem(i));
            if( item->updateView( &downloadStatusDescr ) )
                finished << item;
        }
    }
    if( ! finished.isEmpty() )
        downloadsFinished( finished );
}

/*!
    \fn QAjDownloadModule::findDownload(const QString& id)
 */
QAjDownloadItem* QAjDownloadModule::findDownload(const QString& id) {
    if (downloads.contains( id )) {
        return downloads[id];
    }
    return NULL;
}

/*!
    \fn QAjDownloadModule::findDownload(const QString& size, const QString& hash)
 */
QAjDownloadItem* QAjDownloadModule::findDownload(const QString& size, const QString& hash) {
    QHash<QString,QAjDownloadItem*>::const_iterator i;
    for(i = downloads.constBegin(); i != downloads.constEnd(); i++) {
        if(QString::number((int)(*i)->getSize()) == size && (*i)->getHash() == hash)
            return *i;
    }
    return NULL;
}

/*!
    \fn QAjDownloadModule::remove(const QString& id)
    removes either download or user
 */
bool QAjDownloadModule::remove(const QString& id)
{
    if(removeDownload(id)) {
        return true;
    } else {
        DownloadUser du = findParent(id);
        if(du.user != NULL) {
            du.download->removeUser(id);
            return true;
        }
    }
    return false;
}

/*!
    \fn QAjDownloadModule::removeDownload(const QString& id)
    removes a download
 */
bool QAjDownloadModule::removeDownload(const QString& id)
{
    if(downloads.contains(id)) {
        // -- first remove it form the hashtable, than delete it --
        QAjDownloadItem* item = downloads[id];
        downloads.remove(id);
        delete item;
        return true;
    }
    return false;
}

/*!
    \fn QAjDownloadModule::findParent(const QString& id)
 */
QAjDownloadModule::DownloadUser QAjDownloadModule::findParent(const QString& id) {
    DownloadUser du;
    QHash<QString,QAjDownloadItem*>::const_iterator i;
    for(i = downloads.constBegin(); i != downloads.constEnd(); i++) {
        du.download = i.value();
        du.user = du.download->findUser(id);
        if(du.user != NULL) {
            break;
        }
    }
    return du;
}

/*!
    \fn QAjDownloadModule::processSelected(QXMLModule::Type type, const QString& request, const QString& para)
 */
void QAjDownloadModule::processSelected(QXMLModule::Type type, const QString& request, const QString& para) {
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    for(QList<QTreeWidgetItem*>::iterator i = items.begin(); i!=items.end(); i++) {
        xml->make(type, request, para + "&id=" + ((QAjItem*)(*i))->getId());
    }
}

/*!
    \fn QAjDownloadModule::getSelected(const QString& request, const QString& para)
 */
void QAjDownloadModule::getSelected(const QString& request, const QString& para) {
    processSelected(QXMLModule::GET, request, para);
}

/*!
    \fn QAjDownloadModule::setSelected(const QString& request, const QString& para)
 */
void QAjDownloadModule::setSelected(const QString& request, const QString& para) {
    processSelected(QXMLModule::SET, request, para);
}

void QAjDownloadModule::cancelSlot() {
    QString text = tr("Do you realy want to cancel") + " "
                    + QString::number(treeWidget->selectedItems().size()) + " "
                    + (treeWidget->selectedItems().size()>1?tr("downloads?"):tr("download?"));
    if(QMessageBox::question(juicer, tr("Confirm"), text, QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
        setSelected("canceldownload");
    }
}

void QAjDownloadModule::cleanSlot() {
    xml->set("cleandownloadlist");
}

void QAjDownloadModule::resumeSlot() {
    setSelected("resumedownload");
}

void QAjDownloadModule::pauseSlot() {
    setSelected("pausedownload");
}

void QAjDownloadModule::partListSlot() {
    getSelected("downloadpartlist");
}

void QAjDownloadModule::renameSlot() {
    QString oldFilename, newFilename;
    bool ok;

    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    QList<QTreeWidgetItem *>::iterator item;
    for(item = selectedItems.begin(); item != selectedItems.end(); item++) {
        oldFilename = (*item)->text(QAjDownloadItem::FILENAME_COL);
        newFilename = QInputDialog::getText(juicer, tr("rename download"), tr("enter new filename for ") + oldFilename, QLineEdit::Normal, oldFilename, &ok);
        newFilename = QString( QUrl::toPercentEncoding(newFilename));
        if(ok && !newFilename.isEmpty()) {
            xml->set("renamedownload", "&id=" + ((QAjItem*)(*item))->getId() + "&name=" + newFilename);
        }
    }
}

void QAjDownloadModule::renamePlusSlot() {
    QString oldFilename, newFilename;
    QString newFilenameBase = qApp->clipboard()->text( QClipboard::Clipboard );

    int i = 0;
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    QList<QTreeWidgetItem *>::iterator item;
    for(item = selectedItems.begin(); item != selectedItems.end(); item++) {
        oldFilename = (*item)->text(QAjDownloadItem::FILENAME_COL);
        newFilename = newFilenameBase;
        if(selectedItems.size() > 1) {
            newFilename += "_" + QString::number(i+1);
        }
        QStringList s = oldFilename.split(".");
        if(s.size() > 1) {
            newFilename += "." + s[s.size() - 1];
        }
        newFilename = QString(QUrl::toPercentEncoding(newFilename));
        if(!newFilename.isEmpty()) {
            xml->set("renamedownload", "&id=" + ((QAjItem*)(*item))->getId() + "&name=" + newFilename);
        }
        i++;
    }
}

void QAjDownloadModule::openSlot() {
    QStringList args = Juicer::getExec();
    QString exec = args.takeFirst();

    QString iDir, tDir;
    // -- determine the path --
    AjSettings::LOCATION location = (AjSettings::LOCATION)QAjOptionsDialog::getSetting( "location", AjSettings::SAME ).toInt();

    if(location == AjSettings::SPECIFIC) {
        iDir = QAjOptionsDialog::getSetting("incomingDirSpecific", "/").toString() + QDir::separator();
        tDir = QAjOptionsDialog::getSetting("tempDirSpecific", "/").toString() + QDir::separator();
    }
    else if(location == AjSettings::SAME) {
        iDir = incomingDir.absolutePath() + QDir::separator();
        tDir = tempDir.absolutePath() + QDir::separator();
    }
    // -- ftp --
    else {
        // TODO
        return;
    }

    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    QList<QTreeWidgetItem *>::iterator item;
    for(item = selectedItems.begin(); item != selectedItems.end(); item++) {
        QAjDownloadItem* ajDownloadItem = (QAjDownloadItem*)(*item);
        if(ajDownloadItem->getStatus() == DOWN_FINISHED) {
            args.push_back(iDir + ajDownloadItem->text(QAjDownloadItem::FILENAME_COL));
        } else {
            args.push_back(tDir + ajDownloadItem->getTempNumber() + ".data");
        }
        QProcess::startDetached(exec, args);
        args.pop_back();
    }
}

void QAjDownloadModule::linkSlot() {
    QString link = ((QAjDownloadItem*)(treeWidget->selectedItems()[0]))->getLinkAJFSP();
    QApplication::clipboard()->setText(link);
}

/*!
    \fn QAjDownloadModule::applyPowerDownload()
 */
void QAjDownloadModule::applyPowerDownload() {
    float value = powerCheck->isChecked()?powerSpin->value():1.0;
    setSelected("setpowerdownload", "&Powerdownload=" + QConvert::power(value));
}

/*!
    \fn QAjDownloadModule::applyPowerDownload(const QString& id, double value)
 */
void QAjDownloadModule::applyPowerDownload(const QString& id, double value) {
    xml->set("setpowerdownload", "&Powerdownload="+QConvert::power( value )+"&id="+id);
}

/*!
    \fn QAjDownloadModule::maxPowerDownload()
 */
void QAjDownloadModule::maxPowerDownload() {
    QList<QString> ids = downloads.keys();
    for(QList<QString>::iterator i = ids.begin(); i != ids.end(); i++) {
        xml->set("setpowerdownload", "&Powerdownload="+QConvert::power( 50 )+"&id="+(*i));
    }
}

/*!
    \fn QAjDownloadModule::hidePausedSlot(bool checked)
 */
void QAjDownloadModule::hidePausedSlot(bool checked) {
    QHash<QString,QAjDownloadItem*>::const_iterator i;
    for(i = downloads.constBegin(); i != downloads.constEnd(); i++) {
        (*i)->setHidden(checked && ((*i)->getStatus() == DOWN_PAUSED));
    }
}

/*!
    \fn QAjDownloadModule::selectionChanged()
 */
void QAjDownloadModule::selectionChanged() {
    bool onePaused = false;
    bool oneActive = false;
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    bool oneSelected = !selectedItems.empty();
    QList<QTreeWidgetItem *>::iterator i;
    for(i = selectedItems.begin(); i != selectedItems.end(); i++) {
        QAjItem* downloadItem = (QAjItem*)(*i);
        if(downloadItem->getStatus() == DOWN_PAUSED) {
            onePaused = true;
        }
        if((downloadItem->getStatus() == DOWN_SEARCHING) || (downloadItem->getStatus() == DOWN_LOADING)) {
            oneActive = true;
        }
        if(onePaused && oneActive) {
            break;
        }
    }
    juicer->actionPause->setEnabled(oneActive);
    juicer->actionResume->setEnabled(onePaused);
    juicer->actionCancel->setEnabled(oneSelected);
    juicer->actionShow_Part_List->setEnabled(oneSelected);
    juicer->actionRename->setEnabled(oneSelected);
    juicer->actionRename_By_Clipboard->setEnabled(oneSelected);
    juicer->actionOpen->setEnabled(oneSelected);
    juicer->actionCopy_Link->setEnabled(oneSelected);
    juicer->actionCreate_Link_List->setEnabled(oneSelected);
    if(!oneSelected) {
        juicer->partsWidget->clear();
    }
}


/*!
    \fn QAjDownloadModule::getNextIdRoundRobin()
 */
QString QAjDownloadModule::getNextIdRoundRobin() {
    if(treeWidget->topLevelItemCount() < 1) {
        return "";
    }
    currIdRoundRobin ++;
    currIdRoundRobin %= treeWidget->topLevelItemCount();
    return ((QAjDownloadItem*)treeWidget->topLevelItem(currIdRoundRobin))->getId();
}


/*!
    \fn QAjDownloadModule::setDirs( const QFileInfo& tmpDir, const QFileInfo& inDir )
 */
void QAjDownloadModule::setDirs( const QFileInfo& tmpDir, const QFileInfo& inDir ) {
    this->tempDir = tmpDir;
    this->incomingDir = inDir;
}


/*!
    \fn QAjDownloadModule::findDownloadByTempNum(const QFileInfo& tempFile)
 */
QString QAjDownloadModule::findDownloadByTempNum(const QFileInfo& tempFile) {
    if(tempDir.absolutePath() == tempFile.absolutePath()) {
        QString tempNum = tempFile.baseName();
        QHash<QString,QAjDownloadItem*>::const_iterator i;
        for(i = downloads.constBegin(); i != downloads.constEnd(); i++) {
            if((*i)->getTempNumber() == tempNum) {
                return (*i)->text(QAjDownloadItem::FILENAME_COL);
            }
        }
    }
    return tempFile.fileName();
}

/*!
    \fn QAjDownloadModule::storeDownloadFtp()
 */
void QAjDownloadModule::storeDownloadFtp() {
    FTP* ftp = NULL;
    QString filename, localDir;
    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();

    QString dir = QAjOptionsDialog::getSetting( "ftp", "dir", "/" ).toString();

    if(!dir.endsWith('/')) {
        dir += '/';
    }
    ftp = new FTP(this);

    QList<QTreeWidgetItem *>::iterator item;
    for(item = selectedItems.begin(); item != selectedItems.end(); item++) {
        filename = (*item)->text(QAjDownloadItem::FILENAME_COL);
        localDir = QFileDialog::getExistingDirectory(juicer, "save \"" + filename + "\" + to");
        if ( localDir != "" ) {
            if(!localDir.endsWith(QDir::separator())) {
                localDir += QDir::separator();
            }
            QFile* dstFile = new QFile( localDir + filename );
            if(!dstFile->exists()) {
                dstFile->open(QIODevice::WriteOnly);
                ftp->add(dir + filename, dstFile);
            }
        }
    }
    ftp->start();
}


/*!
    \fn QAjDownloadModule::setPartList(const QString& id, qulonglong size, QLinkedList<QAjPartsWidget::Part>& partList)
 */
void QAjDownloadModule::setPartList(const QString& id, qulonglong size, QLinkedList<QAjPartsWidget::Part>& partList) {
    QAjDownloadItem* item = findDownload(id);
    if( item != NULL ) {
        if(item->getPartListDialog()->isVisible()) {
            item->getPartListDialog()->update(size, partList);
        }
        if(juicer->partsWidget->isVisible() && item->isSelected()) {
            juicer->partsWidget->update(size, partList);
        }
    }
}


/*!
    \fn QAjDownloadModule::partListWidgetSlot()
 */
void QAjDownloadModule::partListWidgetSlot()
{
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    for(QList<QTreeWidgetItem*>::iterator i = items.begin(); i!=items.end(); i++) {
         ((QAjDownloadItem*)(*i))->getPartListDialog()->show();
    }

}

/*!
    \fn QAjDownloadModule::partListDockVisible(bool visible)
 */
void QAjDownloadModule::partListDockVisible(bool visible) {
    QAjOptionsDialog::setSetting("PartListDock", "visible", visible);
}

