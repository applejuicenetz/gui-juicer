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

#include "iconwidget.h"

IconWidget::IconWidget(const QString& icon, const QString& text, const QString& toolTip, QBoxLayout::Direction dir, QWidget *parent, int spacing, int margin) : QWidget(parent) {
    iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(icon));
    textLabel = new QLabel(text, this);
    textLabel->adjustSize();

    QBoxLayout* layout = new QBoxLayout(dir);
    layout->setMargin(margin);
    layout->setSpacing(spacing);
    layout->addWidget(iconLabel, 0, Qt::AlignHCenter);
    layout->addWidget(textLabel, 0, Qt::AlignHCenter);

    this->setToolTip(toolTip);
    this->setLayout(layout);
    this->adjustSize();
}


IconWidget::~IconWidget() {
}


/*!
    \fn IconWidget::setText(const QString& text)
 */
void IconWidget::setText(const QString& text) {
    textLabel->setText(text);
}
