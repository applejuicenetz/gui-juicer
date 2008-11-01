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
    initPopup();
    newSelection( false );

    waitLabel = new QLabel( tr("please wait..."), this );

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
    openButton = toolBar->addAction( QIcon(":/exec.png"), tr("open"), this, SLOT( open() ) );
    saveButton = toolBar->addAction( QIcon(":/save.png"), tr("copy"), this, SLOT( save() ) );
    removeButton = toolBar->addAction( QIcon(":/cancel.png"), tr("delete"), this, SLOT( remove() ) );
    toolBar->addSeparator();
    reloadButton = toolBar->addAction( QIcon(":/reload.png"), tr("reload"), this, SLOT( reload() ) );

    connect( this, SIGNAL( newSelection( bool ) ) , openButton, SLOT( setEnabled( bool ) ) );
    connect( this, SIGNAL( newSelection( bool ) ) , saveButton, SLOT( setEnabled( bool ) ) );
    connect( this, SIGNAL( newSelection( bool ) ) , removeButton, SLOT( setEnabled( bool ) ) );
}

/*!
    \fn QAjIncomingWidget::reloadFtp()
 */
void QAjIncomingWidget::reloadFtp()
{
    this->clear();
    waitLabel->move( (this->width() - waitLabel->width()) /2, (this->height() - waitLabel->height()) / 2);
    waitLabel->show();

    QString server = QAjOptionsDialog::getSetting( "ftp", "server", "localhost" ).toString();
    int port = QAjOptionsDialog::getSetting( "ftp", "port", "21" ).toInt();
    QString user = QAjOptionsDialog::getSetting( "ftp", "user", "anonymous" ).toString();
    QString password = QAjOptionsDialog::getSetting( "ftp", "password", "" ).toString();
    QString dir = QAjOptionsDialog::getSetting( "ftp", "inDir", "/" ).toString();
    QFtp::TransferMode mode = (QFtp::TransferMode)QAjOptionsDialog::getSetting( "ftp", "mode", QFtp::Active ).toInt();

    if ( ftp->state() != QFtp::Unconnected )
    {
        ftp->close();
    }
    ftp->connectToHost( server, port );
    ftp->login( user, password );
    ftp->setTransferMode( mode );
    ftp->list( dir );
}


/*!
    \fn QAjIncomingWidget::save()
 */
void QAjIncomingWidget::save()
{
    QString actDir;
    // determine the path
    AjSettings::LOCATION location = getLocation();
    if(location == AjSettings::FTP)
    {
        storeFtp();
    }
    else
    {
        if( location == AjSettings::SPECIFIC )
        {
            actDir = QAjOptionsDialog::getSetting( "incomingDirSpecific", "/" ).toString() + QDir::separator();
        }
        else if( location == AjSettings::SAME )
        {
            actDir = this->dir + QDir::separator();
        }
        QList<QAjItem*> items = selectedAjItems();
        int i;
        for( i=0; i<items.size(); i++ )
        {
            QString newDir = QFileDialog::getExistingDirectory(this, tr("copy to"), actDir)
                    + QDir::separator();
            if(!newDir.isEmpty())
            {
                QString newFilename = items[i]->text( FILENAME_INCOMING_INDEX );
                while(!newFilename.isEmpty() && QFile::exists(newDir + newFilename))
                {
                    newFilename = QInputDialog::getText(this, tr("file already exists"),
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
    \fn QAjIncomingWidget::removeFtp()
 */
void QAjIncomingWidget::removeFtp()
{
    QList<QAjItem *>  selectedItems = this->selectedAjItems();
    if(confirmRemove( selectedItems ) )
    {
        QFtp* ftp = new QFtp( this );
        QString server = QAjOptionsDialog::getSetting( "ftp", "server", "localhost" ).toString();
        int port = QAjOptionsDialog::getSetting( "ftp", "port", "21" ).toInt();
        QString user = QAjOptionsDialog::getSetting( "ftp", "user", "anonymous" ).toString();
        QString password = QAjOptionsDialog::getSetting( "ftp", "password", "" ).toString();
        QString dir = QAjOptionsDialog::getSetting( "ftp", "inDir", "/" ).toString();
        QFtp::TransferMode mode = (QFtp::TransferMode)QAjOptionsDialog::getSetting( "ftp", "mode", QFtp::Active ).toInt();
        if ( ! dir.endsWith( '/' ) )
            dir += '/';
        ftp->connectToHost( server, port );
        ftp->login( user, password );
        ftp->setTransferMode( mode );
        int i;
        for ( i=0; i<selectedItems.size(); i++ )
        {
            ftp->remove( dir + selectedItems.at(i)->text( FILENAME_INCOMING_INDEX ) );
        }
    }
}


/*!
    \fn QAjIncomingWidget::remove()
 */
void QAjIncomingWidget::remove()
{
    QList<QAjItem*> items = selectedAjItems();
    if( confirmRemove( items ) )
    {
        QString actDir;
        // determine the path
        AjSettings::LOCATION location = getLocation();
        if(location == AjSettings::FTP)
        {
            removeFtp();
        }
        else
        {
            if( location == AjSettings::SPECIFIC )
            {
                actDir = QAjOptionsDialog::getSetting( "incomingDirSpecific", "/" ).toString() + QDir::separator();
            }
            else if( location == AjSettings::SAME )
            {
                actDir = this->dir + QDir::separator();
            }
            int i;
            for( i=0; i<items.size(); i++ )
            {
                if(!QFile::remove( actDir + items[i]->text( FILENAME_INCOMING_INDEX ) ))
                {
                QMessageBox::critical( this, "Error", "Could not remove\n" + actDir + items[i]->text( FILENAME_INCOMING_INDEX ) );
                }
            }
            reload();
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

    QString dir = QAjOptionsDialog::getSetting( "ftp", "inDir", "/" ).toString();

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
        localDir = QFileDialog::getExistingDirectory( this, tr("save")+"\"" + filename + "\" "+tr("to") );
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
                QMessageBox::critical( this, tr("error"), "\"" + dstFile->fileName() + "\" "+tr("already exists"), QMessageBox::Ok, QMessageBox::NoButton );
            }
        }
    }
    ftp->start();
}


/*!
    \fn QAjIncomingWidget::openFtp()
 */
void QAjIncomingWidget::openFtp()
{
    QString filename;

    QString dir = QAjOptionsDialog::getSetting( "ftp", "inDir", "/" ).toString();

    // TODO: check if the core filesystem separator is a better choice
    if ( ! dir.endsWith( '/' ) )
    {
        dir += '/';
    }
    FTP* ftp = new FTP( this );

    bool full = QAjOptionsDialog::getSetting( "ftp", "full", false ).toBool();
    if( full )
    {
        connect( ftp, SIGNAL( downloadFinished( QFile*, FTP* ) ), this, SLOT( ftpReadyRead( QFile*, FTP* ) ) );
    }
    else
    {
        connect( ftp, SIGNAL( readyRead( QFile*, FTP* ) ), this, SLOT( ftpReadyRead( QFile*, FTP* ) ) );
    }

    QList<QTreeWidgetItem *>  selectedItems = this->selectedItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        filename = selectedItems.at(i)->text( FILENAME_INCOMING_INDEX );
        ftp->add( dir + filename );
    }
    ftp->start();
}


/*!
    \fn QAjIncomingWidget::reload()
 */
void QAjIncomingWidget::reload()
{
    this->clear();
    waitLabel->setVisible( false );
    AjSettings::LOCATION location = getLocation();
    if(location == AjSettings::FTP)
    {
        reloadFtp();
    }
    else
    {
        QString actDir;
        if(location == AjSettings::SAME)
        {
            if(this->dir.isEmpty()) {
                QLabel* l = new QLabel(tr("There is no connection to the core.\nYou may want to define the incoming directory manually at the options dialog."), this);
                l->adjustSize();
                l->move((this->width() - l->width()) / 2, (this->height() - l->height()) / 2);
                l->show();
                return;
            }
            actDir = this->dir;
        }
        else
        {
            actDir = QAjOptionsDialog::getSetting( "incomingDirSpecific", "/" ).toString();
        }
        QDir dirDir(actDir);
        if(dirDir.exists())
        {
            QFileInfoList list = dirDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
            int i;
            for(i=0; i<list.size(); i++)
            {
                QAjIncomingItem* item = new QAjIncomingItem( list[i].size(), list[i].lastModified().toLocalTime(), this );
                item->setText(FILENAME_INCOMING_INDEX, list[i].fileName());
                item->setText(SIZE_INCOMING_INDEX, QConvert::bytes((double)list[i].size(), 2));
                item->setText(DATE_INCOMING_INDEX, list[i].lastModified().toLocalTime().toString( Qt::LocalDate ) );
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
    AjSettings::LOCATION location = getLocation();
    if( location == AjSettings::SPECIFIC )
    {
        actDir = QAjOptionsDialog::getSetting( "incomingDirSpecific", "/" ).toString() + QDir::separator();
    }
    else if( location == AjSettings::SAME )
    {
        actDir = this->dir + QDir::separator();
    }
    else // ftp
    {
        return openFtp();
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
    waitLabel->setVisible( false );
    if ( info.isFile() || info.isDir() )
    {
        QAjIncomingItem *item = new QAjIncomingItem( info.size(), info.lastModified().toLocalTime(), this );
        item->setText( FILENAME_INCOMING_INDEX, info.name() );
        item->setText( SIZE_INCOMING_INDEX, QConvert::bytes( (double)info.size(), 2 ) );
        item->setText( DATE_INCOMING_INDEX, info.lastModified().toLocalTime().toString() );
        adjustSizeOfColumns();
    }
}


/*!
    \fn QAjIncomingWidget::initPopup()
 */
void QAjIncomingWidget::initPopup()
{
    popup->setTitle( tr("Incoming") );
    popup->addAction( openButton );
    popup->addAction( saveButton );
    popup->addAction( removeButton );
    popup->addSeparator();
    popup->addAction( reloadButton );
}


/*!
    \fn QAjIncomingWidget::getLocation()
 */
AjSettings::LOCATION QAjIncomingWidget::getLocation()
{
    return (AjSettings::LOCATION)QAjOptionsDialog::getSetting( "location", AjSettings::SAME ).toInt();
}


/*!
    \fn QAjIncomingWidget::ftpReadyRead( QFile* dstFile, FTP* ftp )
 */
void QAjIncomingWidget::ftpReadyRead( QFile* dstFile, FTP* ftp )
{
    QStringList args = Juicer::getExec();
    QString exec = args.takeFirst();

    args << dstFile->fileName();
    QProcess::startDetached( exec, args );
}


/*!
    \fn QAjIncomingWidget::confirmRemove( QList<QAjItem *> items )
 */
bool QAjIncomingWidget::confirmRemove( QList<QAjItem *> items )
{
    int i;
    int maxFilesToShow = 10;
    QString list = "<b>"+tr("Delete") + " " + QString::number(items.size()) + " "
            + (items.size()>1?tr("files"):tr("file"))+"?</b><br>";
    for( i=0; i<items.size() && i<maxFilesToShow; i++ )
    {
        list += "<br>" + items[i]->text( FILENAME_INCOMING_INDEX );
    }
    if(items.size() > maxFilesToShow)
    {
        list += "<br>(" + QString::number(items.size() - maxFilesToShow) + " more)";
    }
    return QMessageBox::question( this, "Confirmation", list, QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes;
}
