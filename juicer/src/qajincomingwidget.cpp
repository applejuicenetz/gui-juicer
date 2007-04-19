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
#include "qajincomingwidget.h"

#include "juicer.h"

QAjIncomingWidget::QAjIncomingWidget( QXMLModule* xml, QWidget *parent ) : QAjListWidget( xml, parent )
{
    QStringList headers;
    int i;
    for ( i=0; i<NUM_INCOMING_COL; i++)
    {
        switch (i)
        {
        case FILENAME_INCOMING_INDEX:
            headers.append( tr("filename") );
            break;
        case SIZE_INCOMING_INDEX:
            headers.append( tr("size") );
            break;
        case DATE_INCOMING_INDEX:
            headers.append( tr("last modified") );
            break;
        }
    }
    setHeaderLabels( headers );

    ftp = new QFtp( this );

    connect( ftp, SIGNAL( listInfo ( QUrlInfo ) ), this, SLOT( insert( QUrlInfo ) ) );
    connect( this, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ), this, SLOT( open() ) );

    initToolBar();
}


QAjIncomingWidget::~QAjIncomingWidget()
{
}


/*!
    \fn QAjIncomingWidget::initToolBar()
 */
void QAjIncomingWidget::initToolBar()
{
    toolBar = new QToolBar( "incoming operations", this );
    reloadButton = toolBar->addAction( QIcon(":/reload.png"), "reload", this, SLOT( reload() ) );
    openButton = toolBar->addAction( QIcon(":/exec.png"), "open", this, SLOT( open() ) );
    saveButton = toolBar->addAction( QIcon(":/save.png"), "save/copy", this, SLOT( save() ) );
}

/*!
    \fn QAjIncomingWidget::reloadFtp()
 */
void QAjIncomingWidget::reloadFtp()
{
    this->clear();

    QSettings lokalSettings;
    lokalSettings.beginGroup("ftp");
    QString server = lokalSettings.value( "server", "localhost" ).toString();
    int port = lokalSettings.value( "port", "21" ).toInt();
    QString user = lokalSettings.value( "user", "anonymous" ).toString();
    QString password = lokalSettings.value( "password", "foobar@msn.com" ).toString();
    QString dir = lokalSettings.value( "dir", "/" ).toString();
    lokalSettings.endGroup();

    if ( ftp->state() != QFtp::Unconnected )
    {
        ftp->close();
    }
    ftp->connectToHost( server, port );
    ftp->login( user, password );
    ftp->setTransferMode( QFtp::Passive );
    ftp->list( dir );
}


/*!
    \fn QAjIncomingWidget::save()
 */
void QAjIncomingWidget::save()
{
    QString actDir;
    // determine the path
    QSettings lokalSettings;
    QString location = lokalSettings.value( "location", "same" ).toString();
    if(location == "ftp")
    {
        storeFtp();
    }
    else
    {
        if( location == "specific" )
        {
            actDir = lokalSettings.value( "incomingDirSpecific", "/" ).toString() + QDir::separator();
        }
        else if( location == "same" )
        {
            actDir = this->dir + QDir::separator();
        }
        QList<QAjItem*> items = selectedAjItems();
        int i;
        for( i=0; i<items.size(); i++ )
        {
            QString newDir = QFileDialog::getExistingDirectory(this, "copy to", actDir)
                    + QDir::separator();
            if(!newDir.isEmpty())
            {
                QString newFilename = items[i]->text( FILENAME_INCOMING_INDEX );
                while(!newFilename.isEmpty() && QFile::exists(newDir + newFilename))
                {
                    newFilename = QInputDialog::getText(this, "file already exists",
                            "filename", QLineEdit::Normal, items[i]->text( FILENAME_INCOMING_INDEX ));
                }
                if(!newFilename.isEmpty())
                {
                    (new CopyThread(actDir + items[i]->text( FILENAME_INCOMING_INDEX ),
                                newDir + newFilename))->start();
                }
            }
        }
    }
}


/*!
    \fn QAjIncomingWidget::storeFtp()
 */
void QAjIncomingWidget::storeFtp()
{
    QString filename, localDir;
    QList<QTreeWidgetItem *>  selectedItems = this->selectedItems();
    
    QSettings lokalSettings;
    lokalSettings.beginGroup("ftp");
    QString dir = lokalSettings.value( "incoming", "/" ).toString();
    lokalSettings.endGroup();

    // TODO: check if the core filesystem separator is a better choice
    if ( ! dir.endsWith( '/' ) )
    {
        dir += '/';
    }
    FTP* ftp = new FTP( this );

    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        filename = selectedItems.at(i)->text( FILENAME_INCOMING_INDEX );
        localDir = QFileDialog::getExistingDirectory( this, "save \"" + filename + "\" + to" );
        if ( localDir != "" )
        {
            if ( ! localDir.endsWith( QDir::separator() ) )
            {
                localDir += QDir::separator();
            }
            QFile* dstFile = new QFile( localDir + filename );
            if ( ! dstFile->exists() )
            {
                dstFile->open( QIODevice::WriteOnly );
                ftp->add( dir + filename, dstFile );
            }
            else
            {
                QMessageBox::critical( this, "error", "\"" + dstFile->fileName() + "\" already exists", QMessageBox::Ok, QMessageBox::NoButton );
            }
        }
    }
    ftp->start();
}


/*!
    \fn QAjIncomingWidget::reload()
 */
void QAjIncomingWidget::reload()
{
    this->clear();
    QSettings lokalSettings;
    QString mode = lokalSettings.value( "location", "same" ).toString();
    if(mode == "ftp")
    {
        reloadFtp();
    }
    else
    {
        QString actDir;
        if(mode == "same")
        {
            if(this->dir.isEmpty())
                return;
            actDir = this->dir;
        }
        else
        {
            actDir = lokalSettings.value( "incomingDirSpecific", "/" ).toString();
        }
        QDir dirDir(actDir);
        if(dirDir.exists())
        {
            QFileInfoList list = dirDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
            int i;
            for(i=0; i<list.size(); i++)
            {
                QAjItem* item = new QAjItem( this );
                item->setText(FILENAME_INCOMING_INDEX, list[i].fileName());
                item->setText(SIZE_INCOMING_INDEX, QConvert::bytes((double)list[i].size(), 2));
                item->setText(DATE_INCOMING_INDEX, list[i].lastModified().toLocalTime().toString() );
                this->addTopLevelItem( item );
            }
        }
    }
    adjustSizeOfColumns();
}


/*!
    \fn QAjIncomingWidget::setDir( QString dir )
 */
void QAjIncomingWidget::setDir( QString dir )
{
    this->dir = dir;
}


/*!
    \fn QAjIncomingWidget::open()
 */
void QAjIncomingWidget::open()
{
    QStringList args = Juicer::getExec();
    QString exec = args.takeFirst();

    QString actDir;
    // determine the path
    QSettings lokalSettings;
    QString location = lokalSettings.value( "location", "same" ).toString();
    if( location == "specific" )
    {
        actDir = lokalSettings.value( "incomingDirSpecific", "/" ).toString() + QDir::separator();
    }
    else if( location == "same" )
    {
        actDir = this->dir + QDir::separator();
    }
    else // ftp
    {
        QMessageBox::information(this, "open file", "opening via ftp currently not supported");
        return;
    }

    QList<QAjItem*> items = selectedAjItems();
    int i;
    for (i=0; i<items.size(); i++)
    {
        args <<  actDir + items[i]->text( FILENAME_INCOMING_INDEX );
        QProcess::startDetached( exec, args );
        args.removeLast();
    }
}

/*!
    \fn QAjIncomingWidget::insert( QUrlInfo info )
 */
void QAjIncomingWidget::insert( QUrlInfo info )
{
    if ( info.isFile() || info.isDir() )
    {
        QAjItem *item = new QAjItem( this );
        item->setText( FILENAME_INCOMING_INDEX, info.name() );
        item->setText( SIZE_INCOMING_INDEX, QConvert::bytes( (double)info.size(), 2 ) );
        item->setText( DATE_INCOMING_INDEX, info.lastModified().toLocalTime().toString() );
        adjustSizeOfColumns();
    }
}

