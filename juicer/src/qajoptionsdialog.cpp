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

QAjOptionsDialog::QAjOptionsDialog( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

#if QT_VERSION >= 0x040203
    IconWidget* l = new IconWidget(":/options/core.png", tr("Core"), listWidget);
    QListWidgetItem* item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/limits.png", tr("Limits"), listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/appearance.png", tr("Appearance"), listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/behaviour.png", tr("Behaviour"), listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/launching.png", tr("Launching"), listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);

    l = new IconWidget(":/options/ftp.png", tr("FTP"), listWidget);
    item = new QListWidgetItem(listWidget);
    item->setSizeHint(l->size());
    listWidget->setItemWidget(item, l);
#else
    listWidget->setIconSize(QSize(32,32));
    listWidget->setWrapping(true);
    listWidget->setSpacing( 5 );
    QListWidgetItem* item;
    item = new QListWidgetItem(QIcon(":/options/core.png"), "Core", listWidget);
    item = new QListWidgetItem(QIcon(":/options/limits.png"), "Limits", listWidget);
    item = new QListWidgetItem(QIcon(":/options/appearance.png"), "Appearance", listWidget);
    item = new QListWidgetItem(QIcon(":/options/behaviour.png"), "Behaviour", listWidget);
    item = new QListWidgetItem(QIcon(":/options/launching.png"), "Launching", listWidget);
    item = new QListWidgetItem(QIcon(":/options/ftp.png"), "FTP", listWidget);
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

    connect( listWidget, SIGNAL(currentRowChanged( int ) ), stackedWidget , SLOT(setCurrentIndex( int ) ) );

    listWidget->setCurrentRow( 0 );
}

QAjOptionsDialog::~QAjOptionsDialog()
{}

AjSettings QAjOptionsDialog::getAjSettings()
{
    AjSettings settings;
    settings.nick = nickEdit->text();
    settings.password = passwordEdit->text();
    settings.coreAddress = coreEdit->text();
    settings.xmlPort = xmlEdit->text();
    settings.incomingDir = incomingEdit->text();
    settings.tempDir = tempEdit->text();

    settings.serverURL = serverEdit->text();

    settings.refresh = refreshSpin->value();

    settings.autoconnect = autoConnect->isChecked()?"true":"false";
    settings.savePassword = savePassword->isChecked()?"true":"false";
    settings.showSplash = showSplash->isChecked()?"true":"false";

    settings.maxDown = QString::number( downSpin->value() );
    settings.maxUp = QString::number( upSpin->value() );
    settings.maxSlot = QString::number( slotSpin->value() );
    settings.maxSources = QString::number( sourcesSpin->value() );
    settings.maxCon = QString::number( connectionsSpin->value() );
    settings.maxNewCon = QString::number( newSpin->value() );

    settings.tcpPort = tcpEdit->text();

    settings.launcher = launchCombo->currentText();
    if( sameComputerRadio->isChecked() )
        settings.location = AjSettings::SAME;
    else if( specificRadio->isChecked() )
        settings.location = AjSettings::SPECIFIC;
    else
        settings.location = AjSettings::FTP;
    settings.tempDirSpecific = tempSpecificEdit->text();
    settings.incomingDirSpecific = incomingSpecificEdit->text();

    settings.ftpServer = ftpServerEdit->text();
    settings.ftpPort = ftpPortEdit->text();
    settings.ftpUser = ftpUserEdit->text();
    settings.ftpPassword = ftpPasswordEdit->text();
    settings.ftpDir = ftpInDirEdit->text();

    settings.fetchServersOnStartup = this->fetchServersCheckBox->isChecked();

    settings.language = languageComboBox->itemData(languageComboBox->currentIndex());

    return settings;
}

void QAjOptionsDialog::setAjSettings( AjSettings settings )
{
    nickEdit->setText( settings.nick );
    passwordEdit->setText( settings.password );
    coreEdit->setText( settings.coreAddress );
    xmlEdit->setText( settings.xmlPort );
    incomingEdit->setText( settings.incomingDir );
    tempEdit->setText( settings.tempDir );
    serverEdit->setText( settings.serverURL );

    refreshSpin->setValue( settings.refresh );

    autoConnect->setChecked( ( settings.autoconnect == "true" ) );
    savePassword->setChecked( ( settings.savePassword == "true" ) );
    showSplash->setChecked( ( settings.showSplash == "true" ) );

    downSpin->setValue( settings.maxDown.toInt() / 1024 );
    upSpin->setValue( settings.maxUp.toInt() / 1024 );
    slotSpin->setValue( settings.maxSlot.toInt() );
    sourcesSpin->setValue( settings.maxSources.toInt() );
    connectionsSpin->setValue( settings.maxCon.toInt() );
    newSpin->setValue( settings.maxNewCon.toInt() );

    tcpEdit->setText( settings.tcpPort );

    launchCombo->setEditText( settings.launcher );
    sameComputerRadio->setChecked( settings.location == AjSettings::SAME );
    specificRadio->setChecked( settings.location == AjSettings::SPECIFIC );
    ftpRadio->setChecked( settings.location == AjSettings::FTP );
    incomingSpecificEdit->setText( settings.incomingDirSpecific );
    tempSpecificEdit->setText( settings.tempDirSpecific );

    ftpServerEdit->setText( settings.ftpServer );
    ftpPortEdit->setText( settings.ftpPort );
    ftpUserEdit->setText( settings.ftpUser );
    ftpPasswordEdit->setText( settings.ftpPassword );
    ftpInDirEdit->setText( settings.ftpDir );

    fetchServersCheckBox->setChecked( settings.fetchServersOnStartup );

    languageComboBox->setCurrentIndex(languageComboBox->findData(settings.language.toString().split("_")[0]));
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
    QString file = QFileDialog::getOpenFileName( this, "Select a executable", launchCombo->currentText(), "Executable (*.exe)" );
    #else
    QString file = QFileDialog::getOpenFileName( this, "Select a executable", launchCombo->currentText(), "Executable (*.exe)" );
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
