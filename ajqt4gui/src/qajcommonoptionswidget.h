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
#ifndef QAJCOMMONOPTIONSWIDGET_H
#define QAJCOMMONOPTIONSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QSpinBox>

#include "types.h"

#define ACT_S(widget) s=(( widget->x() + widget->width() ) > s ) ? widget->x() + widget->width() : s
#define ACT_E(widget) e=(( widget->x() + widget->width() ) > e ) ? widget->x() + widget->width() : e

/**
@author Matthias Reif
*/
class QAjCommonOptionsWidget : public QWidget
{
Q_OBJECT
public:
	QAjCommonOptionsWidget( QWidget *parent = 0, const char *name = 0);

	~QAjCommonOptionsWidget();
	
	void setAjSettigs( AjSettings settings );
	
	QString getNick() { return nickEdit->text(); }
	QString getPassword() { return passwordEdit->text(); }
	QString getCoreAddress() { return coreAddressEdit->text(); }
	QString getXmlPort() { return xmlPortEdit->text(); }
	QString getIncomingDir() { return incomingDirEdit->text(); }
	QString getTempDir() { return tempDirEdit->text(); }
	QString getServerURL() { return serverURLEdit->text(); }
	int getTimer() { return timerSpin->value(); }
	QString getAutoconnect() { if(autoconnectCheckBox->isChecked()) return QString("true"); else return QString("false"); }
	QString getAllowBrowse() { if(allowBrowseCheckBox->isChecked()) return QString("true"); else return QString("false"); }
	QString getSavePassword() { if(savePasswordCheckBox->isChecked()) return QString("true"); else return QString("false"); }

protected:
	QLabel *nickLabel;
	QLineEdit *nickEdit;
	QLabel *passwordLabel;
	QLineEdit *passwordEdit;
	QLabel *passwordConfirmLabel;
	QLineEdit *passwordConfirmEdit;
	
	QLabel *coreAddressLabel;
	QLineEdit *coreAddressEdit;
	QLabel *xmlPortLabel;
	QLineEdit *xmlPortEdit;
	
	QLabel *incomingDirLabel;
	QLineEdit *incomingDirEdit;
	QPushButton *incomingDirButton;
	QLabel *tempDirLabel;
	QLineEdit *tempDirEdit;
	QPushButton *tempDirButton;
	
	QLabel *serverURLLabel;
	QLineEdit *serverURLEdit;
	
	QLabel *timerLabel;
	QLabel *timerLabel2;
	QSpinBox *timerSpin;
	
	QCheckBox *autoconnectCheckBox;
	QCheckBox *allowBrowseCheckBox;
	QCheckBox *savePasswordCheckBox;

protected slots:
	void selectIncomingDir();
	void selectTempDir();
};

#endif
