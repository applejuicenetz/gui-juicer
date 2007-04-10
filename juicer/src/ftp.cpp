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

FTP::FTP( QString host, int port, QString user, QString password, bool binary, QObject *parent ) : QThread( parent )
{
    this->host = host;
    this->port = port;
    this->user = user;
    this->password = password;
    this->binary = binary;
    dstFile = NULL;

    ftp = new QFtp( );
    connect( ftp, SIGNAL( stateChanged( int ) ), this, SLOT( stateChangedSlot( int ) ) );
    connect( ftp, SIGNAL( commandFinished( int, bool ) ), this, SLOT( commandFinishedSlot( int, bool ) ) );
    connect( ftp, SIGNAL( dataTransferProgress( qint64, qint64 ) ), this, SLOT( dataTransferProgressSlot( qint64, qint64 ) ) );
    connect( ftp, SIGNAL( listInfo ( QUrlInfo ) ), this, SLOT( listInfoSlot( QUrlInfo ) ) );

    progressDialog = new QProgressDialog();
    progressDialog->hide();
    connect( progressDialog, SIGNAL( canceled() ), this, SLOT( abort() ) );
}

FTP::~FTP()
{
    ftp->close();
    delete ftp;
}


/*!
    \fn FTP::stateChangedSlot( int state )
 */
void FTP::stateChangedSlot( int state )
{
    switch ( state )
    {
    case( QFtp::Connected ):
        break;
    case( QFtp::LoggedIn ):
        getNext();
        break;
    default:
        break;
    }
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
        downloadFinished( dstFile );
//		QMessageBox::information ( NULL, "finished", dstFile->fileName() + " finished", QMessageBox::Ok );
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
    ftp->setTransferMode( QFtp::Passive );
    exec();
}


/*!
    \fn FTP::dataTransferProgressSlot( qint64 done, qint64 total )
 */
void FTP::dataTransferProgressSlot( qint64 done, qint64 total )
{
    fprintf(stderr, "%d %d\n", done, total );
    if ( dstFile != NULL )
    {
        progressDialog->setLabelText( dstFile->fileName() );
    }
    progressDialog->setRange( 0, total );
    progressDialog->show();
    progressDialog->setValue( done );
}


/*!
    \fn FTP::getNext()
 */
void FTP::getNext()
{
    if ( ! queue.empty() )
    {
        StoreInfo s = queue.takeFirst();
        dstFile = s.dstFile;
        getFile = ftp->get( s.srcFile, s.dstFile );//, binary?(QFtp::Binary):(QFtp::Ascii) );
    }
}


/*!
    \fn FTP::add( QString srcFilename, QFile* dstFile )
 */
void FTP::add( QString srcFilename, QFile* dstFile )
{
    StoreInfo s;
    s.srcFile = srcFilename;
    s.dstFile = dstFile;
    queue.append( s );
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
    }
}


/*!
    \fn FTP::list( QString directory )
 */
void FTP::list( QString directory )
{
    ftp->connectToHost( host, port );
    ftp->login( user, password );
    ftp->list( directory );
}


/*!
    \fn FTP::listInfoSlot( QUrlInfo info )
 */
void FTP::listInfoSlot( QUrlInfo info )
{
    listEntry( info );
}
