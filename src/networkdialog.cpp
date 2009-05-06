/***************************************************************************
 *   Copyright (C) 2007 by Matthias Reif   *
 *   matthias.reif@iupr.dfki.de   *
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


#include "networkdialog.h"

NetworkDialog::NetworkDialog(QWidget* parent, Qt::WFlags fl) : QDialog( parent, fl ), Ui::NetworkDialog()
{
    setupUi(this);
    this->setWindowIcon( QIcon(":/juicer.png") );
}

NetworkDialog::~NetworkDialog()
{
}


/*!
    \fn NetworkDialog::setValues( const QString& users, const QString& files, const QString& size, const QString& ip, const QString& firewalled )
 */
void NetworkDialog::setValues( const QString& users, const QString& files, const QString& size, const QString& ip, const QString& firewalled )
{
    this->usersLabel->setText( users );
    this->filesLabel->setText( files );
    this->sizeLabel->setText( size );
    this->ipLabel->setText( ip );
    this->firewalledLabel->setText( firewalled );
}
