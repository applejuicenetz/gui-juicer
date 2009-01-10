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
#include "qajservermodule.h"
#include "juicer.h"

QAjServerModule::QAjServerModule(Juicer* juicer) : QAjModuleBase(juicer, juicer->serverTreeWidget, juicer->serverToolBar)
{
    zeroTime = QDateTime( QDate(1970,1,1), QTime(0,0), Qt::UTC );
    connectedWithId = "";
    connectingToId = "";

    QObject::connect(treeWidget, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ), this, SLOT( connectSlot() ) );
    QObject::connect(juicer->actionConnect_Server, SIGNAL(triggered()), this, SLOT(connectSlot()));
    QObject::connect(juicer->actionRemove_Server, SIGNAL(triggered()), this, SLOT(removeSlot()));
    QObject::connect(juicer->actionSearch_Server, SIGNAL(triggered()), this, SLOT(searchSlot()));

    serverHttp = new QHttp( this );
    QObject::connect( serverHttp, SIGNAL( requestFinished ( int , bool ) ), this, SLOT( gotServer( int , bool ) ) );

    QObject::connect(juicer->actionShow_Welcome_Message, SIGNAL(triggered(bool)), juicer->welcomeDock, SLOT(setVisible(bool)));
    QObject::connect(juicer->welcomeDock, SIGNAL(visibilityChanged(bool)), juicer->actionShow_Welcome_Message, SLOT(setChecked(bool)));
//     QObject::connect(juicer->actionShow_Welcome_Message, SIGNAL(triggered(bool)), this, SLOT(welcomeDockVisible(bool)));
    QObject::connect(juicer->welcomeDock->toggleViewAction(), SIGNAL(triggered(bool)), this, SLOT(welcomeDockVisible(bool)));

    juicer->welcomeDock->setVisible(QAjOptionsDialog::getSetting("WelcomeDock", "visible", true).toBool());
    selectionChanged();
}


QAjServerModule::~QAjServerModule() {
}


void QAjServerModule::insertServer( QString id, QString name, QString host, QString port, QString lastseen, QString tests )
{
    QAjServerItem *item = findServer( id );
    if ( item == NULL )
    {
        QString time = zeroTime.addMSecs( lastseen.toULongLong() ).toLocalTime().toString( Qt::LocalDate );
        item = new QAjServerItem( id, treeWidget );
        servers[ id ] = item;
        item->setText( QAjServerItem::NAME_COL, name );
        item->setText( QAjServerItem::HOST_COL, host );
        item->setText( QAjServerItem::PORT_COL, port );
        item->setText( QAjServerItem::LASTSEEN_COL, time );
        item->setText( QAjServerItem::TESTS_COL, tests );
        if( id == connectedWithId )
            item->setIcon( QAjServerItem::NAME_COL, QIcon(":/small/connected.png") );
        else if( id == connectingToId )
            item->setIcon( QAjServerItem::NAME_COL, QIcon(":/small/connect.png") );
    }
}

void QAjServerModule::removeSlot()
{
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    for(QList<QTreeWidgetItem*>::iterator i = items.begin(); i!=items.end(); i++) {
        xml->set( "removeserver", "&id=" + ((QAjItem*)(*i))->getId() );
    }
}

void QAjServerModule::connectSlot()
{
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    if(!items.empty()) {
        xml->set("serverlogin", "&id=" + ((QAjItem*)items.first())->getId());
    }
}

void QAjServerModule::searchSlot()
{
    QString serverURL = QAjOptionsDialog::getSetting( "serverURL", "http://www.applejuicenet.de/18.0.html" ).toString();

    QString host;
    if ( serverURL.indexOf( "http://", 0, Qt::CaseInsensitive ) == 0 )
        host = serverURL.section( '/', 2, 2);
    else
        host = serverURL.section( '/', 0, 0);

    serverHttp->setHost( host );
    serverHttp->get( serverURL );
}

void QAjServerModule::connectedWith( QString id )
{
    if ( ! connectingToId.isEmpty() && servers.contains( connectingToId ) )
    {
        servers[ connectingToId ]->setIcon( QAjServerItem::NAME_COL, QIcon() );
    }
    if ( ! connectedWithId.isEmpty() && servers.contains( connectedWithId ) )
    {
        servers[ connectedWithId ]->setIcon( QAjServerItem::NAME_COL, QIcon() );
    }
    if ( servers.contains( id ) )
    {
        servers[ id ]->setIcon( QAjServerItem::NAME_COL, QIcon(":/small/connected.png") );
        //connected( servers[ id ]->text( NAME_COL ) );
    }
    connectedWithId = id;
    connectingToId = "";
}

void QAjServerModule::connectingTo( QString id )
{
    if ( ! connectingToId.isEmpty() && servers.contains( connectingToId ) )
    {
        servers[ connectingToId ]->setIcon( QAjServerItem::NAME_COL, QIcon() );
    }
    if ( servers.contains( id ) )
    {
        servers[ id ]->setIcon( QAjServerItem::NAME_COL, QIcon(":/small/connect.png") );
    }
    connectingToId = id;
}

QAjServerItem* QAjServerModule::findServer( QString id )
{
    if ( servers.contains( id ) )
        return servers[ id ];
    else
        return NULL;
}

bool QAjServerModule::remove( QString id )
{
    QTreeWidgetItem *item = findServer( id );
    if( item != NULL )
    {
        servers.remove( id );
        delete item;
        return true;
    }
    return false;
}

void QAjServerModule::gotServer( int , bool error )
{
    if ( error )
    {
        QMessageBox::critical( NULL, tr("error"), tr("Could not fetch server source.") , QMessageBox::Abort, QMessageBox::Cancel );
    }
    else
    {
        QString data( serverHttp->readAll() );
        // TODO: use regular expressions
        int begin, start, end;
        begin = 0;
        QString link;
        // while we found an ajfsp server link
        while ( ( start = data.indexOf( "ajfsp://server|", begin, Qt::CaseInsensitive ) ) != -1 )
        {
            end = data.indexOf( "/", start + 15 );
            link = QString( QUrl::toPercentEncoding( data.mid(start, end - start +1) ) );
            xml->set( "processlink", "&link=" + link );
            begin = end;
        }
    }
}


/*!
    \fn QAjServerModule::selectionChanged()
 */
void QAjServerModule::selectionChanged() {

    bool oneSelected = !treeWidget->selectedItems().empty();
    juicer->actionConnect_Server->setEnabled(oneSelected);
    juicer->actionRemove_Server->setEnabled(oneSelected);
}


/*!
    \fn QAjServerModule::welcomeDockVisible(bool visible)
 */
void QAjServerModule::welcomeDockVisible(bool visible) {
    QAjOptionsDialog::setSetting("WelcomeDock", "visible", visible);
}
