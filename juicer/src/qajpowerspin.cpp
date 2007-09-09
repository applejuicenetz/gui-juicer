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
#include "qajpowerspin.h"

QAjPowerSpin::QAjPowerSpin(QString id, QWidget *parent) : QWidget(parent)
{
    this->id = id;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    layout->insertSpacing(0, 10);
    check = new QCheckBox();
    layout->addWidget(check);

    spin = new QDoubleSpinBox();
    spin->setFrame(false);
    spin->setRange(2.2, 50.0);
    spin->setSingleStep(0.1);
    spin->setDecimals(1);
    layout->addWidget(spin);

    setLayout(layout);

    connect(check, SIGNAL(toggled(bool)), spin, SLOT(setEnabled(bool)));
    connect(check, SIGNAL(toggled(bool)), this, SLOT(powerChanged()));
    connect(spin, SIGNAL(valueChanged(double)), this, SLOT(powerChanged()));
}


QAjPowerSpin::~QAjPowerSpin()
{
}

void QAjPowerSpin::powerChanged()
{
    if(check->isChecked())
        powerChanged(id, spin->value());
    else
        powerChanged(id, 1.0);
}
