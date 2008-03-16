/***************************************************************************
 *   Copyright (C) 2007 by Matthias Reif   *
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
#include "qajservermetawidget.h"

QAjServerMetaWidget::QAjServerMetaWidget(QWidget *parent) : QMainWindow(parent)
{
    dock = new AjDockWidget( tr("server welcome message"), this );
    #if QT_VERSION >= 0x040300
        connect( dock, SIGNAL(dockLocationChanged( Qt::DockWidgetArea )), this, SLOT(welcomeDockLocationChanged(Qt::DockWidgetArea)));
    #endif

    welcomeMessage = new AjTextEdit( dock );
    welcomeMessage->setReadOnly( true );
    dock->setWidget(welcomeMessage);

    QSettings localSettings;
    localSettings.beginGroup("WelcomeDock");
    addDockWidget((Qt::DockWidgetArea)localSettings.value("pos", Qt::RightDockWidgetArea).toInt(), dock);
    dock->setVisible(localSettings.value("visible", true).toBool());
    localSettings.endGroup();
}


QAjServerMetaWidget::~QAjServerMetaWidget()
{
}

/*!
    \fn QAjServerMetaWidget::welcomeDockLocationChanged( Qt::DockWidgetArea area )
    prepared for qt 4.3
 */
void QAjServerMetaWidget::welcomeDockLocationChanged( Qt::DockWidgetArea area )
{
    QSettings localSettings;
    localSettings.beginGroup("WelcomeDock");
    localSettings.setValue("pos", area);
    localSettings.endGroup();
}


/*!
    \fn QAjServerMetaWidget::setServerWidget(QAjServerWidget* serverWidget)
 */
void QAjServerMetaWidget::setServerWidget(QAjServerWidget* serverWidget)
{
    this->serverWidget = serverWidget;
    this->setCentralWidget(serverWidget);

    serverWidget->popup->addSeparator();
    showWelcomeAction = new QAction(tr("show welcome message"), this );
    showWelcomeAction->setCheckable( true );
    serverWidget->popup->addAction( showWelcomeAction );
    QObject::connect(showWelcomeAction, SIGNAL(toggled(bool)), dock, SLOT(setVisible(bool)));
    QObject::connect(showWelcomeAction, SIGNAL(toggled(bool)), this, SLOT(dockVisibleSlot(bool)));

    QSettings localSettings;
    localSettings.beginGroup("WelcomeDock");
    showWelcomeAction->setChecked(localSettings.value("visible", true).toBool());
    localSettings.endGroup();
}


/*!
    \fn QAjServerMetaWidget::dockVisibleSlot( bool visible )
 */
void QAjServerMetaWidget::dockVisibleSlot( bool visible )
{
    dock->enabled = visible;
}
