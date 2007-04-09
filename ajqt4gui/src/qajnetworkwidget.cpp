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
#include "qajnetworkwidget.h"

QAjNetworkWidget::QAjNetworkWidget(QWidget *parent, const char *name) : QMessageBox( parent )
{
    setWindowTitle( "network info");
    setWindowIcon( QIcon(":/juicer.png") );

    secondCol = 0;
    usersLabel1 = new QLabel(this);
    usersLabel2 = new QLabel(this);
    filesLabel1 = new QLabel(this);
    filesLabel2 = new QLabel(this);
    sizeLabel1 = new QLabel(this);
    sizeLabel2 = new QLabel(this);
    ipLabel1 = new QLabel(this);
    ipLabel2 = new QLabel(this);

    usersLabel1->setText(tr("Users:"));
    filesLabel1->setText(tr("Files:"));
    sizeLabel1->setText(tr("Size:"));
    ipLabel1->setText(tr("Your ip:"));
    usersLabel1->adjustSize();
    filesLabel1->adjustSize();
    sizeLabel1->adjustSize();
    ipLabel1->adjustSize();

    usersLabel1->move(20, 20);
    filesLabel1->move(usersLabel1->x(), usersLabel1->y() + usersLabel1->height() + VER_SPACING);
    sizeLabel1->move(usersLabel1->x(), filesLabel1->y() + filesLabel1->height() + VER_SPACING);
    ipLabel1->move(usersLabel1->x(), sizeLabel1->y() + sizeLabel1->height() + VER_SPACING);

    actSecondCol( usersLabel1->x() + usersLabel1->width() );
    actSecondCol( filesLabel1->x() + filesLabel1->width() );
    actSecondCol( sizeLabel1->x() + sizeLabel1->width() );
    actSecondCol( ipLabel1->x() + ipLabel1->width() );

    usersLabel2->move( secondCol + HOR_SPACING, usersLabel1->y() );
    filesLabel2->move( secondCol + HOR_SPACING, filesLabel1->y() );
    sizeLabel2->move( secondCol + HOR_SPACING, sizeLabel1->y() );
    ipLabel2->move( secondCol + HOR_SPACING, ipLabel1->y() );

    usersLabel2->setText( tr("Please wait...") );
    usersLabel2->adjustSize();
}


QAjNetworkWidget::~QAjNetworkWidget()
{}
