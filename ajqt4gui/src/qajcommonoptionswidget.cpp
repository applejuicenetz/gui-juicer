/***************************************************************************
 *   Copyright (C) 2004 by Matthias Reif                                   *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
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
#include "qajcommonoptionswidget.h"


QAjCommonOptionsWidget::QAjCommonOptionsWidget( QWidget *parent, const char *name) : QWidget( parent )
{
	int s = 0;
	nickLabel = new QLabel(this);
	nickEdit = new QLineEdit(this);
	
	passwordLabel = new QLabel(this);
	passwordEdit = new QLineEdit(this);
	
	passwordConfirmLabel = new QLabel(this);
	passwordConfirmEdit = new QLineEdit(this);
	
	coreAddressLabel = new QLabel(this);
	coreAddressEdit = new QLineEdit(this);
	
	xmlPortLabel = new QLabel(this);
	xmlPortEdit = new QLineEdit(this);

	incomingDirLabel = new QLabel(this);
	incomingDirEdit = new QLineEdit(this);
	incomingDirButton = new QPushButton(this);

	tempDirLabel = new QLabel(this);
	tempDirEdit = new QLineEdit(this);
	tempDirButton = new QPushButton(this);

	serverURLLabel = new QLabel(this);
	serverURLEdit = new QLineEdit(this);
	
	timerLabel = new QLabel(this);
	timerLabel2 = new QLabel(this);
	timerSpin = new QSpinBox( this );
	timerSpin->setRange( 1, 10 );
	timerSpin->setSingleStep( 1 );
	
	autoconnectCheckBox = new QCheckBox( this );
	allowBrowseCheckBox = new QCheckBox( this );
	savePasswordCheckBox = new QCheckBox( this );
	
	nickLabel->setText( tr("Nick:") );
	passwordLabel->setText( tr("New Password:") );
	passwordConfirmLabel->setText( tr("Retype Password:") );
	coreAddressLabel->setText( tr("Core Address:") );
	xmlPortLabel->setText( tr("XML Port:") );
	incomingDirLabel->setText( tr("Incoming Directory:") );
	tempDirLabel->setText( tr("Temporary Directory:") );
	incomingDirButton->setText( tr("Select...") );
	tempDirButton->setText( tr("Select...") );
	serverURLLabel->setText( tr("Server source:") );
	timerLabel->setText( tr("Refresh:") );
	timerLabel2->setText( tr("sec.") );
	autoconnectCheckBox->setText( tr("autoconnect") );
	allowBrowseCheckBox->setText( tr("allow browse") );
	savePasswordCheckBox->setText( tr("save password") );

	nickEdit->setAlignment( Qt::AlignRight );
	coreAddressEdit->setAlignment( Qt::AlignRight );
	xmlPortEdit->setAlignment( Qt::AlignRight );
	passwordEdit->setAlignment( Qt::AlignRight );
	passwordConfirmEdit->setAlignment( Qt::AlignRight );
	
	nickLabel->adjustSize();
	passwordLabel->adjustSize();
	passwordConfirmLabel->adjustSize();
	coreAddressLabel->adjustSize();
	xmlPortLabel->adjustSize();
	incomingDirLabel->adjustSize();
	tempDirLabel->adjustSize();
	serverURLLabel->adjustSize();
	timerLabel->adjustSize();
	timerLabel2->adjustSize();
	autoconnectCheckBox->adjustSize();
	allowBrowseCheckBox->adjustSize();
	savePasswordCheckBox->adjustSize();
	
	nickEdit->resize( 150, nickLabel->height() +4 );
	passwordEdit->resize( 150, passwordLabel->height() +4 );
	passwordConfirmEdit->resize( 150, passwordConfirmLabel->height() +4 );
	coreAddressEdit->resize( 150, coreAddressLabel->height() +4 );
	xmlPortEdit->resize( 150, xmlPortLabel->height() +4 );
	incomingDirEdit->resize( 200, incomingDirLabel->height() +4);
	tempDirEdit->resize( 200, tempDirLabel->height() +4 );
	serverURLEdit->resize( 200, serverURLLabel->height() +4 );
	timerSpin->resize( 60, timerLabel->height() +4 );
	incomingDirButton->setMaximumHeight( incomingDirEdit->height()+2 );
	incomingDirButton->adjustSize();
	tempDirButton->setMaximumHeight( tempDirEdit->height()+2 );
	tempDirButton->adjustSize();

	int verStart = 10;
	int verSpace = nickLabel->height() + 14;
	
	nickLabel->move( 10, verStart + verSpace * 0 );
	passwordLabel->move( 10, verStart + verSpace * 1 );
	passwordConfirmLabel->move( 10, verStart + verSpace * 2 );
	coreAddressLabel->move( 10, verStart + verSpace * 3 );
	xmlPortLabel->move( 10, verStart + verSpace * 4 );
	incomingDirLabel->move( 10, verStart + verSpace * 5 );
	tempDirLabel->move( 10, verStart + verSpace * 6 );
	serverURLLabel->move( 10, verStart + verSpace * 7 );
	timerLabel->move( 10, verStart + verSpace * 8 );
	autoconnectCheckBox->move( 10, verStart + verSpace * 9 );
	allowBrowseCheckBox->move( autoconnectCheckBox->x() + autoconnectCheckBox->width() + 20, verStart + verSpace * 9 );
	savePasswordCheckBox->move( allowBrowseCheckBox->x() + allowBrowseCheckBox->width() + 20, verStart + verSpace * 9 );
	
	ACT_S( nickLabel );
	ACT_S( passwordLabel );
	ACT_S( passwordConfirmLabel );
	ACT_S( coreAddressLabel );
	ACT_S( xmlPortLabel );
	ACT_S( incomingDirLabel );
	ACT_S( tempDirLabel );
	ACT_S( serverURLLabel );
	ACT_S( timerLabel );

	nickEdit->move( s + 15, nickLabel->y() -2 );
	passwordEdit->move( s + 15, passwordLabel->y() -2 );
	passwordConfirmEdit->move( s + 15, passwordConfirmLabel->y() -2 );
	coreAddressEdit->move( s + 15, coreAddressLabel->y() -2 );
	xmlPortEdit->move( s + 15, xmlPortLabel->y() -2 );
	incomingDirEdit->move( s + 15, incomingDirLabel->y() -2 );
	tempDirEdit->move( s + 15, tempDirLabel->y() -2 );
	incomingDirButton->move( incomingDirEdit->x() + incomingDirEdit->width() + 10, incomingDirEdit->y() -1 );
	tempDirButton->move( tempDirEdit->x() + tempDirEdit->width() + 10, tempDirEdit->y() -1 );
	serverURLEdit->move( s + 15, serverURLLabel->y() -2 );
	timerSpin->move( s + 15, timerLabel->y() -2 );
	timerLabel2->move( timerSpin->x() + timerSpin->width() + 10, timerLabel->y() -2 );
	
	setMinimumHeight( autoconnectCheckBox->y() + autoconnectCheckBox->height() + 50 );
	setMinimumWidth( tempDirButton->x() + tempDirButton->width() + 50 );

	connect( incomingDirButton, SIGNAL( clicked() ), this, SLOT( selectIncomingDir() ) );
	connect( tempDirButton, SIGNAL( clicked() ), this, SLOT( selectTempDir() ) );
	
	// allow browse is deprecated
	allowBrowseCheckBox->hide();
}


QAjCommonOptionsWidget::~QAjCommonOptionsWidget()
{
	delete nickLabel;
	delete nickEdit;
	delete passwordLabel;
	delete passwordEdit;
	delete passwordConfirmLabel;
	delete passwordConfirmEdit;
	
	delete coreAddressLabel;
	delete coreAddressEdit;
	delete xmlPortLabel;
	delete xmlPortEdit;
	
	delete incomingDirLabel;
	delete incomingDirEdit;
	delete incomingDirButton;
	delete tempDirLabel;
	delete tempDirEdit;
	delete tempDirButton;
	
	delete serverURLLabel;
	delete serverURLEdit;
	
	delete timerLabel;
	delete timerLabel2;
	delete timerSpin;
	
	delete autoconnectCheckBox;
	delete allowBrowseCheckBox;
	delete savePasswordCheckBox;
}

void QAjCommonOptionsWidget::selectIncomingDir()
{
	QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory", incomingDirEdit->text() );
	if( dir != "" )
		incomingDirEdit->setText( dir );
}

void QAjCommonOptionsWidget::selectTempDir()
{
	QString dir = QFileDialog::getExistingDirectory( this, "Choose a directory", tempDirEdit->text() );
	if( dir != "" )
		tempDirEdit->setText( dir );
}

void QAjCommonOptionsWidget::setAjSettigs( AjSettings settings )
{
	nickEdit->setText( settings.nick );
	coreAddressEdit->setText( settings.coreAddress );
	xmlPortEdit->setText( settings.xmlPort );
	incomingDirEdit->setText( settings.incomingDir );
	tempDirEdit->setText( settings.tempDir );
	serverURLEdit->setText( settings.serverURL );
	timerSpin->setValue( settings.refresh );
	passwordEdit->setEchoMode( QLineEdit::Password );
	passwordConfirmEdit->setEchoMode( QLineEdit::Password );
	autoconnectCheckBox->setChecked( (settings.autoconnect == "true") );
	allowBrowseCheckBox->setChecked( (settings.allowBrowse == "true") );
	savePasswordCheckBox->setChecked( (settings.savePassword == "true") );
	int e = 0;
	ACT_E( nickEdit );
	ACT_E( passwordEdit );
	ACT_E( passwordConfirmEdit );
	ACT_E( coreAddressEdit );
	ACT_E( xmlPortEdit );
	ACT_E( incomingDirEdit );
	ACT_E( tempDirEdit );
	ACT_E( incomingDirButton );
	ACT_E( tempDirButton );
	ACT_E( savePasswordCheckBox );
	setMinimumWidth( e + 10 );
}

