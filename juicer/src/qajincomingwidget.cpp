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

QAjIncomingWidget::QAjIncomingWidget( QXMLModule* xml, QWidget *parent ) : QAjListWidget( xml, parent )
{

    QStringList headers;
    int i;
    for ( i=0; i<NUM_FTP_COL; i++)
    {
        switch (i)
        {
        case FILENAME_FTP_INDEX:
            headers.append( tr("filename") );
            break;
        case SIZE_FTP_INDEX:
            headers.append( tr("size") );
            break;
        }
    }
    setHeaderLabels( headers );

//     ftp = new FTP( );
//     connect( ftp, SIGNAL( listInfo ( QUrlInfo ) ), this->ajFtpWidget, SLOT( insert( QUrlInfo ) ) );
//     connect( ajFtpWidget, SIGNAL( itemDoubleClicked ( QTreeWidgetItem*, int ) ), this, SLOT( storeFtp( ) ) );
//     connect( ajFtpWidget, SIGNAL( newSelection( bool ) ), storeFtpButton, SLOT( setEnabled( bool ) ) );

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
//     openIncomingButton = incomingToolBar->addAction( QIcon(":/exec.png"), "open file", this, SLOT( openDownload() ) );
}

/*!
    \fn QAjIncomingWidget::reloadFtp()
 */
void QAjIncomingWidget::reloadFtp()
{
    this->clear();

    QSettings lokalSettings;
    lokalSettings.beginGroup("ftp");
    QString server = lokalSettings.value( "server", "/" ).toString();
    int port = lokalSettings.value( "port", "/" ).toInt();
    QString user = lokalSettings.value( "user", "/" ).toString();
    QString password = lokalSettings.value( "password", "/" ).toString();
    QString dir = lokalSettings.value( "dir", "/" ).toString();
    lokalSettings.endGroup();

    QFtp* ftp = new QFtp( this );

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
        filename = selectedItems.at(i)->text( FILENAME_FTP_INDEX );
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
