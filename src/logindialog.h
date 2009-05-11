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

#ifndef QAJLOGINDIALOG_H
#define QAJLOGINDIALOG_H

#include <QDialog>
#include "ui_logindialogbase.h"

class LoginDialog : public QDialog, private Ui::LoginDialogBase
{
  Q_OBJECT

public:
  LoginDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~LoginDialog();

  QString getHost() { return hostEdit->text(); }
  int getPort() { return portSpinBox->value(); }
  QString getPassword() { return passwordEdit->text(); }
  bool getSavePassword() { return savePasswordCheckBox->isChecked(); }
  void setHost( const QString& host ) { hostEdit->setText( host ); }
  void setPort( int port ) { portSpinBox->setValue( port ); }
  void setPassword( const QString& password ) { passwordEdit->setText( password ); }
  void setSavePassword( bool save ) { savePasswordCheckBox->setChecked(save); }
  void setHeader( const QString& text ) { headerLabel->setText( text ); }
  bool ignore;
public slots:

protected:

protected slots:

public slots:
    void clicked(QAbstractButton* button);
    void resetPort();
};

#endif

