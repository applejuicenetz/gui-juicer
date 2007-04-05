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
#ifndef QAJNETWORKWIDGET_H
#define QAJNETWORKWIDGET_H

#include <qmessagebox.h>
#include <qlabel.h>
#include <qlineedit.h>

#define VER_SPACING 10
#define HOR_SPACING 20

/**
@author Matthias Reif
*/
class QAjNetworkWidget : public QMessageBox
{
Q_OBJECT
public:
	QAjNetworkWidget(QWidget *parent = 0, const char *name = 0);

	~QAjNetworkWidget();

	void setUsers(QString users) { usersLabel2->setText(users); usersLabel2->adjustSize(); }
	void setFiles(QString files) { filesLabel2->setText(files); filesLabel2->adjustSize(); }
	void setSize(QString size) { sizeLabel2->setText(size); sizeLabel2->adjustSize(); }
	void setIp(QString ip) { ipLabel2->setText(ip); ipLabel2->adjustSize(); }

protected:
	QLabel *usersLabel1, *usersLabel2;
	QLabel *filesLabel1, *filesLabel2;
	QLabel *sizeLabel1, *sizeLabel2;
	QLabel *ipLabel1, *ipLabel2;

	int secondCol;

	void actSecondCol(int x) { secondCol = x>secondCol?x:secondCol; }
};

#endif
