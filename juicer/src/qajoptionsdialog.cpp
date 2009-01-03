/***************************************************************************
 *   Copyright (C) 2005 by Matthias Reif   *
 *   matthias.reif@informatik.tu-chemnitz.de   *
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

#include "qajoptionsdialog.h"
#include "qajapplication.h"


QAjOptionsDialog::QAjOptionsDialog( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
#ifndef Q_WS_WIN
     handlerGroupBox->setHidden(true);
#endif

    okButton->setIcon(QIcon(QString::fromUtf8(":/ok.png")));
    cancelButton->setIcon(QIcon(QString::fromUtf8(":/cancel.png")));

#if QT_VERSION >= 0x040203
    QAjIconWidget* l = new QAjIconWidget(":/options/core.png", tr("Core"), QBoxLayout::TopToBottom, listWidget);
    QListWidgetItem* item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new QAjIconWidget(":/options/limits.png", tr("Limits"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new QAjIconWidget(":/options/appearance.png", tr("Appearance"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new QAjIconWidget(":/options/behaviour.png", tr("Behaviour"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new QAjIconWidget(":/options/launching.png", tr("Launching"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new QAjIconWidget(":/options/ftp.png", tr("FTP"), QBoxLayout::TopToBottom, listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);
#else
    listWidget->setIconSize(QSize(32,32));
    listWidget->setWrapping(true);
    listWidget->setSpacing( 5 );
    QListWidgetItem* item;
    item = new QListWidgetItem(QIcon(":/options/core.png"), tr("Core"), listWidget);
    item = new QListWidgetItem(QIcon(":/options/limits.png"), tr("Limits"), listWidget);
    item = new QListWidgetItem(QIcon(":/options/appearance.png"), tr("Appearance"), listWidget);
    item = new QListWidgetItem(QIcon(":/options/behaviour.png"), tr("Behaviour"), listWidget);
    item = new QListWidgetItem(QIcon(":/options/launching.png"), tr("Launching"), listWidget);
    item = new QListWidgetItem(QIcon(":/options/ftp.png"), tr("FTP"), listWidget);
#endif

    languageComboBox->addItem(QIcon(":/options/de.png"), "deutsch", "de");
    languageComboBox->addItem(QIcon(":/options/gb.png"), "english", "en");

    this->setWindowIcon( QIcon(":/juicer.png") );
    connect( incomingButton, SIGNAL( clicked() ), this, SLOT( selectIncomingDir() ) );
    connect( tempButton, SIGNAL( clicked() ), this, SLOT( selectTempDir() ) );

    connect( launcherButton, SIGNAL( clicked() ), this, SLOT( selectLauncher() ) );

    connect( incomingSpecificButton, SIGNAL( clicked() ), this, SLOT( selectIncomingDirSpecific() ) );
    connect( tempSpecificButton, SIGNAL( clicked() ), this, SLOT( selectTempDirSpecific() ) );

    connect( specificRadio, SIGNAL( toggled( bool ) ), this, SLOT( specificRadioToggled( bool ) ) );

    launchCombo->addItem( DEFAULT_LAUNCHER );
    if( QString(DEFAULT_LAUNCHER) == QString(KDE_LAUNCHER) )
        launchCombo->addItem( GNOME_LAUNCHER );

    specificRadioToggled( false );

    connect( listWidget, SIGNAL( currentRowChanged( int ) ), stackedWidget , SLOT(setCurrentIndex( int ) ) );
    connect( jumpFtpButton, SIGNAL( clicked() ), this , SLOT(jumpToFtpSlot() ) );

    connect( fontComboBox, SIGNAL( currentFontChanged( const QFont& ) ), this, SLOT( setFontSizes( const QFont& ) ) );

    connect( this, SIGNAL( accepted() ), this, SLOT( acceptedSlot() ) );

    connect(this->handlerPushButton, SIGNAL(clicked()), this, SLOT(setAjfspHandler()));

    listWidget->setCurrentRow( 0 );
}

QAjOptionsDialog::~QAjOptionsDialog()
{}

AjSettings QAjOptionsDialog::getAjSettings()
{
    AjSettings settings;
    settings.nick = nickEdit->text();
    settings.xmlPort = xmlEdit->text();
    settings.incomingDir = incomingEdit->text();
    settings.tempDir = tempEdit->text();

    settings.autoconnect = autoConnect->isChecked()?"true":"false";

    settings.maxDown = QString::number( downSpin->value() );
    settings.maxUp = QString::number( upSpin->value() );
    settings.maxSlot = QString::number( slotSpin->value() );
    settings.maxSources = QString::number( sourcesSpin->value() );
    settings.maxCon = QString::number( connectionsSpin->value() );
    settings.maxNewCon = QString::number( newSpin->value() );

    settings.tcpPort = tcpEdit->text();

    return settings;
}

void QAjOptionsDialog::setAjSettings( const AjSettings& settings )
{
    nickEdit->setText( settings.nick );
    xmlEdit->setText( settings.xmlPort );
    incomingEdit->setText( settings.incomingDir );
    tempEdit->setText( settings.tempDir );

    autoConnect->setChecked( ( settings.autoconnect == "true" ) );

    downSpin->setValue( settings.maxDown.toInt() / 1024 );
    upSpin->setValue( settings.maxUp.toInt() / 1024 );
    slotSpin->setValue( settings.maxSlot.toInt() );
    sourcesSpin->setValue( settings.maxSources.toInt() );
    connectionsSpin->setValue( settings.maxCon.toInt() );
    newSpin->setValue( settings.maxNewCon.toInt() );

    tcpEdit->setText( settings.tcpPort );
}

void QAjOptionsDialog::setSettings()
{
    passwordEdit->setText( getSetting( "password", "" ).toString() );
    coreEdit->setText( getSetting( "coreAddress", "localhost" ).toString() );

    refreshSpin->setValue( getSetting( "refresh", 3 ).toInt() );

    savePassword->setChecked( getSetting( "savePassword", false ).toBool() );
    showSplash->setChecked( getSetting( "showSplash", true ).toBool() );
    trayCheckBox->setChecked( getSetting( "useTray", false ).toBool() );
    altRowsCheckBox->setChecked( getSetting( "altRows", false ).toBool() );

    serverEdit->setText( getSetting( "serverURL", "http://www.applejuicenet.de/18.0.html" ).toString() );

    launchCombo->setEditText( getSetting( "launcher", launchCombo->itemText(0)).toString() );

    int location = getSetting( "location", AjSettings::SAME ).toInt();
    sameComputerRadio->setChecked( location == AjSettings::SAME );
    specificRadio->setChecked( location == AjSettings::SPECIFIC );
    ftpRadio->setChecked( location == AjSettings::FTP );
    incomingSpecificEdit->setText( getSetting("incomingDirSpecific", "/" ).toString() );
    tempSpecificEdit->setText( getSetting( "tempDirSpecific", "/" ).toString() );

    ftpServerEdit->setText( getSetting( "ftp", "server", "localhost" ).toString() );
    ftpPortEdit->setText( getSetting( "ftp", "port", "21" ).toString() );
    ftpUserEdit->setText( getSetting( "ftp", "user", "anonymous" ).toString() );
    ftpPasswordEdit->setText( getSetting( "ftp", "password", "" ).toString() );
    ftpInDirEdit->setText( getSetting( "ftp", "inDir", "/" ).toString() );
    ftpTmpDirEdit->setText( getSetting( "ftp", "tmpDir", "/" ).toString() );
    ftpActiveRadioButton->setChecked( getSetting( "ftp", "mode", QFtp::Active ) == QFtp::Active );
    ftpPassiveRadioButton->setChecked( getSetting( "ftp", "mode", QFtp::Active ) == QFtp::Passive );

    ftpMbSpinBox->setValue( getSetting( "ftp", "mb", "10" ).toInt() );
    bool ftpFull = getSetting( "ftp", "full", false ).toBool();
    ftpFullRadioButton->setChecked( ftpFull );
    ftpMbRadioButton->setChecked( !ftpFull );

    fetchServersCheckBox->setChecked( getSetting( "fetchServersOnStartup", false ).toBool() );

    languageComboBox->setCurrentIndex(languageComboBox->findData(getSetting( "language", "en" ).toString().split("_")[0]));

    QStringList statusbarComponents = getSetting( "statusbarComponents", getDefaultStatusbarComponents() ).toStringList();
    statusbarList->clearSelection();
    int i;
    for(i=0; i<statusbarComponents.size(); i++) {
        statusbarList->item( statusbarComponents[i].toInt() )->setSelected( true );
    }

    QFont font = getSetting( "font", QApplication::font() ).value<QFont>();
    QApplication::setFont( font );
    fontComboBox->setCurrentFont( font );
    setFontSizes(font);
#ifdef Q_WS_WIN
    handlerCheckCheckBox->setChecked(!QAjOptionsDialog::hasSetting("handler") || QAjOptionsDialog::getSetting("handler", false).toBool());
    handlerDefaultCheckBox->setChecked(QAjOptionsDialog::hasSetting("handler") && QAjOptionsDialog::getSetting("handler", false).toBool());
    handlerDefaultCheckBox->setEnabled(handlerCheckCheckBox->isChecked());
#endif
}

void QAjOptionsDialog::selectIncomingDir()
{
    QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory", incomingEdit->text() );
    if( ! dir.isEmpty() )
        incomingEdit->setText( dir );
}

void QAjOptionsDialog::selectTempDir()
{
    QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory", tempEdit->text() );
    if( ! dir.isEmpty() )
        tempEdit->setText( dir );
}

void QAjOptionsDialog::selectLauncher()
{
    #ifdef Q_WS_WIN
    QString file = QFileDialog::getOpenFileName(this, "Select a executable", launchCombo->currentText(), "Executable (*.exe)");
    #else
    QString file = QFileDialog::getOpenFileName(this, "Select a executable", launchCombo->currentText());
    #endif
    if( ! file.isEmpty() )
        launchCombo->setEditText( file );
}

void QAjOptionsDialog::selectIncomingDirSpecific()
{
    QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory", incomingSpecificEdit->text() );
    if( ! dir.isEmpty() )
        incomingSpecificEdit->setText( dir );
}

void QAjOptionsDialog::selectTempDirSpecific()
{
    QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory", tempSpecificEdit->text() );
    if( ! dir.isEmpty() )
        tempSpecificEdit->setText( dir );
}

/*!
    \fn QAjOptionsDialog::specificRadioToggled( bool checked )
 */
void QAjOptionsDialog::specificRadioToggled( bool checked )
{
    incomingSpecificEdit->setEnabled( checked );
    tempSpecificEdit->setEnabled( checked );
    incomingSpecificLabel->setEnabled( checked );
    tempSpecificLabel->setEnabled( checked );
    incomingSpecificButton->setEnabled( checked );
    tempSpecificButton->setEnabled( checked );
}


/*!
    \fn QAjOptionsDialog::getDefaultStatusbarComponents()
 */
QStringList QAjOptionsDialog::getDefaultStatusbarComponents()
{
    QStringList x;
    int i;
    for(i=0; i<statusbarList->count(); i++)
    {
        x << QString::number(i);
    }
    return x;
}


/*!
    \fn QAjOptionsDialog::jumpToFtpSlot()
 */
void QAjOptionsDialog::jumpToFtpSlot()
{
    stackedWidget->setCurrentIndex(stackedWidget->count() -1);
}


/*!
    \fn QAjOptionsDialog::writeSettings()
 */
void QAjOptionsDialog::writeSettings()
{
    if(savePassword->isChecked()) {
        setSetting("password", passwordEdit->text());
    } else {
        removeSetting("password");
    }

    setSetting( "coreAddress", coreEdit->text() );
    setSetting( "savePassword", savePassword->isChecked() );
    setSetting( "showSplash", showSplash->isChecked() );
    setSetting( "useTray", trayCheckBox->isChecked() );
    setSetting( "altRows", altRowsCheckBox->isChecked() );
    setSetting( "serverURL", serverEdit->text() );
    setSetting( "refresh", refreshSpin->value() );
    setSetting( "launcher", launchCombo->currentText() );

    if( sameComputerRadio->isChecked() )
        setSetting( "location", AjSettings::SAME );
    else if( specificRadio->isChecked() )
        setSetting( "location", AjSettings::SPECIFIC );
    else
        setSetting( "location", AjSettings::FTP );

    setSetting( "incomingDirSpecific", incomingSpecificEdit->text() );
    setSetting( "tempDirSpecific", tempSpecificEdit->text() );

    setSetting( "ftp", "server", ftpServerEdit->text() );
    setSetting( "ftp", "port", ftpPortEdit->text() );
    setSetting( "ftp", "user", ftpUserEdit->text() );
    setSetting( "ftp", "password", ftpPasswordEdit->text() );
    setSetting( "ftp", "inDir", ftpInDirEdit->text() );
    setSetting( "ftp", "tmpDir", ftpTmpDirEdit->text() );
    setSetting( "ftp", "mode", ftpActiveRadioButton->isChecked()?QFtp::Active:QFtp::Passive );
    setSetting( "ftp", "full", ftpFullRadioButton->isChecked() );
    setSetting( "ftp", "mb", ftpMbSpinBox->value() );

    setSetting( "fetchServersOnStartup",  fetchServersCheckBox->isChecked() );
    setSetting( "language",  languageComboBox->itemData(languageComboBox->currentIndex()) );

    QStringList statusbarComponents;
    QList<QListWidgetItem *> items = statusbarList->selectedItems();
    int i;
    for(i=0; i<items.size(); i++)
    {
        statusbarComponents << QString::number(statusbarList->row(items[i]));
    }
    setSetting( "statusbarComponents",  statusbarComponents );
    setSetting( "font", getFont() );

#ifdef Q_WS_WIN
    if(handlerCheckCheckBox->isChecked() && !handlerDefaultCheckBox->isChecked()) {
        QAjOptionsDialog::removeSetting("handler");
    } else {
        QAjOptionsDialog::setSetting("handler", handlerCheckCheckBox->isChecked() && handlerDefaultCheckBox->isChecked());
    }
#endif
}


/*!
    \fn QAjOptionsDialog::getSetting( const QString& key, QVariant defaultValue )
 */
QVariant QAjOptionsDialog::getSetting( const QString& key, QVariant defaultValue )
{
    QSettings lokalSettings;
    return lokalSettings.value( key, defaultValue);
}


/*!
    \fn QAjOptionsDialog::setSetting( const QString& key, QVariant value )
 */
void QAjOptionsDialog::setSetting( const QString& key, QVariant value )
{
    QSettings lokalSettings;
    lokalSettings.setValue(key, value);
}



/*!
    \fn QAjOptionsDialog::getSetting( const QString& group, const QString& key, QVariant defaultValue )
 */
QVariant QAjOptionsDialog::getSetting( const QString& group, const QString& key, QVariant defaultValue )
{
    QSettings lokalSettings;
    lokalSettings.beginGroup( group );
    QVariant value =  lokalSettings.value( key, defaultValue);
    lokalSettings.endGroup();
    return value;
}


/*!
    \fn QAjOptionsDialog::setSetting( const QString& group, const QString& key, QVariant value )
 */
void QAjOptionsDialog::setSetting( const QString& group, const QString& key, QVariant value )
{
    QSettings lokalSettings;
    lokalSettings.beginGroup( group );
    lokalSettings.setValue(key, value);
    lokalSettings.endGroup();
}


/*!
    \fn QAjOptionsDialog::acceptedSlot()
 */
void QAjOptionsDialog::acceptedSlot()
{
    writeSettings();
}

/*!
    \fn QAjOptionsDialog::setFontSizes( const QFont& font )
 */
void QAjOptionsDialog::setFontSizes( const QFont& font )
{
    QList<int> sizes = fontDatabase.pointSizes( font.family() );
    fontSizeComboBox->clear();
    int index = 0;
    int i = 0;
    while( ! sizes.isEmpty() )
    {
        int size = sizes.takeFirst();
        fontSizeComboBox->addItem(QString::number(size));
        if(size == QApplication::font().pointSize())
        {
            index = i;
        }
        i++;
    }
    fontSizeComboBox->setCurrentIndex( index );
}

/*!
    \fn QAjOptionsDialog::getFont()
 */
QFont QAjOptionsDialog::getFont() {
    QFont font = fontComboBox->currentFont();
    font.setPointSize(fontSizeComboBox->currentText().toInt());
    return font;
}


/*!
    \fn QAjOptionsDialog::setConnected(bool connected)
 */
void QAjOptionsDialog::setConnected(bool connected)
{
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
    \fn QAjOptionsDialog::hasSetting(const QString& key)
 */
bool QAjOptionsDialog::hasSetting(const QString& key)
{
    QSettings lokalSettings;
    return lokalSettings.contains(key);
}


/*!
    \fn QAjOptionsDialog::hasSetting(const QString& group, const QString& key)
 */
bool QAjOptionsDialog::hasSetting(const QString& group, const QString& key)
{
    QSettings lokalSettings;
    lokalSettings.beginGroup( group );
    bool ret = lokalSettings.contains(key);
    lokalSettings.endGroup();
    return ret;
}


/*!
    \fn QAjOptionsDialog::removeSetting(const QString& key)
 */
void QAjOptionsDialog::removeSetting(const QString& key)
{
    QSettings lokalSettings;
    lokalSettings.remove(key);
}


/*!
    \fn QAjOptionsDialog::removeSetting(const QString& group, const QString& key)
 */
void QAjOptionsDialog::removeSetting(const QString& group, const QString& key)
{
    QSettings lokalSettings;
    lokalSettings.beginGroup( group );
    lokalSettings.remove(key);
    lokalSettings.endGroup();
}


/*!
    \fn QAjOptionsDialog::setAjfspHandler()
 */
void QAjOptionsDialog::setAjfspHandler()
{
    printf("%s\n", ((QAjApplication*)qApp)->appPath.toLatin1().data());
}
