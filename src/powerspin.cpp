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

#include "powerspin.h"

PowerSpin::PowerSpin(const QString& id, QWidget *parent) : QWidget(parent) {
    this->id = id;
    QHBoxLayout *layout = new QHBoxLayout;

    layout->setSpacing(0);
    layout->setMargin(0);

    layout->addSpacing(5);
    check = new QCheckBox();
    layout->addWidget(check);

    spin = new QDoubleSpinBox();
    spin->setFrame(true);
    spin->setRange(2.2, 50.0);
    spin->setSingleStep(0.1);
    spin->setDecimals(1);
    layout->addWidget(spin);
    layout->addStretch(1);

    setLayout(layout);

    connect(check, SIGNAL(toggled(bool)), spin, SLOT(setEnabled(bool)));
    connect(check, SIGNAL(toggled(bool)), this, SLOT(powerChanged()));
    connect(spin, SIGNAL(valueChanged(double)), this, SLOT(powerChanged()));
}


PowerSpin::~PowerSpin() {
}

void PowerSpin::powerChanged() {
    powerChanged(id, check->isChecked()?spin->value():1.0);
}
