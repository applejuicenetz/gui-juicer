/***************************************************************************
 *   Copyright (C) 2006 by Matthias Reif   *
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
#include "ftp.h"

#include "juicer.h"

FTP::FTP( QString host, int port, QString user, QString password, bool binary, QObject *parent ) : QThread( parent )
{
    this->host = host;
    this->port = port;
    this->user = user;
    this->password = password;
    this->binary = binary;
    init();
}

FTP::FTP( QObject *parent ) : QThread( parent )
{
    host = QAjOptionsDialog::getSetting( "ftp", "server", "localhost" ).toString();
    port = QAjOptionsDialog::getSetting( "ftp", "port", "21" ).toInt();
    user = QAjOptionsDialog::getSetting( "ftp", "user", "anonymous" ).toString();
    password = QAjOptionsDialog::getSetting( "ftp", "password", "" ).toString();
    binary = true;
    init();
}

FTP::~FTP()
{
}


/*!
    \fn FTP::init()
 */
void FTP::init()
{
    dstFile = NULL;
    tmpMode = false;
    ftp = new QFtp( this );
    connect( ftp, SIGNAL( stateChanged( int ) ), this, SLOT( stateChangedSlot( int ) ) );
    connect( ftp, SIGNAL( commandFinished( int, bool ) ), this, SLOT( commandFinishedSlot( int, bool ) ) );
    connect( ftp, SIGNAL( dataTransferProgress( qint64, qint64 ) ), this, SLOT( dataTransferProgressSlot( qint64, qint64 ) ) );

    progressDialog = new QProgressDialog();
    progressDialog->hide();
    connect( progressDialog, SIGNAL( canceled() ), this, SLOT( abort() ) );
}


/*!
    \fn FTP::stateChangedSlot( int state )
 */
void FTP::stateChangedSlot( int state )
{
    if( state == QFtp::LoggedIn )
        getNext();
}


/*!
    \fn FTP::commandFinishedSlot( int id, bool error )
 */
void FTP::commandFinishedSlot( int id, bool error )
{
    if ( error )
    {
        errorOccured( ftp->errorString() );
        abort();
    }
    if ( id == getFile )
    {
        dstFile->flush();
        dstFile->close();
        downloadFinished( dstFile, this );
        dstFile = NULL;
        if(progressDialog->isVisible())
            progressDialog->hide();
        getNext();
    }
}


/*!
    \fn FTP::run()
 */
void FTP::run()
{
    ftp->connectToHost( host, port );
    ftp->login( user, password );
    ftp->setTransferMode( (QFtp::TransferMode)QAjOptionsDialog::getSetting( "ftp", "mode", QFtp::Active ).toInt() );
    exec();
}


/*!
    \fn FTP::dataTransferProgressSlot( qint64 done, qint64 total )
 */
void FTP::dataTransferProgressSlot( qint64 done, qint64 total )
{
    qint64 min = -1;
    if( tmpMode && ! ready )
    {
        bool full = QAjOptionsDialog::getSetting( "ftp", "full", false ).toBool();
        if( !full )
        {
            min = QAjOptionsDialog::getSetting( "ftp", "mb", 10 ).toInt() * 1024 * 1024;
            total = min;
        }
    }

    if ( dstFile != NULL )
    {
        progressDialog->setLabelText( dstFile->fileName() );
    }
    progressDialog->setRange( 0, total );
    progressDialog->show();
    progressDialog->setValue( done );

    if( tmpMode && !ready && min > 0 && done >= min )
    {
        ready = true;
        readyRead( dstFile, this );
    }
}


/*!
    \fn FTP::getNext()
 */
void FTP::getNext()
{
    if ( ! queue.empty() )
    {
        FTP::StoreInfo s = queue.takeFirst();
        dstFile = s.dstFile;
        getFile = ftp->get( s.srcFile, s.dstFile );//, binary?(QFtp::Binary):(QFtp::Ascii) );
    }
}


/*!
    \fn FTP::add( QString srcFilename, QFile* dstFile )
 */
void FTP::add( QString srcFilename, QFile* dstFile )
{
    FTP::StoreInfo s;
    s.srcFile = srcFilename;
    s.dstFile = dstFile;
    queue.append( s );
}


/*!
    \fn FTP::add( QString srcFilename )
 */
void FTP::add( QString srcFilename )
{
    tmpMode = true;
    QTemporaryFile* dstFile = new QTemporaryFile( QDir::tempPath()+"juicer", this );
    dstFile->open();
    add( srcFilename, dstFile );
}


/*!
    \fn FTP::abort()
 */
void FTP::abort()
{
    ftp->abort();
    progressDialog->hide();
    if ( dstFile != NULL )
    {
        dstFile->flush();
        dstFile->close();
        if( tmpMode )
        {
            dstFile->remove();
        }
    }
}
