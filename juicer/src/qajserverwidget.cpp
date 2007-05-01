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
#include "qajserverwidget.h"

QAjServerWidget::QAjServerWidget( QXMLModule* xml, QWidget *parent ) : QAjListWidget( xml, parent )
{
    connectedWithId = "";
    connectingToId = "";
    setColumnCount( NUM_SERVER_COL );
    QStringList headers;
    int i;
    for ( i=0; i<NUM_SERVER_COL; i++)
    {
        switch (i)
        {
        case NAME_SERVER_INDEX:
            headers.append( tr("name") );
            break;
        case HOST_SERVER_INDEX:
            headers.append( tr("host") );
            break;
        case PORT_SERVER_INDEX:
            headers.append( tr("port") );
            break;
        case LASTSEEN_SERVER_INDEX:
            headers.append( tr("last seen") );
            break;
        case TESTS_SERVER_INDEX:
            headers.append( tr("tests") );
            break;
        }
    }
    setHeaderLabels( headers );

    QObject::connect( this, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ), this, SLOT( connectSlot() ) );

    serverHttp = new QHttp( this );
    QObject::connect( serverHttp, SIGNAL( requestFinished ( int , bool ) ), this, SLOT( gotServer( int , bool ) ) );

    initToolBar();
    initPopup();
    newSelection(false);
}


QAjServerWidget::~QAjServerWidget()
{}


/*!
    \fn QAjServerWidget::initToolBar()
 */
void QAjServerWidget::initToolBar()
{
    toolBar = new QToolBar( "server operations", this );

    connectButton = toolBar->addAction( QIcon(":/connect.png"), "connect to this server", this, SLOT( connectSlot() ) );
    removeButton = toolBar->addAction( QIcon(":/cancel.png"), "remove server", this, SLOT( removeSlot() ) );
    findButton = toolBar->addAction( QIcon(":/find.png"), "find server", this, SLOT( findSlot() ) );

    QObject::connect( this, SIGNAL( newSelection( bool ) ) , connectButton, SLOT( setEnabled( bool ) ) );
    QObject::connect( this, SIGNAL( newSelection( bool ) ) , removeButton, SLOT( setEnabled( bool ) ) );
}


void QAjServerWidget::insertServer( QString id, QString name, QString host, QString port, QString lastseen, QString tests )
{
    QAjServerItem *item = findServer( id );
    if ( item == NULL )
    {
        lastseen = lastseen.left(lastseen.length()-3);
        time_t time = lastseen.toLong();
        struct tm *time2;
        time2 = localtime( &time );
        //QString timeString(asctime(time2));
        QString timeString = "";
        timeString += QConvert::num( time2->tm_mday ) + ". ";
        timeString += QConvert::num( time2->tm_mon + 1 ) + ". ";
        timeString += QConvert::num( time2->tm_year + 1900 );
        timeString += " ";
        timeString += QConvert::num( time2->tm_hour ) + ":";
        timeString += QConvert::num( time2->tm_min );
        item = new QAjServerItem( id, this );
        servers[ id ] = item;
        item->setText( NAME_SERVER_INDEX, name );
        item->setText( HOST_SERVER_INDEX, host );
        item->setText( PORT_SERVER_INDEX, port );
        item->setText( LASTSEEN_SERVER_INDEX, timeString );
        item->setText( TESTS_SERVER_INDEX, tests );
        item->setTextAlignment( TESTS_SERVER_INDEX, Qt::AlignRight );
        if( id == connectedWithId )
            item->setIcon( NAME_SERVER_INDEX, QIcon(":/small/connected.png") );
        else if( id == connectingToId )
            item->setIcon( NAME_SERVER_INDEX, QIcon(":/small/connect.png") );
    }
}

void QAjServerWidget::removeSlot()
{
    QList<QAjItem *>  selectedItems = selectedAjItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        xml->set( "removeserver", "&id=" + selectedItems[i]->getId() );
    }
}

void QAjServerWidget::connectSlot()
{
    QList<QAjItem *>  selectedItems = selectedAjItems();
    if ( ! selectedItems.empty() )
    {
        xml->set( "serverlogin", "&id=" + selectedItems.first()->getId() );
    }
}

void QAjServerWidget::findSlot()
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

void QAjServerWidget::connectedWith( QString id )
{
    if ( ! connectingToId.isEmpty() && servers.contains( connectingToId ) )
    {
        servers[ connectingToId ]->setIcon( NAME_SERVER_INDEX, QIcon() );
    }
    if ( ! connectedWithId.isEmpty() && servers.contains( connectedWithId ) )
    {
        servers[ connectedWithId ]->setIcon( NAME_SERVER_INDEX, QIcon() );
    }
    if ( servers.contains( id ) )
    {
        servers[ id ]->setIcon( NAME_SERVER_INDEX, QIcon(":/small/connected.png") );
        //connected( servers[ id ]->text( NAME_SERVER_INDEX ) );
    }
    connectedWithId = id;
    connectingToId = "";
}

void QAjServerWidget::connectingTo( QString id )
{
    if ( ! connectingToId.isEmpty() && servers.contains( connectingToId ) )
    {
        servers[ connectingToId ]->setIcon( NAME_SERVER_INDEX, QIcon() );
    }
    if ( servers.contains( id ) )
    {
        servers[ id ]->setIcon( NAME_SERVER_INDEX, QIcon(":/small/connect.png") );
    }
    connectingToId = id;
}

QAjServerItem* QAjServerWidget::findServer( QString id )
{
    if ( servers.contains( id ) )
        return servers[ id ];
    else
        return NULL;
}

bool QAjServerWidget::remove( QString id )
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

void QAjServerWidget::gotServer( int , bool error )
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
    \fn QAjServerWidget::initPopup()
 */
void QAjServerWidget::initPopup()
{
    popup->setTitle( tr("Ser&ver") );
    popup->addAction( connectButton );
    popup->addAction( removeButton );
    popup->addSeparator();
    popup->addAction( findButton );
}
