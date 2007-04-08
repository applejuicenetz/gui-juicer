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
#include "qajdownloadwidget.h"

#include "./xpm/resume_small.xpm"
#include "./xpm/pause_small.xpm"
#include "./xpm/cancel_small.xpm"
#include "./xpm/partlist_small.xpm"
#include "./xpm/rename_small.xpm"
#include "./xpm/rename_plus_small.xpm"
#include "./xpm/filter_small.xpm"

#include "./xpm/windows.xpm"
#include "./xpm/linux.xpm"

QAjDownloadWidget::QAjDownloadWidget( QWidget *parent, const char *name) : QAjListWidget( ID_DOWN_INDEX, parent, name)
{
    userStatusDescr["1"] = QObject::tr("unasked ");
    userStatusDescr["2"] = QObject::tr("try to connect ");
    userStatusDescr["3"] = QObject::tr("partner have to old vers. ");
    userStatusDescr["4"] = QObject::tr("partner can't	 open file ");
    userStatusDescr["5"] = QObject::tr("queueing ");
    userStatusDescr["6"] = QObject::tr("no usable parts ");
    userStatusDescr["7"] = QObject::tr("loading ");
    userStatusDescr["8"] = QObject::tr("not enough disk space ");
    userStatusDescr["9"] = QObject::tr("finished ");
    userStatusDescr["11"] = QObject::tr("no connection possible ");
    userStatusDescr["12"] = QObject::tr("try indirect ");
    userStatusDescr["13"] = QObject::tr("paused ");
    userStatusDescr["14"] = QObject::tr("queue full ");
    userStatusDescr["15"] = QObject::tr("own limit reached ");
    userStatusDescr["16"] = QObject::tr("indirect conn. rejected ");
    downloadStatusDescr["-1"] = QObject::tr("loading ");
    downloadStatusDescr["0"] = QObject::tr("searching ");
    downloadStatusDescr["1"] = QObject::tr("error at creating ");
    downloadStatusDescr["12"] = QObject::tr("finishing ");
    downloadStatusDescr["13"] = QObject::tr("error at finishing ");
    downloadStatusDescr["14"] = QObject::tr("finished ");
    downloadStatusDescr["15"] = QObject::tr("canceling ");
    downloadStatusDescr["16"] = QObject::tr("creating .dat ");
    downloadStatusDescr["17"] = QObject::tr("canceled ");
    downloadStatusDescr["18"] = QObject::tr("paused ");

    linuxIcon = new QIcon(QPixmap(linux_xpm));
    windowsIcon = new QIcon(QPixmap(windows_xpm));
    otherOsIcon = new QIcon();

    currIdRoundRobin = -1;
    QStringList headers;
    int i;
    for ( i=0; i<NUM_DOWN_COL; i++)
    {
        switch (i)
        {
        case ID_DOWN_INDEX:
            headers.append( tr("id") );
            break;
        case FILENAME_DOWN_INDEX:
            headers.append( tr("filename") );
            break;
        case SPEED_DOWN_INDEX:
            headers.append( tr("speed") );
            break;
        case STATUS_DOWN_INDEX:
            headers.append( tr("status") );
            break;
        case SOURCES_DOWN_INDEX:
            headers.append( tr("sources") );
            break;
        case FINISHED_DOWN_INDEX:
            headers.append( tr("finished") );
            break;
        case SIZE_DOWN_INDEX:
            headers.append( tr("size") );
            break;
        case REMAIN_SIZE_DOWN_INDEX:
            headers.append( tr("remaining") );
            break;
        case FINISHED_SIZE_DOWN_INDEX:
            headers.append( tr("finished") );
            break;
        case REMAIN_TIME_DOWN_INDEX:
            headers.append( tr("eta") );
            break;
        case POWER_DOWN_INDEX:
            headers.append( tr("power") );
            break;
        case MISSING_DOWN_INDEX:
            headers.append( tr("not seen") );
            break;
        }
    }
    setHeaderLabels( headers );

    setColumnHidden( ID_DOWN_INDEX, true );

    popup->setTitle( tr("&Download") );
    pauseId = popup->addAction( QIcon(QPixmap(pause_small_xpm)), "pause", this, SLOT(pauseSlot()) );
    resumeId = popup->addAction( QIcon(QPixmap(resume_small_xpm)), "resume", this, SLOT(resumeSlot()) );
    cancelId = popup->addAction( QIcon(QPixmap(cancel_small_xpm)), "cancel", this, SLOT(cancelSlot()) );
    partListId = popup->addAction( QIcon(QPixmap(partlist_small_xpm)), "part list", this, SLOT(partListSlot()) );
    renameId = popup->addAction( QIcon(QPixmap(rename_small_xpm)), "rename", this, SLOT(renameSlot()) );
    renamePlusId = popup->addAction( QIcon(QPixmap(rename_plus_small_xpm)), "rename by clipboard", this, SLOT(renamePlusSlot()) );
    popup->addSeparator();
    popup->addAction( QIcon(QPixmap(filter_small_xpm)), "remove finished/canceld", this, SLOT(cleanSlot()) );
    pauseId->setEnabled( false );
    resumeId->setEnabled( false );
    cancelId->setEnabled( false );
    QObject::connect( this, SIGNAL( newSelection( bool ) ) , this, SLOT( selectionChanged1( bool ) ) );

    setIconSize( QSize( 100, 20 ) );
}

QAjDownloadWidget::~QAjDownloadWidget()
{
    delete linuxIcon;
    delete windowsIcon;
    delete otherOsIcon;
}

void QAjDownloadWidget::insertDownload(QString id, QString fileName, QString status, QString size, QString ready, QString power, QString tempNumber)
{
    QAjDownloadItem *downloadItem = findDownload( id );
    if ( downloadItem == NULL )
    {
        downloadItem = new QAjDownloadItem( this );
        downloads[ id ] = downloadItem;
        downloadItem->setText( ID_DOWN_INDEX, id );
    }
    downloadItem->update( fileName, status, size, ready, power, tempNumber );
}


void QAjDownloadWidget::insertUser(QString downloadId, QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString os)
{
    QAjDownloadItem *downloadItem = findDownload( downloadId );
    if ( downloadItem == NULL )
    {
        downloadItem = new QAjDownloadItem( this );
        downloads[ downloadId ] = downloadItem;
        downloadItem->setText( ID_DOWN_INDEX, downloadId );
        downloadItem->setText( FILENAME_DOWN_INDEX, fileName );
    }
    QIcon *osIcon;
    if ( os == LINUX )
        osIcon = linuxIcon;
    else if ( os == WINDOWS )
        osIcon = windowsIcon;
    else
        osIcon = otherOsIcon;
    downloadItem->updateUser( id, fileName, speed, status, power, queuePos, userStatusDescr[status], osIcon );
}


bool QAjDownloadWidget::remove( QString id )
{
    QAjDownloadItem* downloadItem = removeDownload( id );
    if ( downloadItem != NULL )
    {
        delete downloadItem;
        return true;
    }
    else
    {
        DownloadUser du = findParent( id );
        if ( du.user != NULL )
        {
            du.download->removeUser( id );
            du.download->decSources( du.user->getStatus() );
            delete du.user;
            return true;
        }
    }
    return false;
}

void QAjDownloadWidget::cancelSlot()
{
    cancel();
}
void QAjDownloadWidget::cleanSlot()
{
    clean();
}
void QAjDownloadWidget::resumeSlot()
{
    resume();
}
void QAjDownloadWidget::pauseSlot()
{
    pause();
}
void QAjDownloadWidget::partListSlot()
{
    partListRequest();
}
void QAjDownloadWidget::renameSlot()
{
    rename();
}
void QAjDownloadWidget::renamePlusSlot()
{
    renamePlus();
}

void QAjDownloadWidget::selectionChanged1(  bool oneSelected  )
{
    pauseId->setEnabled( oneSelected );
    resumeId->setEnabled( oneSelected );
    cancelId->setEnabled( oneSelected );
}

void QAjDownloadWidget::updateView()
{
    if ( this->isVisible() )
    {
        int i;
        for (i=0; i<topLevelItemCount(); i++)
        {
            ((QAjDownloadItem*)topLevelItem(i))->updateView( &downloadStatusDescr );
        }
    }
}

QAjDownloadItem* QAjDownloadWidget::findDownload( QString id )
{
    if (downloads.contains( id ))
        return downloads[id];
    else
        return NULL;
}

QAjDownloadItem* QAjDownloadWidget::removeDownload( QString id )
{
    QAjDownloadItem* item = NULL;
    if (downloads.contains( id ))
    {
        item = downloads[id];
        downloads.remove( id );
    }
    return item;
}


DownloadUser QAjDownloadWidget::findParent( QString id )
{
    DownloadUser du;
    du.download = NULL;
    du.user = NULL;
    int i;
    for (i=0; i<topLevelItemCount() && du.user == NULL; i++)
    {
        du.download = (QAjDownloadItem*)topLevelItem(i);
        du.user = du.download->findUser( id );
    }
    return du;
}


/*!
    \fn QAjDownloadWidget::getNextIdRoundRobin()
 */
QString QAjDownloadWidget::getNextIdRoundRobin()
{
    if (topLevelItemCount() < 1)
        return "";
    currIdRoundRobin ++;
    currIdRoundRobin %= topLevelItemCount();
    return topLevelItem( currIdRoundRobin )->text( ID_DOWN_INDEX );
}


/*!
    \fn QAjDownloadWidget::findDownloadByTempNum( QString tempNum )
 */
QAjDownloadItem* QAjDownloadWidget::findDownloadByTempNum( QString tempNum )
{
    QAjDownloadItem* item;
    int i;
    for ( i=0; i<topLevelItemCount(); i++ )
    {
        item = (QAjDownloadItem*)topLevelItem(i);
        if ( item->getTempNumber() == tempNum )
            return item;
    }
    return NULL;
}
