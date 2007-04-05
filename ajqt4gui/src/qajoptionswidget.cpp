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
#include "qajoptionswidget.h"

QAjOptionsWidget::QAjOptionsWidget(QWidget *parent, const char *name) : QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr("Configure") );
	setModal( true );
	
	tabWidget = new QTabWidget( this );
	commonOptionsWidget = new QAjCommonOptionsWidget( NULL );
	netOptionsWidget = new QAjNetOptionsWidget( NULL );

	tabWidget->addTab( commonOptionsWidget, tr("Common") );
	tabWidget->addTab( netOptionsWidget, tr("Network") );
	
	tabWidget->move( 10, 10 );
	tabWidget->resize( commonOptionsWidget->size() );
	
	okButton = new QPushButton( "Ok", this );
	cancelButton = new QPushButton( "Cancel", this );
	
	okButton->adjustSize();
	cancelButton->adjustSize();
	
	cancelButton->move( tabWidget->x() + tabWidget->width() - cancelButton->width(), tabWidget->y() + tabWidget->height() + 20 );
	okButton->move( cancelButton->x() - okButton->width() - 20, cancelButton->y() );
	
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

	adjustSize();
}

QAjOptionsWidget::~QAjOptionsWidget()
{
	delete commonOptionsWidget;
	delete netOptionsWidget;
}

AjSettings QAjOptionsWidget::getAjSettings()
{
	AjSettings settings;
	settings.nick = commonOptionsWidget->getNick();
	settings.password = commonOptionsWidget->getPassword();
	settings.coreAddress = commonOptionsWidget->getCoreAddress();
	settings.xmlPort = commonOptionsWidget->getXmlPort();
	settings.incomingDir = commonOptionsWidget->getIncomingDir();
	settings.tempDir = commonOptionsWidget->getTempDir();
	
	settings.serverURL = commonOptionsWidget->getServerURL();
	
	settings.refresh = commonOptionsWidget->getTimer();
	
	settings.autoconnect = commonOptionsWidget->getAutoconnect();
	settings.allowBrowse = commonOptionsWidget->getAllowBrowse();
	settings.savePassword = commonOptionsWidget->getSavePassword();

	settings.maxDown = netOptionsWidget->getMaxDown();
	settings.maxUp = netOptionsWidget->getMaxUp();
	settings.maxSlot = netOptionsWidget->getMaxSlot();
	settings.maxSources = netOptionsWidget->getMaxSources();
	settings.maxCon = netOptionsWidget->getMaxCon();
	settings.maxNewCon = netOptionsWidget->getMaxNewCon();

	settings.tcpPort = netOptionsWidget->getTcpPort();
	return settings;
}

void QAjOptionsWidget::setAjSettings( AjSettings settings )
{
	commonOptionsWidget->setAjSettigs( settings );
	netOptionsWidget->setAjSettigs( settings );
}
