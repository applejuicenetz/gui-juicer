/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
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
