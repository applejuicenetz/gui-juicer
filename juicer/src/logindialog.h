//
// C++ Interface: 
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

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

