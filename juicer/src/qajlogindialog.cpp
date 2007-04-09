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
#include "qajlogindialog.h"
//Added by qt3to4:
#include <QLabel>

QAjLoginDialog::QAjLoginDialog(QString host, QString port, QString password, QString errorMsg, QWidget *parent, const char *name) : QDialog( parent )
{
    messageLabel = new QLabel( this );
    hostLabel = new QLabel( this );
    hostEdit = new QLineEdit( this );
    portLabel = new QLabel( this );
    portEdit = new QLineEdit( this );
    passwordLabel = new QLabel( this );
    passwordEdit = new QLineEdit( this );
    retryButton = new QPushButton( this );
    cancelButton = new QPushButton( this );

    messageLabel->setText( errorMsg );
    hostLabel->setText( tr("Host: ") );
    portLabel->setText( tr("XML port: ") );
    passwordLabel->setText( tr("Password: ") );

    passwordEdit->setEchoMode( QLineEdit::Password );

    hostEdit->setText( host );
    portEdit->setText( port );
    passwordEdit->setText( password );

    retryButton->setText( tr("Retry") );
    cancelButton->setText( tr("Cancel") );

    messageLabel->adjustSize();
    hostLabel->adjustSize();
    portLabel->adjustSize();
    passwordLabel->adjustSize();

    cancelButton->adjustSize();
    retryButton->adjustSize();

    hostEdit->resize( 100, hostLabel->height() + 4 );
    portEdit->resize( 100, hostLabel->height() + 4 );
    passwordEdit->resize( 100, hostLabel->height() + 4 );

    int s = hostLabel->width();
    if ( portLabel->width() > s )
        s = portLabel->width();
    if ( passwordLabel->width() > s )
        s = passwordLabel->width();

    int l = portLabel->height() + 10;
    int start = 20;

    messageLabel->move( 10, start + l*0 );
    hostLabel->move( 10, start + l*1 );
    portLabel->move( 10, start + l*2 );
    passwordLabel->move( 10, start + l*3 );

    hostEdit->move( s+15, start + l*1 -2 );
    portEdit->move( s+15, start + l*2 -2 );
    passwordEdit->move( s+15, start + l*3 -2 );

    cancelButton->move( 10, start + l*4 + 20);
    retryButton->move( cancelButton->x() + cancelButton->width() + 20, start + l*4 +20 );

    adjustSize();

    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelClicked() ) );
    connect( retryButton, SIGNAL( clicked() ), this, SLOT( retryClicked() ) );
}

QAjLoginDialog::~QAjLoginDialog()
{}

void QAjLoginDialog::retryClicked()
{
    done( QDialog::Accepted );
}

void QAjLoginDialog::cancelClicked()
{
    done( QDialog::Rejected );
}
