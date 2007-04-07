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

QAjDownloadWidget::QAjDownloadWidget( QAjIcons *icons, QWidget *parent, const char *name) : QAjListWidget(icons, ID_DOWN_INDEX, parent, name)
{
    currIdRoundRobin = -1;
	QStringList headers;
	int i;
	for( i=0; i<NUM_DOWN_COL; i++)
	{
		switch(i)
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
	
	//setColumnWidthMode(ID_DOWN_INDEX, Q3ListView::Manual );
	setColumnHidden( ID_DOWN_INDEX, true );
/*	setColumnAlignment( SOURCES_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( FINISHED_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( SPEED_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( STATUS_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( SIZE_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( REMAIN_SIZE_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( FINISHED_SIZE_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( POWER_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( REMAIN_TIME_DOWN_INDEX, Qt::AlignRight );
	setColumnAlignment( MISSING_DOWN_INDEX, Qt::AlignRight );
*/
	popup->setTitle( tr("&Download") );
	pauseId = popup->addAction( *icons->downloadPauseSmallIcon, "pause", this, SLOT(pauseSlot()) );
	resumeId = popup->addAction( *icons->downloadResumeSmallIcon, "resume", this, SLOT(resumeSlot()) );
	cancelId = popup->addAction( *icons->downloadCancelSmallIcon, "cancel", this, SLOT(cancelSlot()) );
	partListId = popup->addAction( *icons->partListSmallIcon, "part list", this, SLOT(partListSlot()) );
	renameId = popup->addAction( *icons->downloadRenameSmallIcon, "rename", this, SLOT(renameSlot()) );
   renamePlusId = popup->addAction( *icons->downloadRenamePlusSmallIcon, "rename by clipboard", this, SLOT(renamePlusSlot()) );
	popup->addSeparator();
	popup->addAction( *icons->downloadFilterSmallIcon, "remove finished/canceld", this, SLOT(cleanSlot()) );
	pauseId->setEnabled( false );
	resumeId->setEnabled( false );
	cancelId->setEnabled( false );
	QObject::connect( this, SIGNAL( newSelection( bool ) ) , this, SLOT( selectionChanged1( bool ) ) );
	
	setIconSize( QSize( 100, 20 ) );
}

QAjDownloadWidget::~QAjDownloadWidget()
{
}

int QAjDownloadWidget::insertDownload(QString id, QString fileName, QString status, QString size, QString ready, QString power)
{
	QAjDownloadItem *downloadItem = findDownload( id );
	if( downloadItem == NULL )
	{
		downloadItem = new QAjDownloadItem( &descriptions, icons, this );
		downloads[ id ] = downloadItem;
		downloadItem->setText( ID_DOWN_INDEX, id );
	}
	downloadItem->update( fileName, status, size, ready, power );
	return 0;
}


int QAjDownloadWidget::insertUser(QString downloadId, QString id, QString fileName, QString speed, QString status, QString power, QString queuePos, QString os)
{
	QAjDownloadItem *downloadItem = findDownload( downloadId );
	if( downloadItem == NULL )
	{
		downloadItem = new QAjDownloadItem( &descriptions, icons, this );
		downloads[ downloadId ] = downloadItem;
		downloadItem->setText( ID_DOWN_INDEX, downloadId );
		downloadItem->setText( FILENAME_DOWN_INDEX, fileName );
	}
	downloadItem->updateUser( id, fileName, speed, status, power, queuePos, os );
	return 0;
}


bool QAjDownloadWidget::remove( QString id )
{
	QAjDownloadItem* downloadItem = removeDownload( id );
	if( downloadItem != NULL )
	{
		delete downloadItem;
		return true;
	}
	else
	{
		DownloadUser du = findParent( id );
		if( du.user != NULL )
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
    if( this->isVisible() )
    {
        int i;
        for(i=0; i<topLevelItemCount(); i++) {
            ((QAjDownloadItem*)topLevelItem(i))->updateView();
        }
    }
}

QAjDownloadItem* QAjDownloadWidget::findDownload( QString id )
{
    if(downloads.contains( id ))
        return downloads[id];
    else
        return NULL;
}

QAjDownloadItem* QAjDownloadWidget::removeDownload( QString id )
{
    QAjDownloadItem* item = NULL;
    if(downloads.contains( id ))
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
    for(i=0; i<topLevelItemCount() && du.user == NULL; i++) {
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
    if(topLevelItemCount() < 1)
        return "";
    currIdRoundRobin ++;
    currIdRoundRobin %= topLevelItemCount();
    return topLevelItem( currIdRoundRobin )->text(ID_DOWN_INDEX );
}
