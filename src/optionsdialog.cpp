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

#include "optionsdialog.h"
#include "application.h"
#include "juicer.h"

OptionsDialog::OptionsDialog(Juicer* juicer) : QDialog(juicer) {
    setupUi(this);
    this->juicer = juicer;
#ifndef Q_WS_WIN
    handlerGroupBox->setHidden(true);
#endif
    IconWidget* l = new IconWidget(":/options/core.png", tr("Core"), tr("Core Settings"), QBoxLayout::TopToBottom, listWidget);
    QListWidgetItem* item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/limits.png", tr("Limits"), tr("Limitations"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/appearance.png", tr("Appearance"), tr("Appearance of the GUI"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/behaviour.png", tr("Behaviour"), tr("Behaviour of the GUI"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/search.png", tr("Search"), tr("Search Settings"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/launching.png", tr("Launching"), tr("Opening of Files"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/ftp.png", tr("FTP"), tr("FTP Settings"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/reset.png", tr("Reset"), tr("Configuration Reset"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    languageComboBox->addItem(QIcon(":/options/de.png"), "deutsch", "de");
    languageComboBox->addItem(QIcon(":/options/gb.png"), "english", "en");

    launchCombo->addItem(DEFAULT_LAUNCHER);
    if(DEFAULT_LAUNCHER == KDE_LAUNCHER)
        launchCombo->addItem(GNOME_LAUNCHER);

    specificRadioToggled(false);

    connect(incomingButton, SIGNAL(clicked()), this, SLOT(selectIncomingDir()));
    connect(tempButton, SIGNAL(clicked()), this, SLOT(selectTempDir()));
    connect(launcherButton, SIGNAL(clicked()), this, SLOT(selectLauncher()));
    connect(incomingSpecificButton, SIGNAL(clicked()), this, SLOT(selectIncomingDirSpecific()));
    connect(tempSpecificButton, SIGNAL(clicked()), this, SLOT(selectTempDirSpecific()));
    connect(specificRadio, SIGNAL(toggled(bool)), this, SLOT(specificRadioToggled(bool)));
    connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget , SLOT(setCurrentIndex(int)));
    connect(jumpFtpButton, SIGNAL(clicked()), this , SLOT(jumpToFtpSlot()));
    connect(fontComboBox, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(setFontSizes(const QFont&)));
    connect(this, SIGNAL(accepted()), this, SLOT(acceptedSlot()));
    connect(resetPushButton, SIGNAL(clicked()), this, SLOT(reset()));

    connect(profilesList, SIGNAL(itemSelectionChanged()), this, SLOT(profileSelectionChanged()));
    connect(addProfileButton, SIGNAL(clicked()), this, SLOT(addProfile()));
    connect(changeProfileButton, SIGNAL(clicked()), this, SLOT(changeProfile()));
    connect(removeProfileButton, SIGNAL(clicked()), this, SLOT(removeProfile()));

    listWidget->setCurrentRow(0);
    loadProfiles();
}

OptionsDialog::~OptionsDialog() {
}

AjSettings OptionsDialog::getAjSettings() {
    AjSettings settings;
    settings.nick = nickEdit->text();
    settings.xmlPort = xmlEdit->text();
    settings.incomingDir = incomingEdit->text();
    settings.tempDir = tempEdit->text();
    settings.autoconnect = autoConnect->isChecked()?"true":"false";
    settings.maxDown = QString::number(downSpin->value());
    settings.maxUp = QString::number(upSpin->value());
    settings.maxSlot = QString::number(slotSpin->value());
    settings.maxSources = QString::number(sourcesSpin->value());
    settings.maxCon = QString::number(connectionsSpin->value());
    settings.maxNewCon = QString::number(newSpin->value());
    settings.tcpPort = tcpEdit->text();
    return settings;
}

void OptionsDialog::setAjSettings(const AjSettings& settings) {
    nickEdit->setText(settings.nick);
    xmlEdit->setText(settings.xmlPort);
    incomingEdit->setText(settings.incomingDir);
    tempEdit->setText(settings.tempDir);
    autoConnect->setChecked((settings.autoconnect.toLower() == "true"));
    downSpin->setValue(settings.maxDown.toInt() / 1024);
    upSpin->setValue(settings.maxUp.toInt() / 1024);
    slotSpin->setValue(settings.maxSlot.toInt());
    sourcesSpin->setValue(settings.maxSources.toInt());
    connectionsSpin->setValue(settings.maxCon.toInt());
    newSpin->setValue(settings.maxNewCon.toInt());
    tcpEdit->setText(settings.tcpPort);
}

void OptionsDialog::setSettings() {
    passwordEdit->setText(getSetting("password", "").toString());
    coreEdit->setText(getSetting("coreAddress", "localhost").toString());
    refreshSpin->setValue(getSetting("refresh", 3).toInt());
    savePassword->setChecked(getSetting("savePassword", false).toBool());
    showSplash->setChecked(getSetting("showSplash", true).toBool());
    trayCheckBox->setChecked(getSetting("useTray", false).toBool());
    altRowsCheckBox->setChecked(getSetting("altRows", false).toBool());
    serverEdit->setText(getSetting("serverURL", "http://www.applejuicenet.de/18.0.html").toString());
    launchCombo->setEditText(getSetting("launcher", launchCombo->itemText(0)).toString());
    int location = getSetting("location", AjSettings::SAME).toInt();
    sameComputerRadio->setChecked(location == AjSettings::SAME);
    specificRadio->setChecked(location == AjSettings::SPECIFIC);
    ftpRadio->setChecked(location == AjSettings::FTP);
    incomingSpecificEdit->setText(getSetting("incomingDirSpecific", "/").toString());
    tempSpecificEdit->setText(getSetting("tempDirSpecific", "/").toString());
    ftpServerEdit->setText(getSetting("ftp", "server", "localhost").toString());
    ftpPortEdit->setText(getSetting("ftp", "port", "21").toString());
    ftpUserEdit->setText(getSetting("ftp", "user", "anonymous").toString());
    ftpPasswordEdit->setText(getSetting("ftp", "password", "").toString());
    ftpInDirEdit->setText(getSetting("ftp", "inDir", "/").toString());
    ftpTmpDirEdit->setText(getSetting("ftp", "tmpDir", "/").toString());
    ftpActiveRadioButton->setChecked(getSetting("ftp", "mode", QFtp::Active) == QFtp::Active);
    ftpPassiveRadioButton->setChecked(getSetting("ftp", "mode", QFtp::Active) == QFtp::Passive);
    ftpMbSpinBox->setValue(getSetting("ftp", "mb", "10").toInt());
    bool ftpFull = getSetting("ftp", "full", false).toBool();
    ftpFullRadioButton->setChecked(ftpFull);
    ftpMbRadioButton->setChecked(!ftpFull);
    fetchServersCheckBox->setChecked(getSetting("fetchServersOnStartup", false).toBool());
    languageComboBox->setCurrentIndex(languageComboBox->findData(getSetting("language", "en").toString().split("_")[0]));

    // -- lists for optionaly showing columns/labales --
    statusbarList->clear();
    QList<QVariant> statusbarShow = getStatusbarShows(juicer->statusBarWidgets.size());
    for(int i=0; i<juicer->statusBarWidgets.size(); i++) {
        statusbarList->addItem(juicer->statusBarWidgets.at(i)->toolTip());
        statusbarList->item(statusbarList->count()-1)->setCheckState(
            statusbarShow.at(i).toBool()?Qt::Checked:Qt::Unchecked);
    }
    downloadTabList->clear();
    QTreeWidgetItem* h = juicer->downloadsTreeWidget->headerItem();
    for(int i=0; i<h->columnCount(); i++) {
        downloadTabList->addItem(h->text(i));
        downloadTabList->item(downloadTabList->count()-1)->setCheckState(
            juicer->downloadsTreeWidget->isColumnHidden(i)?Qt::Unchecked:Qt::Checked);
    }
    uploadTabList->clear();
    h = juicer->uploadsTreeWidget->headerItem();
    for(int i=0; i<h->columnCount(); i++) {
        uploadTabList->addItem(h->text(i));
        uploadTabList->item(uploadTabList->count()-1)->setCheckState(
            juicer->uploadsTreeWidget->isColumnHidden(i)?Qt::Unchecked:Qt::Checked);
    }

    QFont font;
    QVariant v = getSetting("font", QApplication::font());
    if (v.isValid() && ! v.toString().isEmpty()) {
        font = v.value<QFont>();
    }
    else {
      font = QFont("Arial", 9, QFont::Normal);
    }
    QApplication::setFont(font);
    fontComboBox->setCurrentFont(font);
    setFontSizes(font);
#ifdef Q_WS_WIN
    handlerCheckCheckBox->setChecked(!OptionsDialog::hasSetting("handler") || OptionsDialog::getSetting("handler", false).toBool());
    handlerDefaultCheckBox->setChecked(OptionsDialog::hasSetting("handler") && OptionsDialog::getSetting("handler", false).toBool());
    handlerDefaultCheckBox->setEnabled(handlerCheckCheckBox->isChecked());
#endif
    checkUpdatesCheckBox->setChecked(getSetting("checkUpdates", true).toBool());
    observeClipboardCheckBox->setChecked(getSetting("observeClipboard", false).toBool());
    quitGUIAfterCoreExitCheckBox->setChecked(getSetting("quitGUIAfterCoreExit", true).toBool());
    videoEdit->setText(video());
    audioEdit->setText(audio());
    imageEdit->setText(image());
    textEdit->setText(text());
    archiveEdit->setText(archive());
    cddvdEdit->setText(cddvd());
}

void OptionsDialog::selectIncomingDir()  {
    QString dir = QFileDialog::getExistingDirectory(this, "Choose a directory", incomingEdit->text());
    if(!dir.isEmpty()) {
        incomingEdit->setText(dir);
    }
}

void OptionsDialog::selectTempDir() {
    QString dir = QFileDialog::getExistingDirectory(this, "Choose a directory", tempEdit->text());
    if(!dir.isEmpty()) {
        tempEdit->setText(dir);
    }
}

void OptionsDialog::selectLauncher() {
    #ifdef Q_WS_WIN
    QString file = QFileDialog::getOpenFileName(this, "Select a executable", launchCombo->currentText(), "Executable (*.exe)");
    #else
    QString file = QFileDialog::getOpenFileName(this, "Select a executable", launchCombo->currentText());
    #endif
    if(!file.isEmpty()) {
        launchCombo->setEditText(file);
    }
}

void OptionsDialog::selectIncomingDirSpecific() {
    QString dir = QFileDialog::getExistingDirectory(this, "Choose a directory", incomingSpecificEdit->text());
    if(!dir.isEmpty()) {
        incomingSpecificEdit->setText(dir);
    }
}

void OptionsDialog::selectTempDirSpecific() {
    QString dir = QFileDialog::getExistingDirectory(this, "Choose a directory", tempSpecificEdit->text());
    if(!dir.isEmpty()) {
        tempSpecificEdit->setText(dir);
    }
}

/*!
    \fn OptionsDialog::specificRadioToggled(bool checked)
 */
void OptionsDialog::specificRadioToggled(bool checked) {
    incomingSpecificEdit->setEnabled(checked);
    tempSpecificEdit->setEnabled(checked);
    incomingSpecificLabel->setEnabled(checked);
    tempSpecificLabel->setEnabled(checked);
    incomingSpecificButton->setEnabled(checked);
    tempSpecificButton->setEnabled(checked);
}


/*!
    \fn OptionsDialog::jumpToFtpSlot()
 */
void OptionsDialog::jumpToFtpSlot() {
    stackedWidget->setCurrentIndex(stackedWidget->count() -1);
}


/*!
    \fn OptionsDialog::writeSettings()
 */
void OptionsDialog::writeSettings() {
    if(savePassword->isChecked()) {
        setSetting("password", passwordEdit->text());
    } else {
        removeSetting("password");
    }

    setSetting("coreAddress", coreEdit->text());
    setSetting("savePassword", savePassword->isChecked());
    setSetting("showSplash", showSplash->isChecked());
    setSetting("useTray", trayCheckBox->isChecked());
    setSetting("altRows", altRowsCheckBox->isChecked());
    setSetting("serverURL", serverEdit->text());
    setSetting("refresh", refreshSpin->value());
    setSetting("launcher", launchCombo->currentText());

    if(sameComputerRadio->isChecked()) {
        setSetting("location", AjSettings::SAME);
    } else if(specificRadio->isChecked()) {
        setSetting("location", AjSettings::SPECIFIC);
    } else {
        setSetting("location", AjSettings::FTP);
    }

    setSetting("incomingDirSpecific", incomingSpecificEdit->text());
    setSetting("tempDirSpecific", tempSpecificEdit->text());

    setSetting("ftp", "server", ftpServerEdit->text());
    setSetting("ftp", "port", ftpPortEdit->text());
    setSetting("ftp", "user", ftpUserEdit->text());
    setSetting("ftp", "password", ftpPasswordEdit->text());
    setSetting("ftp", "inDir", ftpInDirEdit->text());
    setSetting("ftp", "tmpDir", ftpTmpDirEdit->text());
    setSetting("ftp", "mode", ftpActiveRadioButton->isChecked()?QFtp::Active:QFtp::Passive);
    setSetting("ftp", "full", ftpFullRadioButton->isChecked());
    setSetting("ftp", "mb", ftpMbSpinBox->value());

    setSetting("fetchServersOnStartup",  fetchServersCheckBox->isChecked());
    setSetting("language",  languageComboBox->itemData(languageComboBox->currentIndex()));

    QList<QVariant> statusbarComponents;
    for(int i=0; i<statusbarList->count(); i++) {
        statusbarComponents.append(statusbarList->item(i)->checkState() == Qt::Checked);
    }
    setSetting("statusbarComponents",  statusbarComponents);

    setSetting("font", getFont());

#ifdef Q_WS_WIN
    if(handlerCheckCheckBox->isChecked() && !handlerDefaultCheckBox->isChecked()) {
        OptionsDialog::removeSetting("handler");
    } else {
        OptionsDialog::setSetting("handler", handlerCheckCheckBox->isChecked() && handlerDefaultCheckBox->isChecked());
    }
#endif

    OptionsDialog::setSetting("checkUpdates", checkUpdatesCheckBox->isChecked());
    OptionsDialog::setSetting("observeClipboard", observeClipboardCheckBox->isChecked());
    OptionsDialog::setSetting("quitGUIAfterCoreExit", quitGUIAfterCoreExitCheckBox->isChecked());

    OptionsDialog::setSetting("dataTypes", "video", videoEdit->text());
    OptionsDialog::setSetting("dataTypes", "audio", audioEdit->text());
    OptionsDialog::setSetting("dataTypes", "image", imageEdit->text());
    OptionsDialog::setSetting("dataTypes", "text", textEdit->text());
    OptionsDialog::setSetting("dataTypes", "archive", archiveEdit->text());
    OptionsDialog::setSetting("dataTypes", "cddvd", cddvdEdit->text());
}


/*!
    \fn OptionsDialog::getSetting(const QString& key, QVariant defaultValue)
 */
QVariant OptionsDialog::getSetting(const QString& key, QVariant defaultValue) {
    QSettings lokalSettings;
    return lokalSettings.value(key, defaultValue);
}


/*!
    \fn OptionsDialog::setSetting(const QString& key, QVariant value)
 */
void OptionsDialog::setSetting(const QString& key, QVariant value)
{
    QSettings lokalSettings;
    lokalSettings.setValue(key, value);
}


/*!
    \fn OptionsDialog::getSetting(const QString& group, const QString& key, QVariant defaultValue)
 */
QVariant OptionsDialog::getSetting(const QString& group, const QString& key, QVariant defaultValue) {
    QSettings lokalSettings;
    lokalSettings.beginGroup(group);
    QVariant value =  lokalSettings.value(key, defaultValue);
    lokalSettings.endGroup();
    return value;
}

/*!
    \fn OptionsDialog::getGroupSetting(const QString& group, const QString& key)
 */
QVariant OptionsDialog::getGroupSetting(const QString& group, const QString& key){
    return getSetting(group, key, QVariant());
}

/*!
    \fn OptionsDialog::setSetting(const QString& group, const QString& key, QVariant value)
 */
void OptionsDialog::setSetting(const QString& group, const QString& key, QVariant value) {
    QSettings lokalSettings;
    lokalSettings.beginGroup(group);
    lokalSettings.setValue(key, value);
    lokalSettings.endGroup();
}


/*!
    \fn OptionsDialog::acceptedSlot()
 */
void OptionsDialog::acceptedSlot() {
    writeSettings();
}

/*!
    \fn OptionsDialog::setFontSizes(const QFont& font)
 */
void OptionsDialog::setFontSizes(const QFont& font) {
    QList<int> sizes = fontDatabase.pointSizes(font.family());
    fontSizeComboBox->clear();
    int index = 0;
    int i = 0;
    while(!sizes.isEmpty()) {
        int size = sizes.takeFirst();
        fontSizeComboBox->addItem(QString::number(size));
        if(size == QApplication::font().pointSize()) {
            index = i;
        }
        i++;
    }
    fontSizeComboBox->setCurrentIndex(index);
}

/*!
    \fn OptionsDialog::getFont()
 */
QFont OptionsDialog::getFont() {
    QFont font = fontComboBox->currentFont();
    font.setPointSize(fontSizeComboBox->currentText().toInt());
    return font;
}


/*!
    \fn OptionsDialog::setConnected(bool connected)
 */
void OptionsDialog::setConnected(bool connected) {
    nickLabel->setEnabled(connected);
    nickEdit->setEnabled(connected);
    passwordLabel->setEnabled(connected);
    passwordEdit->setEnabled(connected);
    retypeLabel->setEnabled(connected);
    retypeEdit->setEnabled(connected);
    incomingLabel->setEnabled(connected);
    incomingEdit->setEnabled(connected);
    incomingButton->setEnabled(connected);
    tempLabel->setEnabled(connected);
    tempEdit->setEnabled(connected);
    tempButton->setEnabled(connected);
    tcpLabel->setEnabled(connected);
    tcpEdit->setEnabled(connected);
    upLabel->setEnabled(connected);
    upSpin->setEnabled(connected);
    downLabel->setEnabled(connected);
    downSpin->setEnabled(connected);
    slotLabel->setEnabled(connected);
    slotSpin->setEnabled(connected);
    sourcesLabel->setEnabled(connected);
    sourcesSpin->setEnabled(connected);
    connectionsLabel->setEnabled(connected);
    connectionsSpin->setEnabled(connected);
    newLabel->setEnabled(connected);
    newSpin->setEnabled(connected);
}


/*!
    \fn OptionsDialog::hasSetting(const QString& key)
 */
bool OptionsDialog::hasSetting(const QString& key) {
    QSettings lokalSettings;
    return lokalSettings.contains(key);
}


/*!
    \fn OptionsDialog::hasSetting(const QString& group, const QString& key)
 */
bool OptionsDialog::hasSetting(const QString& group, const QString& key) {
    QSettings lokalSettings;
    lokalSettings.beginGroup(group);
    bool ret = lokalSettings.contains(key);
    lokalSettings.endGroup();
    return ret;
}


/*!
    \fn OptionsDialog::removeSetting(const QString& key)
 */
void OptionsDialog::removeSetting(const QString& key) {
    QSettings lokalSettings;
    lokalSettings.remove(key);
}


/*!
    \fn OptionsDialog::removeSetting(const QString& group, const QString& key)
 */
void OptionsDialog::removeSetting(const QString& group, const QString& key) {
    QSettings lokalSettings;
    lokalSettings.beginGroup(group);
    lokalSettings.remove(key);
    lokalSettings.endGroup();
}

/*!
    \fn OptionsDialog::reset()
 */
void OptionsDialog::reset() {
    if(QMessageBox::question(this, tr("Reset"), tr("Are you sure to reset Juicer?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QSettings lokalSettings;
        lokalSettings.clear();
        setSettings();
    }
}

QString OptionsDialog::video() {
    return getSetting("dataTypes", "video", "avi mpg mpeg wmv vob asf mov").toString();
}

QString OptionsDialog::audio() {
    return getSetting("dataTypes", "audio", "wav mp3 aac ogg").toString();
}

QString OptionsDialog::text() {
    return getSetting("dataTypes", "text", "txt rtf doc ppt odt sxw pdf").toString();
}

QString OptionsDialog::image() {
    return getSetting("dataTypes", "image", "png jpg jpeg bmp tif itff gif ppm pnm").toString();
}

QString OptionsDialog::archive() {
    return getSetting("dataTypes", "archive", "zip rar exe arj gz tar bz2 bzip").toString();
}

QString OptionsDialog::cddvd() {
    return getSetting("dataTypes", "cddvd", "iso bin cue mdf mds nrg").toString();
}


/*!
    \fn OptionsDialog::getStatusbarShows()
 */
QList<QVariant> OptionsDialog::getStatusbarShows(int n) {
    QList<QVariant> show = getSetting("statusbarComponents", QList<QVariant>()).toList();
    // -- if option is incompatible (e.g. through update) show all --
    if(show.size() != n) {
        show.clear();
        for(int i=0; i<n; i++) {
            show.append(true);
        }
    }
    return show;
}


/*!
    \fn OptionsDialog::addProfile()
 */
void OptionsDialog::addProfile() {
    bool ok;
    QString name = QInputDialog::getText(this, tr("Profile name"), tr("Profile Name:"), QLineEdit::Normal, "", &ok);
    if(ok && !name.isEmpty()) {
        QStringList profiles = getSetting("profiles").toStringList();
        profiles << name;
        setSetting("profiles", profiles);
        loadProfiles();
        updateProfile(QHash<QString, QVariant>(), name);
    }
}


/*!
    \fn OptionsDialog::changeProfile()
 */
void OptionsDialog::changeProfile() {
    QList<QListWidgetItem*> selection = profilesList->selectedItems();
    if(!selection.isEmpty()) {
        QString name = selection.first()->text();
        updateProfile(getGroupSetting("profile", name).toHash(), name);
    }
}


/*!
    \fn OptionsDialog::removeProfile()
 */
void OptionsDialog::removeProfile() {
    QStringList profiles = getSetting("profiles").toStringList();
    QList<QListWidgetItem*> selected = profilesList->selectedItems();
    if(!selected.isEmpty()) {
        profiles.removeAll(selected.first()->text());
        setSetting("profiles", profiles);
        loadProfiles();
    }
}


/*!
    \fn OptionsDialog::loadProfiles()
 */
void OptionsDialog::loadProfiles() {
    QStringList profiles = getSetting("profiles").toStringList();
    profilesList->clear();
    profilesList->addItems(profiles);
    profileSelectionChanged();
}


/*!
    \fn OptionsDialog::profileSelectionChanged()
 */
void OptionsDialog::profileSelectionChanged() {
    QList<QListWidgetItem*> selection = profilesList->selectedItems();
    bool selected = !selection.isEmpty();
    changeProfileButton->setEnabled(selected);
    removeProfileButton->setEnabled(selected);
    if(selected) {
        QHash<QString, QVariant> profile = getGroupSetting("profile", selection.first()->text()).toHash();
        downSpin->setValue(profile["maxDown"].toInt());
        upSpin->setValue(profile["maxUp"].toInt());
        connectionsSpin->setValue(profile["maxCon"].toInt());
        slotSpin->setValue(profile["maxSlot"].toInt());
        sourcesSpin->setValue(profile["maxSources"].toInt());
        newSpin->setValue(profile["maxNewCon"].toInt());
    }
}


/*!
    \fn OptionsDialog::updateProfile(QHash<QString, QVariant> profile, const QString& name)
 */
void OptionsDialog::updateProfile(QHash<QString, QVariant> profile, const QString& name) {
    profile["maxDown"] = downSpin->value();
    profile["maxUp"] = upSpin->value();
    profile["maxCon"] = connectionsSpin->value();
    profile["maxSlot"] = slotSpin->value();
    profile["maxSources"] = sourcesSpin->value();
    profile["maxNewCon"] = newSpin->value();
    setSetting("profile", name, profile);
}
