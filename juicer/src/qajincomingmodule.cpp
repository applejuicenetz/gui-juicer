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
#include "qajincomingmodule.h"

#include "juicer.h"

QAjIncomingModule::QAjIncomingModule(Juicer* juicer) : QAjModuleBase(juicer, juicer->incomingTreeWidget, juicer->incomingToolBar) {
    ftp = new QFtp( this );
    waitLabel = new QLabel( tr("please wait..."), treeWidget);
    connect( ftp, SIGNAL( listInfo ( QUrlInfo ) ), this, SLOT( insert( QUrlInfo ) ) );
    connect( treeWidget, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ), this, SLOT( open() ) );
    connect(juicer->actionOpen_Incoming, SIGNAL(triggered()), this, SLOT(open()));
    connect(juicer->actionCopy_Incoming, SIGNAL(triggered()), this, SLOT(copy()));
    connect(juicer->actionDelete_Incoming, SIGNAL(triggered()), this, SLOT(remove()));
    connect(juicer->actionReload_Incoming, SIGNAL(triggered()), this, SLOT(reload()));
    selectionChanged();
}


QAjIncomingModule::~QAjIncomingModule()
{
}


/*!
    \fn QAjIncomingModule::reloadFtp()
 */
void QAjIncomingModule::reloadFtp()
{
    treeWidget->clear();
    waitLabel->move( (treeWidget->width() - treeWidget->width()) /2, (treeWidget->height() - waitLabel->height()) / 2);
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
    \fn QAjIncomingModule::copy()
 */
void QAjIncomingModule::copy()
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
        QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
        for(int i=0; i<selectedItems.size(); i++ ) {
            QString newDir = QFileDialog::getExistingDirectory(juicer, tr("copy to"), actDir)
                    + QDir::separator();
            if(!newDir.isEmpty())
            {
                QString newFilename = selectedItems[i]->text( FILENAME_INCOMING_INDEX );
                while(!newFilename.isEmpty() && QFile::exists(newDir + newFilename))
                {
                    newFilename = QInputDialog::getText(juicer, tr("file already exists"),
                            "filename", QLineEdit::Normal, selectedItems[i]->text( FILENAME_INCOMING_INDEX ));
                }
                if(!newFilename.isEmpty())
                {
                    (new CopyThread(actDir + selectedItems[i]->text( FILENAME_INCOMING_INDEX ),
                                newDir + newFilename))->start();
                }
            }
        }
    }
}


/*!
    \fn QAjIncomingModule::removeFtp()
 */
void QAjIncomingModule::removeFtp()
{
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    if(confirmRemove(selectedItems)) {
        QFtp* ftp = new QFtp(this);
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
        for(int i=0; i<selectedItems.size(); i++ ) {
            ftp->remove( dir + selectedItems[i]->text( FILENAME_INCOMING_INDEX ) );
        }
    }
}


/*!
    \fn QAjIncomingModule::remove()
 */
void QAjIncomingModule::remove()
{
    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    if(confirmRemove(selectedItems)) {
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
            for( i=0; i<selectedItems.size(); i++ )
            {
                if(!QFile::remove( actDir + selectedItems[i]->text( FILENAME_INCOMING_INDEX ) ))
                {
                QMessageBox::critical(juicer, "Error", "Could not remove\n" + actDir + selectedItems[i]->text(FILENAME_INCOMING_INDEX));
                }
            }
            reload();
        }
    }
}


/*!
    \fn QAjIncomingModule::storeFtp()
 */
void QAjIncomingModule::storeFtp()
{
    QString filename, localDir;

    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();

    QString dir = QAjOptionsDialog::getSetting( "ftp", "inDir", "/" ).toString();

    // TODO: check if the core filesystem separator is a better choice
    if ( ! dir.endsWith( '/' ) )
    {
        dir += '/';
    }
    FTP* ftp = new FTP( this );
    for (int  i=0; i<selectedItems.size(); i++ )
    {
        filename = selectedItems.at(i)->text( FILENAME_INCOMING_INDEX );
        localDir = QFileDialog::getExistingDirectory( juicer, tr("copy")+"\"" + filename + "\" "+tr("to") );
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
                QMessageBox::critical( juicer, tr("error"), "\"" + dstFile->fileName() + "\" "+tr("already exists"), QMessageBox::Ok, QMessageBox::NoButton );
            }
        }
    }
    ftp->start();
}


/*!
    \fn QAjIncomingModule::openFtp()
 */
void QAjIncomingModule::openFtp()
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

    QList<QTreeWidgetItem *>  selectedItems = treeWidget->selectedItems();
    int i;
    for ( i=0; i<selectedItems.size(); i++ )
    {
        filename = selectedItems.at(i)->text( FILENAME_INCOMING_INDEX );
        ftp->add( dir + filename );
    }
    ftp->start();
}


/*!
    \fn QAjIncomingModule::reload()
 */
void QAjIncomingModule::reload()
{
    treeWidget->clear();
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
                QLabel* l = new QLabel(tr("There is no connection to the core.\nYou may want to define the incoming directory manually at the options dialog."), treeWidget);
                l->adjustSize();
                l->move((treeWidget->width() - l->width()) / 2, (treeWidget->height() - l->height()) / 2);
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
                QAjIncomingItem* item = new QAjIncomingItem( list[i].size(), list[i].lastModified().toLocalTime(), treeWidget );
                item->setText(FILENAME_INCOMING_INDEX, list[i].fileName());
                item->setText(SIZE_INCOMING_INDEX, QConvert::bytes((double)list[i].size(), 2));
                item->setText(DATE_INCOMING_INDEX, list[i].lastModified().toLocalTime().toString( Qt::LocalDate ) );
                treeWidget->addTopLevelItem( item );
            }
        }
    }
    adjustSizeOfColumns();
}


/*!
    \fn QAjIncomingModule::setDir( QString dir )
 */
void QAjIncomingModule::setDir( QString dir )
{
    this->dir = dir;
}


/*!
    \fn QAjIncomingModule::open()
 */
void QAjIncomingModule::open()
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

    QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
    for(int i=0; i<selectedItems.size(); i++ ) {
        args <<  actDir + selectedItems[i]->text( FILENAME_INCOMING_INDEX );
        QProcess::startDetached( exec, args );
        args.removeLast();
    }
}

/*!
    \fn QAjIncomingModule::insert( QUrlInfo info )
 */
void QAjIncomingModule::insert( QUrlInfo info )
{
    waitLabel->setVisible( false );
    if ( info.isFile() || info.isDir() )
    {
        QAjIncomingItem *item = new QAjIncomingItem( info.size(), info.lastModified().toLocalTime(), treeWidget );
        item->setText( FILENAME_INCOMING_INDEX, info.name() );
        item->setText( SIZE_INCOMING_INDEX, QConvert::bytes( (double)info.size(), 2 ) );
        item->setText( DATE_INCOMING_INDEX, info.lastModified().toLocalTime().toString() );
        adjustSizeOfColumns();
    }
}


/*!
    \fn QAjIncomingModule::getLocation()
 */
AjSettings::LOCATION QAjIncomingModule::getLocation()
{
    return (AjSettings::LOCATION)QAjOptionsDialog::getSetting( "location", AjSettings::SAME ).toInt();
}


/*!
    \fn QAjIncomingModule::ftpReadyRead( QFile* dstFile, FTP* ftp )
 */
void QAjIncomingModule::ftpReadyRead( QFile* dstFile, FTP* ftp )
{
    ftp = ftp;
    QStringList args = Juicer::getExec();
    QString exec = args.takeFirst();

    args << dstFile->fileName();
    QProcess::startDetached( exec, args );
}

/*!
    \fn QAjIncomingModule::confirmRemove(QList<QTreeWidgetItem *>& items)
 */
bool QAjIncomingModule::confirmRemove(QList<QTreeWidgetItem *>& items) {
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
    return QMessageBox::question( juicer, "Confirmation", list, QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes;
}


void QAjIncomingModule::selectionChanged() {
    bool oneSelected = !treeWidget->selectedItems().empty();
    juicer->actionOpen_Incoming->setEnabled(oneSelected);
    juicer->actionCopy_Incoming->setEnabled(oneSelected);
    juicer->actionDelete_Incoming->setEnabled(oneSelected);
}
