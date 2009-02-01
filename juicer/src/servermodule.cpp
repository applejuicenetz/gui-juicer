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
#include "servermodule.h"
#include "juicer.h"

ServerModule::ServerModule(Juicer* juicer) : ModuleBase(juicer, juicer->serverTreeWidget, juicer->serverToolBar)
{
    zeroTime = QDateTime( QDate(1970,1,1), QTime(0,0), Qt::UTC );
    connectedSince = QDateTime();
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

    selectionChanged();
}


ServerModule::~ServerModule() {
}


void ServerModule::insertServer(const QString& id, const QString& name, const QString& host, const QString& port, const QString& lastseen, const QString& tests) {
    ServerItem *item = findServer(id);
    if(item == NULL) {
        item = new ServerItem( id, treeWidget );
        servers[ id ] = item;
    }
    QString time = zeroTime.addMSecs(lastseen.toULongLong()).toLocalTime().toString(Qt::LocalDate);
    item->update(name, host, port, lastseen, tests, time);
    if(id == connectedWithId) {
        item->setIcon(ServerItem::NAME_COL, QIcon(":/small/connected.png"));
    } else if(id == connectingToId) {
        item->setIcon(ServerItem::NAME_COL, QIcon(":/small/connect.png"));
    } else {
        item->setIcon(ServerItem::NAME_COL, QIcon());
    }
}

void ServerModule::removeSlot()
{
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    for(QList<QTreeWidgetItem*>::iterator i = items.begin(); i!=items.end(); i++) {
        xml->set( "removeserver", "&id=" + ((Item*)(*i))->getId() );
    }
}

void ServerModule::connectSlot() {
    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    if(!items.empty()) {
        bool doSo = true;
        QDateTime now = zeroTime.addMSecs(xml->getRecentTime().toULongLong());
        if(connectedSince.isValid() && now.isValid()) {
            int minutes = connectedSince.secsTo(now) / 60 + 1;
            if(minutes < 30) {
                doSo = QMessageBox::question(juicer, tr("Connect"),
                    tr("You are connected to a server for %n minute(s). If you try to change the server befor being connected for 30 minutes, you have to wait for 30 minutes without any server connection.", "", minutes)
                    + "\n\n" + tr("Are you sure you want to reconnect?"),
                    QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
            }
        }
        if(doSo) {
            xml->set("serverlogin", "&id=" + ((Item*)items.first())->getId());
        }
    }
}

void ServerModule::searchSlot()
{
    QString serverURL = OptionsDialog::getSetting( "serverURL", "http://www.applejuicenet.de/18.0.html" ).toString();

    QString host;
    if ( serverURL.indexOf( "http://", 0, Qt::CaseInsensitive ) == 0 )
        host = serverURL.section( '/', 2, 2);
    else
        host = serverURL.section( '/', 0, 0);

    serverHttp->setHost( host );
    serverHttp->get( serverURL );
}

void ServerModule::connectedWith( QString id )
{
    if ( ! connectingToId.isEmpty() && servers.contains( connectingToId ) )
    {
        servers[ connectingToId ]->setIcon( ServerItem::NAME_COL, QIcon() );
    }
    if ( ! connectedWithId.isEmpty() && servers.contains( connectedWithId ) )
    {
        servers[ connectedWithId ]->setIcon( ServerItem::NAME_COL, QIcon() );
    }
    if ( servers.contains( id ) )
    {
        servers[ id ]->setIcon( ServerItem::NAME_COL, QIcon(":/small/connected.png") );
        //connected( servers[ id ]->text( NAME_COL ) );
    }
    connectedWithId = id;
    connectingToId = "";
}

void ServerModule::connectingTo( QString id )
{
    if ( ! connectingToId.isEmpty() && servers.contains( connectingToId ) )
    {
        servers[ connectingToId ]->setIcon( ServerItem::NAME_COL, QIcon() );
    }
    if ( servers.contains( id ) )
    {
        servers[ id ]->setIcon( ServerItem::NAME_COL, QIcon(":/small/connect.png") );
    }
    connectingToId = id;
}

ServerItem* ServerModule::findServer( QString id )
{
    if ( servers.contains( id ) )
        return servers[ id ];
    else
        return NULL;
}

bool ServerModule::remove( QString id )
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

void ServerModule::gotServer( int , bool error )
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
    \fn ServerModule::selectionChanged()
 */
void ServerModule::selectionChanged() {
    bool oneSelected = !treeWidget->selectedItems().empty();
    juicer->actionConnect_Server->setEnabled(oneSelected);
    juicer->actionRemove_Server->setEnabled(oneSelected);
}


/*!
    \fn ServerModule::setConnectedSince(const QString& time)
 */
QDateTime& ServerModule::setConnectedSince(const QString& time)
{
    if(time != "0") {
        connectedSince = zeroTime.addMSecs(time.toULongLong());
    } else {
        connectedSince = QDateTime();
    }
    return connectedSince;
}
