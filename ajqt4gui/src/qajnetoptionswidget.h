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
#ifndef QAJNETOPTIONSWIDGET_H
#define QAJNETOPTIONSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>

#include "types.h"

#define ACT_S(widget) s=(( widget->x() + widget->width() ) > s ) ? widget->x() + widget->width() : s
#define ACT_E(widget) e=(( widget->x() + widget->width() ) > e ) ? widget->x() + widget->width() : e

/**
@author Matthias Reif
*/
class QAjNetOptionsWidget : public QWidget
{
Q_OBJECT
public:
	QAjNetOptionsWidget( QWidget *parent = 0, const char *name = 0);

	~QAjNetOptionsWidget();
	
	void setAjSettigs( AjSettings settings );
	
	QString getMaxDown() { return maxDownSpin->text(); }
	QString getMaxUp() { return maxUpSpin->text(); }
	QString getMaxSlot() { return maxSlotSpin->text(); }
	QString getMaxSources() { return maxSourcesSpin->text(); }
	QString getMaxCon() { return maxConSpin->text(); }
	QString getMaxNewCon() { return maxNewConSpin->text(); }
	QString getTcpPort() { return tcpPortEdit->text(); }

protected:
	QLabel *maxDownLabel;
	QLabel *maxDownLabel2;
	QSpinBox *maxDownSpin;
	QLabel *maxUpLabel;
	QLabel *maxUpLabel2;
	QSpinBox *maxUpSpin;
	QLabel *maxSlotLabel;
	QLabel *maxSlotLabel2;
	QSpinBox *maxSlotSpin;
	QLabel *tcpPortLabel;
	QLineEdit *tcpPortEdit;

	QLabel *maxSourcesLabel;
	QSpinBox *maxSourcesSpin;
	QSpinBox *maxConSpin;
	QLabel *maxConLabel;
	QLabel *maxNewConLabel;
	QSpinBox *maxNewConSpin;

};

#endif
