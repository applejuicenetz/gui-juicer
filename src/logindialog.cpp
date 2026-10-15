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

#include "logindialog.h"

LoginDialog::LoginDialog(QWidget* parent, Qt::WFlags fl): QDialog( parent, fl ), Ui::LoginDialogBase()
{
    setupUi(this);
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clicked(QAbstractButton*)));
    connect(resetPortButton, SIGNAL(clicked()), this, SLOT(resetPort()));
}

LoginDialog::~LoginDialog()
{
}


/*!
    \fn LoginDialog::clicked(QAbstractButton* button)
 */
void LoginDialog::clicked(QAbstractButton* button)
{
	ignore = (buttonBox->standardButton(button) == QDialogButtonBox::Ignore);
}


/*!
    \fn LoginDialog::resetPort()
 */
void LoginDialog::resetPort()
{
    portSpinBox->setValue(9851);
}
